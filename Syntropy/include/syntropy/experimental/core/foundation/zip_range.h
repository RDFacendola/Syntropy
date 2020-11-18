
/// \file zip_range.h
/// \brief This header is part of the Syntropy core module. It contains definitions for adapters used to zip multiple ranges together.
///
/// Ranges specifications based on the awesome https://www.slideshare.net/rawwell/iteratorsmustgo
/// 
/// \author Raffaele D. Facendola - November 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/experimental/core/foundation/range.h"

#include "syntropy/core/foundation/tuple.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* ZIP RANGE                                                            */
    /************************************************************************/

    /// \brief Adapter class used to reverse bidirectional ranges.
    /// \author Raffaele D. Facendola - November 2020.
    template <Concepts::ForwardRangeT... TRanges>
    class ZipRange
    {
    public:

        // Create a new zip range.
        ZipRange(const TRanges&... ranges) noexcept;

    private:

        /// \brief Underlying ranges.
        TupleT<TRanges...> ranges_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new range by joining together multiple ranges.
    /// The range is considered to be non-empty until at least one of the ranges becomes empty.
    template <Concepts::ForwardRangeT... TRanges>
    ZipRange<TRanges...> Zip(const TRanges&... ranges) noexcept;

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ZipRange.
    // =========



    // Non-member functions.
    // =====================

    template <Concepts::ForwardRangeT... TRanges>
    ZipRange<TRanges...> Zip(const TRanges&... ranges) noexcept
    {
        return ZipRange(Forward<TRanges>(ranges)...);
    }

}

// ===========================================================================