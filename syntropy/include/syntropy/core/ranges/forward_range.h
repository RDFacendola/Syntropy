
/// \file forward_range.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for ranges whose elements can be visited
///        sequentially.
///
/// \author Raffaele D. Facendola - November 2020
/// \author Raffaele D. Facendola - January 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/core/ranges/range.h"

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* FORWARD RANGE VIEW                                                   */
    /************************************************************************/

    /// \brief Concept for range views whose elements can be visited
    ///        sequentially.
    ///
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRangeView>
    concept ForwardRangeView = requires(TRangeView range_view)
    {
        /// \brief Access the first element in a range view.
        { Ranges::Front(range_view) };

        /// \brief Discard the first element in a range view and return
        ///        a view to the remaining elements.
        { Ranges::PopFront(range_view) };

        /// \brief Check whether a range view is empty.
        { Ranges::IsEmpty(range_view) };
    };

    /************************************************************************/
    /* FORWARD RANGE                                                        */
    /************************************************************************/

    /// \brief Concept for ranges whose elements can be visited sequentially.
    ///
    /// \author Raffaele D. Facendola - March 2021.
    template <typename TRange>
    concept ForwardRange = Range<TRange>
        && requires(Immutable<TRange> range)
        {
            { Ranges::ViewOf(range) } -> ForwardRangeView;
        };

}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // ForwardRange.
    // =============

    /// \brief Apply a function to each element in a range view.
    template <Concepts::ForwardRange TRange, typename TFunction>
    constexpr void
    ForEach(Immutable<TRange> range, TFunction function) noexcept;

    /// \brief Advance both range views in lockstep, copying elements from the
    ///        source to the destination until either is exhausted.
    ///
    /// \return Returns the number of copied elements.
    template <Concepts::ForwardRange TRange, Concepts::ForwardRange URange>
    constexpr Templates::RangeCountType<URange>
    Copy(Immutable<TRange> destination, Immutable<URange> source) noexcept;

    /// \brief Advance both range views in lockstep, moving elements from the
    ///        source to the destination until either is exhausted.
    ///
    /// \return Returns the number of moved elements.
    template <Concepts::ForwardRange TRange, Concepts::ForwardRange URange>
    constexpr Templates::RangeCountType<URange>
    Move(Immutable<TRange> destination, Immutable<URange> source) noexcept;

     /// \brief Advance both range views in lockstep, swapping elements
     ///        between lhs and rhs until either is exhausted.
     ///
     /// \return Returns the number of moved elements.
    template <Concepts::ForwardRange TRange>
    constexpr Templates::RangeCountType<TRange>
    Swap(Immutable<TRange> lhs, Immutable<TRange> rhs) noexcept;

}

// ===========================================================================

#include "details/forward_range.inl"

// ===========================================================================
