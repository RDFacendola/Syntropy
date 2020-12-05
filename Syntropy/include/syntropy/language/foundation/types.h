
/// \file types.h
/// \brief This header is part of Syntropy language module. It contains definitions for fundamental types.
/// 
/// \author Raffaele D. Facendola - Aug 2020

#pragma once

#include <cstdint>
#include <cstddef>

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* FUNDAMENTAL TYPES                                                    */
    /************************************************************************/

    /// \brief Mutable boolean value.
    /// \remarks Standard booleans bind also to numeric values.
    using MutableBool = bool;

    /// \brief Mutable 64-bit integer value.
    using MutableInt = std::int64_t;

    /// \brief Mutable 32-bit single-precision floating point value.
    using MutableFloat = float;

    /// \brief Constant boolean value.
    /// \remarks Standard booleans bind also to numeric values.
    using Bool = const bool;

    /// \brief Constant 64-bit integer value.
    using Int = const std::int64_t;

    /// \brief Constant 32-bit single-precision floating point value.
    using Float = const float;

    /************************************************************************/
    /* FIXED-SIZE TYPES                                                     */
    /************************************************************************/

    // Fixed-size integer types to reduce storage utilization. These types shall be converted to "Int" when evaluating expressions.

    /// \brief Signed 8-bit integer value.
    enum class MutableFix8 : std::int8_t {};

    /// \brief Signed 16-bit integer value.
    enum class MutableFix16 : std::int16_t {};

    /// \brief Signed 32-bit integer value.
    enum class MutableFix32 : std::int32_t {};

    /// \brief Signed 64-bit integer value.
    enum class MutableFix64 : std::int64_t {};

    /// \brief Constant signed 8-bit integer value.
    using Fix8 = const MutableFix8;

    /// \brief Constant signed 16-bit integer value.
    using Fix16 = const MutableFix16;

    /// \brief Constant signed 32-bit integer value.
    using Fix32 = const MutableFix32;

    /// \brief Constant signed 64-bit integer value.
    using Fix64 = const MutableFix64;

    /************************************************************************/
    /* VALUE TYPES                                                          */
    /************************************************************************/

    /// \brief Immutable value type.
    template <typename TType>
    using Val = const TType;

    /// \brief Mutable value type.
    template <typename TType>
    using MutableVal = TType;

    /// \brief Reference to either a constant or a mutable object.
    /// This type is an alias for either Ref<T> or Mutable<T>.
    template <typename TType>
    using XRef = TType&;

    /// \brief A forwarding reference that propagates rvalues as rvalues and lvalues to either lvalues or rvalues.
    /// \remarks The difference between this type and Movable<T> is purely semantic.
    template <typename TType>
    using ForwardRef = TType&&;

    /************************************************************************/
    /* TYPE CASTS                                                           */
    /************************************************************************/

    // Fundamental types.
    // ==================

    /// \brief Convert rhs to a boolean.
    template <typename TType>
    constexpr Bool ToBool(TType rhs) noexcept;

    /// \brief Truncate rhs to an integer value.
    template <typename TNumber>
    constexpr Int ToInt(TNumber rhs) noexcept;

    /// \brief Convert rhs to a floating point value.
    template <typename TNumber>
    constexpr Float ToFloat(TNumber rhs) noexcept;

    // Fixed-size types.
    // =================

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

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* TYPE CASTS                                                           */
    /************************************************************************/

    // Fundamental types.
    // ==================

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

    // Fixed-size types.
    // =================

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
