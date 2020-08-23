
/// \file support.h
/// \brief This header is part of Syntropy language module. It contains useful definitions for accessing and transferring objects and resources.
///
/// \author Raffaele D. Facendola - August  2020

#pragma once

#include <type_traits>

#include "syntropy/language/fundamentals/types.h"
#include "syntropy/language/type_traits/meta.h"
#include "syntropy/language/type_traits/manipulation.h"
#include "syntropy/language/type_traits/properties.h"

namespace Syntropy
{
    /************************************************************************/
    /* POINTER                                                              */
    /************************************************************************/

    /// \brief Type alias for a non-owning pointer to a *read-only* pointee. This pointer doesn't participate to pointee life-time.
    template <typename TType>
    using Pointer = PointerT<const TType>;

    /************************************************************************/
    /* RW POINTER                                                           */
    /************************************************************************/

    /// \brief Type alias for a non-owning pointer to a *read-write* pointee. This pointer doesn't participate to pointee life-time.
    /// TType is required to be non-const.
    template <typename TType, typename = Traits::EnableIf<!Traits::IsConst<TType>>>
    using RWPointer = PointerT<TType>;

    /************************************************************************/
    /* REFERENCE                                                            */
    /************************************************************************/

    /// \brief Type alias for a lvalue reference to a read-only object.
    template <typename TType>
    using Reference = ReferenceT<const TType>;

    /************************************************************************/
    /* RW REFERENCE                                                         */
    /************************************************************************/

    /// \brief Type alias for a lvalue reference to a read-write object.
    template <typename TType, typename = Traits::EnableIf<!Traits::IsConst<TType>>>
    using RWReference = ReferenceT<TType>;

    /************************************************************************/
    /* TRANSIENT                                                            */
    /************************************************************************/

    /// \brief Type alias for a const rvalue reference, that is a read-only literal, anonymous object or reference to a temporary object.
    template <typename TType>
    using Transient = const TType&&;

    /************************************************************************/
    /* RW TRANSIENT                                                         */
    /************************************************************************/

    /// \brief Type alias for a rvalue reference, that is a read-write literal, anonymous object or reference to a temporary object.
    /// This type declares the *intent* of using TType as a rvalue reference, it does not *enforce* it.
    /// TType is expected to be a type name.
    template <typename TType>
    using RWTransient = TType&&;

    /************************************************************************/
    /* FORWARDING                                                           */
    /************************************************************************/

    /// \brief Type alias for a forwarding reference (aka universal reference).
    /// Note that this declares the *intent* of using TType as forwarding reference, it does not *enforce* it.
    /// TType is expected to be bound to a template argument.
    template <typename TType>
    using Forwarding = TType&&;

    /************************************************************************/
    /* MOVE                                                                 */
    /************************************************************************/

    /// \brief Indicate that rhs may be "moved from", allowing for efficient transfer of resources from rhs to another object.
    template <typename TType>
    [[nodiscard]] constexpr Traits::RemoveReference<TType>&& Move(TType&& rhs) noexcept;

    /************************************************************************/
    /* FORWARD                                                              */
    /************************************************************************/

    /// brief Forward an lvalue as either an lvalue or rvalue.
    template <typename TType>
    [[nodiscard]] constexpr TType&& Forward(Traits::RemoveReference<TType>& rhs) noexcept;

    /// \brief Forward an rvalue as an rvalue.
    template <typename TType>
    [[nodiscard]] constexpr TType&& Forward(Traits::RemoveReference<TType>&& rhs) noexcept;

    /************************************************************************/
    /* READ ONLY                                                            */
    /************************************************************************/

     /// \brief Convert rhs to a pointer to a read-only value.
    template <typename TType>
    constexpr Pointer<TType> ReadOnly(Pointer<TType> rhs) noexcept;

    /// \brief Convert rhs to a read-only reference.
    template <typename TType>
    constexpr Reference<TType> ReadOnly(Reference<TType> rhs) noexcept;

    /// \brief R-value references shall not be converted to read-only: they are being thrown away \ moved.
    template <typename TType>
    constexpr void ReadOnly(const TType&& rhs) noexcept = delete;

    /************************************************************************/
    /* READ WRITE                                                           */
    /************************************************************************/

    /// \brief Convert rhs to a pointer to a read-write value.
    /// \remarks If rhs pointee doesn't refer to a read-write value, accessing the result of this method results in undefined behavior.
    template <typename TType>
    constexpr RWPointer<TType> ReadWrite(Pointer<TType> rhs) noexcept;

    /// \brief Convert rhs to a read-write reference.
    /// \remarks If rhs doesn't refer to a read-write reference, accessing the result of this method results in undefined behavior.
    template <typename TType>
    constexpr RWReference<TType> ReadWrite(Reference<TType> rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Move.
    // =====

    template <typename TType>
    constexpr Traits::RemoveReference<TType>&& Move(TType&& rhs) noexcept
    {
        return static_cast<Traits::RemoveReference<TType>&&>(rhs);
    }

    // Forward.
    // ========

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

    // ReadOnly.
    // =========

    template <typename TType>
    constexpr Pointer<TType> ReadOnly(Pointer<TType> rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr Reference<TType> ReadOnly(Reference<TType> rhs) noexcept
    {
        return rhs;
    }

    // ReadWrite.
    // ==========

    template <typename TType>
    constexpr RWPointer<TType> ReadWrite(Pointer<TType> rhs) noexcept
    {
        return rhs ? ReadWrite(*rhs) : nullptr;
    }

    template <typename TType>
    constexpr RWReference<TType> ReadWrite(Reference<TType> rhs) noexcept
    {
        return const_cast<RWReference<TType>>(rhs);
    }

}


