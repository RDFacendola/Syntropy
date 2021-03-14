
/// \file bidirectional_range.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for ranges whose elements can be visited
///        sequentially in either direction.
///
/// Ranges specifications based on the awesome
/// https://www.slideshare.net/rawwell/iteratorsmustgo
///
/// \author Raffaele D. Facendola - November 2020
/// \author Raffaele D. Facendola - January 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/diagnostics/assert.h"

#include "syntropy/core/ranges/forward_range.h"

// ===========================================================================

#include "details/bidirectional_range.details.h"

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* BIDIRECTIONAL RANGE VIEW                                             */
    /************************************************************************/

    /// \brief Minimal interface for ranges views whose elements can be visited
    ///        sequentially in either direction.
    ///
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRangeView>
    concept BaseBidirectionalRangeView = requires(Immutable<TRangeView> range_view)
    {
        /// \brief Access range's last element.
        { Details::RouteBack(range_view) };

        /// \brief Discard range's last element and return the resulting
        ///        view range.
        { Details::RoutePopBack(range_view) };
    };

    /// \brief Range whose elements can be visited sequentially in
    ///        either direction.
    ///
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRangeView>
    concept BidirectionalRangeView
        = BaseBidirectionalRangeView<TRangeView> && ForwardRangeView<TRangeView>;
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Bidirectional range view.
    // =========================

    /// \brief Access range view's last element.
    ///
    /// \remarks Accessing the last element of an empty range
    ///          results in undefined behavior.
    template <Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr decltype(auto)
    Back(Immutable<TRangeView> range_view) noexcept;

    /// \brief Discard range view's last element and return the resulting
    ///        range view.
    ///
    /// \remarks If the provided range is empty, the behavior of this
    ///          method is undefined.
    template <Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr TRangeView
    PopBack(Immutable<TRangeView> range_view) noexcept;
}

// ===========================================================================

#include "details/bidirectional_range.inl"

// ===========================================================================
