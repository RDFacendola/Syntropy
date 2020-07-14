#include "syntropy/memory/virtual_memory.h"

#include "syntropy/hal/hal_virtual_memory.h"

namespace syntropy
{
    /************************************************************************/
    /* VIRTUAL MEMORY                                                       */
    /************************************************************************/

    Bytes VirtualMemory::GetPageSize()
    {
        return HALVirtualMemory::GetPageSize();
    }

    Alignment VirtualMemory::GetPageAlignment()
    {
        return HALVirtualMemory::GetPageAlignment();
    }

    MemorySpan VirtualMemory::Reserve(Bytes size)
    {
        return HALVirtualMemory::Reserve(size);
    }

    MemorySpan VirtualMemory::Allocate(Bytes size)
    {
        return HALVirtualMemory::Allocate(size);
    }

    Bool VirtualMemory::Release(const MemorySpan& memory_span)
    {
        return HALVirtualMemory::Release(memory_span);
    }

    Bool VirtualMemory::Commit(const MemorySpan& memory_span)
    {
        return HALVirtualMemory::Commit(memory_span);
    }

    Bool VirtualMemory::Decommit(const MemorySpan& memory_span)
    {
        return HALVirtualMemory::Decommit(memory_span);
    }

}

