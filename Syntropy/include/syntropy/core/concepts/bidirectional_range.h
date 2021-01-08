
/// \file bidirectional_range.h
/// \brief This header is part of the Syntropy core module. It contains definitions for ranges that can be scanned sequentially in either direction.
///
/// Ranges specifications based on the awesome https://www.slideshare.net/rawwell/iteratorsmustgo
/// 
/// \author Raffaele D. Facendola - Nov 2020
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/core/concepts/range.h"

#include "syntropy/core/foundation/tuple.h"

// ===========================================================================

namespace Syntropy::Concepts
{
    /************************************************************************/
    /* BIDIRECTIONAL RANGE                                                  */
    /************************************************************************/

    /// \brief Range that can be visited sequentially in either direction.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept BidirectionalRange = Range<TRange>
        && requires(Immutable<TRange> range)
        {
            /// \brief Access the first element in the range.
            { Front(range) } -> SameAs<Templates::RangeElementReference<TRange>>;

            /// \brief Discard the first element in the range.
            { PopFront(range) } -> ConvertibleTo<TRange>;

            /// \brief Check whether the range is empty.
            { IsEmpty(range) } -> Boolean;

            /// \brief Access the last element in the range.
            { Back(range) } -> SameAs<Templates::RangeElementReference<TRange>>;

            /// \brief Discard the last element in the range.
            { PopBack(range) } -> ConvertibleTo<TRange>;
        };
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Bidirectional range.
    // ====================

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

}

// ===========================================================================