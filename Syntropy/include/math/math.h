
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

        /// \brief Ceil a number to a multiple.
        /// This overload does participate for overload resolution only for unsigned values.
        /// \return Returns the first number equal or greater than number which is multiple of multiple.
        template <typename Unsigned, typename = std::enable_if_t<std::is_unsigned_v<Unsigned>>>
        static constexpr Unsigned Ceil(Unsigned number, Unsigned multiple)
        {
            return Floor(number + multiple - 1, multiple);
        }

        /// \brief Floor a number to a multiple.
        /// This overload does participate for overload resolution only for unsigned values.
        /// \return Returns the first number equal or lesser than number which is multiple of multiple.
        template <typename Unsigned, typename = std::enable_if_t<std::is_unsigned_v<Unsigned>>>
        static constexpr Unsigned Floor(Unsigned number, Unsigned multiple)
        {
            return (number / multiple) * multiple;
        }

        /// \brief Get the base 2 logarithm of a number and ceil the result to the next integer value.
        /// This overload does participate for overload resolution only for unsigned values.
        /// \return Returns the base 2 logarithm of the provided number rounded up to the next integer value. If the provided number is 0 the result is also 0.
        template <typename Unsigned, typename = std::enable_if_t<std::is_unsigned_v<Unsigned>>>
        static Unsigned CeilLog2(Unsigned number)
        {
            if (number > 0)
            {
                auto msb = platform::GetBuiltIn().GetMostSignificantBit(static_cast<uint64_t>(number));
                return msb + ((number & (number - 1)) >> msb);        // Ceiling required for non-power of 2.
            }

            return 0;
        }

    };

}