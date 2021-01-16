
/// \file contiguous_range.h
/// \brief This header is part of the Syntropy core module. It contains definitions for ranges whose elements are allocated contiguously.
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

#include "syntropy/core/concepts/details/contiguous_range_details.h"

#include "syntropy/core/concepts/random_access_range.h"

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* CONTIGUOUS RANGE                                                     */
    /************************************************************************/

    /// \brief Range whose elements are allocated contiguously.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept ContiguousRange = RandomAccessRange<TRange>
        && requires(Immutable<TRange> range)
        {
            /// \brief Access range's element storage.
            { Details::DataRouter{}(range) };
        };

}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Contiguous range.
    // =================

    /// \brief Access range's element storage.
    template <Concepts::ContiguousRange TRange>
    [[nodiscard]] constexpr Templates::RangeElementPointerType<TRange> Data(Immutable<TRange> range) noexcept;

    /// \brief Check whether lhs and rhs are equal.
    template <Concepts::ContiguousRange TRange, Concepts::ContiguousRange URange>
    [[nodiscard]] constexpr Bool AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept;
}

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* CONTIGUOUS RANGE EXTENSIONS                                          */
    /************************************************************************/

    // Functors to extend ranges support to custom types.

    /// \brief Access range's element storage.
    template <typename TType>
    struct Data;
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    // Contiguous range.

    template <Concepts::ContiguousRange TRange>
    [[nodiscard]] constexpr Templates::RangeElementPointerType<TRange> Data(Immutable<TRange> range) noexcept
    {
        return Details::DataRouter{}(range);
    }

    template <Concepts::ContiguousRange TRange, Concepts::ContiguousRange URange>
    [[nodiscard]] constexpr Bool AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        return (Ranges::Count(lhs) == Ranges::Count(rhs)) && (Ranges::IsEmpty(lhs) || (Ranges::Data(lhs) == Ranges::Data(rhs)));
    }
}

// ===========================================================================