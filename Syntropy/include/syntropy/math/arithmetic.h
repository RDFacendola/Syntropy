
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

    /// \brief Check whether rhs is a power of 2.
    template <typename TNumber>
    constexpr bool IsPow2(TNumber rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Arithmetic.

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