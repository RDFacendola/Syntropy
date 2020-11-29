
/// \file foundation.h
/// \brief This header is part of Syntropy language module. It contains fundamental definitions of types and functions.
/// 
/// This file defines aliases around standard types both for naming consistency, pinning down supported types and, ultimately, to promote better coding style.
/// 
/// \author Raffaele D. Facendola - Aug 2020 (types.h)
///         Raffaele D. Facendola - Nov 2020 (foundation.h)

#pragma once

#include <cstdint>
#include <cstddef>

#include "syntropy/language/templates/type_transform.h"

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

    /************************************************************************/
    /* REFERENCE TYPES                                                      */
    /************************************************************************/

    /// \brief Reference to an immutable object.
    template <typename TType>
    using Ref = const TType&;

    /// \brief Reference to a mutable object.
    /// \remarks In C++ mutable references are not a thing! The prefix "Mutable" highlights the non-constant nature of the referenced object.
    template <typename TType>
    using MutableRef = TType&;

    /// \brief Reference to either a constant or a mutable object.
    /// This type is an alias for either Ref<T> or MutableRef<T>.
    template <typename TType>
    using XRef = TType&;

    /// \brief A rvalue reference to an object whose resources can be moved from.
    /// \remarks The difference between this type and ForwardRef<T> is purely semantic.
    template <typename TType>
    using MoveRef = TType&&;

    /// \brief A rvalue reference to a const object.
    /// \remarks Differently than MoveRef, this type is never a forwarding reference, therefore it can be used to delete function overloads for rvalue references.
    template <typename TType>
    using ConstMoveRef = const TType&&;

    /// \brief A forwarding reference that propagates rvalues as rvalues and lvalues to either lvalues or rvalues.
    /// \remarks The difference between this type and MoveRef<T> is purely semantic.
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
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Type casts.
    // ===========

    /// \brief Convert rhs to a boolean.
    template <typename TType>
    constexpr Bool ToBool(TType rhs) noexcept;

    /// \brief Truncate rhs to an integer value.
    template <typename TNumber>
    constexpr Int ToInt(TNumber rhs) noexcept;

    /// \brief Convert rhs to a floating point value.
    template <typename TNumber>
    constexpr Float ToFloat(TNumber rhs) noexcept;

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

    /// \brief Convert a reference to a pointer, preserving constness.
    /// The returned value is guaranteed to be the actual address of rhs, even in presence of an overloaded operator&.
    template <typename TType>
    constexpr XPtr<TType> ToPtr(XRef<TType> rhs) noexcept;

    /// \brief Disabled overload for rvalue reference since they have no associated address.
    template <typename TType>
    constexpr XPtr<TType> ToPtr(ConstMoveRef<TType> rhs) noexcept = delete;

    /// \brief Convert rhs to a pointer to UType, preserving constness.
    /// \remarks If the pointee type is not related to UType, the program is ill-formed.
    template <typename TType, typename UType>
    constexpr XPtr<TType> ToPtr(XPtr<UType> rhs) noexcept;

    /// \brief Prevent from getting the address of a rvalue-reference type.
    template <typename TType>
    constexpr void ToPtr(MoveRef<TType> rhs) noexcept = delete;

    /// \brief Convert rhs to a reference to UType, preserving constness.
    template <typename UType>
    constexpr XRef<UType> ToRef(XPtr<UType> rhs) noexcept;

    /// \brief Convert rhs to a reference to UType, preserving constness.
    /// \remarks If the pointee type is not related to UType, the program is ill-formed.
    template <typename TType, typename UType>
    constexpr XRef<TType> ToRef(XRef<UType> rhs) noexcept;

    // Mutability.
    // ===========

    /// \brief Convert rhs to a const reference.
    template <typename TType>
    constexpr Ref<TType> ToConst(Ref<TType> rhs) noexcept;

    /// \brief Convert rhs to a const value.
    /// \remarks This method move-constructs a new constant value since rvalue references may cease to exists when the method returns.
    template <typename TType>
    constexpr Val<TType> ToConst(MoveRef<TType> rhs) noexcept;

    /// \brief Convert rhs to a read-write reference.
    /// The intended use for this method is to write a non-const implementation based on a const implementation, without duplicating code.
    /// Such usage has the form: ReadWrite(F(ReadOnly(x))) where x is non-const and F(.) is a function.
    /// \remarks If rhs doesn't refer to a read-write object, accessing the result of this method results in undefined behavior.
    template <typename TType>
    constexpr MutableRef<TType> ToMutable(Ref<TType> rhs) noexcept;

    /// \brief Convert rhs to a mutable value.
    /// \remarks This method move-constructs a new constant value since rvalue references may cease to exists when the method returns.
    template <typename TType>
    constexpr TType ToMutable(MoveRef<TType> rhs) noexcept;

    /// \brief Convert rhs to a pointer to a const value.
    template <typename TType>
    constexpr PtrRef<TType> ToConst(PtrRef<TType> rhs) noexcept;

    /// \brief Convert rhs to a pointer to a mutable value.
    /// The intended use for this method is to write a mutable implementation based on a const implementation, without duplicating associated code.
    /// Such usage has the form: ToMutable(F(ToConst(x))) where x is non-const and F(.) is a function.
    /// \remarks If rhs pointee doesn't refer to a mutable value, accessing the result of this method results in undefined behavior.
    template <typename TType>
    constexpr Ptr<TType> ToMutable(PtrRef<TType> rhs) noexcept;

    // Move and forward.
    // =================

    /// \brief Indicate that rhs may be "moved from", allowing for efficient transfer of resources from rhs to another object.
    template <typename TType>
    constexpr Templates::RemoveReference<TType>&& Move(TType&& rhs) noexcept;

    /// brief Forward an lvalue as either a lvalue or rvalue and rvalues to rvalues. Forwarding a rvalue as a lvalue is forbidden.
    template <typename TType>
    constexpr ForwardRef<TType> Forward(Templates::RemoveReference<TType>& rhs) noexcept;

    /// brief Forward an lvalue as either a lvalue or rvalue and rvalues to rvalues. Forwarding a rvalue as a lvalue is forbidden.
    template <typename TType>
    constexpr ForwardRef<TType> Forward(Templates::RemoveReference<TType>&& rhs) noexcept;
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    // Type casts.

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

    template <typename TType>
    constexpr XPtr<TType> ToPtr(XRef<TType> rhs) noexcept
    {
        return std::addressof(rhs);
    }

    template <typename TType, typename UType>
    constexpr XPtr<TType> ToPtr(XPtr<UType> rhs) noexcept
    {
        return static_cast<XPtr<TType>>(rhs);
    }

    template <typename UType>
    constexpr XRef<UType> ToRef(XPtr<UType> rhs) noexcept
    {
        return *rhs;
    }

    template <typename TType, typename UType>
    constexpr XRef<TType> ToRef(XRef<UType> rhs) noexcept
    {
        return static_cast<XRef<TType>>(rhs);
    }

    // Mutability.

    template <typename TType>
    constexpr Ref<TType> ToConst(Ref<TType> rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr Val<TType> ToConst(MoveRef<TType> rhs) noexcept
    {
        return Val<TType>{ Move(rhs) };
    }

    template <typename TType>
    constexpr MutableRef<TType> ToMutable(Ref<TType> rhs) noexcept
    {
        return const_cast<MutableRef<TType>>(rhs);
    }

    template <typename TType>
    constexpr TType ToMutable(MoveRef<TType> rhs) noexcept
    {
        return TType{ Move(rhs) };
    }

    template <typename TType>
    constexpr PtrRef<TType> ToConst(PtrRef<TType> rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr Ptr<TType> ToMutable(PtrRef<TType> rhs) noexcept
    {
        return const_cast<Ptr<TType>>(rhs);
    }

    // Move and forward

    template <typename TType>
    constexpr Templates::RemoveReference<TType>&& Move(TType&& rhs) noexcept
    {
        return static_cast<Templates::RemoveReference<TType>&&>(rhs);
    }

    template <typename TType>
    constexpr ForwardRef<TType> Forward(Templates::RemoveReference<TType>& rhs) noexcept
    {
        return static_cast<ForwardRef<TType>>(rhs);
    }

    template <typename TType>
    constexpr ForwardRef<TType> Forward(Templates::RemoveReference<TType>&& rhs) noexcept
    {
        return static_cast<ForwardRef<TType>>(rhs);
    }

}

// ===========================================================================

