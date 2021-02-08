
/// \file string.inl
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/support/compare.h"
#include "syntropy/memory/allocators/allocator.h"
#include "syntropy/memory/foundation/buffer.h"
#include "syntropy/memory/foundation/memory.h"
#include "syntropy/memory/foundation/byte_span.h"

// ===========================================================================

namespace Syntropy::Strings
{
    /************************************************************************/
    /* STRING                                                               */
    /************************************************************************/

    template <Int TSize>
    String
    ::String(String::TCharacters<TSize> characters) noexcept
        : characters_(Memory::ToBytes(TSize))
    {
        Memory::Copy(characters_.GetRange(), Memory::BytesOf(characters));
    }

    [[nodiscard]] inline
    Int String
    ::GetLength() const noexcept
    {
        return ToInt(characters_.GetCount()) - 1;
    }

    [[nodiscard]] inline
    Mutable<Memory::BaseAllocator> String
    ::GetAllocator() const noexcept
    {
        return characters_.GetAllocator();
    }


}

// ===========================================================================
