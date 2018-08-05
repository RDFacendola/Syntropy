#include "memory/virtual_memory.h"

#include "platform/os/os.h"

#include "math/math.h"

#include "diagnostics/diagnostics.h"

namespace syntropy
{

    /************************************************************************/
    /* VIRTUAL MEMORY                                                       */
    /************************************************************************/

    Bytes VirtualMemory::GetPageSize()
    {
        return platform::PlatformMemory::GetPageSize();
    }

    {
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

