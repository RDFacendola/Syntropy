#include "memory/page_allocator.h"

#include <iterator>
#include <algorithm>

#include "syntropy.h"
#include "platform/builtin.h"
#include "math/math.h"

namespace syntropy
{

    //////////////// STATIC PAGE ALLOCATOR ////////////////

    PageAllocator::PageAllocator(size_t pages, size_t page_size)
        : memory_(GetMemory())                                                              // Get a reference to the memory handler
        , page_size_(Math::NextMultipleOf(page_size, memory_.GetAllocationGranularity()))   // Ceil the specified page size to the system actual page boundary
        , storage_(reinterpret_cast<int8_t*>(memory_.Reserve(page_size_ * pages)))          // Reserve the address range for the storage upfront
        , free_list_(nullptr)                                                               // Empty free list
        , head_(reinterpret_cast<Page*>(storage_))                                          // No mapped page
    {

    }

    PageAllocator::~PageAllocator()
    {
        memory_.Release(storage_);
    }

    void PageAllocator::Push(void* page)
    {
        // Set the page as head of the free list
        auto page_ptr = reinterpret_cast<Page*>(page);

        page_ptr->next_ = free_list_;
        free_list_ = page_ptr;
    }

    void* PageAllocator::Pop()
    {
        return free_list_ ?
               PopFreePage() :
               AllocatePage();
    }

    size_t PageAllocator::GetPageSize() const
    {
        return page_size_;
    }

    void* PageAllocator::GetPageAddress(void* address) const
    {
        return reinterpret_cast<void*>((reinterpret_cast<size_t>(address) / page_size_) * page_size_);
    }

    void* PageAllocator::PopFreePage()
    {
        auto page = free_list_;

        // Pop out the head of the free list.
        free_list_ = page->next_;

        return page;
    }

    void* PageAllocator::AllocatePage() 
    {
        auto page = head_;

        // Allocate a new page and move the head forward.
        memory_.Allocate(page, page_size_);
        head_ = reinterpret_cast<Page*>(reinterpret_cast<int8_t*>(head_) + page_size_);

        return page;
    }

    //////////////// SEGREGATED ALLOCATOR :: BLOCK ////////////////

    SegregatedAllocator::Block* SegregatedAllocator::Block::GetNextBlock(size_t block_size)
    {
        return reinterpret_cast<Block*>(reinterpret_cast<int8_t*>(this) + block_size);
    }

    //////////////// SEGREGATED ALLOCATOR :: PAGE ////////////////

    SegregatedAllocator::Block* SegregatedAllocator::Page::GetFirstBlock(size_t block_size)
    {
        return reinterpret_cast<Block*>(reinterpret_cast<int8_t*>(this) +
                                        Math::NextMultipleOf(sizeof(Page), block_size));                        // Discard the page header size
    }

    SegregatedAllocator::Block* SegregatedAllocator::Page::GetLastBlock(size_t page_size, size_t block_size)
    {
        auto max_allocations = (page_size - Math::NextMultipleOf(sizeof(Page), block_size)) / block_size;       // Maximum allocations that can be performed on this page

        return reinterpret_cast<Block*>(reinterpret_cast<int8_t*>(GetFirstBlock(block_size)) + 
                                        (max_allocations - 1) * block_size);
    }

    //////////////// SEGREGATED ALLOCATOR ////////////////

    SegregatedAllocator::SegregatedAllocator(size_t capacity, size_t page_size, size_t maximum_allocation_size)
        : free_list_(Math::NextMultipleOf(capacity, page_size) / page_size + 1,                                 // The extra page is used for bookkeeping purposes
                     page_size)
        , allocators_(reinterpret_cast<Page**>(free_list_.Pop()))                                               // The pointers to the first page of each allocator are stored inside the first page
        , maximum_allocation_size_(Math::NextMultipleOf(maximum_allocation_size, kMinimumAllocationSize))       // Maximum allowed allocation, rounded to the next minimum allocation boundary
    {
        // Wipe the allocators page pointers
        auto count = maximum_allocation_size_ / kMinimumAllocationSize;

        std::fill(&allocators_[0], &allocators_[count], nullptr);
    }

    SegregatedAllocator::~SegregatedAllocator()
    {

    }

    void* SegregatedAllocator::Allocate(size_t size, size_t /*alignment*/)
    {
        if (size > maximum_allocation_size_)
        {
            return nullptr;     // TODO: throw bad_alloc
        }

        auto allocator_index = (size - 1) / kMinimumAllocationSize;

        auto& allocator = allocators_[allocator_index];

        // Allocate a new page if no one is available
        if (!allocator)
        {
            allocator = AllocatePage((allocator_index + 1) * kMinimumAllocationSize);
        }

        // Get a free block from the current page's free list
        auto block = allocator->free_block_;
        ++allocator->allocations_;
        allocator->free_block_ = block->next_;                                                                          // Consume the free list's head

        // If the current page is full move to the next one
        if (!allocator->free_block_)
        {
            allocator = allocator->next_;
        }

        return block;
    }

    void SegregatedAllocator::Deallocate(void* address)
    {
        auto page = reinterpret_cast<Page*>(free_list_.GetPageAddress(address));
        SYNTROPY_UNUSED(page);
    }

    SegregatedAllocator::Page* SegregatedAllocator::AllocatePage(size_t block_size)
    {
        auto page = reinterpret_cast<Page*>(free_list_.Pop());

        auto first = page->GetFirstBlock(block_size);
        auto last = page->GetLastBlock(free_list_.GetPageSize(), block_size);

        page->allocations_ = 0;
        page->free_block_ = first;
        page->next_ = nullptr;

        // Initialize the free block list
        while (first != last)
        {
            first->next_ = first->GetNextBlock(block_size);
            first = first->next_;
        }

        last->next_ = nullptr;

        return page;
    }

}
