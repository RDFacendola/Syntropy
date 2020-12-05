
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
    /* ENUMERATION CLASS TYPES                                              */
    /************************************************************************/

    // Fixed underlying types for enumeration classes. For fixed-size integers see Fix8, Fix16, Fix32, Fix64.

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
    /* POINTER TYPES                                                        */
    /************************************************************************/

    /// \brief Type of the nullptr constant.
    using Null = std::nullptr_t;

    /// \brief Constant non-owning pointer to a mutable object.
    /// \remarks Please note that if TType provides an immutable interface, this type will behave as a pointer to a const object (e.g.: In Ptr<Int> the pointee is const).
    template <typename TType>
    using Ptr = TType* const;

    /// \brief Constant non-owning pointer to an immutable object.
    /// \remarks In C++ pointer-to-references are not a thing! The "Ref" suffix in the name simply highlights the "immutable" nature of the pointee. See Ref<T>.
    template <typename TType>
    using PtrRef = const TType* const;

    /// \brief A constant pointer to either a mutable or immutable object.
    /// This type is an alias for either Ptr<T> or PtrRef<T>.
    template <typename TType>
    using XPtr = TType* const;

    /// \brief Mutable non-owning pointer to a mutable object.
    template <typename TType>
    using MutablePtr = TType*;

    /// \brief Constant non-owning pointer to an immutable object.
    /// \remarks In C++ pointer-to-references are not a thing! The "Ref" suffix in the name simply highlights the "immutable" nature of the pointee. See Ref<T>.
    template <typename TType>
    using MutablePtrRef = const TType*;

    /// \brief A mutable pointer to either a mutable or immutable object.
    /// This type is an alias for either MutablePtr<T> or MutablePtrRef<T>.
    template <typename TType>
    using XMutablePtr = TType*;

    /************************************************************************/
    /* TYPELESS POINTER TYPES                                               */
    /************************************************************************/

    /// \brief Constant non-owning pointer to a mutable typeless object.
    using TypelessPtr = void* const;

    /// \brief Constant non-owning pointer to a typeless immutable object.
    /// \remarks In C++ pointer-to-references are not a thing! The "Ref" suffix in the name simply highlights the "immutable" nature of the pointee. See Ref<T>.
    using TypelessPtrRef = const void* const;

    /// \brief Mutable non-owning pointer to a mutable typeless object.
    using MutableTypelessPtr = void*;

    /// \brief Mutable non-owning pointer to an immutable typeless object.
    /// \remarks In C++ pointer-to-references are not a thing! The "Ref" suffix in the name simply highlights the "immutable" nature of the pointee. See Ref<T>.
    using MutableTypelessPtrRef = const void*;

}
