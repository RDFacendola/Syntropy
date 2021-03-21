
/// \file random_access_range.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for ranges whose elements can be visited
///        in any order.
///
/// Ranges specifications based on the awesome
///     https://www.slideshare.net/rawwell/iteratorsmustgo
///
/// \author Raffaele D. Facendola - November 2020
/// \author Raffaele D. Facendola - January 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/core/ranges/bidirectional_range.h"

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* RANDOM ACCESS RANGE                                                  */
    /************************************************************************/

    /// \brief Concept for ranges whose elements can be visited in any order.
    ///
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept RandomAccessRange = BidirectionalRange<TRange>
        && requires(Immutable<TRange> range,
                    Immutable<Templates::RangeCountType<TRange>> index)
        {
            /// \brief Access range element by index.
            { Ranges::At(range, index) };
        };

}

// ===========================================================================

#include "details/random_access_range.inl"

// ===========================================================================
