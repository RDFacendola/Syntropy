
/// \file string_view.inl
///
/// \author Raffaele D. Facendola - February 2021.

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
        : code_units_(code_points)
    {

    }

    [[nodiscard]] inline Immutable<Memory::ByteSpan>
    StringView
    ::GetCodeUnits() const noexcept
    {
        return code_units_;
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Comparison.
    // ===========

    inline Bool
    operator==(Immutable<StringView> lhs, Immutable<StringView> rhs)
    noexcept
    {
        return (lhs.GetCodeUnits() == rhs.GetCodeUnits());
    }

    template <Int TSize>
    inline Bool
    operator==(Immutable<StringView> lhs, StringLiteral<TSize> rhs)
    noexcept
    {
        return (lhs.GetCodeUnits() == Memory::MakeByteSpan(rhs));
    }

    template <Int TSize>
    inline Bool
    operator==(StringLiteral<TSize> lhs, Immutable<StringView> rhs)
    noexcept
    {
        return (Memory::MakeByteSpan(lhs) == rhs.GetCodeUnits());
    }

}

// ===========================================================================
