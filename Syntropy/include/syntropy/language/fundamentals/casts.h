
/// \file casts.h
/// \brief This header is part of Syntropy language module. It contains definitions to convert between fundamental types.
///
/// \author Raffaele D. Facendola - August  2020

#pragma once

#include <type_traits>

#include "syntropy/language/fundamentals/types.h"

namespace Syntropy
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Convert rhs to a boolean.
    template <typename TType>
    constexpr Bool ToBool(TType rhs) noexcept;

    /// \brief Truncate rhs to an integer value.
    template <typename TNumber>
    constexpr Int ToInt(TNumber rhs) noexcept;

    /// \brief Convert rhs to a floating point value.
    template <typename TNumber>
    constexpr Float ToFloat(TNumber rhs) noexcept;

    /// \brief Convert rhs to a pointer to UType.
    /// \remarks If the pointee type is not related to UType, the program is ill-formed.
    template <typename TType, typename UType>
    constexpr PointerT<TType> ToPointer(PointerT<UType> rhs) noexcept;

    /// \brief Cast rhs to a 8-bit integer value.
    template <typename TNumber>
    constexpr Fix8 ToFix8(TNumber rhs) noexcept;

    /// \brief Cast rhs to a 16-bit integer value.
    template <typename TNumber>
    constexpr Fix16 ToFix16(TNumber rhs) noexcept;

    /// \brief Cast rhs to a 32-bit integer value.
    template <typename TNumber>
    constexpr Fix32 ToFix32(TNumber rhs) noexcept;

    /// \brief Cast rhs to a 64-bit integer value.
    template <typename TNumber>
    constexpr Fix64 ToFix64(TNumber rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    template <typename TType>
    constexpr Bool ToBool(TType rhs) noexcept
    {
        return !!rhs;
    }

    template <typename TNumber>
    constexpr Int ToInt(TNumber rhs) noexcept
    {
        return static_cast<Int>(rhs);
    }

    template <typename TNumber>
    constexpr Float ToFloat(TNumber rhs) noexcept
    {
        return static_cast<Float>(rhs);
    }

    template <typename TType, typename UType>
    constexpr PointerT<TType> ToPointer(PointerT<UType> rhs) noexcept
    {
        return static_cast<PointerT<TType>>(rhs);
    }

    template <typename TNumber>
    constexpr Fix8 ToFix8(TNumber rhs) noexcept
    {
        return static_cast<Fix8>(rhs);
    }

    template <typename TNumber>
    constexpr Fix16 ToFix16(TNumber rhs) noexcept
    {
        return static_cast<Fix16>(rhs);
    }

    template <typename TNumber>
    constexpr Fix32 ToFix32(TNumber rhs) noexcept
    {
        return static_cast<Fix32>(rhs);
    }

    template <typename TNumber>
    constexpr Fix64 ToFix64(TNumber rhs) noexcept
    {
        return static_cast<Fix64>(rhs);
    }

}


