
/// \file string.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for strings.
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/memory/allocators/allocator.h"
#include "syntropy/memory/foundation/buffer.h"
#include "syntropy/memory/foundation/size.h"
#include "syntropy/memory/foundation/byte.h"

#include "syntropy/core/support/compare.h"
#include "syntropy/core/strings/string_view.h"

// ===========================================================================

namespace Syntropy::Strings
{
    /************************************************************************/
    /* STRING                                                               */
    /************************************************************************/

    /// \brief An UTF-8-encoded contiguous sequence of immutable characters.
    //
    /// \author Raffaele D. Facendola - February 2021.
    class String
    {
    public:

        /// \brief Create an empty string.
        String () noexcept = default;

        /// \brief Create an empty string.
        String (Null) noexcept;

        /// \brief Initialize a string from a characters sequence.
        template <Int TSize>
        String(StringLiteral<TSize> characters) noexcept;

        /// \brief Default copy constructor.
        String(Immutable<String> rhs) noexcept = default;

        /// \brief Default move constructor.
        String(Movable<String> rhs) noexcept = default;

        /// \brief Default copy-assignment operator.
        Mutable<String>
        operator=(Immutable<String> rhs) noexcept = default;

        /// \brief Default move-assignment operator.
        /// \remarks @rfacendola Clang doesn't recognize Movable<String> rhs.
        Mutable<String>
        operator=(String&& rhs) noexcept = default;

        /// \brief Default destructor.
        ~String() noexcept = default;

        /// \brief Implicit conversion to StringView.
        operator StringView() const noexcept;

        /// \brief Swap this string with another one.
        void
        Swap(Movable<String> rhs) noexcept;

        /// \brief Access the underlying code-points.
        [[nodiscard]] Memory::ByteSpan
        GetCodePoints() const noexcept;

        /// \brief Get the allocator the string was allocated on.
        [[nodiscard]] Mutable<Memory::BaseAllocator>
        GetAllocator() const noexcept;

    private:

        /// \brief Sequence of code-points. Each character is encoded by no
        ///        more than four bytes.
        Memory::Buffer code_points_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Comparison.
    // ===========

    /// \brief Check whether two strings are equal.
    Bool
    operator==(Immutable<String> lhs, Immutable<String> rhs) noexcept;

    /// \brief Check whether a string is equal to a characetr sequence.
    template <Int TSize> Bool
    operator==(Immutable<String> lhs, StringLiteral<TSize> rhs) noexcept;

    /// \brief Check whether a character sequence is equal to a string.
    template <Int TSize> Bool
    operator==(StringLiteral<TSize> rhs, Immutable<String> lhs) noexcept;

    // Ranges.
    // =======

    /// \brief Get a view to an immutable string.
    [[nodiscard]] StringView
    ViewOf(Immutable<String> string) noexcept;

    /// \brief Prevent from getting a view to a temporary string.
    void
    ViewOf(Immovable<String> string) noexcept = delete;

}

// ===========================================================================

#include "string.inl"

// ===========================================================================
