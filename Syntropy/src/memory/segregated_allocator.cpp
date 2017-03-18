#include "memory/segregated_allocator.h"

#include <iterator>
#include <algorithm>
#include <new>

#include "memory/memory.h"
#include "diagnostics/debug.h"
#include "math/math.h"
#include "platform/builtin.h"

namespace syntropy 
{

    /************************************************************************/
    /* LINEAR SEGREGATED FIT ALLOCATOR :: PAGE                              */
    /************************************************************************/

    LinearSegregatedFitAllocator::Page::Page(size_t block_size, size_t page_size)
        : next_(nullptr)
        , previous_(nullptr)
        , block_size_(block_size)
        , allocated_blocks_(0)
        , free_(GetFirstBlock())
    {
        // Fill the free-block linked list
        auto last_block = GetLastBlock(page_size);
        
        SYNTROPY_ASSERT(Memory::GetDistance(free_, last_block) >= 0);

        for(auto block = free_; block != last_block; block = block->next_)
        {
            block->next_ = Memory::AddOffset(block, block_size_);
        }
        
        last_block->next_ = nullptr;
    }

    void* LinearSegregatedFitAllocator::Page::AllocateBlock()
    {
        SYNTROPY_ASSERT(!IsFull());

        auto block = free_;
        free_ = block->next_;
        ++allocated_blocks_;

        return block;
    }

    void LinearSegregatedFitAllocator::Page::FreeBlock(void* block)
    {
        SYNTROPY_ASSERT(!IsEmpty());

        auto free_block = reinterpret_cast<Block*>(block);

        --allocated_blocks_;
        free_block->next_ = free_;
        free_ = free_block;
    }

    bool LinearSegregatedFitAllocator::Page::IsFull() const
    {
        return !free_;
    }

    bool LinearSegregatedFitAllocator::Page::IsEmpty() const
    {
        return allocated_blocks_ == 0;
    }

    LinearSegregatedFitAllocator::Block* LinearSegregatedFitAllocator::Page::GetFirstBlock()
    {
        // Find the first address past the page which is aligned to the block size.
        return reinterpret_cast<Block*>(Memory::Align(Memory::AddOffset(this, sizeof(Page)), block_size_));
    }

    LinearSegregatedFitAllocator::Block* LinearSegregatedFitAllocator::Page::GetLastBlock(size_t page_size)
    {
        page_size -= 2u * block_size_ + 1u;     // Ensures that there's enough space for a block and the required padding.

        return reinterpret_cast<Block*>(Memory::Align(Memory::AddOffset(this, page_size), block_size_));
    }

    /************************************************************************/
    /* LINEAR SEGREGATED FIT ALLOCATOR                                      */
    /************************************************************************/

    LinearSegregatedFitAllocator::LinearSegregatedFitAllocator(const HashedString& name, size_t capacity, size_t class_size, size_t order, size_t page_size)
        : Allocator(name)
        , allocator_(capacity, page_size)
        , free_lists_(order)
        , class_size_(class_size)
    {
        CheckPreconditions();
    }

    LinearSegregatedFitAllocator::LinearSegregatedFitAllocator(const HashedString& name, const MemoryRange& memory_range, size_t class_size, size_t order, size_t page_size)
        : Allocator(name)
        , allocator_(memory_range, page_size)
        , free_lists_(order)
        , class_size_(class_size)
    {
        CheckPreconditions();
    }

    void* LinearSegregatedFitAllocator::Allocate(size_t size)
    {
        SYNTROPY_ASSERT(size > 0);

        // Get a page from the list whose size is at least as big as the requested allocation size.

        auto list_index = GetListIndexBySize(size);
        auto page = free_lists_[list_index];

        if (!page)
        {
            page = AllocatePage(size);          // If no free page is found allocate a new one.
        }

        auto block = page->AllocateBlock();     // Allocate a new block inside the page.
        
        if (page->IsFull())
        {
            DiscardPage(list_index);            // If the page is full move to the next one.
        }

        return block;
    }

    void* LinearSegregatedFitAllocator::Allocate(size_t size, size_t alignment)
    {
        SYNTROPY_ASSERT(Math::IsPow2(alignment));

        // Since blocks are aligned to their own size, we are looking for a block large enough that is also a multiple of the requested alignment.
        auto block = Allocate(Math::Ceil(size, alignment));

        SYNTROPY_ASSERT(Memory::IsAlignedTo(block, alignment));

        return block;
    }

    void LinearSegregatedFitAllocator::Free(void* address)
    {
        auto page = reinterpret_cast<Page*>(Memory::AlignDown(address, GetPageSize()));     // Page the block belongs to

        auto is_full = page->IsFull();

        page->FreeBlock(address);               // Add the block to free block list

        if (page->IsEmpty())
        {
            FreePage(page);                     // If the page became empty return it to the allocator so it can be recycled by other lists.
        }
        else if (is_full)
        {
            RestorePage(page);                  // If the page was full insert it into the proper free list so that new blocks can be allocated inside it.
        }
    }

    bool LinearSegregatedFitAllocator::Belongs(void* block) const
    {
        return allocator_.GetRange().Contains(block);
    }

    size_t LinearSegregatedFitAllocator::GetMaxAllocationSize() const
    {
        return GetOrder() * class_size_;        // Size of the largest class the allocator can handle.
    }

    size_t LinearSegregatedFitAllocator::GetOrder() const
    {
        return free_lists_.size();          // One free list per order.
    }

    size_t LinearSegregatedFitAllocator::GetPageSize() const
    {
        return allocator_.GetBlockSize();
    }

    const MemoryRange& LinearSegregatedFitAllocator::GetRange() const
    {
        return allocator_.GetRange();
    }

    size_t LinearSegregatedFitAllocator::GetListIndexBySize(size_t size) const
    {
        return (size - 1u) / class_size_;
    }

    LinearSegregatedFitAllocator::Page* LinearSegregatedFitAllocator::AllocatePage(size_t block_size)
    {
        auto storage = reinterpret_cast<Page*>(allocator_.Allocate());          // Storage for a new page.

        block_size = Math::Ceil(block_size, class_size_);                       // The allocation granularity is determined by the class size.

        auto page = new (storage) Page(block_size, GetPageSize());              // Create a new page.

        auto list_index = GetListIndexBySize(block_size);

        free_lists_[list_index] = page;                                         // Add the page to the proper list

        return page;
    }

    void LinearSegregatedFitAllocator::DiscardPage(size_t list_index)
    {
        // Pop the page from the head of the free list. Note that any reference to that page is lost until one block inside is freed.

        auto page = free_lists_[list_index];

        page = page->next_;

        if (page)
        {
            page->previous_ = nullptr;
        }

        free_lists_[list_index] = page;
    }

    void LinearSegregatedFitAllocator::FreePage(Page* page)
    {
        // Fix the double-linked list pointers

        if (page->previous_)
        {
            page->previous_->next_ = page->next_;       // The page is in the middle of the segregated list.
        }
        else
        {
            auto list_index = GetListIndexBySize(page->block_size_);

            free_lists_[list_index] = page->next_;      // The page was the head of a free list.
        }

        if (page->next_)
        {
            page->next_->previous_ = page->previous_;   // The page is not the last in the segregated list.
        }

        allocator_.Free(page);                          // Return the page to the shared allocator.
    }

    void LinearSegregatedFitAllocator::RestorePage(Page* page)
    {
        // Push the page on top of the proper free list.

        auto list_index = GetListIndexBySize(page->block_size_);

        page->next_ = free_lists_[list_index];

        if (page->next_)
        {
            page->next_->previous_ = page;
        }

        free_lists_[list_index] = page;
    }

    void LinearSegregatedFitAllocator::CheckPreconditions() const
    {
        // Each allocated block must be large enough to fit an address since free blocks form a linked list.
        SYNTROPY_ASSERT(class_size_ >= kMinimumAllocationSize);

        // The class size must be a power of 2.
        SYNTROPY_ASSERT(Math::IsPow2(class_size_));

        // Each page must be large enough to contain the header and at least one block as big as the maximum allocation, aligned to its own size.
        SYNTROPY_ASSERT(GetPageSize() <= sizeof(Page) + GetMaxAllocationSize() + GetMaxAllocationSize() - 1);
    }

    /************************************************************************/
    /* EXPONENTIAL SEGREGATED FIT ALLOCATOR                                 */
    /************************************************************************/

    ExponentialSegregatedFitAllocator::ExponentialSegregatedFitAllocator(const HashedString& name, size_t capacity, size_t class_size, size_t order)
        : Allocator(name)
        , memory_pool_(capacity, Memory::CeilToPageSize(class_size))        // Allocate a new virtual address range.
        , memory_range_(memory_pool_)                                       // Get the full range out of the memory pool.
    {
        InitializeAllocators(order, Memory::CeilToPageSize(class_size));
    }

    ExponentialSegregatedFitAllocator::ExponentialSegregatedFitAllocator(const HashedString& name, const MemoryRange& memory_range, size_t class_size, size_t order)
        : Allocator(name)
        , memory_range_(memory_range, Memory::CeilToPageSize(class_size))   // Align the input memory range. Doesn't take ownership.
    {
        InitializeAllocators(order, Memory::CeilToPageSize(class_size));
    }

    void* ExponentialSegregatedFitAllocator::Allocate(size_t size)
    {
        return GetAllocatorBySize(size).Allocate(size);
    }

    void* ExponentialSegregatedFitAllocator::Allocate(size_t size, size_t alignment)
    {
        SYNTROPY_ASSERT(Math::IsPow2(alignment));

        auto block = Allocate(size);        // A memory page is always aligned to some power of 2 which is almost always bigger than any requested alignment.

        SYNTROPY_ASSERT(Memory::IsAlignedTo(block, alignment));

        return block;
    }

    void ExponentialSegregatedFitAllocator::Free(void* address)
    {
        SYNTROPY_ASSERT(memory_range_.Contains(address));

        // The entire memory range is divided evenly among all the allocators

        auto distance = Memory::GetDistance(*memory_range_, address);

        auto index = distance / GetAllocatorCapacity();

        allocators_[index].Free(address);
    }

    bool ExponentialSegregatedFitAllocator::Belongs(void* block) const
    {
        return memory_range_.Contains(block);
    }

    size_t ExponentialSegregatedFitAllocator::GetMaxAllocationSize() const
    {
        return allocators_.back().GetBlockSize();       // Size of the last allocation class (which is also the biggest).
    }

    void* ExponentialSegregatedFitAllocator::Reserve(size_t size)
    {
        return GetAllocatorBySize(size).Reserve();
    }

    void* ExponentialSegregatedFitAllocator::Reserve(size_t size, size_t alignment)
    {
        SYNTROPY_ASSERT(Math::IsPow2(alignment));

        auto block = Reserve(size);         // A memory page is always aligned to some power of 2 which is almost always bigger than any requested alignment.

        SYNTROPY_ASSERT(Memory::IsAlignedTo(block, alignment));

        return block;
    }

    size_t ExponentialSegregatedFitAllocator::GetOrder() const
    {
        return allocators_.size();
    }

    const MemoryRange& ExponentialSegregatedFitAllocator::GetRange() const
    {
        return memory_range_;
    }

    void ExponentialSegregatedFitAllocator::InitializeAllocators(size_t order, size_t class_size)
    {
        SYNTROPY_ASSERT(allocators_.empty());           // Check for multiple initializations.
        SYNTROPY_ASSERT(Math::IsPow2(class_size));      // Allocations classes must be a power of 2 (for alignment requirements and because memory pages size is always a power of 2).
        SYNTROPY_ASSERT(order >= 1);                    // At least one allocator is needed.

        allocators_.reserve(order);

        auto capacity = memory_range_.GetSize() / order;        // The entire memory range is divided evenly among the classes.

        for(size_t index = 0; index < order; ++index)
        {
            allocators_.emplace_back(
                MemoryRange(Memory::AddOffset(*memory_range_, index * capacity), capacity),         // Split the range into sub ranges.
                class_size << index);                                                               // Class size is doubled at each iteration.

            SYNTROPY_ASSERT(memory_range_.Contains(allocators_.back().GetRange()));
        }

    }

    BlockAllocator& ExponentialSegregatedFitAllocator::GetAllocatorBySize(size_t block_size)
    {
        SYNTROPY_ASSERT(block_size <= GetMaxAllocationSize());
        SYNTROPY_ASSERT(block_size >= 0u);

        auto offset = Math::FloorLog2(allocators_.front().GetBlockSize());

        auto index = Math::CeilLog2(block_size);

        return index >= offset ?
            allocators_[index - offset] :
            allocators_.front();                // Allocations smaller than the smallest allocation class are handled by that allocator.
    }

    size_t ExponentialSegregatedFitAllocator::GetAllocatorCapacity() const
    {
        return memory_range_.GetSize() / allocators_.size();
    }

    /************************************************************************/
    /* TWO LEVEL SEGREGATED FIT ALLOCATOR :: BLOCK HEADER                   */
    /************************************************************************/

    size_t TwoLevelSegregatedFitAllocator::BlockHeader::GetSize() const
    {
        return size_ & ~kSizeMask;
    }

    void TwoLevelSegregatedFitAllocator::BlockHeader::SetSize(size_t size)
    {
        SYNTROPY_ASSERT((size & kSizeMask) == 0)        // The new size must not interfere with the status bits at the end!.

        size_ = size | (size_ & kSizeMask);             // Preserve the status of the two flags at the end.
    }

    bool TwoLevelSegregatedFitAllocator::BlockHeader::IsBusy() const
    {
        return (size_ & kBusyBlockFlag) != 0;
    }

    void TwoLevelSegregatedFitAllocator::BlockHeader::SetBusy(bool is_busy)
    {
        if (is_busy)
        {
            size_ |= kBusyBlockFlag;
        }
        else
        {
            size_ &= ~kBusyBlockFlag;
        }
    }

    bool TwoLevelSegregatedFitAllocator::BlockHeader::IsLast() const
    {
        return (size_ & kLastBlockFlag) != 0;
    }

    void TwoLevelSegregatedFitAllocator::BlockHeader::SetLast(bool is_last)
    {
        if (is_last)
        {
            size_ |= kLastBlockFlag;
        }
        else
        {
            size_ &= ~kLastBlockFlag;
        }
    }

    void* TwoLevelSegregatedFitAllocator::BlockHeader::begin()
    {
        return Memory::AddOffset(this, sizeof(BlockHeader));
    }

    void* TwoLevelSegregatedFitAllocator::BlockHeader::end()
    {
        return Memory::AddOffset(this, GetSize());
    }

    /************************************************************************/
    /* TWO LEVEL SEGREGATED FIT ALLOCATOR :: FREE BLOCK HEADER              */
    /************************************************************************/

    void* TwoLevelSegregatedFitAllocator::FreeBlockHeader::begin()
    {
        return Memory::AddOffset(this, sizeof(FreeBlockHeader));
    }

    void* TwoLevelSegregatedFitAllocator::FreeBlockHeader::end()
    {
        return Memory::AddOffset(this, GetSize());
    }

    /************************************************************************/
    /* TWO LEVEL SEGREGATED FIT ALLOCATOR                                   */
    /************************************************************************/
    
    TwoLevelSegregatedFitAllocator::TwoLevelSegregatedFitAllocator(const HashedString& name, size_t capacity, size_t second_level_index)
        : Allocator(name)
        , allocator_(capacity, Memory::GetPageSize())
    {
        Initialize(second_level_index);
    }

    TwoLevelSegregatedFitAllocator::TwoLevelSegregatedFitAllocator(const HashedString& name, const MemoryRange& memory_range, size_t second_level_index)
        : Allocator(name)
        , allocator_(memory_range, Memory::GetPageSize())
    {
        Initialize(second_level_index);
    }

    void* TwoLevelSegregatedFitAllocator::Allocate(size_t size)
    {
        return GetFreeBlockBySize(size)->begin();
    }

    void* TwoLevelSegregatedFitAllocator::Allocate(size_t size, size_t alignment)
    {
        SYNTROPY_ASSERT(false);     // Not yet implemented

        return GetFreeBlockBySize(size + alignment)->begin();
    }

    void TwoLevelSegregatedFitAllocator::Free(void* block)
    {
        auto free_block = reinterpret_cast<BlockHeader*>(Memory::SubOffset(block, sizeof(BlockHeader)));

        PushBlock(free_block);
    }

    bool TwoLevelSegregatedFitAllocator::Belongs(void* block) const
    {
        return GetRange().Contains(block);
    }

    size_t TwoLevelSegregatedFitAllocator::GetMaxAllocationSize() const
    {
        return GetRange().GetSize();
    }

    const MemoryRange& TwoLevelSegregatedFitAllocator::GetRange() const
    {
        return allocator_.GetRange();
    }

    void TwoLevelSegregatedFitAllocator::Initialize(size_t second_level_count)
    {
        last_block_ = nullptr;

        first_level_count_ = Math::FloorLog2(allocator_.GetRange().GetSize()) + 1u;
        second_level_count_ = second_level_count;

        free_lists_.resize(first_level_count_ * (static_cast<size_t>(1u) << second_level_count_));

        // sizeof(...) * 8u => size in bits
        SYNTROPY_ASSERT(sizeof(first_level_bitmap_) * 8u >= first_level_count_);
        SYNTROPY_ASSERT(sizeof(decltype(second_level_bitmap_)::value_type) * 8u >= (static_cast<size_t>(1u) << second_level_count_));

        // Bitmaps are zero initialized
        first_level_bitmap_ = 0;
        second_level_bitmap_.resize(first_level_count_);
    }

    TwoLevelSegregatedFitAllocator::BlockHeader* TwoLevelSegregatedFitAllocator::GetFreeBlockBySize(size_t size)
    {
        SYNTROPY_ASSERT(size >= 0);

        size += sizeof(BlockHeader);                            // Reserve space for the header.
        size = std::max(size, kMinimumBlockSize);               // The size must be at least as big as the minimum size allowed.
        size = Math::Ceil(size, BlockHeader::kSizeMask + 1);    // The size must not interfere with the status bits of the block.

        size_t first_level_index;
        size_t second_level_index;

        GetFreeListIndex(size, first_level_index, second_level_index);

        auto index = GetFreeListIndex(first_level_index, second_level_index);

        if (!free_lists_[index])
        {
            // Check whether there's a bigger free block in the same first-level
            auto filtered_bitmap = second_level_bitmap_[first_level_index] & ~((static_cast<uint64_t>(1u) << second_level_index) - 1u);        // Mask out all the second-level classes smaller than the requested size.

            if (filtered_bitmap != 0)
            {
                second_level_index = platform::GetBuiltIn().GetLeastSignificantBit(static_cast<uint64_t>(filtered_bitmap));

                index = GetFreeListIndex(first_level_index, second_level_index);
            }
            else
            {
                // Check the subsequent first-level classes.
                filtered_bitmap = first_level_bitmap_ & ~((static_cast<uint64_t>(2u) << first_level_index) - 1u);                               // Mask out all the first-level classes smaller than the requested size.

                if (filtered_bitmap != 0)
                {
                    first_level_index = platform::GetBuiltIn().GetLeastSignificantBit(static_cast<uint64_t>(filtered_bitmap));
                    second_level_index = platform::GetBuiltIn().GetLeastSignificantBit(static_cast<uint64_t>(second_level_bitmap_[first_level_index]));

                    index = GetFreeListIndex(first_level_index, second_level_index);
                }
                else
                {
                    // No free list found
                    index = free_lists_.size();
                }
            }
        }

        BlockHeader* block;

        if (index < free_lists_.size())
        {
            SYNTROPY_ASSERT(!!free_lists_[index]);

            // Pop a free block from the given segregated free list.
            block = PopBlock(index);

            // Split the block (if needed).
            SplitBlock(block, size);
        }
        else
        {
            // Get a new block from the pool
            block = AllocateBlock(size);
        }

        MemoryDebug::MarkUninitialized(block->begin(), block->end());

        return block;
    }

    void TwoLevelSegregatedFitAllocator::SetBitmap(size_t first_level_index, size_t second_level_index)
    {
        first_level_bitmap_ |= (static_cast<size_t>(1u) << first_level_index);
        second_level_bitmap_[first_level_index] |= (static_cast<size_t>(1u) << second_level_index);
    }

    void TwoLevelSegregatedFitAllocator::ResetBitmap(size_t first_level_index, size_t second_level_index)
    {
        second_level_bitmap_[first_level_index] &= ~(static_cast<size_t>(1u) << second_level_index);

        // If all the second-level classes in the current first-level class are empty, the first-level is marked as 'empty' as well.
        if (second_level_bitmap_[first_level_index] == 0)
        {
            first_level_bitmap_ &= ~(static_cast<size_t>(1u) << first_level_index);
        }
    }

    TwoLevelSegregatedFitAllocator::BlockHeader* TwoLevelSegregatedFitAllocator::PopBlock(size_t index)
    {
        auto block = free_lists_[index];

        if (block)
        {
            SYNTROPY_ASSERT(!block->IsBusy());

            // Promote the next free block as head of the list
            auto next_free = block->next_free_;

            free_lists_[index] = next_free;

            if (next_free)
            {
                next_free->previous_free_ = nullptr;
            }

            // Fix-up the bitmaps
            if (!free_lists_[index])
            {
                auto first_level_index = index / (static_cast<size_t>(1u) << second_level_count_);
                auto second_level_index = index % (static_cast<size_t>(1u) << second_level_count_);

                ResetBitmap(first_level_index, second_level_index);
            }

            // Mark the popped block as "busy"
            block->SetBusy(true);
        }

        return reinterpret_cast<BlockHeader*>(block);
    }

    void TwoLevelSegregatedFitAllocator::PushBlock(BlockHeader* block)
    {
        auto merged_block = reinterpret_cast<FreeBlockHeader*>(block);

        auto previous_block = reinterpret_cast<FreeBlockHeader*>(block->previous_);
        auto next_block = reinterpret_cast<FreeBlockHeader*>(block->end());

        // Merge the block with the previous physical block

        if (previous_block && !previous_block->IsBusy())
        {
            RemoveBlock(previous_block);                                                // Remove the previous block from its segregated list.

            previous_block->SetSize(previous_block->GetSize() + block->GetSize());      // Grow the block size.
            previous_block->SetLast(block->IsLast());                                   // Merging with the 'last' block yields another 'last' block.

            merged_block = previous_block;                                              // The previous block and the original one are now merged
        }

        // Merge the block with the next physical block

        if (!merged_block->IsLast() && !next_block->IsBusy())
        {
            RemoveBlock(next_block);                                                    // Remove the next block from its segregated list.

            merged_block->SetSize(merged_block->GetSize() + next_block->GetSize());     // Grow the block size.
            merged_block->SetLast(merged_block->IsLast());                              // Merging with the 'last' block yields another 'last' block.
        }

        if (merged_block->IsLast())
        {
            last_block_ = merged_block;                                                 // Update the pointer to last block (The previous last block was merged into the new block).
        }
        else
        {
            next_block = reinterpret_cast<FreeBlockHeader*>(merged_block->end());
            next_block->previous_ = merged_block;                                       // The pointer to the previous physical block of the next block is no longer valid since that block may have been merged.
        }

        // Ensures that the block cannot be merged any further

        SYNTROPY_ASSERT(!merged_block->previous_ || merged_block->previous_->IsBusy());
        SYNTROPY_ASSERT(merged_block->IsLast() || reinterpret_cast<BlockHeader*>(merged_block->end())->IsBusy());

        // Insert the merged block in the proper free list

        merged_block->SetBusy(false);                                                   // The block is no longer busy

        // TODO: If the merged block is the last one, returns it to the allocator.

        MemoryDebug::MarkFree(merged_block->begin(), merged_block->end());

        InsertBlock(merged_block);
    }

    TwoLevelSegregatedFitAllocator::BlockHeader* TwoLevelSegregatedFitAllocator::AllocateBlock(size_t size)
    {
        auto block = reinterpret_cast<BlockHeader*>(allocator_.Allocate(size));

        block->SetSize(size);
        block->SetBusy(true);
        block->SetLast(true);

        block->previous_ = last_block_;

        if (last_block_)
        {
            last_block_->SetLast(false);            // The last block is no longer the last
        }

        last_block_ = block;

        return block;
    }

    void TwoLevelSegregatedFitAllocator::SplitBlock(BlockHeader* block, size_t size)
    {
        SYNTROPY_ASSERT(block->IsBusy());                               // Make sure the original block is busy, otherwise PushBlock will merge again block and remaining_block!

        if (block->GetSize() - size >= kMinimumBlockSize)               // Do not split if the remaining block size would fall below the minimum size allowed.
        {
            auto remaining_block = Memory::AddOffset(block, size);

            // Setup the new block
            remaining_block->previous_ = block;                         // Previous physical block
            remaining_block->SetBusy(false);                            // This block is free
            remaining_block->SetLast(block->IsLast());                  // This block is the last only if the original one was the last
            remaining_block->SetSize(block->GetSize() - size);          // Remaining size

            // Update the block info
            block->SetSize(size);                                       // Shrink the size
            block->SetLast(false);                                      // It can't be the last block anymore

            PushBlock(remaining_block);                                 // Store the remaining block for further allocations
        }
    }

    void TwoLevelSegregatedFitAllocator::RemoveBlock(FreeBlockHeader* block)
    {
        // Fix-up the double linked list

        if (block->next_free_)
        {
            block->next_free_->previous_free_ = block->previous_free_;
        }

        if (block->previous_free_)
        {
            block->previous_free_->next_free_ = block->next_free_;
        }
        else
        {
            // The block has no previous block: fix the head of the free list
            size_t first_level_index;
            size_t second_level_index;

            GetFreeListIndex(block->GetSize(), first_level_index, second_level_index);

            auto index = GetFreeListIndex(first_level_index, second_level_index);

            free_lists_[index] = block->next_free_;

            if (!free_lists_[index])
            {
                ResetBitmap(first_level_index, second_level_index);
            }
        }
    }

    void TwoLevelSegregatedFitAllocator::InsertBlock(FreeBlockHeader* block)
    {
        // Push on the head of the proper segregated list
        size_t first_level_index;
        size_t second_level_index;

        GetFreeListIndex(block->GetSize(), first_level_index, second_level_index);

        SetBitmap(first_level_index, second_level_index);

        auto index = GetFreeListIndex(first_level_index, second_level_index);

        block->previous_free_ = nullptr;
        block->next_free_ = free_lists_[index];

        if (block->next_free_)
        {
            block->next_free_->previous_free_ = block;
        }

        free_lists_[index] = block;
    }

    void TwoLevelSegregatedFitAllocator::GetFreeListIndex(size_t size, size_t& first_level_index, size_t& second_level_index) const
    {
        first_level_index = Math::FloorLog2(size);

        second_level_index = (size ^ (static_cast<size_t>(1u) << first_level_index)) >> (first_level_index - second_level_count_);

        SYNTROPY_ASSERT(second_level_index < (static_cast<size_t>(1u) << second_level_count_));
    }

    size_t TwoLevelSegregatedFitAllocator::GetFreeListIndex(size_t first_level_index, size_t second_level_index) const
    {
        return first_level_index * (static_cast<size_t>(1u) << second_level_count_) + second_level_index;
    }

}
