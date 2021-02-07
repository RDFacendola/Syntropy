
/// \file bidirectional_range.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for ranges whose elements can be visited
///        sequentially in either direction.
///
/// Ranges specifications based on the awesome
/// https://www.slideshare.net/rawwell/iteratorsmustgo
///
/// \author Raffaele D. Facendola - Nov 2020
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/diagnostics/assert.h"

#include "syntropy/core/ranges/forward_range.h"

// ===========================================================================

#include "bidirectional_range.details.h"

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* BIDIRECTIONAL RANGE                                                  */
    /************************************************************************/

    /// \brief Minimal interface for ranges whose elements can be visited
    ///        sequentially in either direction.
    ///
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept BaseBidirectionalRange = requires(Immutable<TRange> range)
    {
        /// \brief Access range's last element.
        { Details::RouteBack(range) };

        /// \brief Discard range's last element and return the resulting range.
        { Details::RoutePopBack(range) };
    };

    /// \brief Range whose elements can be visited sequentially in
    ///        either direction.
    ///
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept BidirectionalRange
        = BaseBidirectionalRange<TRange> && ForwardRange<TRange>;
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Bidirectional range.
    // ====================

    /// \brief Access range's last element.
    ///
    /// \remarks Accessing the last element of an empty range
    ///          results in undefined behavior.
    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr decltype(auto)
    Back(Immutable<TRange> range) noexcept;

    /// \brief Discard range's last element and return the resulting range.
    ///
    /// \remarks If the provided range is empty, the behavior of this
    ///          method is undefined.
    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr TRange
    PopBack(Immutable<TRange> range) noexcept;
}

// ===========================================================================

#include "bidirectional_range.inl"

// ===========================================================================
