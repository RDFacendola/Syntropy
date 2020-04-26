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

    MemoryRange VirtualMemory::Reserve(Bytes size)
    {
        return HALVirtualMemory::Reserve(size);
    }

    MemoryRange VirtualMemory::Allocate(Bytes size)
    {
        return HALVirtualMemory::Allocate(size);
    }

    bool VirtualMemory::Release(const MemoryRange& memory_range)
    {
        return HALVirtualMemory::Release(memory_range);
    }

    bool VirtualMemory::Commit(const MemoryRange& memory_range)
    {
        return HALVirtualMemory::Commit(memory_range);
    }

    bool VirtualMemory::Decommit(const MemoryRange& memory_range)
    {
        return HALVirtualMemory::Decommit(memory_range);
    }

}

