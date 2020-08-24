
/// \file references.h
/// \brief This header is part of Syntropy language module. It contains definitions for references.
///
/// \author Raffaele D. Facendola - August  2020

#pragma once

#include <type_traits>

#include "syntropy/language/fundamentals/types.h"
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
    template <typename TType, typename = Traits::EnableIf<!Traits::IsConst<TType>>>
    using RWReference = TType&;

    /// \brief Type alias for a lvalue reference to either a *read-only* or *read-write* object.
    template <typename TType>
    using XReference = TType&;

    /************************************************************************/
    /* TRANSIENT                                                            */
    /************************************************************************/

    /// \brief Type alias for a rvalue reference to a *read-only* object.
    template <typename TType, typename = Traits::EnableIf<Traits::IsRValueReference<Traits::AddRValueReference<TType>>>>
    using Transient = const TType&&;

    /// \brief Type alias for a rvalue reference to a *read-write* object.
    template <typename TType, typename = Traits::EnableIf<Traits::IsRValueReference<Traits::AddRValueReference<TType>> && !Traits::IsConst<Traits::RemoveReference<TType>>>>
    using RWTransient = TType&&;

    /// \brief Type alias for a rvalue reference to either a *read-only* or *read-write* object.
    template <typename TType, typename = Traits::EnableIf<Traits::IsRValueReference<TType>>>
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
    [[nodiscard]] constexpr XTransient<Traits::RemoveReference<TType>> Move(Forwarding<TType> rhs) noexcept;

    /// brief Forward an lvalue (reference) as either an lvalue (reference) or rvalue (transient).
    template <typename TType>
    [[nodiscard]] constexpr Forwarding<TType> Forward(XReference<Traits::RemoveReference<TType>> rhs) noexcept;

    /// \brief Forward an rvalue (transient) as an rvalue (transient).
    template <typename TType>
    [[nodiscard]] constexpr XTransient<TType> Forward(XTransient<Traits::RemoveReference<TType>> rhs) noexcept;

    /// \brief Convert rhs to a read-only reference.
    template <typename TType>
    [[nodiscard]] constexpr Reference<TType> ReadOnly(Reference<TType> rhs) noexcept;

    /// \brief Rvalues shall not be converted to read-only.
    template <typename TType>
    constexpr void ReadOnly(XTransient<TType> rhs) noexcept = delete;

    /// \brief Convert rhs to a read-write reference.
    /// The intended use for this method is to write a non-const implementation based on a const implementation, without duplicating associated code.
    /// Such usage has the form: ReadWrite(F(ReadOnly(x))), x is non-const.
    /// \remarks If rhs doesn't refer to a read-write object, accessing the result of this method results in undefined behavior.
    template <typename TType>
    [[nodiscard]] constexpr RWReference<TType> ReadWrite(Reference<TType> rhs) noexcept;

    /// \brief Rvalues shall not be converted to read-write.
    template <typename TType>
    constexpr void ReadWrite(XTransient<TType> rhs) noexcept = delete;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    template <typename TType>
    constexpr XTransient<Traits::RemoveReference<TType>> Move(Forwarding<TType> rhs) noexcept
    {
        static_assert(Traits::IsSame<XTransient<TType>, decltype(std::move(rhs))>, "Divergent behavior detected.");

        return static_cast<XTransient<Traits::RemoveReference<TType>>>(rhs);
    }

    template <typename TType>
    constexpr Forwarding<TType> Forward(XReference<Traits::RemoveReference<TType>> rhs) noexcept
    {
        static_assert(Traits::IsSame<Forwarding<TType>, decltype(std::forward<TType>(rhs))>, "Divergent behavior detected.");

        return static_cast<Forwarding<TType>>(rhs);
    }

    template <typename TType>
    constexpr XTransient<TType> Forward(XTransient<Traits::RemoveReference<TType>> rhs) noexcept
    {
        static_assert(Traits::IsSame<XTransient<TType>, decltype(std::forward<TType>(rhs))>, "Divergent behavior detected.");

        return static_cast<XTransient<TType>>(rhs);
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


