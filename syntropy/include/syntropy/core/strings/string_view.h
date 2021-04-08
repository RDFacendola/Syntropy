
/// \file string_view.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for string views.
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

#include "syntropy/memory/foundation/byte_span.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* STRING VIEW                                                          */
    /************************************************************************/

    /// \brief A view to an UTF-8-encoded contiguous sequence of immutable
    ///        characters.
    //
    /// \author Raffaele D. Facendola - February 2021.
    class StringView
    {
    public:

        /// \brief Create an empty string view.
        constexpr
        StringView() noexcept = default;

        /// \brief Create an empty string view.
        constexpr
        StringView(Null) noexcept;

        /// \brief Create a view to a string.
        constexpr
        explicit StringView(Immutable<Memory::ByteSpan> code_points) noexcept;

        /// \brief Default copy-constructor.
        constexpr
        StringView(Immutable<StringView> rhs) noexcept;

        /// \brief Default copy-assignment operator.
        constexpr Mutable<StringView>
        operator=(Immutable<StringView> rhs) noexcept;

        /// \brief Default destructor.
        ~StringView() noexcept = default;

        /// \brief Access the string view code-points.
        [[nodiscard]] Immutable<Memory::ByteSpan>
        GetCodePoints() const noexcept;

    private:

        /// \brief Code-points this view refers to.
        Memory::ByteSpan code_points_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Comparison.
    // ===========

    /// \brief Check whether two string views are equal.
    template <typename TTraits, typename UTraits> Bool
    operator==(Immutable<StringView> lhs, Immutable<StringView> rhs) noexcept;

    /// \brief Check whether a string view is equal to a characetr sequence.
    template <typename TTraits, Int TSize> Bool
    operator==(Immutable<StringView> lhs, StringLiteral<TSize> rhs)
    noexcept;

    /// \brief Check whether a character sequence is equal to a string view.
    template <Int TSize, typename UTraits> Bool
    operator==(StringLiteral<TSize> lhs, Immutable<StringView> rhs)
    noexcept;

}

// ===========================================================================

#include "details/string_view.inl"

// ===========================================================================
