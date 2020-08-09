#include "syntropy/memory/virtual_memory.h"

#include "syntropy/hal/hal_virtual_memory.h"

namespace syntropy
{
    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    Bytes Memory::VirtualPageSize() noexcept
    {
        return HALMemory::VirtualPageSize();
    }

    Alignment Memory::VirtualPageAlignment() noexcept
    {
        return HALMemory::VirtualPageAlignment();
    }

    RWByteSpan Memory::VirtualReserve(Bytes size) noexcept
    {
        return HALMemory::VirtualReserve(size);
    }

    RWByteSpan Memory::VirtualAllocate(Bytes size) noexcept
    {
        return HALMemory::VirtualAllocate(size);
    }

    Bool Memory::VirtualRelease(const RWByteSpan& byte_span) noexcept
    {
        return HALMemory::VirtualRelease(byte_span);
    }

    Bool Memory::VirtualCommit(const RWByteSpan& byte_span) noexcept
    {
        return HALMemory::VirtualCommit(byte_span);
    }

    Bool Memory::VirtualDecommit(const RWByteSpan& byte_span) noexcept
    {
        return HALMemory::VirtualDecommit(byte_span);
    }

}

