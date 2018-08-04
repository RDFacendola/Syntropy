#include "memory/allocators/segregated_allocator.h"

#include <iterator>
#include <algorithm>
#include <new>

#include "memory/memory.h"
#include "memory/virtual_memory.h"

#include "diagnostics/assert.h"

#include "math/math.h"

#include "platform/builtin.h"

namespace syntropy 
{

    /************************************************************************/
    /* LINEAR SEGREGATED FIT ALLOCATOR :: PAGE                              */
    /************************************************************************/

    LinearSegregatedFitAllocator::Page::Page(Bytes block_size, Bytes page_size)
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
        return reinterpret_cast<Block*>(Memory::Align(Memory::AddOffset(this, Bytes(sizeof(Page))), block_size_));
    }

    LinearSegregatedFitAllocator::Block* LinearSegregatedFitAllocator::Page::GetLastBlock(Bytes page_size)
    {
        // Ensures that there's enough space for a block and the required padding.

        auto size = page_size - (block_size_ * 2u) - 1_Bytes;

        return reinterpret_cast<Block*>(Memory::Align(Memory::AddOffset(this, size), block_size_));
    }

    /************************************************************************/
    /* LINEAR SEGREGATED FIT ALLOCATOR                                      */
    /************************************************************************/

    const Bytes LinearSegregatedFitAllocator::kMinimumAllocationSize = Bytes(sizeof(uintptr_t));

    LinearSegregatedFitAllocator::LinearSegregatedFitAllocator(const HashedString& name, Bytes capacity, Bytes class_size, size_t order, Bytes page_size)
        : Allocator(name)
        , allocator_(capacity, page_size)
        , free_lists_(order)
        , class_size_(class_size)
    {
        CheckPreconditions();
    }

    LinearSegregatedFitAllocator::LinearSegregatedFitAllocator(const HashedString& name, const MemoryRange& memory_range, Bytes class_size, size_t order, Bytes page_size)
        : Allocator(name)
        , allocator_(memory_range, page_size)
        , free_lists_(order)
        , class_size_(class_size)
    {
        CheckPreconditions();
    }

    LinearSegregatedFitAllocator::LinearSegregatedFitAllocator(LinearSegregatedFitAllocator&& other)
        : Allocator(std::move(other))
        , allocator_(std::move(other.allocator_))
        , free_lists_(std::move(other.free_lists_))
        , class_size_(other.class_size_)
    {

    }

    void* LinearSegregatedFitAllocator::Allocate(Bytes size)
    {
        SYNTROPY_PRECONDITION(size > 0_Bytes);

        std::lock_guard<std::mutex> lock(mutex_);

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

    void* LinearSegregatedFitAllocator::Allocate(Bytes size, Bytes alignment)
    {
        SYNTROPY_PRECONDITION(Math::IsPow2(alignment));

        // Since blocks are aligned to their own size, we are looking for a block large enough that is also a multiple of the requested alignment.

        auto block = Allocate(Math::Ceil(size, alignment));

        SYNTROPY_ASSERT(Memory::IsAlignedTo(block, alignment));

        return block;
    }

    void LinearSegregatedFitAllocator::Free(void* address)
    {
        std::lock_guard<std::mutex> lock(mutex_);

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

    bool LinearSegregatedFitAllocator::Owns(void* block) const
    {
        return allocator_.GetRange().Contains(block);
    }

    Bytes LinearSegregatedFitAllocator::GetMaxAllocationSize() const
    {
        return GetOrder() * class_size_;                // Size of the largest class the allocator can handle.
    }

    size_t LinearSegregatedFitAllocator::GetOrder() const
    {
        return free_lists_.size();          // One free list per order.
    }

    Bytes LinearSegregatedFitAllocator::GetPageSize() const
    {
        return allocator_.GetBlockSize();
    }

    const MemoryRange& LinearSegregatedFitAllocator::GetRange() const
    {
        return allocator_.GetRange();
    }

    size_t LinearSegregatedFitAllocator::GetListIndexBySize(Bytes size) const
    {
        return (size - 1_Bytes) / class_size_;
    }

    LinearSegregatedFitAllocator::Page* LinearSegregatedFitAllocator::AllocatePage(Bytes block_size)
    {
        auto storage = reinterpret_cast<Page*>(allocator_.Allocate());                  // Storage for a new page.

        block_size = Math::Ceil(block_size, class_size_);                               // The allocation granularity is determined by the class size.

        auto page = new (storage) Page(block_size, GetPageSize());                      // Create a new page.

        auto list_index = GetListIndexBySize(block_size);

        free_lists_[list_index] = page;                                                 // Add the page to the proper list

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
        SYNTROPY_ASSERT(GetPageSize() >= Bytes(sizeof(Page)) + GetMaxAllocationSize() + GetMaxAllocationSize() - 1_Bytes);
    }

    /************************************************************************/
    /* EXPONENTIAL SEGREGATED FIT ALLOCATOR                                 */
    /************************************************************************/

    ExponentialSegregatedFitAllocator::ExponentialSegregatedFitAllocator(const HashedString& name, Bytes capacity, Bytes class_size, size_t order)
        : Allocator(name)
        , memory_pool_(capacity, VirtualMemory::CeilToPageSize(class_size))         // Allocate a new virtual address range.
        , memory_range_(memory_pool_)                                               // Get the full range out of the memory pool.
    {
        InitializeAllocators(order, VirtualMemory::CeilToPageSize(class_size));
    }

    ExponentialSegregatedFitAllocator::ExponentialSegregatedFitAllocator(const HashedString& name, const MemoryRange& memory_range, Bytes class_size, size_t order)
        : Allocator(name)
        , memory_range_(memory_range, VirtualMemory::CeilToPageSize(class_size))    // Align the input memory range. Doesn't take ownership.
    {
        InitializeAllocators(order, VirtualMemory::CeilToPageSize(class_size));
    }

    ExponentialSegregatedFitAllocator::ExponentialSegregatedFitAllocator(ExponentialSegregatedFitAllocator&& other)
        : Allocator(std::move(other))
        , memory_pool_(std::move(other.memory_pool_))
        , memory_range_(std::move(other.memory_range_))
        , allocators_(std::move(other.allocators_))
    {

    }

    void* ExponentialSegregatedFitAllocator::Allocate(Bytes size)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        return GetAllocatorBySize(size).Allocate(size);
    }

    void* ExponentialSegregatedFitAllocator::Allocate(Bytes size, Bytes alignment)
    {
        SYNTROPY_PRECONDITION(Math::IsPow2(alignment));

        auto block = Allocate(size);        // A memory page is always aligned to some power of 2 which is almost always bigger than any requested alignment.

        SYNTROPY_ASSERT(Memory::IsAlignedTo(block, alignment));

        return block;
    }

    void ExponentialSegregatedFitAllocator::Free(void* address)
    {
        SYNTROPY_PRECONDITION(memory_range_.Contains(address));

        std::lock_guard<std::mutex> lock(mutex_);

        // The entire memory range is divided evenly among all the allocators

        auto distance = Memory::GetDistance(*memory_range_, address);

        auto index = Bytes(distance) / GetAllocatorCapacity();

        allocators_[index].Free(address);
    }

    bool ExponentialSegregatedFitAllocator::Owns(void* block) const
    {
        return memory_range_.Contains(block);
    }

    Bytes ExponentialSegregatedFitAllocator::GetMaxAllocationSize() const
    {
        return allocators_.back().GetBlockSize();       // Size of the last allocation class (which is also the biggest).
    }

    void* ExponentialSegregatedFitAllocator::Reserve(Bytes size)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        return GetAllocatorBySize(size).Reserve();
    }

    void* ExponentialSegregatedFitAllocator::Reserve(Bytes size, Bytes alignment)
    {
        SYNTROPY_PRECONDITION(Math::IsPow2(alignment));

        auto block = Reserve(size);         // A memory page is always aligned to some power of 2 which is almost always bigger than any requested alignment.

        SYNTROPY_ASSERT(Memory::IsAlignedTo(block, alignment));

        return block;
    }

    size_t ExponentialSegregatedFitAllocator::GetOrder() const
    {
        return allocators_.size();
    }

    Bytes ExponentialSegregatedFitAllocator::GetClassSize() const
    {
        return allocators_[0].GetBlockSize();
    }

    const MemoryRange& ExponentialSegregatedFitAllocator::GetRange() const
    {
        return memory_range_;
    }

    void ExponentialSegregatedFitAllocator::InitializeAllocators(size_t order, Bytes class_size)
    {
        SYNTROPY_ASSERT(allocators_.empty());                                                       // Check for multiple initializations.
        SYNTROPY_ASSERT(Math::IsPow2(class_size));                                                  // Allocations classes must be a power of 2 (for alignment requirements and because memory pages size is always a power of 2).
        SYNTROPY_ASSERT(order >= 1);                                                                // At least one allocator is needed.

        allocators_.reserve(order);

        auto capacity = memory_range_.GetSize() / order;                                            // The entire memory range is divided evenly among the classes.

        for(size_t index = 0; index < order; ++index)
        {
            allocators_.emplace_back(
                MemoryRange(Memory::AddOffset(*memory_range_, index * capacity), capacity),         // Split the range into sub ranges.
                class_size * (std::size_t(1u) << index));                                           // Class size is doubled at each iteration.

            SYNTROPY_ASSERT(memory_range_.Contains(allocators_.back().GetRange()));
        }
    }

    BlockAllocator& ExponentialSegregatedFitAllocator::GetAllocatorBySize(Bytes block_size)
    {
        SYNTROPY_ASSERT(block_size <= GetMaxAllocationSize());
        SYNTROPY_ASSERT(block_size >= 0_Bytes);

        auto offset = Math::FloorLog2(std::size_t(allocators_.front().GetBlockSize()));

        auto index = Math::CeilLog2(std::size_t(block_size));

        return index >= offset ?
            allocators_[index - offset] :
            allocators_.front();                // Allocations smaller than the smallest allocation class are handled by that allocator.
    }

    Bytes ExponentialSegregatedFitAllocator::GetAllocatorCapacity() const
    {
        return memory_range_.GetSize() / allocators_.size();
    }

    /************************************************************************/
    /* TWO LEVEL SEGREGATED FIT ALLOCATOR :: BLOCK HEADER                   */
    /************************************************************************/

    Bytes TwoLevelSegregatedFitAllocator::BlockHeader::GetSize() const
    {
        return size_ & Bytes(~kSizeMask);
    }

    void TwoLevelSegregatedFitAllocator::BlockHeader::SetSize(Bytes size)
    {
        SYNTROPY_ASSERT((size & Bytes(kSizeMask)) == 0_Bytes)               // The new size must not interfere with the status bits at the end!.

        size_ = size | (size_ & Bytes(kSizeMask));                          // Preserve the status of the two flags at the end.
    }

    bool TwoLevelSegregatedFitAllocator::BlockHeader::IsBusy() const
    {
        return (size_ & Bytes(kBusyBlockFlag)) != 0_Bytes;
    }

    void TwoLevelSegregatedFitAllocator::BlockHeader::SetBusy(bool is_busy)
    {
        if (is_busy)
        {
            size_ = size_ | Bytes(kBusyBlockFlag);
        }
        else
        {
            size_ = size_ & Bytes(kBusyBlockFlag);
        }
    }

    bool TwoLevelSegregatedFitAllocator::BlockHeader::IsLast() const
    {
        return (size_ & Bytes(kLastBlockFlag)) != 0_Bytes;
    }

    void TwoLevelSegregatedFitAllocator::BlockHeader::SetLast(bool is_last)
    {
        if (is_last)
        {
            size_ = size_ | Bytes(kLastBlockFlag);
        }
        else
        {
            size_ = size_ & Bytes(~kLastBlockFlag);
        }
    }

    void* TwoLevelSegregatedFitAllocator::BlockHeader::begin()
    {
        return Memory::AddOffset(this, Bytes(sizeof(BlockHeader)));
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
        return Memory::AddOffset(this, Bytes(sizeof(FreeBlockHeader)));
    }

    void* TwoLevelSegregatedFitAllocator::FreeBlockHeader::end()
    {
        return Memory::AddOffset(this, GetSize());
    }

    /************************************************************************/
    /* TWO LEVEL SEGREGATED FIT ALLOCATOR                                   */
    /************************************************************************/
    
    const Bytes TwoLevelSegregatedFitAllocator::kMinimumBlockSize(32);

    TwoLevelSegregatedFitAllocator::TwoLevelSegregatedFitAllocator(const HashedString& name, Bytes capacity, size_t second_level_index)
        : Allocator(name)
        , allocator_(capacity, VirtualMemory::GetPageSize())
    {
        Initialize(second_level_index);
    }

    TwoLevelSegregatedFitAllocator::TwoLevelSegregatedFitAllocator(const HashedString& name, const MemoryRange& memory_range, size_t second_level_index)
        : Allocator(name)
        , allocator_(memory_range, VirtualMemory::GetPageSize())
    {
        Initialize(second_level_index);
    }

    TwoLevelSegregatedFitAllocator::TwoLevelSegregatedFitAllocator(TwoLevelSegregatedFitAllocator&& other)
        : Allocator(std::move(other))
        , allocator_(std::move(other.allocator_))
        , last_block_(std::move(other.last_block_))
        , first_level_count_(other.first_level_count_)
        , second_level_count_(other.second_level_count_)
        , first_level_bitmap_(other.first_level_bitmap_)
        , second_level_bitmap_(std::move(other.second_level_bitmap_))
        , free_lists_(std::move(other.free_lists_))
    {

    }

    void* TwoLevelSegregatedFitAllocator::Allocate(Bytes size)
    {
        // Structure of the block:
        // || HEADER | BASE_POINTER | ... BLOCK ... ||

        auto block = GetFreeBlockBySize(size + Bytes(sizeof(uintptr_t)));           // Reserve enough space for the block and the base pointer.

        *reinterpret_cast<BlockHeader**>(block->begin()) = block;                   // The base pointer points to the header.

        return Memory::AddOffset(block->begin(), Bytes(sizeof(uintptr_t)));
    }

    void* TwoLevelSegregatedFitAllocator::Allocate(Bytes size, Bytes alignment)
    {
        // Structure of the block:
        // || HEADER | PADDING | BASE_POINTER | ... ALIGNED BLOCK ... ||

        auto block = GetFreeBlockBySize(size + alignment - 1_Bytes + Bytes(sizeof(uintptr_t)));                         // Reserve enough space for the block, the base pointer and the eventual padding.

        auto aligned_begin = Memory::Align(Memory::AddOffset(block->begin(), Bytes(sizeof(uintptr_t))), alignment);     // First address of the requested aligned block.

        *reinterpret_cast<BlockHeader**>(Memory::SubOffset(aligned_begin, Bytes(sizeof(uintptr_t)))) = block;           // The base pointer points to the header.

        return aligned_begin;
    }

    void TwoLevelSegregatedFitAllocator::Free(void* block)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        // The base pointer is guaranteed to be adjacent to the allocated block.

        auto base_pointer = reinterpret_cast<BlockHeader**>(Memory::SubOffset(block, Bytes(sizeof(uintptr_t))));

        PushBlock(*base_pointer);
    }

    bool TwoLevelSegregatedFitAllocator::Owns(void* block) const
    {
        return GetRange().Contains(block);
    }

    Bytes TwoLevelSegregatedFitAllocator::GetMaxAllocationSize() const
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

        first_level_count_ = Math::FloorLog2(std::size_t(allocator_.GetRange().GetSize())) + 1u;
        second_level_count_ = second_level_count;

        // Ensure that the bitmaps can store at least one bit per first or second class.
        SYNTROPY_ASSERT(sizeof(first_level_bitmap_) * 8u >= first_level_count_);
        SYNTROPY_ASSERT(sizeof(decltype(second_level_bitmap_)::value_type) * 8u >= (static_cast<size_t>(1u) << second_level_count_));

        free_lists_.resize(first_level_count_ * (static_cast<size_t>(1u) << second_level_count_));

        // Free lists start empty
        first_level_bitmap_ = 0;
        second_level_bitmap_.resize(first_level_count_);
    }

    TwoLevelSegregatedFitAllocator::BlockHeader* TwoLevelSegregatedFitAllocator::GetFreeBlockBySize(Bytes size)
    {
        SYNTROPY_PRECONDITION(size > 0_Bytes);

        size += Bytes(sizeof(BlockHeader));                                         // Reserve space for the header.
        size = std::max(size, kMinimumBlockSize);                                   // The size must be at least as big as the minimum size allowed.
        size = Math::Ceil(size, Bytes(BlockHeader::kSizeMask) + 1_Bytes);           // The size must not interfere with the status bits of the block.

        size_t first_level_index;
        size_t second_level_index;

        std::lock_guard<std::mutex> lock(mutex_);

        // Start searching from the next class (the current free list may have blocks that are smaller than the requested size)
        GetFreeListIndex(size, first_level_index, second_level_index, true);

        auto index = GetFreeListIndex(first_level_index, second_level_index);

        if (index < free_lists_.size() && !free_lists_[index])
        {
            // Search a free list with a block which is larger than the requested size.

            auto second_level_bitmap = second_level_bitmap_[first_level_index] & ~((static_cast<uint64_t>(1u) << second_level_index) - 1u);         // Mask out smaller second-level classes.
            auto first_level_bitmap = first_level_bitmap_ & ~((static_cast<uint64_t>(2u) << first_level_index) - 1u);                               // Mask out the current and smaller first-level classes.

            if (second_level_bitmap != 0)
            {
                // Search in the current first-level (there are larger second-level lists that are not empty).
                second_level_index = platform::BuiltIn::GetLeastSignificantBit(static_cast<uint64_t>(second_level_bitmap));

                index = GetFreeListIndex(first_level_index, second_level_index);
            }
            else if (first_level_bitmap != 0)
            {
                // Search in a larger first-level class.
                first_level_index = platform::BuiltIn::GetLeastSignificantBit(static_cast<uint64_t>(first_level_bitmap));
                second_level_index = platform::BuiltIn::GetLeastSignificantBit(static_cast<uint64_t>(second_level_bitmap_[first_level_index]));

                index = GetFreeListIndex(first_level_index, second_level_index);
            }
            else
            {
                // No free list found.
                index = free_lists_.size();
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

        SYNTROPY_ASSERT(block->GetSize() >= size);

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

        SYNTROPY_ASSERT(block != nullptr);
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
            merged_block->SetLast(next_block->IsLast());                                // Merging with the 'last' block yields another 'last' block.
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

        MemoryDebug::MarkFree(merged_block->begin(), merged_block->end());

        InsertBlock(merged_block);
    }

    TwoLevelSegregatedFitAllocator::BlockHeader* TwoLevelSegregatedFitAllocator::AllocateBlock(Bytes size)
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

    void TwoLevelSegregatedFitAllocator::SplitBlock(BlockHeader* block, Bytes size)
    {
        SYNTROPY_ASSERT(block->IsBusy());                               // Make sure the original block is busy, otherwise PushBlock will merge again block and remaining_block!

        if (block->GetSize() >= kMinimumBlockSize + size)               // Do not split if the remaining block size would fall below the minimum size allowed.
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

    void TwoLevelSegregatedFitAllocator::GetFreeListIndex(Bytes size, size_t& first_level_index, size_t& second_level_index, bool roundup) const
    {
        if (roundup)
        {
            // Round up to the next class size.
            size = size + Bytes(size_t(1u) << (Math::FloorLog2(std::size_t(size)) - second_level_count_)) - 1_Bytes;
        }

        first_level_index = Math::FloorLog2(std::size_t(size));

        second_level_index = (std::size_t(size) ^ (std::size_t(1u) << first_level_index)) >> (first_level_index - second_level_count_);
    }

    size_t TwoLevelSegregatedFitAllocator::GetFreeListIndex(size_t first_level_index, size_t second_level_index) const
    {
        return first_level_index * (static_cast<size_t>(1u) << second_level_count_) + second_level_index;
    }

}
