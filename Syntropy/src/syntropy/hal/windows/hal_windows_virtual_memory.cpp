#ifdef _WIN64

#include "syntropy/hal/hal_virtual_memory.h"

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_span.h"

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
    /* HAL VIRTUAL MEMORY                                                   */
    /************************************************************************/

    Bytes HALVirtualMemory::GetPageSize()
    {
        SYSTEM_INFO system_info;

        GetSystemInfo(&system_info);

        return Bytes(system_info.dwPageSize);
    }

    Alignment HALVirtualMemory::GetPageAlignment()
    {
        SYSTEM_INFO system_info;

        GetSystemInfo(&system_info);

        // Virtual memory pages are aligned to page-size boundaries.

        return ToAlignment(system_info.dwPageSize);
    }

    MemorySpan HALVirtualMemory::Allocate(Bytes size)
    {
        // Allocate up to the next page boundary.

        if (size > Bytes{ 0 })
        {
            auto data = VirtualAlloc(0, ToInt(size), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

            return { data, size };
        }

        return {};
    }

    MemorySpan HALVirtualMemory::Reserve(Bytes size)
    {
        // Reserve up to the next page boundary.

        if (size > Bytes{ 0 })
        {
            auto data = VirtualAlloc(0, ToInt(size), MEM_RESERVE, PAGE_READWRITE);

            return { data, size };
        }

        return {};
    }

    Bool HALVirtualMemory::Release(const MemorySpan& memory_span)
    {
        if (memory_span)
        {
            // Deallocate the entire previously-allocated range.

            return VirtualFree(memory_span.GetData(), 0, MEM_RELEASE) != 0;
        }

        return true;
    }

    Bool HALVirtualMemory::Commit(const MemorySpan& memory_span)
    {
        if (memory_span)
        {
            auto size = ToInt(memory_span.GetSize());

            // Commit each page containing at least one byte in the range.

            return VirtualAlloc(memory_span.GetData(), size, MEM_COMMIT, PAGE_READWRITE) != nullptr;
        }

        return true;
    }

    Bool HALVirtualMemory::Decommit(const MemorySpan& memory_span)
    {
        if (memory_span)
        {
            auto size = ToInt(memory_span.GetSize());

            // Decommit each page containing at least one byte in the range.

            return VirtualFree(memory_span.GetData(), size, MEM_DECOMMIT) != 0;
        }

        return true;
    }

}

#endif