
/// \file forward_range.h
/// \brief This header is part of the Syntropy core module. It contains definitions for ranges that can be scanned sequentially.
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
    /* FORWARD RANGE                                                        */
    /************************************************************************/

    /// \brief Range that can be visited sequentially.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept ForwardRange = Range<TRange>
        && requires(Immutable<TRange> range)
        {
            /// \brief Access the first element in the range.
            { Front(range) } -> SameAs<Templates::RangeElementReference<TRange>>;

            /// \brief Discard the first element in the range.
            { PopFront(range) } -> ConvertibleTo<TRange>;

            /// \brief Check whether the range is empty.
            { IsEmpty(range) } -> Boolean;
        };
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Forward range.
    // ==============

    /// \brief Apply a function to each element in the range.
    template <Concepts::ForwardRange TRange, typename TFunction>
    constexpr void ForEach(Immutable<TRange> range, TFunction function) noexcept;

    /// \brief Advance both ranges in lockstep, swapping elements from both until either lhs or rhs is exhausted.
    /// \return Returns the ranges to the elements that were not swapped: at least one of the two is guaranteed to be empty.
    template <Concepts::ForwardRange TRange, Concepts::ForwardRange URange>
    constexpr Tuples::Tuple<TRange, URange> MemberwiseSwap(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept;

    /************************************************************************/
    /* RANGE-BASED FOR                                                      */
    /************************************************************************/

    // RangeIterator.
    // ==============

    /// \brief Wraps a range and adapt it for iteration via range-based for loop.
    /// \author Raffaele D. Facendola - December 2020.
    template <Concepts::ForwardRange TRange>
    class RangeIterator
    {
    public:

        /// \brief Reference to a range element.
        using TReference = Templates::RangeElementReference<TRange>;

        /// \brief Create an empty range.
        constexpr RangeIterator() noexcept = default;

        /// \brief Wrap a range for iteration.
        constexpr RangeIterator(Immutable<TRange> range) noexcept;

        /// \brief Access the front element.
        [[nodiscard]] constexpr TReference operator*() const noexcept;

        /// \brief Move to the next element.
        constexpr Mutable<RangeIterator> operator++() noexcept;

        /// \brief Check whether two iterators are equal.
        [[nodiscard]] constexpr Bool operator==(Immutable<RangeIterator> other) const noexcept;

    private:

        /// \brief Iterable range.
        TRange range_;
    };

    // Iterators.
    // ==========

    /// \brief Get an iterator to the first element in a range.
    template <Concepts::ForwardRange TRange>
    constexpr RangeIterator<TRange> begin(Immutable<TRange> range) noexcept;

    /// \brief Get an iterator past the last element in a range.
    template <Concepts::ForwardRange TRange>
    constexpr RangeIterator<TRange> end(Immutable<TRange> range) noexcept;

}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    // Forward range.

    template <Concepts::ForwardRange TRange, typename TFunction>
    constexpr void ForEach(Immutable<TRange> range, TFunction function) noexcept
    {
        for (auto rest = range; !IsEmpty(rest); rest = PopFront(rest))
        {
            function(Front(rest));
        }
    }

    template <Concepts::ForwardRange TRange, Concepts::ForwardRange URange>
    [[nodiscard]] constexpr Tuples::Tuple<TRange, URange> MemberwiseSwap(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        using Syntropy::Swap;

        auto lhs_copy = lhs;
        auto rhs_copy = rhs;

        for (; !IsEmpty(lhs_copy) && !IsEmpty(rhs_copy);)
        {
            Swap(Front(lhs_copy), Front(rhs_copy));

            lhs_copy = PopFront(lhs_copy);
            rhs_copy = PopFront(rhs_copy);
        }

        return { lhs_copy , rhs_copy };
    }

    // Range-based for.
    // ================

    // RangeIterator.

    template <Concepts::ForwardRange TRange>
    constexpr RangeIterator<TRange>::RangeIterator(Immutable<TRange> range) noexcept
        : range_(range)
    {

    }

    template <Concepts::ForwardRange TRange>
    [[nodiscard]] constexpr typename RangeIterator<TRange>::TReference RangeIterator<TRange>::operator*() const noexcept
    {
        return Front(range_);
    }

    template <Concepts::ForwardRange TRange>
    constexpr Mutable<RangeIterator<TRange>> RangeIterator<TRange>::operator++() noexcept
    {
        range_ = PopFront(range_);

        return *this;
    }

    template <Concepts::ForwardRange TRange>
    [[nodiscard]] constexpr Bool RangeIterator<TRange>::operator==(Immutable<RangeIterator> other) const noexcept
    {
        SYNTROPY_ASSERT(IsEmpty(other));

        return IsEmpty(range_);
    }

    // Iterators.

    template <Concepts::ForwardRange TRange>
    constexpr RangeIterator<TRange> begin(Immutable<TRange> range) noexcept
    {
        return RangeIterator<TRange>{ range };
    }

    template <Concepts::ForwardRange TRange>
    constexpr RangeIterator<TRange> end(Immutable<TRange> range) noexcept
    {
        return RangeIterator<TRange>{};
    }

}

// ===========================================================================