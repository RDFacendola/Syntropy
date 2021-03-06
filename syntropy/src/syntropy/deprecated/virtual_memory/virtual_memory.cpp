#include "syntropy/virtual_memory/virtual_memory.h"

#include "syntropy/hal/hal_virtual_memory.h"

namespace Syntropy
{
    /************************************************************************/
    /* VIRTUAL MEMORY                                                       */
    /************************************************************************/

    Memory::Bytes VirtualMemory::PageSize() noexcept
    {
        return HALVirtualMemory::PageSize();
    }

    Memory::Alignment VirtualMemory::PageAlignment() noexcept
    {
        return HALVirtualMemory::PageAlignment();
    }

    Memory::RWByteSpan VirtualMemory::Reserve(Memory::Bytes size) noexcept
    {
        return HALVirtualMemory::Reserve(size);
    }

    Memory::RWByteSpan VirtualMemory::Allocate(Memory::Bytes size) noexcept
    {
        return HALVirtualMemory::Allocate(size);
    }

    Bool VirtualMemory::Release(const Memory::RWByteSpan& byte_span) noexcept
    {
        return HALVirtualMemory::Release(byte_span);
    }

    Bool VirtualMemory::Commit(const Memory::RWByteSpan& byte_span) noexcept
    {
        return HALVirtualMemory::Commit(byte_span);
    }

    Bool VirtualMemory::Decommit(const Memory::RWByteSpan& byte_span) noexcept
    {
        return HALVirtualMemory::Decommit(byte_span);
    }

}

