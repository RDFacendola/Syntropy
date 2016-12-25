#include "memory/page_allocator.h"

#include <iterator>

namespace syntropy
{

    //////////////// PAGE ALLOCATOR ////////////////

    PageAllocator::PageAllocator(size_t capacity, size_t page_size)
        : memory_(GetMemory())
        , page_size_(memory_.Ceil(page_size))
        , head_(0)
        , storage_(reinterpret_cast<int8_t*>(memory_.Reserve(capacity)))
    {

    }

    PageAllocator::~PageAllocator()
    {
        memory_.Release(storage_);
    }

    void* PageAllocator::AllocatePage()
    {
        size_t page_offset;

        if (free_list_.size() > 0)
        {
            // Recycle a free page
            page_offset = free_list_.back();
            free_list_.pop_back();
        }
        else
        {
            // Grab a new page
            page_offset = head_++;
        }

        auto page = storage_ + page_size_ * page_offset;

        return memory_.Allocate(page, page_size_) ?
               page :
               nullptr;
    }

    void PageAllocator::DeallocatePage(void* page)
    {
        // Deallocate the page and mark it as free
        if (memory_.Deallocate(page, page_size_))
        {
            auto offset = std::distance(storage_, reinterpret_cast<int8_t*>(page)) / page_size_;
            free_list_.push_back(offset);
        }
    }

    size_t PageAllocator::GetAllocationSize() const
    {
        return page_size_ * (head_ - free_list_.size());
    }

}
