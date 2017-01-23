#include "memory/segregated_allocator.h"

#include <iterator>
#include <algorithm>
#include <new>

#include "memory/memory.h"
#include "diagnostics/debug.h"
#include "math/math.h"

namespace syntropy 
{

    //////////////// SEGREGATED POOL ALLOCATOR ////////////////

    SegregatedPoolAllocator::SegregatedPoolAllocator(size_t capacity, size_t page_size, size_t maximum_allocation_size)
        : allocator_(capacity, page_size)                                                       // Allocator for the pages.
        , maximum_block_size_(Math::Ceil(maximum_allocation_size, kMinimumAllocationSize))      // Round to the next minimum allocation boundary.
        , free_pages_(maximum_block_size_ / kMinimumAllocationSize)                             // Total amount of segregated lists.
    {
        auto count = free_pages_.GetMaxCount();

        while (count-- > 0)
        {
            free_pages_.PushBack(nullptr);
        }
    }

    void* SegregatedPoolAllocator::Allocate(size_t size)
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

    void* SegregatedPoolAllocator::Allocate(size_t size, size_t alignment)
    {
        SYNTROPY_ASSERT(alignment % kMinimumAllocationSize == 0);       // Must be a multiple of the minimum allocation size

        // Since blocks are aligned to their own size, we are looking for a block large enough that is also a multiple of the requested alignment
        auto block = Allocate(Math::Ceil(size, alignment));

        SYNTROPY_ASSERT(Memory::IsAlignedTo(block, alignment));

        return block;
    }

    void SegregatedPoolAllocator::Free(void* address)
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

    size_t SegregatedPoolAllocator::GetSize() const
    {
        Block* head;
        size_t count;

        auto size = allocator_.GetSize();

        // Remove the memory of free blocks inside each segregated list
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

    size_t SegregatedPoolAllocator::GetEffectiveSize() const
    {
        return allocator_.GetEffectiveSize();
    }

    size_t SegregatedPoolAllocator::GetCapacity() const
    {
        return allocator_.GetCapacity();
    }

    SegregatedPoolAllocator::Page* SegregatedPoolAllocator::AllocatePage(size_t block_size)
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

    void SegregatedPoolAllocator::FreePage(Page* page)
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

    void SegregatedPoolAllocator::DiscardPage(Page*& page)
    {
        page = page->next_;

        if (page)
        {
            page->previous_ = nullptr;
        }
    }

    void SegregatedPoolAllocator::RestorePage(Page* page)
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

    size_t SegregatedPoolAllocator::GetBlockCount(Page* page, size_t block_size, Block*& head) const
    {
        auto page_address = reinterpret_cast<size_t>(page);

        auto header_size = Math::Ceil(page_address + sizeof(Page), block_size) - page_address;      // The header is padded such that blocks in the page are aligned to their own size
        head = reinterpret_cast<Block*>(Memory::Offset(page, header_size));                         // The first available block is just after the page header
        return (allocator_.GetBlockSize() - header_size) / block_size;                              // Amount of available blocks in the page
    }

    //////////////// CLUSTERED POOL ALLOCATOR ////////////////

    ClusteredPoolAllocator::ClusteredPoolAllocator(size_t capacity, size_t minimum_allocation_size, size_t order)
        : order_(order)
        , base_allocation_size_(Memory::CeilToPageSize(minimum_allocation_size))                                // Round to the next memory page boundary
    {
        SYNTROPY_ASSERT(order >= 1);

        // TODO: Move to bookkeeping!!!

        auto maximum_allocation_size = base_allocation_size_ * (static_cast<size_t>(1) << (order - 1));         // Allocation size of the last cluster

        auto cluster_capacity = Math::Floor(capacity / order, maximum_allocation_size);                         // Each allocator must have the same capacity which is also a multiple of the maximum allocation possible.
        
        auto size = order * sizeof(BlockAllocator);                                                             // Size of the storage for cluster allocators.

        allocators_ = reinterpret_cast<BlockAllocator*>(Memory::Allocate(size));                                // 

        while (order-- != 0)
        {
            new (allocators_ + order) BlockAllocator(cluster_capacity, maximum_allocation_size);                // Initialize each cluster allocator
            maximum_allocation_size >>= 1;
        } 

    }

    void* ClusteredPoolAllocator::Allocate(size_t size)
    {
        auto index = Math::CeilLog2((size + base_allocation_size_ - 1) / base_allocation_size_);

        return allocators_[index].Allocate(size);
    }

    void* ClusteredPoolAllocator::Allocate(size_t size, size_t alignment)
    {
        auto index = Math::CeilLog2((size + alignment + base_allocation_size_ - 1) / base_allocation_size_);    // Blocks are not aligned, just reserve enough space for both the block and maximum padding due to its alignment
        
        auto block = Memory::Align(allocators_[index].Allocate(size + alignment), alignment);                   // Allocate the actual padded memory block

        SYNTROPY_ASSERT(reinterpret_cast<size_t>(block) % alignment == 0);

        return block;
    }

    void ClusteredPoolAllocator::Free(void* address)
    {
        for (auto index = order_ - 1; index >= 0; --index)
        {
            if (allocators_[index].ContainsAddress(address))
            {
                allocators_[index].Free(address);
                return;
            }
        }
    }

    size_t ClusteredPoolAllocator::GetSize() const
    {
        return std::accumulate(&allocators_[0],
                               &allocators_[order_],
                               static_cast<size_t>(0),
                               [](size_t accumulator, const BlockAllocator& allocator)
                               {
                                    return accumulator + allocator.GetSize();
                               });
    }

}
