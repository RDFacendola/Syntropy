#include "memory/allocators/std_allocator.h"

#include <limits>

namespace syntropy
{

    STDAllocator g_std_allocator;

    /************************************************************************/
    /* STD ALLOCATOR                                                        */
    /************************************************************************/

    const char* STDAllocator::kName = "STD Allocator";

    STDAllocator::STDAllocator()
        : Allocator(kName)
    {

    }

    void* STDAllocator::Allocate(Bytes size)
    {
        return ::operator new(std::size_t(size));
    }

    void* STDAllocator::Allocate(Bytes size, Alignment alignment)
    {
        return ::operator new(std::size_t(size), alignment);
    }

    void STDAllocator::Free(void* block)
    {
        ::operator delete(block);
    }

    bool STDAllocator::Owns(void*) const
    {
        return true;
    }

    Bytes STDAllocator::GetMaxAllocationSize() const
    {
        return Bytes(std::numeric_limits<size_t>::max());
    }

}
