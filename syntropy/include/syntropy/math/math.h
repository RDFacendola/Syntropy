
/// \file math.h
///
/// \brief This header is part of the Syntropy math module.
///        It contains generic mathematical functions.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/type_traits.h"

// ===========================================================================

namespace Syntropy::Math
{
    /************************************************************************/
    /* MATH                                                                 */
    /************************************************************************/

    /// \brief Get the minimum element among first and rest.
    template <typename TValue, typename... TValues>
    [[nodiscard]] constexpr Templates::CommonType<TValue, TValues...>
    Min(Immutable<TValue> first, Immutable<TValues>... rest) noexcept;

    /// \brief Get the maximum element among first and rest.
    template <typename TValue, typename... TValues>
    [[nodiscard]] constexpr Templates::CommonType<TValue, TValues...>
    Max(Immutable<TValue> first, Immutable<TValues>... rest) noexcept;

    /// \brief Get the smallest integral number greater than or equal to rhs.
    template <typename TNumber>
    [[nodiscard]] constexpr TNumber
    Ceil(TNumber rhs);

    /// \brief Get the largest integral number smaller than or equal to rhs.
    template <typename TNumber>
    [[nodiscard]] constexpr TNumber
    Floor(TNumber rhs);

    /// \brief Ceil a number to a multiple of another value.
    /// \return Returns the first number equal or greater than number which is
    ///         multiple of multiple.
    template <typename TNumber>
    [[nodiscard]] constexpr TNumber
    Ceil(TNumber rhs, TNumber multiple);

    /// \brief Floor a number to a multiple.
    /// \return Returns the first number equal or lesser than number which is
    ///         multiple of multiple.
    template <typename TNumber>
    [[nodiscard]] constexpr TNumber
    Floor(TNumber rhs, TNumber multiple);

    /// \brief Divide lhs by rhs and ceil the result to the next integer value.
    /// \return Returns lhs divided by rhs, rounded up to the next
    ///        integer value.
    template <typename TNumber>
    [[nodiscard]] constexpr auto
    DivCeil(TNumber lhs, TNumber rhs);

    /// \brief Divide lhs by rhs and floor the result to the previous integer
    ///        value.
    /// \return Returns lhs divided by rhs, rounded down to the previous
    ///         integer value.
    template <typename TNumber>
    [[nodiscard]] constexpr auto
    DivFloor(TNumber lhs, TNumber rhs);

}

// ===========================================================================

#include "math.inl"

// ===========================================================================
