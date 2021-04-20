
/// \file string.inl
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/ranges/sized_range.h"
#include "syntropy/core/comparisons/compare.h"
#include "syntropy/core/algorithms/swap.h"

#include "syntropy/memory/allocators/allocator.h"
#include "syntropy/memory/foundation/buffer.h"
#include "syntropy/memory/foundation/memory.h"
#include "syntropy/memory/foundation/byte_span.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* STRING                                                               */
    /************************************************************************/

    inline String
    ::String (Null) noexcept
        : String()
    {

    }

    template <Int TSize>
    String
    ::String(StringLiteral<TSize> characters) noexcept
        : code_units_(Memory::ToBytes(TSize))
    {
        Memory::Copy(code_units_,
                     Memory::MakeByteSpan(characters));
    }

    inline
    String
    ::operator StringView() const noexcept
    {
        return StringView{ code_units_ };
    }

    [[nodiscard]] inline Memory::ByteSpan
    String
    ::GetCodeUnits() const noexcept
    {
        return ViewOf(code_units_);
    }

    [[nodiscard]] inline Mutable<Memory::BaseAllocator>
    String
    ::GetAllocator() const noexcept
    {
        return code_units_.GetAllocator();
    }

    inline void
    String
    ::Swap(Movable<String> rhs) noexcept
    {
        Algorithms::Swap(code_units_, rhs.code_units_);
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Comparison.
    // ===========

    inline Bool
    operator==(Immutable<String> lhs, Immutable<String> rhs) noexcept
    {
        return ViewOf(lhs) == ViewOf(rhs);
    }

    template <Int TSize>
    inline Bool
    operator==(Immutable<String> lhs, StringLiteral<TSize> rhs) noexcept
    {
        return ViewOf(lhs) == rhs;
    }

    template <Int TSize>
    inline Bool
    operator==(StringLiteral<TSize> lhs, Immutable<String> rhs) noexcept
    {
        return lhs == ViewOf(rhs);
    }

    // Ranges.
    // =======

    [[nodiscard]] inline StringView
    ViewOf(Immutable<String> string) noexcept
    {
        return string;
    }

}

// ===========================================================================
