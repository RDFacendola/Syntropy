
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
    concept BaseBidirectionalRangeView =
        requires(Immutable<TRangeView> range_view)
        {
            /// \brief Access the last element in a range view.
            { Details::RouteBack(range_view) };

            /// \brief Discard the last element in a range view and return
            ///        a view to the remaining elements.
            { Details::RoutePopBack(range_view) };
        };

    /// \brief Range view whose elements can be visited sequentially in
    ///        either direction.
    ///
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRangeView>
    concept BidirectionalRangeView = BaseBidirectionalRangeView<TRangeView>
        && ForwardRangeView<TRangeView>;
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Bidirectional range view.
    // =========================

    /// \brief Access the last element in a range view.
    ///
    /// \remarks Undefined behavior if the range view is empty.
    template <Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr decltype(auto)
    Back(Immutable<TRangeView> range_view) noexcept;

    /// \brief Discard the last element in a range view and return a view to
    ///        the remaining elements.
    ///
    /// \remarks Undefined behavior if the range view is empty.
    template <Concepts::BidirectionalRangeView TRangeView>
    [[nodiscard]] constexpr TRangeView
    PopBack(Immutable<TRangeView> range_view) noexcept;
}

// ===========================================================================

#include "details/bidirectional_range.inl"

// ===========================================================================
