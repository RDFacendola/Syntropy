#include "memory/std_allocator.h"

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

    void* STDAllocator::Allocate(size_t size)
    {
        return ::operator new(size);
    }

    void* STDAllocator::Allocate(size_t size, size_t /*alignment*/)
    {
        return ::operator new(size);
    }

    void STDAllocator::Free(void* block)
    {
        ::operator delete(block);
    }

}
