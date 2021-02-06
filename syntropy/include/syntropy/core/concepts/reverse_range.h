
/// \file reverse_range.h
///
/// \brief This header is part of the Syntropy core module.
///        it contains definitions for adapters used to iterate a range in
///        reverse ordedr.
///
/// Ranges specifications based on the awesome
/// https://www.slideshare.net/rawwell/iteratorsmustgo
///
/// \author Raffaele D. Facendola - Nov 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/concepts/bidirectional_range.h"

#include "syntropy/core/concepts/details/reverse_range_details.h"

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* REVERSE RANGE                                                        */
    /************************************************************************/

    /// \brief Obtain a reversed range.
    template <Ranges::Concepts::BidirectionalRange TRange>
    auto Reverse(Immutable<TRange> range) noexcept;
}

// ===========================================================================

#include "reverse_range.inl"

// ===========================================================================
