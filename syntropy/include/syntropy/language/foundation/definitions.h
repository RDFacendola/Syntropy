
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

    /// \brief Obtain an immutable reference to rhs.
    template <typename TType>
    constexpr Immutable<TType>
    Copy(Immutable<TType> rhs) noexcept;

    /// \brief Obtain either a movable reference or an immovable reference to
    ///        rhs, depending on whether rhs is mutable or immutable.
    template <typename TType>
    constexpr Forwarding<Templates::QualifiedOf<TType>>
    Move(Forwarding<TType> rhs) noexcept;

    /// \brief Forward mutable references as mutable references and immutable
    ///        references as immutable references.
    template <typename TType>
    constexpr Forwarding<TType>
    Forward(Reference<Templates::QualifiedOf<TType>> rhs) noexcept;

    /// \brief Forward movable reference as movable references and immovable
    ///        references as immovable references.
    template <typename TType>
    constexpr Forwarding<TType>
    Forward(Forwarding<Templates::QualifiedOf<TType>> rhs) noexcept;

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

#include "definitions.inl"

// ===========================================================================
