
/// \file math.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <cstdint>
#include <type_traits>

#include "platform/builtin.h"

namespace syntropy
{

    class Math
    {
    public:

        /// \brief Check whether a number is a power of 2.
        /// \return Returns true if number is a power of 2, returns false otherwise.
        template <typename Unsigned, typename = std::enable_if_t<std::is_unsigned<Unsigned>::value>>
        static constexpr bool IsPow2(Unsigned number)
        {
            return (number & (number - 1u)) == 0u;
        }

        /// \brief Ceil a number to a multiple.
        /// This overload does participate for overload resolution only for unsigned values.
        /// \return Returns the first number equal or greater than number which is multiple of multiple.
        template <typename Unsigned, typename = std::enable_if_t<std::is_unsigned<Unsigned>::value>>
        static constexpr Unsigned Ceil(Unsigned number, Unsigned multiple)
        {
            return Floor(number + multiple - 1, multiple);
        }

        /// \brief Floor a number to a multiple.
        /// This overload does participate for overload resolution only for unsigned values.
        /// \return Returns the first number equal or lesser than number which is multiple of multiple.
        template <typename Unsigned, typename = std::enable_if_t<std::is_unsigned<Unsigned>::value>>
        static constexpr Unsigned Floor(Unsigned number, Unsigned multiple)
        {
            return (number / multiple) * multiple;
        }

        /// \brief Get the base 2 logarithm of a number and ceil the result to the next integer value.
        /// This overload does participate for overload resolution only for unsigned values.
        /// \return Returns the base 2 logarithm of the provided number rounded up to the next integer value. If the provided number is 0 the result is also 0.
        template <typename Unsigned, typename = std::enable_if_t<std::is_unsigned<Unsigned>::value>>
        static Unsigned CeilLog2(Unsigned number)
        {
            if (number > 0)
            {
                auto msb = platform::GetBuiltIn().GetMostSignificantBit(static_cast<uint64_t>(number));
                return msb + ((number & (number - 1)) >> msb);        // Ceiling required for non-power of 2.
            }

            return 0;
        }

        /// \brief Get the base 2 logarithm of a number and floor the result to the previous integer value.
        /// This overload does participate for overload resolution only for unsigned values.
        /// \return Returns the base 2 logarithm of the provided number rounded down to the previous integer value. If the provided number is 0 the result is also 0.
        template <typename Unsigned, typename = std::enable_if_t<std::is_unsigned<Unsigned>::value>>
        static Unsigned FloorLog2(Unsigned number)
        {
            return number > 0 ?
                   platform::GetBuiltIn().GetMostSignificantBit(static_cast<uint64_t>(number)) :
                   0;
        }

        /// \brief Round the argument up to the next power of 2.
        /// This overload does participate for overload resolution only for unsigned values.
        /// \return Returns the smallest power of 2 greater than or equal to number.
        template <typename Unsigned, typename = std::enable_if_t<std::is_unsigned<Unsigned>::value>>
        static Unsigned NextPow2(Unsigned number)
        {
            return IsPow2(number) ?
                number :
                2 << platform::GetBuiltIn().GetMostSignificantBit(static_cast<uint64_t>(number));
        }

    };

}