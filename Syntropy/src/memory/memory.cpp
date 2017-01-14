#include "memory/memory.h"

#include <cstdlib>
#include <cstring>
#include <algorithm>

#include "platform/os.h"

#include "syntropy.h"

void* operator new(std::size_t size)
{
    return std::malloc(size);
}

void operator delete(void* pointer) noexcept 
{
    std::free(pointer);
}

namespace syntropy
{

    const diagnostics::Context MemoryCtx("Memory");

    //////////////// MEMORY ////////////////

    VirtualMemoryRange::VirtualMemoryRange(void* base, void* end)
        : base_(reinterpret_cast<int8_t*>(base))
        , end_(reinterpret_cast<int8_t*>(end))
    {
        SYNTROPY_ASSERT(base_);                                                                     // Non-null memory range
        SYNTROPY_ASSERT(end_);
        SYNTROPY_ASSERT(reinterpret_cast<uintptr_t>(base_) <= reinterpret_cast<uintptr_t>(end_));   // End address past the base address.
    }

    int8_t* VirtualMemoryRange::operator*() const
    {
        return base_;
    }

    size_t VirtualMemoryRange::GetCapacity() const
    {
        return Memory::GetRangeSize(base_, end_);
    }

    bool VirtualMemoryRange::Allocate(void* address, size_t size)
    {
        SYNTROPY_ASSERT(reinterpret_cast<uintptr_t>(address) >= reinterpret_cast<uintptr_t>(base_));                            // Allocation out of bounds
        SYNTROPY_ASSERT(reinterpret_cast<uintptr_t>(Memory::Offset(address, size)) <= reinterpret_cast<uintptr_t>(end_));

        return GetMemory().Commit(address, size);
    }

    bool VirtualMemoryRange::Free(void* address, size_t size)
    {
        SYNTROPY_ASSERT(reinterpret_cast<uintptr_t>(address) >= reinterpret_cast<uintptr_t>(base_));                            // Allocation out of bounds
        SYNTROPY_ASSERT(reinterpret_cast<uintptr_t>(Memory::Offset(address, size)) <= reinterpret_cast<uintptr_t>(end_));

        return GetMemory().Decommit(address, size);
    }

    void VirtualMemoryRange::Free()
    {
        auto result = GetMemory().Decommit(base_, GetCapacity());

        SYNTROPY_ASSERT(result);    // Was the memory freed?
    }

    //////////////// MEMORY ////////////////

    Memory& Memory::GetInstance()
    {
        return platform::PlatformMemory::GetInstance();
    }

    Memory& GetMemory()
    {
        return Memory::GetInstance();
    }

    size_t Memory::CeilToAllocationGranularity(size_t size)
    {
        return Math::Ceil(size, GetMemory().GetAllocationGranularity());
    }

}