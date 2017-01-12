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
        : page_allocator_(capacity, page_size)
        , maximum_block_size_(Math::NextMultipleOf(maximum_allocation_size, kMinimumAllocationSize))                    // Rounds to the next minimum allocation boundary
    {
        // TODO: Move to bookkeeping!!!

        auto& memory = GetMemory();

        auto count = maximum_block_size_ / kMinimumAllocationSize;          // Sub-allocators count

        auto size = count * sizeof(Page*);                                  // Size needed to store the head of each sub-allocator

        allocators_ = reinterpret_cast<Page**>(memory.Allocate(size));      // Allocate the memory needed to store sub-allocator infos

        std::fill(&allocators_[0], &allocators_[count], nullptr);           // Each sub-allocator starts with no page to allocate memory blocks. Pages are allocated on demand.
    }

    void* SegregatedPoolAllocator::Allocate(size_t size)
    {
        SYNTROPY_ASSERT(size <= maximum_block_size_);
        SYNTROPY_ASSERT(size > 0);

        auto& allocator = GetAllocator(Math::NextMultipleOf(size, kMinimumAllocationSize));
        
        // Get a block from the free block list
        auto block = allocator->free_;
        allocator->free_ = block->next_;
        ++allocator->allocated_blocks_;

        if (allocator->free_ == nullptr)        // The page is full
        {
            DiscardPage(allocator);             // Discard the current page from the allocator and move to the next one.
        }

        return block;
    }

    void* SegregatedPoolAllocator::Allocate(size_t size, size_t alignment)
    {
        // Since blocks are aligned to their own size, we are looking for a block large enough that is also a multiple of the requested alignment
        auto block = Allocate(Math::NextMultipleOf(size, alignment));

        SYNTROPY_ASSERT(reinterpret_cast<size_t>(block) % alignment == 0);

        return block;
    }

    void SegregatedPoolAllocator::Free(void* address)
    {
        auto block = reinterpret_cast<Block*>(address);

        auto page = reinterpret_cast<Page*>(GetPageAddress(block));

        // Add the block to free block list
        --page->allocated_blocks_;
        block->next_ = page->free_;
        page->free_ = block;

        if (page->allocated_blocks_ == 0)       // No allocated blocks.
        {
            FreePage(page);                     // Free the empty page.
        }
        else if (!page->free_->next_)           // The page was full and now has exactly one free block.
        {
            RestorePage(page);                  // Send the page back to the allocator list so that new allocations can be performed inside it.
        }
    }

    size_t SegregatedPoolAllocator::GetSize() const
    {
        return page_allocator_.GetSize();
    }

    SegregatedPoolAllocator::Page*& SegregatedPoolAllocator::GetAllocator(size_t block_size)
    {
        auto& allocator = allocators_[(block_size - 1) / kMinimumAllocationSize];

        if (!allocator)
        {
            allocator = AllocatePage(block_size);
        }

        return allocator;
    }

    SegregatedPoolAllocator::Page* SegregatedPoolAllocator::AllocatePage(size_t block_size)
    {
        auto page = reinterpret_cast<Page*>(page_allocator_.Allocate());                                    // Get a new page
        
        auto page_address = reinterpret_cast<size_t>(page);

        auto header_size = Math::NextMultipleOf(page_address + sizeof(Page), block_size) - page_address;    // The header is padded such that blocks in the page are aligned to their own size
        auto head = reinterpret_cast<Block*>(Memory::Offset(page, header_size));                            // The first available block is just after the page header
        auto count = (page_allocator_.GetBlockSize() - header_size) / block_size;                            // Amount of free block in the page

        SYNTROPY_ASSERT(reinterpret_cast<size_t>(head) % block_size == 0);                                  // Blocks must be aligned to their size

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
            page->previous_->next_ = page->next_;
        }
        else
        {
            allocators_[(page->block_size_ - 1) / kMinimumAllocationSize] = page->next_;
        }

        if (page->next_)
        {
            page->next_->previous_ = page->previous_;
        }

        // Send the page to the allocator for recycling
        page_allocator_.Free(page);
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
        auto& head = allocators_[(page->block_size_ - 1) / kMinimumAllocationSize];

        page->next_ = head;

        if (head)
        {
            head->previous_ = page;
        }

        head = page;
    }

    void* SegregatedPoolAllocator::GetPageAddress(void* address) const
    {
        return reinterpret_cast<void*>(Math::PreviousMultipleOf(reinterpret_cast<size_t>(address), page_allocator_.GetBlockSize()));
    }

    //////////////// CLUSTERED POOL ALLOCATOR ////////////////

    ClusteredPoolAllocator::ClusteredPoolAllocator(size_t capacity, size_t minimum_allocation_size, size_t order)
        : order_(order)
        , base_allocation_size_(Math::NextMultipleOf(minimum_allocation_size, GetMemory().GetAllocationGranularity()))      // Round to the next memory page boundary
    {
        SYNTROPY_ASSERT(order >= 1);

        // TODO: Move to bookkeeping!!!

        auto maximum_allocation_size = base_allocation_size_ * (static_cast<size_t>(1) << (order - 1));         // Allocation size of the last cluster

        auto cluster_capacity = Math::PreviousMultipleOf(capacity / order, maximum_allocation_size);            // Each allocator must have the same capacity which is also a multiple of the maximum allocation possible.

        auto& memory = GetMemory();

        auto size = order * sizeof(BlockAllocator);                                                             // Size of the storage for cluster allocators.

        allocators_ = reinterpret_cast<BlockAllocator*>(memory.Allocate(size));                                 // 

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
