
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

#include "syntropy/core/ranges/sized_range.h"

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* BIDIRECTIONAL RANGE                                                  */
    /************************************************************************/

    /// \brief Concept for ranges whose elements can be visited sequentially
    ///        in either direction.
    ///
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept BidirectionalRange = SizedRange<TRange>
        && requires(Immutable<TRange> range)
        {
            /// \brief Access the last element in a range.
            { Ranges::Back(range) };

            /// \brief Discard the last element in a range and return a range
            ///        to the remaining elements.
            { Ranges::PopBack(range) };
        };

}

// ===========================================================================

#include "details/bidirectional_range.inl"

// ===========================================================================
