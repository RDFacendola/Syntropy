#include "memory/memory.h"

#include <algorithm>

#include "platform/compiler/compiler.h"
#include "macro.h"
#include "diagnostics/assert.h"

#include "syntropy.h"

namespace syntropy
{
    const diagnostics::Context MemoryCtx("SyntropyMemory");

    /************************************************************************/
    /* MEMORY DEBUG                                                         */
    /************************************************************************/

    void MemoryDebug::MarkUninitialized(void* begin, void* end)
    {
        SYNTROPY_UNUSED(begin);
        SYNTROPY_UNUSED(end);

        SYNTROPY_DEBUG_ONLY(
            std::fill(
                reinterpret_cast<int8_t*>(begin),
                reinterpret_cast<int8_t*>(end),
                kUninitializedMemoryPattern);
        );
    }

    void MemoryDebug::MarkFree(void* begin, void* end)
    {
        SYNTROPY_UNUSED(begin);
        SYNTROPY_UNUSED(end);

        SYNTROPY_DEBUG_ONLY(
            std::fill(
                reinterpret_cast<int8_t*>(begin),
                reinterpret_cast<int8_t*>(end),
                kFreeMemoryPattern);
        );
    }

    /************************************************************************/
    /* MEMORY RANGE                                                         */
    /************************************************************************/

    MemoryRange::MemoryRange()
        : base_(nullptr)
        , top_(nullptr)
    {

    }

    MemoryRange::MemoryRange(void* base, void* top)
        : base_(base)
        , top_(top)
    {
        SYNTROPY_ASSERT(reinterpret_cast<uintptr_t>(base) <= reinterpret_cast<uintptr_t>(top));
    }

    MemoryRange::MemoryRange(void* base, size_t size)
        : MemoryRange(base, Memory::AddOffset(base, size))
    {

    }

    MemoryRange::MemoryRange(const MemoryRange& other, size_t alignment)
        : MemoryRange(Memory::Align(other.base_, alignment), other.top_)
    {
        SYNTROPY_ASSERT(other.Contains(*this));
    }

    void* MemoryRange::operator*() const
    {
        return base_;
    }

    void* MemoryRange::operator[](size_t offset) const
    {
        auto ptr = Memory::AddOffset(base_, offset);

        SYNTROPY_ASSERT(Contains(ptr));

        return ptr;
    }

    void* MemoryRange::GetTop() const
    {
        return top_;
    }

    size_t MemoryRange::GetSize() const
    {
        return static_cast<size_t>(Memory::GetDistance(base_, top_));
    }

    bool MemoryRange::Contains(const MemoryRange& memory_range) const
    {
        return Memory::GetDistance(base_, memory_range.base_) >= 0 &&
            Memory::GetDistance(top_, memory_range.top_) <= 0;
    }

    bool MemoryRange::Contains(void* address) const
    {
        return Memory::GetDistance(base_, address) >= 0 &&
            Memory::GetDistance(top_, address) < 0;
    }

}

