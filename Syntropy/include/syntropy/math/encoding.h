/// \file encoding.h
/// \brief This header is part of the syntropy math system. It contains encoding methods for numbers.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <cstdint>
#include <cstdlib>

namespace syntropy
{

    /// \brief Encode a binary number according to the Gray code.
    /// \param rhs Number to encode.
    /// \return Returns rhs encoded according to the Gray code.
    template <typename TNumber>
    constexpr TNumber ToGrayCode(TNumber rhs);

    /// \brief Decode a Gray code-encoded number to binary.
    /// \param rhs Number to decode.
    /// \return Returns rhs decoded according to the Gray code.
    template <typename TNumber>
    constexpr TNumber FromGrayCode(TNumber rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    template <typename TNumber>
    constexpr TNumber ToGrayCode(TNumber rhs)
    {
        static_assert(std::is_integral_v<TNumber>, "TNumber must be integral.");

        return rhs ^ (rhs >> TNumber(1));
    }

    template <typename TNumber>
    constexpr TNumber FromGrayCode(TNumber rhs)
    {
        static_assert(std::is_integral_v<TNumber>, "TNumber must be integral.");

        for (auto mask = rhs >> TNumber(1); mask != 0; mask >>= TNumber(1))
        {
            rhs = rhs ^ mask;
        }

        return rhs;
    }

}
