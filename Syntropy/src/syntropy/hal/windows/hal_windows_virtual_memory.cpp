#ifdef _WIN64

#include "syntropy/hal/hal_virtual_memory.h"

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_range.h"

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

        return static_cast<std::align_val_t>(system_info.dwPageSize);
    }

    MemoryRange HALVirtualMemory::Allocate(Bytes size)
    {
        // Allocate up to the next page boundary.

        if (size > Bytes{ 0 })
        {
            MemoryAddress address = VirtualAlloc(0, *size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

            return { address, address + size };
        }

        return {};
    }

    MemoryRange HALVirtualMemory::Reserve(Bytes size)
    {
        // Reserve up to the next page boundary.

        if (size > Bytes{ 0 })
        {
            MemoryAddress address = VirtualAlloc(0, *size, MEM_RESERVE, PAGE_READWRITE);

            return { address, address + size };
        }

        return {};
    }

    Bool HALVirtualMemory::Release(const MemoryRange& memory_range)
    {
        if (memory_range)
        {
            // Deallocate the entire previously-allocated range.

            return VirtualFree(memory_range.Begin(), 0, MEM_RELEASE) != 0;
        }

        return true;
    }

    Bool HALVirtualMemory::Commit(const MemoryRange& memory_range)
    {
        if (memory_range)
        {
            auto size = *memory_range.GetSize();

            // Commit each page containing at least one byte in the range.

            return VirtualAlloc(memory_range.Begin(), size, MEM_COMMIT, PAGE_READWRITE) != nullptr;
        }

        return true;
    }

    Bool HALVirtualMemory::Decommit(const MemoryRange& memory_range)
    {
        if (memory_range)
        {
            auto size = *memory_range.GetSize();

            // Decommit each page containing at least one byte in the range.

            return VirtualFree(memory_range.Begin(), size, MEM_DECOMMIT) != 0;
        }

        return true;
    }

}

#endif