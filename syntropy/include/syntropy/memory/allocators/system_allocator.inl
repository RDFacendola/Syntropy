
/// \file system_allocator.h
///
/// \author Raffaele D. Facendola - 2017

#pragma once

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* SYSTEM ALLOCATOR                                                     */
    /************************************************************************/

    inline RWByteSpan SystemAllocator
    ::Allocate(Bytes size, Alignment alignment) noexcept
    {
        auto size_value = static_cast<std::size_t>(ToInt(size));
        auto alignment_value = static_cast<std::align_val_t>(ToInt(alignment));

        if (auto block = ::operator new(size_value,
                                        alignment_value,
                                        std::nothrow))
        {
            return { ToBytePtr(block), size };
        }

        return {};
    }

    inline void SystemAllocator
    ::Deallocate(Immutable<RWByteSpan> block, Alignment alignment) noexcept
    {
        auto alignment_value = static_cast<std::align_val_t>(ToInt(alignment));

        ::operator delete(block.GetData(), alignment_value, std::nothrow);
    }

}

// ===========================================================================
