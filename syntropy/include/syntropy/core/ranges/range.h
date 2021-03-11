
/// \file range.h
///
/// \brief This header is  part of the Syntropy core module.
///        It contains definition for ranges, homogeneous collection of
///        elements that can be visited in some order.
///
/// \author Raffaele D. Facendola - March 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

// ===========================================================================

#include "range.details.h"

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* RANGE                                                                */
    /************************************************************************/

    /// \brief Minimal interface for ranges whose element can be visited in
    ///        some order.
    /// \author Raffaele D. Facendola - March 2021.
    template <typename TRange>
    concept BaseRange = requires(Immutable<TRange> range)
    {
        /// \brief Get a read-only view to a range' elements.
        { Details::RouteViewOf(range) };
    }
    || requires(Mutable<TRange> range)
    {
        /// \brief Get a read-write view to a range' elements.
        { Details::RouteViewOf(range) };
    };

    /// \brief Range whose elements can be visited in some order.
    /// \author Raffaele D. Facendola - March 2021.
    template <typename TRange>
    concept Range = BaseRange<TRange>;

}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Base range.
    // ===========

    /// \brief Get a read-write view to a range' elements.
    template <Concepts::Range TRange>
    [[nodiscard]] constexpr auto
    ViewOf(Forwarding<TRange> range) noexcept;

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* RANGE-BASED FOR LOOP                                                 */
    /************************************************************************/

    /// \brief Get an iterator to the first element in a range.
    template <Ranges::Concepts::Range TRange>
    constexpr auto begin(Forwarding<TRange> range) noexcept;

    /// \brief Get an iterator past the last element in a range.
    template <Ranges::Concepts::Range TRange>
    constexpr auto end(Forwarding<TRange> range) noexcept;

}

// ===========================================================================

#include "range.inl"

// ===========================================================================
