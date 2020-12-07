
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

    /// \brief Non-owning pointer to an immutable instance of type TType.
    template <typename TType>
    using ImmutablePtr = const TType*;

    /// \brief Non-owning pointer to a mutable instance of type TType.
    template <typename TType>
    using MutablePtr = TType*;

    /************************************************************************/
    /* TEMPLATES                                                            */
    /************************************************************************/

    /// \brief A pointer to either a mutable or immutable instance of type TType.
    /// The difference between this type and MutablePtr<T> is purely semantic.
    template <typename TType>
    using Pointer = TType*;

    /************************************************************************/
    /* TYPELESS POINTER TYPES                                               */
    /************************************************************************/

    /// \brief Non-owning pointer to a typeless immutable object.
    using ImmutableTypelessPtr = ImmutablePtr<void>;

    /// \brief Non-owning pointer to a typeless mutable object.
    using MutableTypelessPtr = MutablePtr<void>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Conversions.
    // ============

    /// \brief Obtain the pointer to an instance of type TType.
    /// The returned value is guaranteed to produce the actual address of rhs, even when operator& is overloaded.
    template <typename TType>
    constexpr Pointer<TType> ToPointer(Reference<TType> rhs) noexcept;

    /// \brief Convert rhs to a pointer to UType.
    /// \remarks If the pointee type is not related to UType, the program is ill-formed.
    template <typename TType, typename UType>
    constexpr Pointer<TType> ToPointer(Pointer<UType> rhs) noexcept;

    /// \brief Prevent from getting the address of a rvalue-reference type.
    template <typename TType>
    constexpr void ToPointer(Immovable<TType> rhs) noexcept = delete;

    // Access.
    // =======

    /// \brief Convert rhs to a pointer to an immutable instance of type TType.
    template <typename TType>
    constexpr ImmutablePtr<TType> ToImmutable(ImmutablePtr<TType> rhs) noexcept;

    /// \brief Convert rhs to a pointer to a mutable instance of type TType.
    /// The intended use for this method is to write a non-const implementation based on a const implementation, without duplicating associated code.
    /// Such usage has the form: ToMutable(F(ToImmutable(x))) where x is non-const and F(.) is a function.
    /// \remarks If rhs pointee doesn't refer to a mutable value, accessing the result of this method results in undefined behavior.
    template <typename TType>
    constexpr MutablePtr<TType> ToMutable(ImmutablePtr<TType> rhs) noexcept;

    // Typeless pointer types.
    // =======================

    /// \brief Convert rhs to a pointer to a typeless immutable object.
    ImmutableTypelessPtr ToTypeless(ImmutableTypelessPtr rhs) noexcept;

    /// \brief Convert rhs to a pointer to a typeless mutable object.
    /// \remarks If the pointee refers to an immutable object, accessing the result of this method results in undefined behavior.
    MutableTypelessPtr ToTypeless(MutableTypelessPtr rhs) noexcept;

    /// \brief Convert rhs to a strongly-typed immutable pointer type.
    /// \remarks If the pointee type is not related to TType, accessing the result of this method results in undefined behavior.
    template <typename TType>
    ImmutablePtr<TType> FromTypeless(ImmutableTypelessPtr rhs) noexcept;

    /// \brief Convert rhs to a strongly-typed mutable pointer type.
    /// \remarks If the pointee type is not related to TType, accessing the result of this method results in undefined behavior.
    template <typename TType>
    MutablePtr<TType> FromTypeless(MutableTypelessPtr rhs) noexcept;
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
    constexpr Pointer<TType> ToPointer(Reference<TType> rhs) noexcept
    {
        return std::addressof(rhs);
    }

    template <typename TType, typename UType>
    constexpr Pointer<TType> ToPointer(Pointer<UType> rhs) noexcept
    {
        return static_cast<Pointer<TType>>(rhs);
    }

    // Access.

    template <typename TType>
    constexpr ImmutablePtr<TType> ToImmutable(ImmutablePtr<TType> rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr MutablePtr<TType> ToMutable(ImmutablePtr<TType> rhs) noexcept
    {
        return const_cast<MutablePtr<TType>>(rhs);
    }

    // Typeless pointer types.

    inline ImmutableTypelessPtr ToTypeless(ImmutableTypelessPtr rhs) noexcept
    {
        return rhs;
    }

    inline MutableTypelessPtr ToTypeless(MutableTypelessPtr rhs) noexcept
    {
        return const_cast<MutableTypelessPtr>(rhs);
    }

    template <typename TType>
    inline ImmutablePtr<TType> FromTypeless(ImmutableTypelessPtr rhs) noexcept
    {
        return reinterpret_cast<ImmutablePtr<TType>>(rhs);
    }

    template <typename TType>
    inline MutablePtr<TType> FromTypeless(MutableTypelessPtr rhs) noexcept
    {
        return reinterpret_cast<MutablePtr<TType>>(rhs);
    }

}

// ===========================================================================
