#include "memory/page_allocator.h"

#include <iterator>

namespace syntropy
{

    //////////////// PAGE POOL ////////////////

    PagePool::PagePool(size_t max_pages)
        : memory_(GetMemory())
        , storage_(reinterpret_cast<size_t*>(memory_.Reserve(max_pages * sizeof(size_t))))
        , size_(0)
        , capacity_(memory_.GetAllocationGranularity() / sizeof(size_t))
        , head_(0)
    {
        // Allocate the first memory block
        memory_.Allocate(storage_, capacity_ * sizeof(size_t));
    }

    PagePool::~PagePool()
    {
        memory_.Release(storage_);
    }

    void PagePool::Push(size_t page_index)
    {
        // Double the storage capacity
        if (size_ >= capacity_)
        {
            memory_.Allocate(storage_ + capacity_, capacity_ * sizeof(size_t));
            capacity_ *= 2;
        }

        // Store the page index
        *(storage_ + size_) = page_index;
        ++size_;
    }

    size_t PagePool::Pop()
    {
        if (size_ == 0)
        {
            // Generate a new page index.
            return head_++;
        }

        // If the current capacity is at least four times bigger than the actual size, the storage capacity is halved to save memory.
        // The capacity may not fall below the size of a single memory page though.
        if (size_ * 4 <= capacity_ &&
            capacity_ >= 2 * memory_.GetAllocationGranularity() / sizeof(size_t))
        {
            capacity_ /= 2;
            memory_.Deallocate(storage_ + capacity_, capacity_ * sizeof(size_t));
        }

        // Return the last page index
        --size_;
        return *(storage_ + size_);
    }

    size_t PagePool::GetAllocatedPages() const
    {
        return head_ - size_;
    }

    //////////////// PAGE ALLOCATOR ////////////////

    PageAllocator::PageAllocator(size_t capacity, size_t page_size)
        : memory_(GetMemory())
        , page_size_(memory_.CeilToPageBoundary(page_size))
        , page_pool_(static_cast<size_t>(std::ceil(static_cast<float>(capacity) / page_size)))
        , storage_(reinterpret_cast<int8_t*>(memory_.Reserve(capacity)))
    {

    }

    PageAllocator::~PageAllocator()
    {
        memory_.Release(storage_);
    }

    void* PageAllocator::AllocatePage()
    {
        auto page = storage_ + page_size_ * page_pool_.Pop();

        return memory_.Allocate(page, page_size_) ?
               page :
               nullptr;
    }

    void PageAllocator::DeallocatePage(void* page)
    {
        if (memory_.Deallocate(page, page_size_))
        {
            page_pool_.Push(std::distance(storage_, reinterpret_cast<int8_t*>(page)) / page_size_);     // Mark the page index as 'free'
        }
    }

    size_t PageAllocator::GetAllocationSize() const
    {
        return page_size_ * page_pool_.GetAllocatedPages();
    }

}
