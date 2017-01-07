#include "memory/segregated_allocator.h"

#include <iterator>
#include <algorithm>
#include <new>

#include "memory/memory.h"
#include "diagnostics/debug.h"
#include "diagnostics/log.h"
#include "math/math.h"

namespace syntropy 
{

    //////////////// PAGE ALLOCATOR ////////////////

    PageAllocator::PageAllocator(size_t capacity, size_t page_size)
        : memory_(GetMemory())                                                                          // Get a reference to the memory handler
        , page_size_(Math::NextMultipleOf(page_size, memory_.GetAllocationGranularity()))               // Ceil the specified page size to the actual system page boundary
        , base_(memory_.Reserve(Math::NextMultipleOf(capacity, page_size_)))                            // Reserve the contiguous address range for the storage upfront
        , head_(reinterpret_cast<Page*>(base_))                                                         // No mapped page
        , free_(nullptr)                                                                                // No free page
    {

    }

    PageAllocator::~PageAllocator()
    {
        memory_.Free(base_);
    }

    void* PageAllocator::Allocate()
    {
        return free_ ?
               PopFreePage() :
               AllocatePage();
    }
    
    void PageAllocator::Free(void* page)
    {
        // Push the page on top of the free pages stack
        auto free_page = reinterpret_cast<Page*>(page);

        free_page->next_ = free_;
        free_ = free_page;
    }

    size_t PageAllocator::GetPageSize() const
    {
        return page_size_;
    }

    size_t PageAllocator::GetSize() const
    {
        return std::distance(reinterpret_cast<int8_t*>(base_), reinterpret_cast<int8_t*>(head_));
    }

    void* PageAllocator::PopFreePage()
    {
        auto page = free_;
        free_ = page->next_;                            // Consume the free page
        return page;
    }

    void* PageAllocator::AllocatePage()
    {
        auto page = head_;
        memory_.Commit(page, page_size_);
        head_ = Memory::Offset(head_, page_size_);      // Move the head forward
        return page;
    }

    //////////////// SEGREGATED POOL ALLOCATOR ////////////////

    SegregatedPoolAllocator::SegregatedPoolAllocator(size_t capacity, size_t page_size, size_t maximum_allocation_size)
        : page_allocator_(capacity, page_size)
        , maximum_block_size_(Math::NextMultipleOf(maximum_allocation_size, kMinimumAllocationSize))                    // Rounds to the next minimum allocation boundary
    {
        // TODO: This memory is not tracked and may cause negligible fragmentation. It would be nice to have this memory allocated just before the page allocator storage.
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
        auto count = (page_allocator_.GetPageSize() - header_size) / block_size;                            // Amount of free block in the page

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
        return reinterpret_cast<void*>(Math::PreviousMultipleOf(reinterpret_cast<size_t>(address), page_allocator_.GetPageSize()));
    }

}
