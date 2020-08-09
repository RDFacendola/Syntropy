#ifdef _WIN64

#include "syntropy/hal/hal_virtual_memory.h"

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"

/************************************************************************/
/* HEADERS & LIBRARIES                                                  */
/************************************************************************/

#pragma warning(push)
#pragma warning(disable:4091)

#include <Windows.h>
#include <DbgHelp.h>

#undef max

#pragma warning(pop)

namespace syntropy
{
    /************************************************************************/
    /* HAL MEMORY                                                           */
    /************************************************************************/

    Bytes HALMemory::VirtualPageSize() noexcept
    {
        SYSTEM_INFO system_info;

        GetSystemInfo(&system_info);

        return Bytes{ system_info.dwPageSize };
    }

    Alignment HALMemory::VirtualPageAlignment() noexcept
    {
        // Virtual memory pages are aligned to page-size boundaries.

        return ToAlignment(VirtualPageSize());
    }

    RWByteSpan HALMemory::VirtualAllocate(Bytes size) noexcept
    {
        // Allocate up to the next page boundary.

        if (size > Bytes{ 0 })
        {
            auto data = VirtualAlloc(0, ToInt(size), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

            return { ToRWBytePtr(data), ToInt(size) };
        }

        return {};
    }

    RWByteSpan HALMemory::VirtualReserve(Bytes size) noexcept
    {
        // Reserve up to the next page boundary.

        if (size > Bytes{ 0 })
        {
            auto data = VirtualAlloc(0, ToInt(size), MEM_RESERVE, PAGE_READWRITE);

            return { ToRWBytePtr(data), ToInt(size) };
        }

        return {};
    }

    Bool HALMemory::VirtualRelease(const RWByteSpan& byte_span) noexcept
    {
        if (byte_span)
        {
            // Deallocate the entire previously-allocated range.

            return VirtualFree(byte_span.GetData(), 0, MEM_RELEASE) != 0;
        }

        return true;
    }

    Bool HALMemory::VirtualCommit(const RWByteSpan& byte_span) noexcept
    {
        if (byte_span)
        {
            auto size = ToInt(Memory::Size(byte_span));

            // Commit each page containing at least one byte in the range.

            return VirtualAlloc(byte_span.GetData(), size, MEM_COMMIT, PAGE_READWRITE) != nullptr;
        }

        return true;
    }

    Bool HALMemory::VirtualDecommit(const RWByteSpan& byte_span) noexcept
    {
        if (byte_span)
        {
            auto size = ToInt(Memory::Size(byte_span));

            // Decommit each page containing at least one byte in the range.

            return VirtualFree(byte_span.GetData(), size, MEM_DECOMMIT) != 0;
        }

        return true;
    }

}

#endif