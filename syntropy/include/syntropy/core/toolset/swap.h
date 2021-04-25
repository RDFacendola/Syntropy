
/// \file swap.h
///
/// \brief This header is part of Syntropy core module.
///        It contains swapping-related definitions.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

// ===========================================================================

namespace Syntropy::Toolset::Extensions
{
    /************************************************************************/
    /* SWAP EXTENSIONS                                                      */
    /************************************************************************/

    /// \brief Swap two elements and return the old value of the first.
    template <typename TType, typename UType, typename = void>
    struct Exchange;

    /// \brief Swap two elements.
    template <typename TType, typename = void>
    struct Swap;
}

// ===========================================================================

namespace Syntropy::Toolset
{
    /************************************************************************/
    /* SWAP                                                                 */
    /************************************************************************/

    /// \brief Swap lhs with rhs and return the old value of lhs.
    template <typename TType, typename UType>
    [[nodiscard]] constexpr TType
    Exchange(Mutable<TType> lhs, Forwarding<UType> rhs) noexcept;

    /// \brief Swap lhs with rhs.
    template <typename TType>
    constexpr void
    Swap(Mutable<TType> lhs, Mutable<TType> rhs) noexcept;

}

// ===========================================================================

#include "details/swap.inl"

// ===========================================================================
