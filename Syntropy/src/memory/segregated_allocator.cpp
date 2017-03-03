#include "memory/segregated_allocator.h"

#include <iterator>
#include <algorithm>
#include <new>

#include "memory/memory.h"
#include "diagnostics/debug.h"
#include "math/math.h"

namespace syntropy 
{

    /************************************************************************/
    /* TINY SEGREGATED FIT ALLOCATOR                                        */
    /************************************************************************/

    TinySegregatedFitAllocator::TinySegregatedFitAllocator(const HashedString& name, size_t capacity, size_t page_size, size_t order)
        : Allocator(name)
        , allocator_(capacity, page_size)                               // Allocator for the pages.
        , maximum_block_size_(kMinimumAllocationSize * order)           // Round to the next minimum allocation boundary.
        , free_pages_(order)                                            // Total amount of segregated lists.
    {
        while (order-- > 0)
        {
            free_pages_.PushBack(nullptr);
        }
    }

    void* TinySegregatedFitAllocator::Allocate(size_t size)
    {
        SYNTROPY_ASSERT(size <= maximum_block_size_);
        SYNTROPY_ASSERT(size > 0);

        // Get a page from the list whose size is at least as big as the requested allocation size.
        auto block_size = Math::Ceil(size, kMinimumAllocationSize);

        auto& page = free_pages_[(block_size - 1) / kMinimumAllocationSize];

        if (!page)
        {
            // Allocate a new page if no free page is present in the segregated list.
            page = AllocatePage(block_size);
        }

        // Get a block from the free block list
        auto block = page->free_;
        page->free_ = block->next_;
        ++page->allocated_blocks_;

        if (page->free_ == nullptr)         // The page is full
        {
            DiscardPage(page);              // Discard the current page from the allocator and move to the next one.
        }

        return block;
    }

    void* TinySegregatedFitAllocator::Allocate(size_t size, size_t alignment)
    {
        SYNTROPY_ASSERT(alignment % kMinimumAllocationSize == 0);       // Must be a multiple of the minimum allocation size

        // Since blocks are aligned to their own size, we are looking for a block large enough that is also a multiple of the requested alignment
        auto block = Allocate(Math::Ceil(size, alignment));

        SYNTROPY_ASSERT(Memory::IsAlignedTo(block, alignment));

        return block;
    }

    void TinySegregatedFitAllocator::Free(void* address)
    {
        auto block = reinterpret_cast<Block*>(address);

        auto page = reinterpret_cast<Page*>(Memory::AlignDown(address, allocator_.GetBlockSize()));     // Page the block belongs to

        // Add the block to free block list
        --page->allocated_blocks_;
        block->next_ = page->free_;
        page->free_ = block;

        if (page->allocated_blocks_ == 0)       // No allocated blocks.
        {
            FreePage(page);                     // Return the page to the allocator so it can be recycled by any segregated list.
        }
        else if (!page->free_->next_)           // The page was full and now has exactly one free block.
        {
            RestorePage(page);                  // Return the page to the segregated free list so new allocations can be performed inside it.
        }
    }

    bool TinySegregatedFitAllocator::Belongs(void* block) const
    {
        return allocator_.ContainsAddress(block);
    }

    size_t TinySegregatedFitAllocator::GetMaxAllocationSize() const
    {
        return maximum_block_size_;
    }

    size_t TinySegregatedFitAllocator::GetSize() const
    {
        Block* head;
        size_t count;

        auto size = allocator_.GetSize();

        // Remove the memory of free blocks inside each segregated free list
        for (auto free_page : free_pages_)
        {
            while (free_page)
            {
                count = GetBlockCount(free_page, free_page->block_size_, head);
                size -= free_page->block_size_ * (count - free_page->allocated_blocks_);
                free_page = free_page->next_;
            }
        }

        return size;
    }

    size_t TinySegregatedFitAllocator::GetEffectiveSize() const
    {
        return allocator_.GetEffectiveSize();
    }

    size_t TinySegregatedFitAllocator::GetCapacity() const
    {
        return allocator_.GetCapacity();
    }

    TinySegregatedFitAllocator::Page* TinySegregatedFitAllocator::AllocatePage(size_t block_size)
    {
        auto page = reinterpret_cast<Page*>(allocator_.Allocate());             // Get a new page

        Block* head;
        auto count = GetBlockCount(page, block_size, head);

        SYNTROPY_ASSERT(Memory::IsAlignedTo(head, block_size));                 // Blocks must be aligned to their size

        page->next_ = nullptr;
        page->previous_ = nullptr;
        page->free_ = head;
        page->block_size_ = block_size;
        page->allocated_blocks_ = 0;

        // Fill the free-block list
        while (count-- > 1)
        {
            head->next_ = Memory::Offset(head, block_size);
            head = head->next_;
        }

        head->next_ = nullptr;

        return page;
    }

    void TinySegregatedFitAllocator::FreePage(Page* page)
    {
        // Fix the double-linked list pointers
        if (page->previous_)
        {
            page->previous_->next_ = page->next_;   // The page is in the middle of the segregated list.
        }
        else
        {
            free_pages_[(page->block_size_ - 1) / kMinimumAllocationSize] = page->next_;    // The page is the head of the segregated list.
        }

        if (page->next_)
        {
            page->next_->previous_ = page->previous_;   // The page is not the last in the segregated list.
        }

        // Send the page to the allocator
        allocator_.Free(page);
    }

    void TinySegregatedFitAllocator::DiscardPage(Page*& page)
    {
        page = page->next_;

        if (page)
        {
            page->previous_ = nullptr;
        }
    }

    void TinySegregatedFitAllocator::RestorePage(Page* page)
    {
        // The page becomes the head of the segregated list
        auto& head = free_pages_[(page->block_size_ - 1) / kMinimumAllocationSize];

        page->next_ = head;

        if (head)
        {
            head->previous_ = page;
        }

        head = page;
    }

    size_t TinySegregatedFitAllocator::GetBlockCount(Page* page, size_t block_size, Block*& head) const
    {
        auto page_address = reinterpret_cast<size_t>(page);

        auto header_size = Math::Ceil(page_address + sizeof(Page), block_size) - page_address;      // The header is padded such that blocks in the page are aligned to their own size
        head = reinterpret_cast<Block*>(Memory::Offset(page, header_size));                         // The first available block is just after the page header
        return (allocator_.GetBlockSize() - header_size) / block_size;                              // Amount of available blocks in the page
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
        return Memory::Offset(this, sizeof(BlockHeader));
    }

    void* TwoLevelSegregatedFitAllocator::BlockHeader::end()
    {
        return Memory::Offset(this, GetSize());
    }

    /************************************************************************/
    /* TWO LEVEL SEGREGATED FIT ALLOCATOR :: FREE BLOCK HEADER              */
    /************************************************************************/

    void* TwoLevelSegregatedFitAllocator::FreeBlockHeader::begin()
    {
        return Memory::Offset(this, sizeof(FreeBlockHeader));
    }

    void* TwoLevelSegregatedFitAllocator::FreeBlockHeader::end()
    {
        return Memory::Offset(this, GetSize());
    }

    /************************************************************************/
    /* TWO LEVEL SEGREGATED FIT ALLOCATOR                                   */
    /************************************************************************/
    
    TwoLevelSegregatedFitAllocator::TwoLevelSegregatedFitAllocator(const HashedString& name, size_t capacity, size_t second_level_index)
        : Allocator(name)
        , pool_(capacity, 1)
        , last_block_(nullptr)
        , second_level_index_(second_level_index)
        , free_lists_((Math::FloorLog2(capacity) + 1ull) * (1ull << second_level_index_))
    {

        // Initialize the free lists.
        auto count = free_lists_.GetMaxCount();

        while (count-- > 0)
        {
            free_lists_.PushBack(nullptr);
        }
    }

    void* TwoLevelSegregatedFitAllocator::Allocate(size_t size)
    {
        return GetFreeBlockBySize(size)->begin();
    }

    void* TwoLevelSegregatedFitAllocator::Allocate(size_t size, size_t alignment)
    {
        return GetFreeBlockBySize(size + alignment)->begin();
    }

    void TwoLevelSegregatedFitAllocator::Free(void* block)
    {
        auto free_block = reinterpret_cast<BlockHeader*>(Memory::Offset(block, -static_cast<int64_t>(sizeof(BlockHeader))));

        PushBlock(free_block);
    }

    bool TwoLevelSegregatedFitAllocator::Belongs(void* block) const
    {
        return pool_.ContainsAddress(block);
    }

    size_t TwoLevelSegregatedFitAllocator::GetMaxAllocationSize() const
    {
        return pool_.GetCapacity();
    }

    TwoLevelSegregatedFitAllocator::BlockHeader* TwoLevelSegregatedFitAllocator::GetFreeBlockBySize(size_t size)
    {
        size += sizeof(BlockHeader);                            // Reserve more space for the header.
        size = std::max(size, kMinimumBlockSize);               // The size must be at least as big as the minimum size allowed.
        size = Math::Ceil(size, BlockHeader::kSizeMask + 1);    // The size must not interfere with the status bits of the block.

        auto index = GetFreeListIndexBySize(size);

        // Search a free block big enough to handle the allocation
        while (index < free_lists_.GetMaxCount() && !free_lists_[index])
        {
            ++index;
        }

        BlockHeader* block;

        if (index < free_lists_.GetMaxCount())
        {
            // Pop a free block from the given segregated free list.
            block = PopBlock(index);

            SplitBlock(block, size);                // Split the block (if needed).
        }
        else
        {
            // Get a new block from the pool
            block = reinterpret_cast<BlockHeader*>(pool_.Allocate(size));

            block->SetSize(size);
            block->SetBusy(true);
            block->SetLast(true);
            block->previous_ = last_block_;

            if (last_block_)
            {
                last_block_->SetLast(false);            // The last block is no longer the last
            }

            last_block_ = block;
        }

        // Fill the payload with some recognizable pattern (uninitialized memory).
        SYNTROPY_DEBUG_ONLY
        (
            std::fill(
                reinterpret_cast<int8_t*>(block->begin()),
                reinterpret_cast<int8_t*>(block->end()),
                kUninitializedMemoryPattern);
        );

        return block;
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

        if (!block->IsLast() && !next_block->IsBusy())
        {
            RemoveBlock(next_block);                                                    // Remove the next block from its segregated list.

            block->SetSize(block->GetSize() + next_block->GetSize());                   // Grow the block size.
            block->SetLast(next_block->IsLast());                                       // Merging with the 'last' block yields another 'last' block.
        }

        if (merged_block->IsLast())
        {
            last_block_ = merged_block;                                                 // Update the pointer to last block.
        }
        else
        {
            next_block = reinterpret_cast<FreeBlockHeader*>(merged_block->end());
            next_block->previous_ = merged_block;                                       // The pointer to the previous physical block of the next block is no longer valid since that block may have been merged.
        }

        // Insert the merged block in the proper free list

        merged_block->SetBusy(false);                                                   // The block is no longer busy

        // Fill the payload with some recognizable pattern (free memory)
        SYNTROPY_DEBUG_ONLY
        (
            std::fill(
                reinterpret_cast<int8_t*>(merged_block->begin()),
                reinterpret_cast<int8_t*>(merged_block->end()),
                kFreeMemoryPattern);
        );

        InsertBlock(merged_block);
    }

    void TwoLevelSegregatedFitAllocator::SplitBlock(BlockHeader* block, size_t size)
    {
        if (block->GetSize() > size + kMinimumBlockSize)                // Do not split if the remaining block size would fall below the minimum size allowed.
        {
            auto remaining_block = Memory::Offset(block, size);

            // Setup the new block
            remaining_block->previous_ = block;                         // Previous physical block
            remaining_block->SetBusy(false);                            // This block is free
            remaining_block->SetLast(block->IsLast());                  // This block is the last only if the original one was the last
            remaining_block->SetSize(block->GetSize() - size);          // Remaining size

            // Update the block info
            block->SetSize(size);                                       // Shrink the size
            block->SetLast(false);                                      // It can't be the last block anymore

            SYNTROPY_ASSERT(block->IsBusy());                           // Make sure the original block is busy, otherwise PushBlock will merge again block and remaining_block!

            PushBlock(remaining_block);
        }
    }

    void TwoLevelSegregatedFitAllocator::RemoveBlock(FreeBlockHeader* block)
    {
        // Fix-up the double linked list

        if (block->previous_free_)
        {
            block->previous_free_->next_free_ = block->next_free_;
        }
        else
        {
            // The block has no previous block: fix the head of the free list
            auto index = GetFreeListIndexBySize(block->GetSize());

            free_lists_[index] = block->next_free_;
        }

        if (block->next_free_)
        {
            block->next_free_->previous_free_ = block->previous_free_;
        }
    }

    void TwoLevelSegregatedFitAllocator::InsertBlock(FreeBlockHeader* block)
    {
        // Push on the head of the proper segregated list
        auto index = GetFreeListIndexBySize(block->GetSize());

        block->previous_free_ = nullptr;
        block->next_free_ = free_lists_[index];
        free_lists_[index] = block;
    }

    size_t TwoLevelSegregatedFitAllocator::GetFreeListIndexBySize(size_t size) const
    {
        auto first_level = Math::FloorLog2(size);

        auto second_level = (size ^ (1ull << first_level)) >> (first_level - second_level_index_);

        SYNTROPY_ASSERT(second_level < (1ull << second_level_index_));

        return first_level * (1ull << second_level_index_) + second_level;          // Flatten the index.
    }

    /************************************************************************/
    /* EXPONENTIAL SEGREGATED FIT ALLOCATOR                                 */
    /************************************************************************/

    ExponentialSegregatedFitAllocator::ExponentialSegregatedFitAllocator(const HashedString& name, size_t capacity, size_t base_allocation_size, size_t order)
        : Allocator(name)
        , allocators_(order)
        , base_allocation_size_(Memory::CeilToPageSize(base_allocation_size))           // Round to the next memory page boundary.
    {
        SYNTROPY_ASSERT(order >= 1);
        
        capacity /= order;                                                              // Distribute the capacity evenly among the different classes.

        auto class_size = base_allocation_size_;

        while (order-- > 0)
        {
            allocators_.EmplaceBack(capacity, class_size);
            class_size <<= 1;                                                           // Double the allocation size for the next class.
        }
    }

    void* ExponentialSegregatedFitAllocator::Allocate(size_t size)
    {
        return GetAllocatorBySize(size).Allocate(size);
    }

    void* ExponentialSegregatedFitAllocator::Allocate(size_t size, size_t alignment)
    {
        // Allocate enough space for both the block and the maximum padding due to the requested alignment
        size += alignment - 1;

        auto block = Memory::Align(GetAllocatorBySize(size).Allocate(size), alignment);
        
        SYNTROPY_ASSERT(reinterpret_cast<size_t>(block) % alignment == 0);

        return block;
    }

    void ExponentialSegregatedFitAllocator::Free(void* address)
    {
        auto it = std::find_if(allocators_.begin(),
                               allocators_.end(),
                               [address](const BlockAllocator& allocator)
                               {
                                    return allocator.ContainsAddress(address);
                               });

        SYNTROPY_ASSERT(it != allocators_.end());

        it->Free(address);
    }

    bool ExponentialSegregatedFitAllocator::Belongs(void* block) const
    {
        return std::any_of(
            allocators_.begin(), 
            allocators_.end(),
            [block](const BlockAllocator& allocator)
            {
                return allocator.ContainsAddress(block);
            });
    }

    size_t ExponentialSegregatedFitAllocator::GetMaxAllocationSize() const
    {
        return base_allocation_size_ * (1ull << (allocators_.GetSize() - 1ull));      // base * 2^(order-1)
    }

    void* ExponentialSegregatedFitAllocator::Reserve(size_t size)
    {
        return GetAllocatorBySize(size).Reserve(size);
    }

    void* ExponentialSegregatedFitAllocator::Reserve(size_t size, size_t alignment)
    {
        // Allocate enough space for both the block and the maximum padding due to the requested alignment
        size += alignment - 1;

        auto block = Memory::Align(GetAllocatorBySize(size).Reserve(size), alignment);

        SYNTROPY_ASSERT(reinterpret_cast<size_t>(block) % alignment == 0);

        return block;
    }

    size_t ExponentialSegregatedFitAllocator::GetUpperBoundSize() const
    {
        return std::accumulate(allocators_.begin(),
                               allocators_.end(),
                               static_cast<size_t>(0),
                               [](size_t accumulator, const BlockAllocator& allocator)
                               {
                                    return accumulator + allocator.GetUpperBoundSize();
                               });
    }

    size_t ExponentialSegregatedFitAllocator::GetCapacity() const
    {
        return std::accumulate(allocators_.begin(),
                               allocators_.end(),
                               static_cast<size_t>(0),
                               [](size_t accumulator, const BlockAllocator& allocator)
                               {
                                    return accumulator + allocator.GetCapacity();
                               });
    }

    BlockAllocator& ExponentialSegregatedFitAllocator::GetAllocatorBySize(size_t block_size)
    {
        auto index = Math::CeilLog2((block_size + base_allocation_size_ - 1) / base_allocation_size_);

        SYNTROPY_ASSERT(index < allocators_.GetSize());

        return allocators_[index];
    }

}
