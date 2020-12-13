
/// \file pointers.h
/// \brief This header is part of Syntropy language module. It contains definitions for pointer types.
/// 
/// \author Raffaele D. Facendola - Dic 2020

#pragma once

#include <cstdint>
#include <cstddef>

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* POINTER TYPES                                                        */
    /************************************************************************/

    /// \brief Type of the nullptr constant.
    using Null = std::nullptr_t;

    /// \brief A pointer to an instance of type TType.
    /// The difference between this type and RWPtr<T> is purely semantic.
    template <typename TType>
    using BasePtr = TType*;

    /// \brief Non-owning pointer to a read-only instance of type TType.
    template <typename TType>
    using Ptr = BasePtr<const TType>;

    /// \brief Non-owning pointer to a read-write instance of type TType.
    template <typename TType>
    using RWPtr = BasePtr<TType>;

    /************************************************************************/
    /* TYPELESS POINTER TYPES                                               */
    /************************************************************************/

    /// \brief Non-owning pointer to a typeless object.
    /// The difference between this type and RWTypelessPtr<T> is purely semantic.
    using BaseTypelessPtr = BasePtr<void>;

    /// \brief Non-owning pointer to a typeless read-only object.
    using TypelessPtr = Ptr<void>;

    /// \brief Non-owning pointer to a typeless read-write object.
    using RWTypelessPtr = RWPtr<void>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Conversions.
    // ============

    /// \brief Obtain the pointer to an instance of type TType.
    /// The returned value is guaranteed to produce the actual address of rhs, even when operator& is overloaded.
    template <typename TType>
    constexpr BasePtr<TType> ToPointer(Reference<TType> rhs) noexcept;

    /// \brief Convert rhs to a pointer to UType.
    /// \remarks If the pointee type is not related to UType, the program is ill-formed.
    template <typename TType, typename UType>
    constexpr BasePtr<TType> ToPointer(BasePtr<UType> rhs) noexcept;

    /// \brief Prevent from getting the address of a rvalue-reference type.
    template <typename TType>
    constexpr void ToPointer(Immovable<TType> rhs) noexcept = delete;

    // Access.
    // =======

    /// \brief Convert rhs to a pointer to an immutable instance of type TType.
    template <typename TType>
    constexpr Ptr<TType> ToImmutable(Ptr<TType> rhs) noexcept;

    /// \brief Convert rhs to a pointer to a mutable instance of type TType.
    /// The intended use for this method is to write a non-const implementation based on a const implementation, without duplicating associated code.
    /// Such usage has the form: ToMutable(F(ToImmutable(x))) where x is non-const and F(.) is a function.
    /// \remarks If rhs pointee doesn't refer to a mutable value, accessing the result of this method results in undefined behavior.
    template <typename TType>
    constexpr RWPtr<TType> ToMutable(Ptr<TType> rhs) noexcept;

    // Typeless pointer types.
    // =======================

    /// \brief Convert rhs to a pointer to a typeless immutable object.
    TypelessPtr ToTypeless(TypelessPtr rhs) noexcept;

    /// \brief Convert rhs to a pointer to a typeless mutable object.
    /// \remarks If the pointee refers to an immutable object, accessing the result of this method results in undefined behavior.
    RWTypelessPtr ToTypeless(RWTypelessPtr rhs) noexcept;

    /// \brief Convert rhs to a strongly-typed immutable pointer type.
    /// \remarks If the pointee type is not related to TType, accessing the result of this method results in undefined behavior.
    template <typename TType>
    Ptr<TType> FromTypeless(TypelessPtr rhs) noexcept;

    /// \brief Convert rhs to a strongly-typed mutable pointer type.
    /// \remarks If the pointee type is not related to TType, accessing the result of this method results in undefined behavior.
    template <typename TType>
    RWPtr<TType> FromTypeless(RWTypelessPtr rhs) noexcept;
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

    template <typename TType>
    constexpr BasePtr<TType> ToPointer(Reference<TType> rhs) noexcept
    {
        return std::addressof(rhs);
    }

    template <typename TType, typename UType>
    constexpr BasePtr<TType> ToPointer(BasePtr<UType> rhs) noexcept
    {
        return static_cast<BasePtr<TType>>(rhs);
    }

    // Access.

    template <typename TType>
    constexpr Ptr<TType> ToImmutable(Ptr<TType> rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr RWPtr<TType> ToMutable(Ptr<TType> rhs) noexcept
    {
        return const_cast<RWPtr<TType>>(rhs);
    }

    // Typeless pointer types.

    inline TypelessPtr ToTypeless(TypelessPtr rhs) noexcept
    {
        return rhs;
    }

    inline RWTypelessPtr ToTypeless(RWTypelessPtr rhs) noexcept
    {
        return const_cast<RWTypelessPtr>(rhs);
    }

    template <typename TType>
    inline Ptr<TType> FromTypeless(TypelessPtr rhs) noexcept
    {
        return reinterpret_cast<Ptr<TType>>(rhs);
    }

    template <typename TType>
    inline RWPtr<TType> FromTypeless(RWTypelessPtr rhs) noexcept
    {
        return reinterpret_cast<RWPtr<TType>>(rhs);
    }

}

// ===========================================================================
