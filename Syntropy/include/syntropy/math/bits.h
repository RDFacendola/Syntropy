
/// \file bits.h
/// \brief This header is part of the Syntropy math module. It contains mathematical functions specific for binary numbers and powers of 2.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <cstdint>
#include <cmath>
#include <limits>

#include "syntropy/core/types.h"
#include "syntropy/math/numeric.h"
#include "syntropy/core/types.h"
#include "syntropy/platform/intrinsics.h"

namespace syntropy
{
    /************************************************************************/
    /* MATH                                                                 */
    /************************************************************************/

    /// \brief Exposes arithmetic functions specific for binary numbers and powers of 2.
    /// \author Raffaele D. Facendola - May 2020.
    namespace Math
    {
        /// \brief Get the base 2 logarithm of a number and ceil the result to the next integer value.
        /// If the provided number is 0 or less the result is undefined.
        /// \return Returns the base 2 logarithm of the provided number rounded up to the next integer value.
        template <typename TNumber>
        TNumber CeilLog2(TNumber rhs);

        /// \brief Get the base 2 logarithm of a number and floor the result to the previous integer value.
        /// If the provided number is 0 or less the result is undefined.
        /// \return Returns the base 2 logarithm of the provided number rounded down to the previous integer value.
        template <typename TNumber>
        TNumber FloorLog2(TNumber rhs);

        /// \brief Round the argument up to the next power of 2.
        /// \return Returns the smallest power of 2 greater than or equal to rhs.
        template <typename TNumber>
        TNumber NextPow2(TNumber rhs);

        /// \brief Check whether rhs is a power of 2.
        template <typename TNumber>
        constexpr Bool IsPow2(TNumber rhs);

        /// \brief Count the number of bits equal to 1 in the provided number.
        /// \return Returns the number of bits equal to 1 in the provided number.
        constexpr Int HammingWeight(Int rhs);

        /// \brief Get the Hamming distance between two numbers.
        /// \return Returns the Hamming distance between lhs and rhs, that is the number of different bits in both numbers.
        constexpr Int HammingDistance(Int lhs, Int rhs);
    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Math.

    template <typename TNumber>
    inline TNumber Math::CeilLog2(TNumber rhs)
    {
        if constexpr (std::is_integral_v<TNumber>)
        {
            // Bit magic for integral numbers.

            auto msb = Intrinsics::GetMostSignificantBit(static_cast<int64_t>(rhs));

            return static_cast<TNumber>(msb + ((rhs & (rhs - TNumber(1))) >> msb));
        }
        else
        {
            // Standard functions for real numbers.

            return std::ceil(std::log2(rhs));
        }
    }

    template <typename TNumber>
    inline TNumber Math::FloorLog2(TNumber rhs)
    {
        if constexpr (std::is_integral_v<TNumber>)
        {
            // Bit magic for integral numbers.

            return Intrinsics::GetMostSignificantBit(static_cast<int64_t>(rhs));
        }
        else
        {
            // Standard functions for real numbers.

            return std::floor(std::log2(rhs));
        }
    }

    template <typename TNumber>
    inline TNumber Math::NextPow2(TNumber rhs)
    {
        if constexpr (std::is_integral_v<TNumber>)
        {
            // Bit magic for integral values.

            if (rhs > TNumber{ 0 })
            {
                return IsPow2(rhs) ? rhs : TNumber(2 << Intrinsics::GetMostSignificantBit(static_cast<int64_t>(rhs)));
            }

            return TNumber{ 0 };
        }
        else
        {
            // Decomposition into exponent\fraction for real numbers.

            auto exponent= int{};
            auto fraction = std::frexp(rhs, &exponent);
            
            return std::scalbn(std::ceil(fraction), exponent);
        }
    }

    template <typename TNumber>
    constexpr Bool Math::IsPow2(TNumber rhs)
    {
        if constexpr (std::is_integral_v<TNumber>)
        {
            return (rhs > 0) ? (rhs & (rhs - 1)) == 0 : false;
        }
        else
        {
            auto exponent = int{};

            return Math::EssentiallyEqual(std::frexp(rhs, &exponent), 0.5f);
        }
    }

    constexpr Int Math::HammingWeight(Int rhs)
    {
        // Keringhan's algorithm, runs in O(log(bits)).

        auto count = Int{ 0 };

        for (; rhs != 0; rhs &= (rhs - 1))
        {
            ++count;
        }

        return count;
    }

    constexpr Int Math::HammingDistance(Int lhs, Int rhs)
    {
        return Math::HammingWeight(lhs ^ rhs);
    }

}