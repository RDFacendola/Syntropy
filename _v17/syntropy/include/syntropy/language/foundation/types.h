
/// \file types.h
///
/// \brief This header is part of Syntropy language module.
///        It contains definitions for fundamental types.
///
/// \author Raffaele D. Facendola - August 2020.

// ===========================================================================

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
    using Bool = bool;

    /// \brief 64-bit integer value.
    using Int = std::int64_t;

    /// \brief 32-bit single-precision floating point value.
    using Float = float;

    /************************************************************************/
    /* VALUE TYPES                                                          */
    /************************************************************************/

    /// \brief Read-only value-type o TType.
    template <typename TType>
    using ReadOnly = const TType;

    /// \brief Read-write value-type of TType.
    template <typename TType>
    using ReadWrite = TType;

    /************************************************************************/
    /* REFERENCE TYPES                                                      */
    /************************************************************************/

    /// \brief Reference to an immutable instance of TType.
    template <typename TType>
    using Immutable = ReadOnly<TType>&;

    /// \brief Reference to a mutable instance of TType.
    template <typename TType>
    using Mutable = ReadWrite<TType>&;

    /// \brief Reference to an immutable instance of TType whose resources
    ///        can be efficiently moved to another instance.
    ///
    /// \remarks This type is meant to be used as a mean of deleting functions
    ///          overloads for which movable instances are not desiderable.
    template <typename TType>
    using Immovable = ReadOnly<TType>&&;

    /// \brief Reference to a mutable instance of TType whose resources
    ///        can be efficiently moved to another instance.
    template <typename TType>
    using Movable = ReadWrite<TType>&&;

    /************************************************************************/
    /* POINTER TYPES                                                        */
    /************************************************************************/

    /// \brief Type of the nullptr constant.
    using Null = std::nullptr_t;

    /// \brief A non-owning pointer to an immutable instance of TType.
    template <typename TType>
    using Ptr = ReadOnly<TType>*;

    /// \brief A non-owning pointer to a mutable instance of TType.
    template <typename TType>
    using RWPtr = ReadWrite<TType>*;

    /// \brief A non-owning pointer to a typeless immutable instance.
    using TypelessPtr = Ptr<void>;

    /// \brief A non-owning pointer to a typeless mutable instance.
    using RWTypelessPtr = RWPtr<void>;

    /************************************************************************/
    /* ENUMERATION TYPES                                                    */
    /************************************************************************/

    // Fixed underlying types for enumeration classes.
    // For fixed-size integers see Fix8, Fix16, Fix32, Fix64.

    /// \brief 8-bit enum class type.
    /// \usage enum class MyEnum : Enum8 { ... };
    using Enum8 = std::int8_t;

    /// \brief 16-bit enum class type.
    /// \usage enum class MyEnum : Enum16 { ... };
    using Enum16 = std::int16_t;

    /// \brief 32-bit enum class type.
    /// \usage enum class MyEnum : Enum32 { ... };
    using Enum32 = std::int32_t;

    /// \brief 64-bit enum class type.
    /// \usage enum class MyEnum : Enum64 { ... };
    using Enum64 = std::int64_t;

    /************************************************************************/
    /* FIXED-SIZE TYPES                                                     */
    /************************************************************************/

    // Fixed-size integer types to reduce storage utilization.
    // These types shall be converted to "Int" when evaluating expressions.

    /// \brief Signed 8-bit integer value.
    enum class Fix8 : std::int8_t {};

    /// \brief Signed 16-bit integer value.
    enum class Fix16 : std::int16_t {};

    /// \brief Signed 32-bit integer value.
    enum class Fix32 : std::int32_t {};

    /// \brief Signed 64-bit integer value.
    enum class Fix64 : std::int64_t {};

    /************************************************************************/
    /* LITERAL TYPES                                                        */
    /************************************************************************/

    /// \brief Type of a user-defined integer literal.
    using IntLiteral = unsigned long long;

    /// \brief Type of an UTF-8 encoded string literal.
    template <Int TSize>
    using StringLiteral = const char8_t(&)[TSize];

    /************************************************************************/
    /* TEMPLATE TYPES                                                       */
    /************************************************************************/

    /// \brief Either an immutable or mutable instance of TType.
    template <typename TType>
    using Reference = TType&;

    /// \brief A forwarding reference that propagates movables as movables and
    ///        mutables and immutables as either mutables, immutables
    ///        or movables.
    template <typename TType>
    using Forwarding = TType&&;

    /// \brief A non-owning pointer to either an immutable or mutable
    ///       instance of TType.
    template <typename TType>
    using BasePtr = TType*;

}
