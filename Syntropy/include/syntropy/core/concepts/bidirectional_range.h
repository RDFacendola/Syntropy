
/// \file bidirectional_range.h
/// \brief This header is part of the Syntropy core module. It contains definitions for ranges whose elements can be visited sequentially in either direction.
///
/// Ranges specifications based on the awesome https://www.slideshare.net/rawwell/iteratorsmustgo
/// 
/// \author Raffaele D. Facendola - Nov 2020
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/diagnostics/assert.h"
#include "syntropy/core/foundation/tuple.h"

#include "syntropy/core/concepts/details/bidirectional_range_details.h"

#include "syntropy/core/concepts/forward_range.h"

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* BIDIRECTIONAL RANGE                                                  */
    /************************************************************************/

    /// \brief Minimal interface for ranges whose elements can be visited sequentially in either direction.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept BaseBidirectionalRange = requires(Immutable<TRange> range)
    {
        /// \brief Access range's last element.
        { Details::RouteBack(range) };

        /// \brief Discard range's last element and return the resulting range.
        { Details::RoutePopBack(range) };
    };

    /// \brief Range whose elements can be visited sequentially in either direction.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept BidirectionalRange = BaseBidirectionalRange<TRange> && ForwardRange<TRange>;
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
    /// \remarks Accessing the last element of an empty range results in undefined behavior.
    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr Templates::RangeElementReferenceType<TRange> Back(Immutable<TRange> range) noexcept;

    /// \brief Discard range's last element and return the resulting range.
    /// \remarks If the provided range is empty, the behavior of this method is undefined.
    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr TRange PopBack(Immutable<TRange> range) noexcept;
}

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* BIDIRECTIONAL RANGE EXTENSIONS                                       */
    /************************************************************************/

    /// \brief Access range's last element.
    template <typename TType>
    struct Back;

    /// \brief Discard range's last element and return the resulting range.
    template <typename TType>
    struct PopBack;
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    // Bidirectional range.

    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr Templates::RangeElementReferenceType<TRange> Back(Immutable<TRange> range) noexcept
    {
        return Details::RouteBack(range);
    }

    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr TRange PopBack(Immutable<TRange> range) noexcept
    {
        return Details::RoutePopBack(range);
    }
}

// ===========================================================================
