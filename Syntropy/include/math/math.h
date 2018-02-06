
/// \file math.h
/// \brief This header is part of the syntropy math system. It contains generic math functions.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <cstdint>
#include <cstdlib>
#include <type_traits>
#include <cmath>
#include <algorithm>

#include "platform/builtin.h"

namespace syntropy
{
    /// \brief Exposes math functionalities.
    /// \author Raffaele D. Facendola - 2017
    class Math
    {
    public:

        /// \brief Check whether a number is a power of 2.
        /// \return Returns true if number is a power of 2, returns false otherwise.
        template <typename TUnsigned, typename = std::enable_if_t<std::is_unsigned<TUnsigned>::value>>
        static constexpr bool IsPow2(TUnsigned number)
        {
            return (number & (number - 1u)) == 0u;
        }

        /// \brief Ceil a number to a multiple.
        /// This overload does participate for overload resolution only for unsigned values.
        /// \return Returns the first number equal or greater than number which is multiple of multiple.
        template <typename TUnsigned, typename = std::enable_if_t<std::is_unsigned<TUnsigned>::value>>
        static constexpr TUnsigned Ceil(TUnsigned number, TUnsigned multiple)
        {
            return Floor(number + multiple - 1, multiple);
        }

        /// \brief Floor a number to a multiple.
        /// This overload does participate for overload resolution only for unsigned values.
        /// \return Returns the first number equal or lesser than number which is multiple of multiple.
        template <typename TUnsigned, typename = std::enable_if_t<std::is_unsigned<TUnsigned>::value>>
        static constexpr TUnsigned Floor(TUnsigned number, TUnsigned multiple)
        {
            return (number / multiple) * multiple;
        }

        /// \brief Get the base 2 logarithm of a number and ceil the result to the next integer value.
        /// This overload does participate for overload resolution only for unsigned values.
        /// \return Returns the base 2 logarithm of the provided number rounded up to the next integer value. If the provided number is 0 the result is also 0.
        template <typename TUnsigned, typename = std::enable_if_t<std::is_unsigned<TUnsigned>::value>>
        static TUnsigned CeilLog2(TUnsigned number)
        {
            if (number > 0)
            {
                auto msb = platform::BuiltIn::GetMostSignificantBit(static_cast<uint64_t>(number));
                return msb + ((number & (number - 1)) >> msb);        // Ceiling required for non-power of 2.
            }

            return 0;
        }

        /// \brief Get the base 2 logarithm of a number and floor the result to the previous integer value.
        /// This overload does participate for overload resolution only for unsigned values.
        /// \return Returns the base 2 logarithm of the provided number rounded down to the previous integer value. If the provided number is 0 the result is also 0.
        template <typename TUnsigned, typename = std::enable_if_t<std::is_unsigned<TUnsigned>::value>>
        static TUnsigned FloorLog2(TUnsigned number)
        {
            return number > 0 ?
                platform::BuiltIn::GetMostSignificantBit(static_cast<uint64_t>(number)) :
                0;
        }

        /// \brief Round the argument up to the next power of 2.
        /// This overload does participate for overload resolution only for unsigned values.
        /// \return Returns the smallest power of 2 greater than or equal to number.
        template <typename TUnsigned, typename = std::enable_if_t<std::is_unsigned<TUnsigned>::value>>
        static TUnsigned NextPow2(TUnsigned number)
        {
            return IsPow2(number) ?
                number :
                2 << platform::BuiltIn::GetMostSignificantBit(static_cast<uint64_t>(number));
        }

        // Utilities.

        /// \brief Get the remainder after the division of lhs by rhs.
        /// \remarks The sign of the remainder is the same as lhs's.
        template <typename TNumber>
        static TNumber Modulus(TNumber lhs, TNumber rhs)
        {
            static_assert(std::is_arithmetic_v<TNumber>, "TNumber must be floating point or integer.");

            if constexpr(std::is_floating_point_v<TNumber>)
            {
                return std::fmod(lhs,rhs);
            }
            else
            {
                return lhs % rhs;
            }
        }

        // Comparison.

        /// \brief Check whether two real numbers are approximately equal.
        /// \param epsilon Error percentage relative to the absolute larger number.
        template <typename TReal>
        static bool ApproximatelyEqual(TReal lhs, TReal rhs, TReal epsilon = 0.01)
        {
            static_assert(std::is_floating_point_v<TReal>, "TReal must be floating point.");

            auto abs_lhs = FastAbs(lhs);
            auto abs_rhs = FastAbs(rhs);

            return FastAbs(lhs - rhs) <= ((abs_lhs < abs_rhs ? abs_rhs : abs_lhs) * epsilon);1
        }

        /// \brief Check whether two real numbers are essentially equal.
        /// \param epsilon Error percentage relative to the absolute smaller number.
        template <typename TReal>
        static bool EssentiallyEqual(TReal lhs, TReal rhs, TReal epsilon = 0.01)
        {
            static_assert(std::is_floating_point_v<TReal>, "TReal must be floating point.");

            auto abs_lhs = FastAbs(lhs);
            auto abs_rhs = FastAbs(rhs);

            return FastAbs(lhs - rhs) <= ((abs_lhs > abs_rhs ? abs_rhs : abs_lhs) * epsilon);
        }

        // Fast approximation functions.

        /// \brief Get the absolute value of number.
        template <typename TNumber>
        static TNumber FastAbs(TNumber rhs)
        {
            static_assert(std::is_arithmetic_v<TNumber>, "TNumber must be floating point or integer.");

            return rhs > 0 ? rhs : -rhs;
        }

        /// \brief Get the approximated inverse square root of number.
        static float FastInvSqrt(float rhs);

        /// \brief Get the square root of number.
        static float FastSqrt(float rhs);

        /// \brief Get the approximated hyperbolic tangent of rhs.
        static float FastTanh(float rhs);

        // Distance.

        /// \brief Get the Manhattan distance of point from the origin.
        /// \param coordinates Coordinates of the point.
        /// \return Returns the Manhattan distance of the point from the origin.
        template <typename... TNumbers>
        static decltype(auto) GetManhattanDistance(TNumbers... coordinates)
        {
            return ((FastAbs(coordinates) + ...));
        }

        /// \brief Get the Chebyshev distance of a point from the origin.
        /// \param coordinates Coordinates of the point.
        /// \return Returns the Manhattan distance of the point from the origin.
        template <typename... TNumbers>
        static decltype(auto) GetChebyshevDistance(TNumbers... coordinates)
        {
            return std::max({ FastAbs(coordinates) ... });
        }
    };

}