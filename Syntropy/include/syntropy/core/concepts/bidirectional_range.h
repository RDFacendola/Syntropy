
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

#include "syntropy/core/concepts/forward_range.h"

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* BIDIRECTIONAL RANGE INTERFACE                                        */
    /************************************************************************/

    /// \brief Minimal interface for ranges whose element can be visited sequentially in either direction.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept BidirectionalRangeInterface = requires()
        {
            /// \brief Trait used to determine the reference type of an element inside the range.
            typename Templates::ElementReferenceTypeTraits<TRange>::Type;
        }
        && requires(Immutable<TRange> range)
        {
            /// \brief Access the first element in the range.
            { Front(range) } -> Syntropy::Concepts::SameAs<Templates::ElementReference<TRange>>;

            /// \brief Discard the first element in the range.
            { PopFront(range) } -> Syntropy::Concepts::ConvertibleTo<TRange>;

            /// \brief Access the last element in the range.
            { Back(range) } -> Syntropy::Concepts::SameAs<Templates::ElementReference<TRange>>;

            /// \brief Discard the last element in the range.
            { PopBack(range) } -> Syntropy::Concepts::ConvertibleTo<TRange>;

            /// \brief Check whether the range is empty.
            { IsEmpty(range) } -> Syntropy::Concepts::Boolean;
        };

    /************************************************************************/
    /* BIDIRECTIONAL RANGE                                                  */
    /************************************************************************/

    /// \brief Range whose element can be visited sequentially in either direction.
    /// \author Raffaele D. Facendola - January 2021.
    template <typename TRange>
    concept BidirectionalRange = BidirectionalRangeInterface<TRange> && ForwardRange<TRange>;
}

// ===========================================================================
