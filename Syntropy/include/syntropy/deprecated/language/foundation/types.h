
/// \file types.h
/// \brief This header is part of Syntropy language module. It contains fundamental definitions of types.
///
/// \author Raffaele D. Facendola - August  2020

#pragma once

#include <cstdint>
#include <cstddef>

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* FUNDAMENTAL TYPES                                                    */
    /************************************************************************/

    /// \brief Boolean value.
    /// \remarks Standard booleans bind also to numeric values.
    using Bool = const bool;

    /// \brief 64-bit integer value.
    using Int = const std::int64_t;

    /// \brief 32-bit floating point value.
    using Float = const float;

    /// \brief Boolean value.
    /// \remarks Standard booleans bind also to numeric values.
    using RWBool = bool;

    /// \brief 64-bit integer value.
    using RWInt = std::int64_t;

    /// \brief 32-bit floating point value.
    using RWFloat = float;

    /************************************************************************/
    /* FIXED-SIZE TYPES                                                     */
    /************************************************************************/

    /// \brief 8-bit integer value.
    enum class Fix8 : std::int8_t {};

    /// \brief 16-bit integer value.
    enum class Fix16 : std::int16_t {};

    /// \brief 32-bit integer value.
    enum class Fix32 : std::int32_t {};

    /// \brief 64-bit integer value.
    enum class Fix64 : std::int64_t {};

    /************************************************************************/
    /* ENUMERATION CLASS TYPES                                              */
    /************************************************************************/

    /// \brief 8-bit enum class type.
    using Enum8 = std::int8_t;

    /// \brief 16-bit enum class type.
    using Enum16 = std::int16_t;

    /// \brief 32-bit enum class type.
    using Enum32 = std::int32_t;

    /// \brief 64-bit enum class type.
    using Enum64 = std::int64_t;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Convert rhs to a boolean.
    template <typename TType>
    [[nodiscard]] constexpr Bool ToBool(TType rhs) noexcept;

    /// \brief Truncate rhs to an integer value.
    template <typename TNumber>
    [[nodiscard]] constexpr Int ToInt(TNumber rhs) noexcept;

    /// \brief Convert rhs to a floating point value.
    template <typename TNumber>
    [[nodiscard]] constexpr Float ToFloat(TNumber rhs) noexcept;

    /// \brief Cast rhs to a 8-bit integer value.
    template <typename TNumber>
    [[nodiscard]] constexpr Fix8 ToFix8(TNumber rhs) noexcept;

    /// \brief Cast rhs to a 16-bit integer value.
    template <typename TNumber>
    [[nodiscard]] constexpr Fix16 ToFix16(TNumber rhs) noexcept;

    /// \brief Cast rhs to a 32-bit integer value.
    template <typename TNumber>
    [[nodiscard]] constexpr Fix32 ToFix32(TNumber rhs) noexcept;

    /// \brief Cast rhs to a 64-bit integer value.
    template <typename TNumber>
    [[nodiscard]] constexpr Fix64 ToFix64(TNumber rhs) noexcept;

}

// ===========================================================================

namespace Syntropy
{
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

// ===========================================================================
