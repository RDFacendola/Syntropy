
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
    /* RANGE VIEW ITERATOR                                                  */
    /************************************************************************/

    /// \brief Wraps a range view and adapt it for iteration via range-based
    ///        for loop.
    /// \author Raffaele D. Facendola - December 2020.
    template <Concepts::ForwardRangeView TRangeView>
    class RangeViewIterator
    {
    public:

        /// \brief Create an empty range view iterator.
        constexpr
        RangeViewIterator() noexcept = default;

        /// \brief Wrap a range view for iteration.
        template <Concepts::ForwardRange TRange>
        constexpr
        RangeViewIterator(Immutable<TRange> range) noexcept;

        /// \brief Default copy-constructor.
        constexpr
        RangeViewIterator(Immutable<RangeViewIterator> rhs) noexcept = default;

        /// \brief Default copy-assignment operator.
        constexpr
        Mutable<RangeViewIterator>
        operator(Immutable<RangeViewIterator> rhs) noexcept = default;

        /// \brief Default destructor.
        ~RangeViewIterator() = default;

        /// \brief Access the front element.
        [[nodiscard]] constexpr decltype(auto) operator*() const noexcept;

        /// \brief Move to the next element.
        constexpr Mutable<RangeViewIterator> operator++() noexcept;

        /// \brief Check whether two iterators are equal.
        [[nodiscard]] constexpr
        Bool operator==(Immutable<RangeViewIterator> other) const noexcept;

    private:

        /// \brief Iterable range view.
        TRangeView range_view_;
    };

    /// \brief Deduction guieds for SliceRange.
    template<Concepts::BidirectionalRangeView TRangeView>
    RangeViewIterator(Immutable<TRangeView>)) -> RangeViewIterator<TRangeView>;

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* RANGE-BASED FOR LOOP                                                 */
    /************************************************************************/

    /// \brief Get an iterator to the first element in a range view.
    template <Ranges::Concepts::ForwardRange TRange>
    constexpr auto begin(Immutable<TRange> range) noexcept;

    /// \brief Get an iterator past the last element in a range view.
    template <Ranges::Concepts::ForwardRange TRange>
    constexpr auto end(Immutable<TRange> range) noexcept;
}

// ===========================================================================

#include "details/range_iterator.inl"

// ===========================================================================
