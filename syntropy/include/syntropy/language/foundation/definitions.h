
/// \file definitions.h
///
/// \brief This header is part of Syntropy language module.
///        It contains fundamental definitions.
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/type_traits.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* DEFINITIONS                                                          */
    /************************************************************************/

    /// \brief Perform a copy of rhs by means of copy constructor.
    template <typename TType>
    constexpr TType
    Copy(Immutable<TType> rhs) noexcept;

    /// \brief Indicate that rhs may be "moved from", allowing for efficient
    ///        transfer of resources from rhs to another object.
    template <typename TType>
    constexpr Forwarding<Templates::RemoveReference<TType>>
    Move(Forwarding<TType> rhs) noexcept;

    /// \brief Forward mutables and immutables as either mutables, immutables
    ///        or movables.
    template <typename TType>
    constexpr Forwarding<TType>
    Forward(Reference<Templates::RemoveReference<TType>> rhs) noexcept;

    /// \brief Forward movables as movables.
    template <typename TType>
    constexpr Forwarding<TType>
    Forward(Movable<Templates::RemoveReference<TType>> rhs) noexcept;

    /// \brief Obtain the pointer to an instance of TType, reardless of
    ///        whether the operator& was overloaded.
    template <typename TType>
    constexpr BasePtr<TType>
    PtrOf(Reference<TType> rhs) noexcept;

    /// \brief Prevent from getting the address of a temporary instance.
    template <typename TType>
    constexpr void
    PtrOf(Immovable<TType> rhs) noexcept = delete;

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Definitions.
    // ============

    template <typename TType>
    constexpr TType
    Copy(Immutable<TType> rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr Forwarding<Templates::RemoveReference<TType>>
    Move(Forwarding<TType> rhs) noexcept
    {
        return static_cast<Movable<Templates::RemoveReference<TType>>>(rhs);
    }

    template <typename TType>
    constexpr Forwarding<TType>
    Forward(Reference<Templates::RemoveReference<TType>> rhs) noexcept
    {
        return static_cast<Forwarding<TType>>(rhs);
    }

    template <typename TType>
    constexpr Forwarding<TType>
    Forward(Movable<Templates::RemoveReference<TType>> rhs) noexcept
    {
        return static_cast<Forwarding<TType>>(rhs);
    }

    template <typename TType>
    constexpr BasePtr<TType>
    PtrOf(Reference<TType> rhs) noexcept
    {
        return std::addressof(rhs);
    }
}

// ===========================================================================
