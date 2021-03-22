
/// \file range_iterator.h
///
/// \author Raffaele D. Facendola - January 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/ranges/forward_range.h"

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* RANGE ITERATOR                                                       */
    /************************************************************************/

    /// \brief Wraps a range and adapt it for iteration via range-based for
    ///        loop.
    /// \author Raffaele D. Facendola - December 2020.
    template <Concepts::ForwardRange TRangeView>
    class RangeIterator
    {
    public:

        /// \brief Create an empty range iterator.
        constexpr
        RangeIterator() noexcept = default;

        /// \brief Wrap a range for iteration.
        template <Concepts::ForwardRange TRange>
        constexpr
        RangeIterator(Immutable<TRange> range) noexcept;

        /// \brief Default copy-constructor.
        constexpr
        RangeIterator(Immutable<RangeIterator> rhs) noexcept = default;

        /// \brief Default copy-assignment operator.
        constexpr
        Mutable<RangeIterator>
        operator=(Immutable<RangeIterator> rhs) noexcept = default;

        /// \brief Default destructor.
        ~RangeIterator() noexcept = default;

        /// \brief Access the front element.
        [[nodiscard]] constexpr decltype(auto)
        operator*() const noexcept;

        /// \brief Move to the next element.
        constexpr Mutable<RangeIterator>
        operator++() noexcept;

        /// \brief Check whether two iterators are equal.
        [[nodiscard]] constexpr
        Bool
        operator==(Immutable<RangeIterator> other) const noexcept;

    private:

        /// \brief Iterable range view.
        TRangeView range_view_;

    };

    /// \brief Deduction guieds for RangeIterator.
    template<Concepts::ForwardRange TRange>
    RangeIterator(Immutable<TRange>)
        -> RangeIterator<Templates::RangeViewTypeOf<TRange>>;

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* RANGE-BASED FOR LOOP                                                 */
    /************************************************************************/

    /// \brief Get an iterator to the first element in a range.
    template <Ranges::Concepts::ForwardRange TRange>
    constexpr auto begin(Immutable<TRange> range) noexcept;

    /// \brief Get an iterator past the last element in a range.
    template <Ranges::Concepts::ForwardRange TRange>
    constexpr auto end(Immutable<TRange> range) noexcept;
}

// ===========================================================================

#include "details/range_iterator.inl"

// ===========================================================================
