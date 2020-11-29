
/// \file functional.h
/// \brief This header is part of the Syntropy language module. It contains definitions for fundamental functions.
///
/// \author Raffaele D. Facendola - November 2020

#pragma once

#include "syntropy/language/templates/type_traits.h"

#include "syntropy/language/foundation/types.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* TYPE CASTS                                                           */
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

    /************************************************************************/
    /* ACCESS                                                               */
    /************************************************************************/

    /// \brief Convert rhs to a const reference.
    template <typename TType>
    constexpr Ref<TType> ToConst(Ref<TType> rhs) noexcept;

    /// \brief Convert rhs to a const value.
    /// \remarks This method move-constructs a new constant value since rvalue references may cease to exists when the method returns.
    template <typename TType>
    constexpr Val<TType> ToConst(MoveRef<TType> rhs) noexcept;

    /// \brief Convert rhs to a pointer to a const value.
    template <typename TType>
    constexpr PtrRef<TType> ToConst(PtrRef<TType> rhs) noexcept;

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

    /// \brief Convert rhs to a pointer to a mutable value.
    /// The intended use for this method is to write a mutable implementation based on a const implementation, without duplicating associated code.
    /// Such usage has the form: ToMutable(F(ToConst(x))) where x is non-const and F(.) is a function.
    /// \remarks If rhs pointee doesn't refer to a mutable value, accessing the result of this method results in undefined behavior.
    template <typename TType>
    constexpr Ptr<TType> ToMutable(PtrRef<TType> rhs) noexcept;

    /************************************************************************/
    /* MOVE AND FORWARD                                                     */
    /************************************************************************/

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

    // Type casts.
    // ===========

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

    // Access.
    // =======

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

    // Move and forward.
    // =================

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
