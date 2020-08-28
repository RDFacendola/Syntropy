
/// \file references.h
/// \brief This header is part of Syntropy language module. It contains definitions for references.
///
/// \author Raffaele D. Facendola - August  2020

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/type_traits/meta.h"
#include "syntropy/language/type_traits/manipulation.h"
#include "syntropy/language/type_traits/properties.h"
#include "syntropy/language/type_traits/categories.h"

namespace Syntropy
{
    /************************************************************************/
    /* REFERENCE                                                            */
    /************************************************************************/

    /// \brief Type alias for a lvalue reference to a *read-only* object.
    template <typename TType>
    using Reference = const TType&;

    /// \brief Type alias for a lvalue reference to a *read-write* object.
    template <typename TType, typename = Traits::EnableIf<!Traits::IsConst<Traits::RemoveReference<TType&>>>>
    using RWReference = TType&;

    /// \brief Type alias for a lvalue reference to either a *read-only* or *read-write* object.
    template <typename TType>
    using XReference = TType&;

    /************************************************************************/
    /* TRANSIENT                                                            */
    /************************************************************************/

    /// \brief Type alias for a rvalue reference to a *read-only* object.
    template <typename TType, typename = Traits::EnableIf<Traits::IsRValueReference<const TType&&>>>
    using Transient = const TType&&;

    /// \brief Type alias for a rvalue reference to a *read-write* object.
    template <typename TType, typename = Traits::EnableIf<Traits::IsRValueReference<TType&&> && !Traits::IsConst<Traits::RemoveReference<TType&>>>>
    using RWTransient = TType&&;

    /// \brief Type alias for a rvalue reference to either a *read-only* or *read-write* object.
    template <typename TType, typename = Traits::EnableIf<Traits::IsRValueReference<TType&&>>>
    using XTransient = TType&&;

    /************************************************************************/
    /* FORWARDING                                                           */
    /************************************************************************/

    /// \brief Type alias for a forwarding reference (aka universal reference).
    /// Note that this declares the *intent* of using TType as forwarding reference, it does not *enforce* it.
    /// This type behaves as either a XReference or a XTransient.
    /// TType is expected to be bound to a template argument.
    template <typename TType>
    using Forwarding = TType&&;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Indicate that rhs may be "moved from", allowing for efficient transfer of resources from rhs to another object.
    template <typename TType>
    [[nodiscard]] constexpr Traits::RemoveReference<TType>&& Move(TType&& rhs) noexcept;

    /// brief Forward an lvalue as either a lvalue or rvalue and rvalues to rvalues. Forwarding a rvalue as a lvalue is forbidden.
    template <typename TType>
    [[nodiscard]] constexpr TType&& Forward(Traits::RemoveReference<TType>& rhs) noexcept;

    /// brief Forward an lvalue as either a lvalue or rvalue and rvalues to rvalues. Forwarding a rvalue as a lvalue is forbidden.
    template <typename TType>
    [[nodiscard]] constexpr TType&& Forward(Traits::RemoveReference<TType>&& rhs) noexcept;

    /// \brief Convert rhs to a read-only reference.
    template <typename TType>
    [[nodiscard]] constexpr Reference<TType> ReadOnly(Reference<TType> rhs) noexcept;

    /// \brief Rvalues shall not be converted to read-only.
    template <typename TType>
    [[nodiscard]] constexpr void ReadOnly(Transient<TType> rhs) noexcept = delete;

    /// \brief Convert rhs to a read-write reference.
    /// The intended use for this method is to write a non-const implementation based on a const implementation, without duplicating code.
    /// Such usage has the form: ReadWrite(F(ReadOnly(x))), x is non-const.
    /// \remarks If rhs doesn't refer to a read-write object, accessing the result of this method results in undefined behavior.
    template <typename TType>
    [[nodiscard]] constexpr RWReference<TType> ReadWrite(Reference<TType> rhs) noexcept;

    /// \brief Rvalues shall not be converted to read-write.
    template <typename TType>
    [[nodiscard]] constexpr RWTransient<TType> ReadWrite(Transient<TType> rhs) noexcept = delete;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    template <typename TType>
    constexpr Traits::RemoveReference<TType>&& Move(TType&& rhs) noexcept
    {
        return static_cast<Traits::RemoveReference<TType>&&>(rhs);
    }

    template <typename TType>
    constexpr TType&& Forward(Traits::RemoveReference<TType>& rhs) noexcept
    {
        return static_cast<TType&&>(rhs);
    }

    template <typename TType>
    constexpr TType&& Forward(Traits::RemoveReference<TType>&& rhs) noexcept
    {
        return static_cast<TType&&>(rhs);
    }

    template <typename TType>
    constexpr Reference<TType> ReadOnly(Reference<TType> rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr RWReference<TType> ReadWrite(Reference<TType> rhs) noexcept
    {
        return const_cast<RWReference<TType>>(rhs);
    }

}


