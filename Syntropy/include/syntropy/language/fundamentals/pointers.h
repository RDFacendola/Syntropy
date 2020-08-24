
/// \file pointers.h
/// \brief This header is part of Syntropy language module. It contains definitions for pointers.
///
/// \author Raffaele D. Facendola - August  2020

#pragma once

#include <memory>

#include "syntropy/language/type_traits/meta.h"
#include "syntropy/language/type_traits/properties.h"
#include "syntropy/language/fundamentals/references.h"

namespace Syntropy
{
    /************************************************************************/
    /* POINTER                                                              */
    /************************************************************************/

    /// \brief Type alias for a non-owning pointer to a *read-only* pointee. This pointer doesn't participate to pointee life-time.
    template <typename TType>
    using Pointer = const TType*;

    /// \brief Type alias for a non-owning pointer to a *read-write* pointee. This pointer doesn't participate to pointee life-time.
    /// TType is required to be non-const.
    template <typename TType, typename = Traits::EnableIf<!Traits::IsConst<TType>>>
    using RWPointer = TType*;

    /// \brief Type alias for a non-owning pointer to either a *read-only* or *read-write* pointee. This Pointer doesn't participate to pointee life-time.
    template <typename TType>
    using XPointer = TType*;

    /************************************************************************/
    /* NULL                                                                 */
    /************************************************************************/

    /// \brief Type of the null pointer literal (nullptr).
    using Null = std::nullptr_t;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Convert rhs to a pointer to UType.
    /// \remarks If the pointee type is not related to UType, the program is ill-formed.
    template <typename TType, typename UType>
    constexpr XPointer<TType> ToPointer(XPointer<UType> rhs) noexcept;

    /// \brief Obtains the actual address of rhs, even in presence of overloaded operator&.
    template <typename TType>
    constexpr XPointer<TType> AddressOf(XReference<TType> rhs) noexcept;

     /// \brief Convert rhs to a pointer to a read-only value.
    template <typename TType>
    [[nodiscard]] constexpr Pointer<TType> ReadOnly(Pointer<TType> rhs) noexcept;

    /// \brief Convert rhs to a pointer to a read-write object.
    /// The intended use for this method is to write a non-const implementation based on a const implementation, without duplicating associated code.
    /// Such usage has the form: ReadWrite(F(ReadOnly(x))), x is non-const.
    /// \remarks If rhs pointee doesn't refer to a read-write object, accessing the result of this method results in undefined behavior.
    template <typename TType>
    [[nodiscard]] constexpr RWPointer<TType> ReadWrite(Pointer<TType> rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    template <typename TType, typename UType>
    constexpr XPointer<TType> ToPointer(XPointer<UType> rhs) noexcept
    {
        return static_cast<XPointer<TType>>(rhs);
    }

    template <typename TType>
    constexpr XPointer<TType> AddressOf(XReference<TType> rhs) noexcept
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
        return rhs ? AddressOf(ReadWrite(*rhs)) : nullptr;
    }

}


