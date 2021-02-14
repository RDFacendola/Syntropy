
/// \file string.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for strings.
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/support/compare.h"
#include "syntropy/memory/allocators/allocator.h"
#include "syntropy/memory/foundation/buffer.h"

// ===========================================================================

namespace Syntropy::Strings
{
    /************************************************************************/
    /* STRING                                                               */
    /************************************************************************/

    /// \brief An encoding-agnostic contiguous sequence of
    ///        immutable characters.
    ///
    /// \author Raffaele D. Facendola - February 2021.
    class String
    {
    public:

        /// \brief Character array type.
        template <Int TSize>
        using TCharacters = const char8_t(&)[TSize];

        /// \brief Initialize a string from a characters sequence.
        template <Int TSize>
        String(TCharacters<TSize> characters) noexcept;

        /// \brief Get the string length, in characters.
        [[nodiscard]] Int
        GetLength() const noexcept;

        /// \brief Get the allocator the string was allocated on.
        [[nodiscard]] Mutable<Memory::BaseAllocator>
        GetAllocator() const noexcept;

    private:

        /// \brief String characters.
        Memory::Buffer characters_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

}

// ===========================================================================

#include "string.inl"

// ===========================================================================
