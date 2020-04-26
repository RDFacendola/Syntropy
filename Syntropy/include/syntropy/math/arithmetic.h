
/// \file arithmetic.h
/// \brief This header is part of the syntropy math module. It contains generic arithmetic functions.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

namespace syntropy
{
    /************************************************************************/
    /* ARITHMETIC                                                           */
    /************************************************************************/

    /// \brief Divide lhs by rhs and ceil the result to the next integer value.
    /// \return Returns lhs divided by rhs, rounded up to the next integer value.
    template <typename TNumber>
    constexpr auto DivCeil(TNumber lhs, TNumber rhs);

    /// \brief Divide lhs by rhs and floor the result to the previous integer value.
    /// \return Returns lhs divided by rhs, rounded down to the previous value.
    template <typename TNumber>
    constexpr auto DivFloor(TNumber lhs, TNumber rhs);

    /// \brief Ceil a number to a multiple of another value.
    /// \return Returns the first number equal or greater than number which is multiple of multiple.
    template <typename TNumber>
    constexpr TNumber Ceil(TNumber rhs, TNumber multiple);

    /// \brief Floor a number to a multiple.
    /// \return Returns the first number equal or lesser than number which is multiple of multiple.
    template <typename TNumber>
    constexpr TNumber Floor(TNumber rhs, TNumber multiple);

    /// \brief Check whether rhs is a power of 2.
    template <typename TNumber>
    constexpr bool IsPow2(TNumber rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Arithmetic.

    template <typename TNumber>
    constexpr auto DivCeil(TNumber lhs, TNumber rhs)
    {
        return DivFloor(lhs + rhs - TNumber(1), rhs);
    }

    template <typename TNumber>
    constexpr auto DivFloor(TNumber lhs, TNumber rhs)
    {
        auto quotient = lhs / rhs;
        auto remainder = lhs % rhs;

        if (remainder != decltype(remainder){ 0 })
        {
            quotient -= (lhs < TNumber{ 0 }) ^ (rhs < TNumber{ 0 });
        }

        return quotient;
    }

    template <typename TNumber>
    constexpr TNumber Ceil(TNumber rhs, TNumber multiple)
    {
        return DivCeil(rhs, multiple) * multiple;
    }

    template <typename TNumber>
    constexpr TNumber Floor(TNumber rhs, TNumber multiple)
    {
        return DivFloor(rhs, multiple) * multiple;
    }

    template <typename TNumber>
    constexpr bool IsPow2(TNumber rhs)
    {
        if (rhs > TNumber{ 0 })
        {
            return ((rhs & (rhs - TNumber(1))) == TNumber(0));
        }

        return false;
    }

}