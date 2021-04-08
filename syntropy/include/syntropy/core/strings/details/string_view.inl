
/// \file string_view.inl
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* STRING VIEW                                                          */
    /************************************************************************/

    constexpr StringView
    ::StringView(Null) noexcept
        : StringView()
    {

    }

    constexpr StringView
    ::StringView(Immutable<Memory::ByteSpan> code_points) noexcept
        : code_points_(code_points)
    {

    }

    [[nodiscard]] Immutable<Memory::ByteSpan>
    StringView
    ::GetCodePoints() const noexcept
    {
        return code_points_;
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Comparison.
    // ===========

    Bool
    operator==(Immutable<StringView> lhs, Immutable<StringView> rhs)
    noexcept
    {
        return (lhs.GetCodePoints() == rhs.GetCodePoints());
    }

    template <Int TSize> Bool
    operator==(Immutable<StringView> lhs, StringLiteral<TSize> rhs)
    noexcept
    {
        return (lhs.GetCodePoints() == Memory::MakeByteSpan(rhs));
    }

    template <Int TSize> Bool
    operator==(StringLiteral<TSize> lhs, Immutable<StringView> rhs)
    noexcept
    {
        return (Memory::MakeByteSpan(lhs) == rhs.GetCodePoints());
    }

}

// ===========================================================================
