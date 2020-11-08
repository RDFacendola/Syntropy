
/// \file pointers.h
/// \brief This header is part of Syntropy language module. It contains definitions for pointers.
///
/// \author Raffaele D. Facendola - August  2020

#pragma once

#include <memory>

#include "syntropy/language/templates/traits.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* POINTER TYPES                                                        */
    /************************************************************************/

    /// \brief Type alias for a pointer to a *read-only* pointee. This pointer doesn't participate to pointee life-time.
    /// This alias may be used for declaring variables with the syntax: auto p = Pointer<...>{ nullptr }.
    /// \remarks This alias is expected not to be used with templates as it will clutter syntax.
    template <typename TType>
    using Pointer = const TType*;

    /// \brief Type alias for a pointer to a *read-write* pointee. This pointer doesn't participate to pointee life-time.
    /// This alias may be used for declaring variables using the syntax: auto p = RWPointer<...>{ nullptr }.
    template <typename TType, typename = Templates::EnableIf<!Templates::IsConst<TType>>>
    using RWPointer = TType*;

    /// \brief Type of the null pointer literal (nullptr).
    using Null = std::nullptr_t;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Convert rhs to a pointer to UType.
    /// \remarks If the pointee type is not related to UType, the program is ill-formed.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr TType* ToPointer(UType* rhs) noexcept;

    /// \brief Obtains the actual address of rhs, even in presence of overloaded operator&.
    template <typename TType>
    [[nodiscard]] constexpr TType* AddressOf(TType& rhs) noexcept;

    /// \brief Rvalue references have no address.
    template <typename TType>
    [[nodiscard]] constexpr void AddressOf(const TType&& rhs) noexcept = delete;

    /// \brief Convert rhs to a pointer to a read-only value.
    template <typename TType>
    [[nodiscard]] constexpr Pointer<TType> ReadOnly(Pointer<TType> rhs) noexcept;

    /// \brief Convert rhs to a pointer to a read-write value.
    /// The intended use for this method is to write a non-const implementation based on a const implementation, without duplicating associated code.
    /// Such usage has the form: ReadWrite(F(ReadOnly(x))) where x is non-const and F(.) is a function.
    /// \remarks If rhs pointee doesn't refer to a read-write value, accessing the result of this method results in undefined behavior.
    template <typename TType>
    [[nodiscard]] constexpr RWPointer<TType> ReadWrite(Pointer<TType> rhs) noexcept;
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    template <typename TType, typename UType>
    constexpr TType* ToPointer(UType* rhs) noexcept
    {
        return static_cast<TType*>(rhs);
    }

    template <typename TType>
    constexpr TType* AddressOf(TType& rhs) noexcept
    {
        return std::addressof(rhs);
    }

    template <typename TType>
    constexpr Pointer<TType> ReadOnly(Pointer<TType> rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr RWPointer<TType> ReadWrite(Pointer<TType> rhs) noexcept
    {
        return const_cast<RWPointer<TType>>(rhs);
    }

}

// ===========================================================================
