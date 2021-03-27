
/// \file compare.inl
///
/// \author Raffaele D. Facendola - March 2021

#pragma once

// ===========================================================================

namespace Syntropy::Algorithms
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Compare.
    // ========

    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    AreEqual(Immutable<TType> lhs, Immutable<UType> rhs) noexcept
        -> decltype(Details::RouteAreEqual(lhs, rhs))
    {
        return Details::RouteAreEqual(lhs, rhs);
    }

    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    AreEquivalent(Immutable<TType> lhs, Immutable<UType> rhs) noexcept
        -> decltype(Details::RouteAreEquivalent(lhs, rhs))
    {
        return Details::RouteAreEquivalent(lhs, rhs);
    }

    template <typename TType, typename UType>
    [[nodiscard]] constexpr auto
    Compare(Immutable<TType> lhs, Immutable<UType> rhs) noexcept
        -> decltype(Details::RouteCompare(lhs, rhs))
    {
        return Details::RouteCompare(lhs, rhs);
    }

}

// ===========================================================================
