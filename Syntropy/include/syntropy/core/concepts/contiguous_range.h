
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

#include "syntropy/core/concepts/range.h"
#include "syntropy/core/concepts/random_access_range.h"

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* CONTIGUOUS RANGE INTERFACE                                           */
    /************************************************************************/

    /// \brief Minimal interface for ranges whose elements are allocated contiguously.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept ContiguousRangeInterface = requires()
        {
            /// \brief Trait used to determine the reference type of an element inside the range.
            typename Templates::ElementReferenceTypeTraits<TRange>::Type;

            /// \brief Trait used to determine the pointer type of an element inside the range.
            typename Templates::ElementPointerTypeTraits<TRange>::Type;

            /// \brief Trait used to determine the type of the cardinality of the range.
            typename Templates::ElementCountTypeTraits<TRange>::Type;
        }
        && RangeCardinality<Templates::ElementCount<TRange>>
        && Syntropy::Concepts::PointerType<Templates::ElementPointer<TRange>>
        && requires(Immutable<TRange> range)
        {
            /// \brief Get the number of elements in the range.
            { Count(range) } -> Syntropy::Concepts::SameAs<Templates::ElementCount<TRange>>;
        }
        && requires(Immutable<TRange> range)
        {
            /// \brief Access the memory the elements are contiguously allocated on.
            { Data(range) } -> Syntropy::Concepts::SameAs<Templates::ElementPointer<TRange>>;
        };

    /************************************************************************/
    /* CONTIGUOUS RANGE                                                     */
    /************************************************************************/

    /// \brief Range whose element are allocated contiguously.
    /// \author Raffaele D. Facendola - January 2021.
    template <typename TRange>
    concept ContiguousRange = ContiguousRangeInterface<TRange> && RandomAccessRange<TRange>;
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Contiguous range interface.
    // ===========================

    /// \brief Access a sub-range given an offset from the first element and the number of elements.
    /// \brief Exceeding range boundaries results in undefined behavior.
    template <Concepts::ContiguousRangeInterface TRange>
    [[nodiscard]] constexpr auto Select(Immutable<TRange> range, Immutable<Templates::ElementCount<TRange>> offset, Immutable<Templates::ElementCount<TRange>> count) noexcept;

    /// \brief Get an iterator past the last element in a contiguous range.
    /// \brief Exceeding range boundaries results in undefined behavior.
    template <Concepts::ContiguousRangeInterface TRange>
    [[nodiscard]] constexpr decltype(auto) Select(Immutable<TRange> range, Immutable<Templates::ElementCount<TRange>> index) noexcept;

    // Contiguous range.
    // =================

    /// \brief Check whether lhs and rhs are equal.
    template <Concepts::ContiguousRange TRange, Concepts::ContiguousRange URange>
    [[nodiscard]] constexpr Bool AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept;
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    // Contiguous range interface.

    template <Concepts::ContiguousRangeInterface TRange>
    [[nodiscard]] constexpr auto Select(Immutable<TRange> range, Immutable<Templates::ElementCount<TRange>> offset, Immutable<Templates::ElementCount<TRange>> count) noexcept
    {
        return TRange{ Data(range) + offset, count };
    }

    template <Concepts::ContiguousRangeInterface TRange>
    [[nodiscard]] constexpr decltype(auto) Select(Immutable<TRange> range, Immutable<Templates::ElementCount<TRange>> index) noexcept
    {
        return *(Data(range) + index);
    }

    // Contiguous range.

    template <Concepts::ContiguousRange TRange, Concepts::ContiguousRange URange>
    [[nodiscard]] constexpr Bool AreEqual(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        return (Count(lhs) == Count(rhs)) && (IsEmpty(lhs) || (Data(lhs) == Data(rhs)));
    }
}

// ===========================================================================