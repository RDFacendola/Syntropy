#include "syntropy/memory/virtual_memory.h"

#include "syntropy/platform/os/os.h"

#include "syntropy/math/math.h"

#include "syntropy/diagnostics/diagnostics.h"

namespace syntropy
{

    /************************************************************************/
    /* VIRTUAL MEMORY                                                       */
    /************************************************************************/

    Bytes VirtualMemory::GetPageSize()
    {
        return platform::PlatformMemory::GetPageSize();
    }

    Alignment VirtualMemory::GetPageAlignment()
    {
        return platform::PlatformMemory::GetPageAlignment();
    }

    MemoryRange VirtualMemory::Reserve(Bytes size)
    {
        return platform::PlatformMemory::Reserve(size);
    }

    MemoryRange VirtualMemory::Allocate(Bytes size)
    {
        return platform::PlatformMemory::Allocate(size);
    }

    bool VirtualMemory::Release(const MemoryRange& memory_range)
    {
        return platform::PlatformMemory::Release(memory_range);
    }

    bool VirtualMemory::Commit(const MemoryRange& memory_range)
    {
        return platform::PlatformMemory::Commit(memory_range);
    }

    bool VirtualMemory::Decommit(const MemoryRange& memory_range)
    {
        return platform::PlatformMemory::Decommit(memory_range);
    }

}

