
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

#include "syntropy/diagnostics/assert.h"
#include "syntropy/core/foundation/tuple.h"

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* FORWARD RANGE INTERFACE                                              */
    /************************************************************************/

    /// \brief Minimal interface for ranges whose elements can be visited sequentially.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept ForwardRangeInterface = requires()
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

            /// \brief Check whether the range is empty.
            { IsEmpty(range) } -> Syntropy::Concepts::Boolean;
        };

    /************************************************************************/
    /* FORWARD RANGE                                                        */
    /************************************************************************/

    /// \brief Range whose element can be visited sequentially.
    /// \author Raffaele D. Facendola - January 2021.
    template <typename TRange>
    concept ForwardRange = ForwardRangeInterface<TRange>;
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

    /// \brief Advance both ranges in lockstep, copy elements from lhs to rhs until either lhs or rhs is exhausted.
    /// \return Returns the ranges to the elements that were not copied: at least one of the two is guaranteed to be empty.
    template <Concepts::ForwardRange TRange, Concepts::ForwardRange URange>
    constexpr Tuples::Tuple<TRange, URange> Copy(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept;

    /// \brief Advance both ranges in lockstep, moving elements from lhs to rhs until either lhs or rhs is exhausted.
    /// \return Returns the ranges to the elements that were not copied: at least one of the two is guaranteed to be empty.
    template <Concepts::ForwardRange TRange, Concepts::ForwardRange URange>
    constexpr Tuples::Tuple<TRange, URange> Move(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept;

    /// \brief Advance both ranges in lockstep, swapping elements from both until either lhs or rhs is exhausted.
    /// \return Returns the ranges to the elements that were not swapped: at least one of the two is guaranteed to be empty.
    template <Concepts::ForwardRange TRange, Concepts::ForwardRange URange>
    constexpr Tuples::Tuple<TRange, URange> Swap(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept;

    /************************************************************************/
    /* RANGE-BASED FOR                                                      */
    /************************************************************************/

    // RangeIterator.
    // ==============

    /// \brief Wraps a range and adapt it for iteration via range-based for loop.
    /// \author Raffaele D. Facendola - December 2020.
    template <Concepts::ForwardRangeInterface TRange>
    class RangeIterator
    {
    public:

        /// \brief Reference to a range element.
        using TReference = Templates::ElementReference<TRange>;

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
    template <Concepts::ForwardRangeInterface TRange>
    constexpr RangeIterator<TRange> begin(Immutable<TRange> range) noexcept;

    /// \brief Get an iterator past the last element in a range.
    template <Concepts::ForwardRangeInterface TRange>
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
    constexpr Tuples::Tuple<TRange, URange> Copy(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        auto source = lhs;
        auto destination = rhs;

        for (; !IsEmpty(source) && !IsEmpty(destination);)
        {
            Front(destination) = Front(source);

            source = PopFront(source);
            destination = PopFront(destination);
        }

        return { source , destination };
    }

    template <Concepts::ForwardRange TRange, Concepts::ForwardRange URange>
    constexpr Tuples::Tuple<TRange, URange> Move(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        auto source = lhs;
        auto destination = rhs;

        for (; !IsEmpty(source) && !IsEmpty(destination);)
        {
            Front(destination) = Syntropy::Move(Front(source));

            source = PopFront(source);
            destination = PopFront(destination);
        }

        return { source , destination };
    }

    template <Concepts::ForwardRange TRange, Concepts::ForwardRange URange>
    constexpr Tuples::Tuple<TRange, URange> Swap(Immutable<TRange> lhs, Immutable<URange> rhs) noexcept
    {
        auto source = lhs;
        auto destination = rhs;

        for (; !IsEmpty(source) && !IsEmpty(destination);)
        {
            Swap(Front(source), Front(destination));

            source = PopFront(source);
            destination = PopFront(destination);
        }

        return { source , destination };
    }

    // Range-based for.
    // ================

    // RangeIterator.

    template <Concepts::ForwardRangeInterface TRange>
    constexpr RangeIterator<TRange>::RangeIterator(Immutable<TRange> range) noexcept
        : range_(range)
    {

    }

    template <Concepts::ForwardRangeInterface TRange>
    [[nodiscard]] constexpr typename RangeIterator<TRange>::TReference RangeIterator<TRange>::operator*() const noexcept
    {
        return Front(range_);
    }

    template <Concepts::ForwardRangeInterface TRange>
    constexpr Mutable<RangeIterator<TRange>> RangeIterator<TRange>::operator++() noexcept
    {
        range_ = PopFront(range_);

        return *this;
    }

    template <Concepts::ForwardRangeInterface TRange>
    [[nodiscard]] constexpr Bool RangeIterator<TRange>::operator==(Immutable<RangeIterator> other) const noexcept
    {
        SYNTROPY_ASSERT(IsEmpty(other.range_));

        return IsEmpty(range_);
    }

    // Iterators.

    template <Concepts::ForwardRangeInterface TRange>
    constexpr RangeIterator<TRange> begin(Immutable<TRange> range) noexcept
    {
        return RangeIterator<TRange>{ range };
    }

    template <Concepts::ForwardRangeInterface TRange>
    constexpr RangeIterator<TRange> end(Immutable<TRange> range) noexcept
    {
        return RangeIterator<TRange>{};
    }

}

// ===========================================================================