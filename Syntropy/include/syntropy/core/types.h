
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

    /// \brief Represents a single byte. This type is neither a character type, nor an arithmetic type.
    using Byte = std::byte;

    /************************************************************************/
    /* FIXED SIZE TYPES                                                     */
    /************************************************************************/

    // Fixed-size types used by low-level I/O operations and to reduce
    // memory footprint of structures. Those types are not expected to be used
    // during evaluation and shall never be exposed through public interfaces.

    /// \brief 8-bit integer value.
    enum class Fix8 : std::int8_t {};

    /// \brief 16-bit integer value.
    enum class Fix16 : std::int16_t{};

    /// \brief 32-bit integer value.
    enum class Fix32 : std::int32_t {};

    /// \brief 64-bit integer value.
    enum class Fix64 : std::int64_t{};

    /************************************************************************/
    /* OTHER TYPES                                                          */
    /************************************************************************/

    // Those are the fundamental types used by Syntropy.
    // Support for types other than these is not guaranteed.

    /// \brief Type of the null pointer literal, nullptr.
    using Null = std::nullptr_t;

    /************************************************************************/
    /* TYPE CAST                                                            */
    /************************************************************************/

    /// \brief Truncate a value to integer.
    template <typename TNumber>
    constexpr Int ToInt(TNumber rhs) noexcept;

    /// \brief Convert a numeric to a floating point value.
    template <typename TNumber>
    constexpr Float ToFloat(TNumber rhs) noexcept;
    
    /// \brief Convert a pointer to another related pointer type.
    template <typename TType, typename UType>
    constexpr Pointer<TType> ToPointer(Pointer<UType> rhs) noexcept;

    /// \brief Convert a value to a byte value.
    template <typename TNumber>
    constexpr Byte ToByte(TNumber rhs) noexcept;

    /// \brief Cast a value to a 8-bit integer value.
    template <typename TNumber>
    constexpr Fix8 ToFix8(TNumber rhs) noexcept;

    /// \brief Cast a value to a 16-bit integer value.
    template <typename TNumber>
    constexpr Fix16 ToFix16(TNumber rhs) noexcept;

    /// \brief Cast a value to a 43-bit integer value.
    template <typename TNumber>
    constexpr Fix32 ToFix32(TNumber rhs) noexcept;

    /// \brief Cast a value to a 64-bit integer value.
    template <typename TNumber>
    constexpr Fix64 ToFix64(TNumber rhs) noexcept;

    /************************************************************************/
    /* STREAM INSERTION                                                     */
    /************************************************************************/

    /// \brief Stream insertion for byte types.
    std::ostream& operator<<(std::ostream& out, Byte rhs) noexcept;

    /// \brief Stream insertion for 8-bit integer types.
    std::ostream& operator<<(std::ostream& out, Fix8 rhs) noexcept;

    /// \brief Stream insertion for 16-bit integer types.
    std::ostream& operator<<(std::ostream& out, Fix16 rhs) noexcept;

    /// \brief Stream insertion for 32-bit integer types.
    std::ostream& operator<<(std::ostream& out, Fix32 rhs) noexcept;

    /// \brief Stream insertion for 64-bit types.
    std::ostream& operator<<(std::ostream& out, Fix64 rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Type cast.

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
    constexpr Pointer<TType> ToPointer(Pointer<UType> rhs) noexcept
    {
        return static_cast<UType*>(rhs);
    }

    template <typename TNumber>
    constexpr Byte ToByte(TNumber rhs) noexcept
    {
        return static_cast<Byte>(rhs);
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

    // Stream insertion.

    inline std::ostream& operator<<(std::ostream& out, Byte rhs) noexcept
    {
        out << ToInt(rhs);

        return out;
    }

    inline std::ostream& operator<<(std::ostream& out, Fix8 rhs) noexcept
    {
        out << ToInt(rhs);

        return out;
    }

    inline std::ostream& operator<<(std::ostream& out, Fix16 rhs) noexcept
    {
        out << ToInt(rhs);

        return out;
    }

    inline std::ostream& operator<<(std::ostream& out, Fix32 rhs) noexcept
    {
        out << ToInt(rhs);

        return out;
    }

    inline std::ostream& operator<<(std::ostream& out, Fix64 rhs) noexcept
    {
        out << ToInt(rhs);

        return out;
    }

}


