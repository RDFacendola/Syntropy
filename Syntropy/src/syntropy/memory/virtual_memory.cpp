#include "syntropy/memory/virtual_memory.h"

#include "syntropy/hal/hal_virtual_memory.h"

namespace syntropy
{
    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    Bytes Memory::GetPageSize() noexcept
    {
        return HALMemory::GetPageSize();
    }

    Alignment Memory::GetPageAlignment() noexcept
    {
        return HALMemory::GetPageAlignment();
    }

    RWByteSpan Memory::Reserve(Bytes size) noexcept
    {
        return HALMemory::Reserve(size);
    }

    RWByteSpan Memory::Allocate(Bytes size) noexcept
    {
        return HALMemory::Allocate(size);
    }

    Bool Memory::Release(const RWByteSpan& byte_span) noexcept
    {
        return HALMemory::Release(byte_span);
    }

    Bool Memory::Commit(const RWByteSpan& byte_span) noexcept
    {
        return HALMemory::Commit(byte_span);
    }

    Bool Memory::Decommit(const RWByteSpan& byte_span) noexcept
    {
        return HALMemory::Decommit(byte_span);
    }

}

