#include "memory/allocation.h"

namespace syntropy
{

    //////////////// MEMORY PAGE ALLOCATOR ////////////////

    PageAllocator::PageAllocator(size_t range_size, size_t page_size)
        : memory_range_(GetMemory().ReserveVirtualRange(range_size))
        , head_(GetMemory().AllocMemoryBlock(memory_range_, 0, page_size))
        , free_addresses_(range_size)
    {

    }

    PageAllocator::~PageAllocator()
    {
        GetMemory().ReleaseVirtualRange(memory_range_);
    }

    MemoryBlock PageAllocator::AllocPage()
    {
        return MemoryBlock(nullptr, 0);
    }

    void PageAllocator::FreePage(const MemoryBlock& page)
    {
        //*page.GetPointer<uint64_t>() = reinterpret_cast<uint64_t>(head_.GetBaseAddress());
        head_ = page;
    }

}
