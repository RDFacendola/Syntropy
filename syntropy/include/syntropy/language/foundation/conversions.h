
/// \file conversions.h
///
/// \brief This header is part of Syntropy language module.
///        It contains definitions for fundamental types conversions.
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/templates/concepts.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* FUNDAMENTAL TYPES                                                    */
    /************************************************************************/

    /// \brief Convert rhs to a boolean.
    template <typename TType>
    constexpr Bool
    ToBool(TType rhs) noexcept;

    /// \brief Truncate rhs to an integer value.
    template <typename TNumber>
    constexpr Int
    ToInt(TNumber rhs) noexcept;

    /// \brief Convert rhs to a floating point value.
    template <typename TNumber>
    constexpr Float
    ToFloat(TNumber rhs) noexcept;

    /************************************************************************/
    /* REFERENCE TYPES                                                      */
    /************************************************************************/

    /// \brief Convert rhs to a reference to an immutable
    ///        instance of TType.
    template <typename TType>
    constexpr Immutable<TType>
    ToImmutable(Immutable<TType> rhs) noexcept;

    /// \brief Deleted overload to disallow movable arguments.
    template <typename TType>
    constexpr void
    ToImmutable(Immovable<TType> rhs) noexcept = delete;

    /// \brief Convert rhs to a mutable reference to an instance of TType.
    ///
    /// The intended use for this method is to write a non-const implementation
    /// based on a const implementation, without duplicating code.
    /// Such usage has the form: ToMutable(F(ToImmutable(x)))
    /// where x is mutable and F(.) is a function.
    ///
    /// \remarks If rhs doesn't refer to a mutable object, accessing the
    ///          returned value of this method results in undefined behavior.
    template <typename TType>
    constexpr Mutable<TType>
    ToMutable(Immutable<TType> rhs) noexcept;

    /// \brief Deleted overload to disallow movable arguments.
    template <typename TType>
    constexpr TType
    ToMutable(Immovable<TType> rhs) noexcept = delete;

    /// \brief Convert rhs to a reference to an immovable instance of TType.
    template <typename TType>
    constexpr Immovable<TType>
    ToImmovable(Immutable<TType> rhs) noexcept;

    /************************************************************************/
    /* POINTER TYPES                                                        */
    /************************************************************************/

    /// \brief Convert rhs to a pointer to UType preserving constness.
    ///
    /// \remarks If the pointee type is not related to UType,
    ///          the program is ill-formed.
    template <typename TType, typename UType>
    constexpr BasePtr<TType>
    ToPtr(BasePtr<UType> rhs) noexcept;

    /// \brief Convert rhs to a pointer to a read-only instance of TType.
    template <typename TType>
    constexpr Ptr<TType>
    ToReadOnly(Ptr<TType> rhs) noexcept;

    /// \brief Convert rhs to a pointer to a read-write instance of TType.
    ///
    /// The intended use for this method is to write a non-const implementation
    /// based on a const implementation, without duplicating code.
    /// Such usage has the form: ToReadWrite(F(ToReadOnly(x)))
    /// where x is mutable and F(.) is a function.
    ///
    /// \remarks If rhs doesn't point to a read-write object, accessing the
    ///          returned value of this method results in undefined behavior.
    template <typename TType>
    constexpr RWPtr<TType>
    ToReadWrite(Ptr<TType> rhs) noexcept;

    /************************************************************************/
    /* FIXED-SIZE TYPES                                                     */
    /************************************************************************/

    /// \brief Cast rhs to a 8-bit integer value.
    template <typename TNumber>
    constexpr Fix8
    ToFix8(TNumber rhs) noexcept;

    /// \brief Cast rhs to a 16-bit integer value.
    template <typename TNumber>
    constexpr Fix16
    ToFix16(TNumber rhs) noexcept;

    /// \brief Cast rhs to a 32-bit integer value.
    template <typename TNumber>
    constexpr Fix32
    ToFix32(TNumber rhs) noexcept;

    /// \brief Cast rhs to a 64-bit integer value.
    template <typename TNumber>
    constexpr Fix64
    ToFix64(TNumber rhs) noexcept;

    /************************************************************************/
    /* TYPELESS POINTER TYPES                                               */
    /************************************************************************/

    /// \brief Convert rhs to a pointer to a typeless read-only object.
    template <Templates::IsImmutable TType>
    TypelessPtr
    ToTypelessPtr(BasePtr<TType> rhs) noexcept;

    /// \brief Convert rhs to a pointer to a typeless read-write object.
    ///
    /// \remarks If the pointee refers to a read-only object, accessing
    ///          the result of this method results in undefined behavior.
    template <Templates::IsMutable TType>
    RWTypelessPtr
    ToTypelessPtr(BasePtr<TType> rhs) noexcept;

    /// \brief Convert rhs to a strongly-typed read-only pointer type.
    ///
    /// \remarks If the pointee type is not related to TType, accessing
    ///          the result of this method results in undefined behavior.
    template <typename TType>
    Ptr<TType>
    FromTypelessPtr(TypelessPtr rhs) noexcept;

    /// \brief Convert rhs to a strongly-typed read-write pointer type.
    ///
    /// \remarks If the pointee type is not related to TType, accessing the
    ///          result of this method results in undefined behavior.
    template <typename TType>
    RWPtr<TType>
    FromTypelessPtr(RWTypelessPtr rhs) noexcept;

}

// ===========================================================================

#include "conversions.inl"

// ===========================================================================
