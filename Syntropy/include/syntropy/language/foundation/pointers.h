
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

    /************************************************************************/
    /* FUNCTIONS                                                            */
    /************************************************************************/

    // Pointer types.
    // ==============

    /// \brief Obtain the pointer to an immutable object referred via rhs.
    /// The returned value is guaranteed to produce the actual address of rhs, even when operator& is overloaded.
    template <typename TType>
    constexpr Ptr<TType> ToPtr(Immutable<TType> rhs) noexcept;

    /// \brief Obtain the pointer to an immutable object referred via rhs.
    /// The returned value is guaranteed to produce the actual address of rhs, even when operator& is overloaded.
    template <typename TType>
    constexpr MutablePtr<TType> ToPtr(Mutable<TType> rhs) noexcept;

    /// \brief Disabled overload for rvalue reference since they have no associated address.
    template <typename TType>
    constexpr XPtr<TType> ToPtr(Immovable<TType> rhs) noexcept = delete;

    /// \brief Convert rhs to a pointer to UType, preserving constness.
    /// \remarks If the pointee type is not related to UType, the program is ill-formed.
    template <typename TType, typename UType>
    constexpr XPtr<TType> ToPtr(XPtr<UType> rhs) noexcept;

    /// \brief Prevent from getting the address of a rvalue-reference type.
    template <typename TType>
    constexpr void ToPtr(Movable<TType> rhs) noexcept = delete;

    // Typeless pointer types.
    // =======================

    /// \brief Convert rhs to a strongly-typed immutable pointer type.
    /// \remarks If the pointee type is not related to TType, accessing the result of this method results in undefined behavior.
    template <typename TType>
    PtrRef<TType> FromTypeless(TypelessPtrRef rhs) noexcept;

    /// \brief Convert rhs to a strongly-typed mutable pointer type.
    /// \remarks If the pointee type is not related to TType, accessing the result of this method results in undefined behavior.
    template <typename TType>
    Ptr<TType> FromTypeless(TypelessPtr rhs) noexcept;

    // Access.
    // =======

    /// \brief Convert rhs to a pointer to a const value.
    template <typename TType>
    constexpr PtrRef<TType> ToImmutable(PtrRef<TType> rhs) noexcept;

    /// \brief Convert rhs to a pointer to a mutable value.
    /// The intended use for this method is to write a mutable implementation based on a const implementation, without duplicating associated code.
    /// Such usage has the form: ToMutable(F(ToImmutable(x))) where x is non-const and F(.) is a function.
    /// \remarks If rhs pointee doesn't refer to a mutable value, accessing the result of this method results in undefined behavior.
    template <typename TType>
    constexpr Ptr<TType> ToMutable(PtrRef<TType> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Pointer types.
    // ==============

    template <typename TType>
    constexpr Ptr<TType> ToPtr(Immutable<TType> rhs) noexcept
    {
        return std::addressof(rhs);
    }

    template <typename TType>
    constexpr MutablePtr<TType> ToPtr(Mutable<TType> rhs) noexcept
    {
        return std::addressof(rhs);
    }

    template <typename TType, typename UType>
    constexpr XPtr<TType> ToPtr(XPtr<UType> rhs) noexcept
    {
        return static_cast<XPtr<TType>>(rhs);
    }

    // Typeless pointer types.
    // =======================

    template <typename TType>
    inline PtrRef<TType> FromTypeless(TypelessPtrRef rhs) noexcept
    {
        return reinterpret_cast<PtrRef<TType>>(rhs);
    }

    template <typename TType>
    inline Ptr<TType> FromTypeless(TypelessPtr rhs) noexcept
    {
        return reinterpret_cast<Ptr<TType>>(rhs);
    }

    // Access.
    // =======

    template <typename TType>
    constexpr PtrRef<TType> ToImmutable(PtrRef<TType> rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr Ptr<TType> ToMutable(PtrRef<TType> rhs) noexcept
    {
        return const_cast<Ptr<TType>>(rhs);
    }

}

// ===========================================================================
