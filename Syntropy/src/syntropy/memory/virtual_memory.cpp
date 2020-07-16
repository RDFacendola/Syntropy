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

    RWByteSpan VirtualMemory::Reserve(Bytes size)
    {
        return HALVirtualMemory::Reserve(size);
    }

    RWByteSpan VirtualMemory::Allocate(Bytes size)
    {
        return HALVirtualMemory::Allocate(size);
    }

    Bool VirtualMemory::Release(const RWByteSpan& byte_span)
    {
        return HALVirtualMemory::Release(byte_span);
    }

    Bool VirtualMemory::Commit(const RWByteSpan& byte_span)
    {
        return HALVirtualMemory::Commit(byte_span);
    }

    Bool VirtualMemory::Decommit(const RWByteSpan& byte_span)
    {
        return HALVirtualMemory::Decommit(byte_span);
    }

}

