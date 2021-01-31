
/// \file pointers.h
/// \brief This header is part of Syntropy language module. It contains definitions for pointer types.
/// 
/// \author Raffaele D. Facendola - Dic 2020

#pragma once

#include <cstdint>
#include <cstddef>

#include "syntropy/language/foundation/references.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/templates/concepts.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* NULL TYPES                                                           */
    /************************************************************************/

    /// \brief Type of the nullptr constant.
    using Null = std::nullptr_t;

    /************************************************************************/
    /* POINTER TYPES                                                        */
    /************************************************************************/

    /// \brief A non-owning pointer to an object instance.
    template <typename TType>
    using BasePtr = TType*;

    /// \brief A non-owning pointer to a read-only object instance.
    template <typename TType>
    using Ptr = BasePtr<const TType>;

    /// \brief A non-owning pointer to a read-write object instance.
    template <typename TType>
    using RWPtr = BasePtr<TType>;

    /************************************************************************/
    /* TYPELESS POINTER TYPES                                               */
    /************************************************************************/

    /// \brief A non-owning pointer to a typeless read-only object.
    using TypelessPtr = Ptr<void>;

    /// \brief A non-owning pointer to a typeless read-write object.
    using RWTypelessPtr = RWPtr<void>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Conversion.
    // ===========

    /// \brief Convert rhs to a pointer to UType preserving constness.
    /// \remarks If the pointee type is not related to UType, the program is ill-formed.
    template <typename TType, typename UType>
    constexpr BasePtr<TType> ToPtr(BasePtr<UType> rhs) noexcept;

    /// \brief Convert rhs to a pointer to a typeless read-only object.
    template <Concepts::ImmutableType TType>
    TypelessPtr ToTypelessPtr(BasePtr<TType> rhs) noexcept;

    /// \brief Convert rhs to a pointer to a typeless read-write object.
    /// \remarks If the pointee refers to a read-only object, accessing the result of this method results in undefined behavior.
    template <Concepts::MutableType TType>
    RWTypelessPtr ToTypelessPtr(BasePtr<TType> rhs) noexcept;

    /// \brief Convert rhs to a strongly-typed read-only pointer type.
    /// \remarks If the pointee type is not related to TType, accessing the result of this method results in undefined behavior.
    template <typename TType>
    Ptr<TType> FromTypelessPtr(TypelessPtr rhs) noexcept;

    /// \brief Convert rhs to a strongly-typed read-write pointer type.
    /// \remarks If the pointee type is not related to TType, accessing the result of this method results in undefined behavior.
    template <typename TType>
    RWPtr<TType> FromTypelessPtr(RWTypelessPtr rhs) noexcept;

    // Access.
    // =======

    /// \brief Convert rhs to a pointer to a read-write instance of type TType.
    template <typename TType>
    constexpr Ptr<TType> ToReadOnly(BasePtr<TType> rhs) noexcept;

    /// \brief Convert rhs to a pointer to a read-write instance of type TType.
    /// The intended use for this method is to write a non-const implementation based on a const implementation, without duplicating associated code.
    /// Such usage has the form: ToReadWrite(F(ToReadOnly(x))) where x is non-const and F(.) is a function.
    /// \remarks If rhs pointee doesn't refer to a read-writable instance, accessing the result of this method results in undefined behavior.
    template <typename TType>
    constexpr RWPtr<Templates::RemoveConst<TType>> ToReadWrite(BasePtr<TType> rhs) noexcept;

    // Utilities.
    // ==========

    /// \brief Obtain the pointer to an instance of type TType.
    /// The returned value is guaranteed to produce the actual address of rhs, even when operator& is overloaded.
    template <typename TType>
    constexpr BasePtr<TType> PtrOf(Reference<TType> rhs) noexcept;

    /// \brief Prevent from getting the address of a rvalue-reference type.
    template <typename TType>
    constexpr void PtrOf(Immovable<TType> rhs) noexcept = delete;
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    // Conversions.

    template <typename TType, typename UType>
    constexpr BasePtr<TType> ToPtr(BasePtr<UType> rhs) noexcept
    {
        return static_cast<BasePtr<TType>>(rhs);
    }

    template <Concepts::MutableType TType>
    inline TypelessPtr ToTypelessPtr(BasePtr<TType> rhs) noexcept
    {
        return rhs;
    }

    template <Concepts::MutableType TType>
    inline RWTypelessPtr ToTypelessPtr(BasePtr<TType> rhs) noexcept
    {
        return const_cast<RWTypelessPtr>(rhs);
    }

    template <typename TType>
    inline Ptr<TType> FromTypelessPtr(TypelessPtr rhs) noexcept
    {
        return reinterpret_cast<Ptr<TType>>(rhs);
    }

    template <typename TType>
    inline RWPtr<TType> FromTypelessPtr(RWTypelessPtr rhs) noexcept
    {
        return reinterpret_cast<RWPtr<TType>>(rhs);
    }

    // Access.

    template <typename TType>
    constexpr Ptr<TType> ToReadOnly(BasePtr<TType> rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr RWPtr<Templates::RemoveConst<TType>> ToReadWrite(BasePtr<TType> rhs) noexcept
    {
        return const_cast<RWPtr<Templates::RemoveConst<TType>>>(rhs);
    }

    // Utilities.

    template <typename TType>
    constexpr BasePtr<TType> PtrOf(Reference<TType> rhs) noexcept
    {
        return std::addressof(rhs);
    }
}

// ===========================================================================
