
/// \file types.h
/// \brief This header is part of Syntropy core module. It contains definitions for fundamental data types.
///
/// \remarks This file shall not have dependency on any other header than standard ones.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <cstdint>
#include <cstddef>
#include <ostream>

namespace syntropy
{
    /************************************************************************/
    /* FUNDAMENTAL TYPES                                                    */
    /************************************************************************/

    /// \brief Boolean value.
    using Bool = bool;

    /// \brief 64-bit integer value.
    using Int = std::int64_t;

    /// \brief 32-bit floating point value.
    using Float = float;

    /// \brief Type alias for a non-owning pointer which doesn't participate to pointee life-time.
    template <typename TType>
    using Pointer = TType*;

    /************************************************************************/
    /* ADDITIONAL TYPES                                                     */
    /************************************************************************/

    /// \brief Type of the null pointer literal, nullptr.
    using Null = std::nullptr_t;

    /// \brief 8-bit value
    /// \remarks This type represents a collection of bits, therefore it is neither a character type nor an arithmetic type.
    using Byte = std::byte;

    /************************************************************************/
    /* FIXED INTEGER TYPES                                                  */
    /************************************************************************/

    /// \brief 8-bit integer value.
    /// Fixed-width types are meant to be used with low-level IO and network systems.
    using Fix8 = std::int8_t;

    /// \brief 16-bit integer value.
    /// Fixed-width types are meant to be used with low-level IO and network systems.
    using Fix16 = std::int16_t;

    /// \brief 32-bit integer value.
    /// Fixed-width types are meant to be used with low-level IO and network systems.
    using Fix32 = std::int32_t;

    /// \brief 64-bit integer value.
    /// Fixed-width types are meant to be used with low-level IO and network systems.
    using Fix64 = std::int64_t;

    /************************************************************************/
    /* CAST FUNCTIONS                                                       */
    /************************************************************************/

    /// \brief Truncate a value to integer.
    template <typename TNumber>
    constexpr Int ToInt(TNumber rhs);

    /// \brief Convert a numeric to a floating point value.
    template <typename TNumber>
    constexpr Float ToFloat(TNumber rhs);

    /// \brief Convert a value to a byte value.
    template <typename TNumber>
    constexpr Byte ToByte(TNumber rhs);

    /// \brief Cast a value to a 8-bit integer value.
    template <typename TNumber>
    constexpr Fix8 ToFix8(TNumber rhs);

    /// \brief Cast a value to a 16-bit integer value.
    template <typename TNumber>
    constexpr Fix16 ToFix16(TNumber rhs);

    /// \brief Cast a value to a 43-bit integer value.
    template <typename TNumber>
    constexpr Fix32 ToFix32(TNumber rhs);

    /// \brief Cast a value to a 64-bit integer value.
    template <typename TNumber>
    constexpr Fix64 ToFix64(TNumber rhs);

    /************************************************************************/
    /* STREAM INSERTION                                                     */
    /************************************************************************/

    /// \brief Stream insertion for byte types.
    std::ostream& operator<<(std::ostream& out, Byte byte) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Cast functions.

    template <typename TNumber>
    constexpr Int ToInt(TNumber rhs)
    {
        return static_cast<Int>(rhs);
    }

    template <typename TNumber>
    constexpr Float ToFloat(TNumber rhs)
    {
        return static_cast<Float>(rhs);
    }

    template <typename TNumber>
    constexpr Byte ToByte(TNumber rhs)
    {
        return static_cast<Byte>(rhs);
    }

    template <typename TNumber>
    constexpr Fix8 ToFix8(TNumber rhs)
    {
        return static_cast<Fix8>(rhs);
    }

    template <typename TNumber>
    constexpr Fix16 ToFix16(TNumber rhs)
    {
        return static_cast<Fix16>(rhs);
    }

    template <typename TNumber>
    constexpr Fix32 ToFix32(TNumber rhs)
    {
        return static_cast<Fix32>(rhs);
    }

    template <typename TNumber>
    constexpr Fix64 ToFix64(TNumber rhs)
    {
        return static_cast<Fix64>(rhs);
    }

    // Stream insertion.

    inline std::ostream& operator<<(std::ostream& out, Byte byte) noexcept
    {
        out << ToInt(byte);

        return out;
    }

}


