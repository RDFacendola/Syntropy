
/// \file reverse_range.h
/// \brief This header is part of the Syntropy core module. It contains definitions for adapters used to iterate a range in reverse order.
///
/// Ranges specifications based on the awesome https://www.slideshare.net/rawwell/iteratorsmustgo
/// 
/// \author Raffaele D. Facendola - Nov 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/concepts/forward_range.h"
#include "syntropy/core/concepts/sized_range.h"
#include "syntropy/core/concepts/bidirectional_range.h"
#include "syntropy/core/concepts/random_access_range.h"
#include "syntropy/core/concepts/contiguous_range.h"

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* REVERSE RANGE                                                        */
    /************************************************************************/

    /// \brief Adapter class used to reverse bidirectional ranges.
    /// \author Raffaele D. Facendola - November 2020.
    template <Concepts::BidirectionalRange TRange>
    class ReverseRange
    {
        template <Concepts::BidirectionalRange URange>
        friend constexpr TRange Reverse(Immutable<ReverseRange<URange>> range) noexcept;

    public:

        /// \brief Create an empty reverse range.
        constexpr ReverseRange() noexcept = default;

        /// \brief Create a new reverse range.
        constexpr ReverseRange(Immutable<TRange> range) noexcept;

        /// \brief Default copy-constructor.
        constexpr ReverseRange(Immutable<ReverseRange> range) noexcept = default;

        /// \brief Default destructor.
        ~ReverseRange() noexcept = default;

        /// \brief Default copy-assignment operator.
        constexpr Mutable<ReverseRange> operator=(Immutable<ReverseRange> range) noexcept = default;

    private:

        /// \brief Underlying range.
        TRange range_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Reverse range.
    // ==============

    /// \brief Access range's first element.
    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto Front(Immutable<ReverseRange<TRange>> range) noexcept -> decltype(Ranges::Back(range));

    /// \brief Discard range's first element and return the resulting range.
    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto PopFront(Immutable<ReverseRange<TRange>> range) noexcept -> decltype(ReverseRange(Ranges::PopBack(range)));

    /// \brief Check whether the range is empty.
    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto IsEmpty(Immutable<ReverseRange<TRange>> range) noexcept -> decltype(Ranges::IsEmpty(range));

    /// \brief Get range's elements count.
    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto Count(Immutable<ReverseRange<TRange>> range) noexcept -> decltype(Ranges::Count(range));

    /// \brief Access range's last element.
    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto Back(Immutable<ReverseRange<TRange>> range) noexcept -> decltype(Ranges::Front(range));

    /// \brief Discard range's last element and return the resulting range.
    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto PopBack(Immutable<ReverseRange<TRange>> range) noexcept -> decltype(ReverseRange(Ranges::PopFront(range)));

    /// \brief Access range's element by index.
    template <Concepts::RandomAccessRange TRange, typename TIndex = Templates::RangeCountType<TRange>>
    [[nodiscard]] constexpr auto At(Immutable<ReverseRange<TRange>> range, Immutable<TIndex> index) noexcept -> decltype(Ranges::At(range, index));

    /// \brief Obtain a view to a sub-range.
    template <Concepts::RandomAccessRange TRange, typename TIndex = Templates::RangeCountType<TRange>, typename TCount = TIndex>
    [[nodiscard]] constexpr auto Slice(Immutable<ReverseRange<TRange>> range, Immutable<TIndex> index, Immutable<TCount> count) noexcept -> decltype(ReverseRange(Ranges::Slice(range, index, count)));

    // Utilities.
    // ==========

    /// \brief Reverse a bidirectional range.
    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr ReverseRange<TRange> Reverse(Immutable<TRange> range) noexcept;

    /// \brief Reverse a reversed range.
    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr TRange Reverse(Immutable<ReverseRange<TRange>> range) noexcept;

}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ReverseRange.
    // ============

    template <Concepts::BidirectionalRange TRange>
    constexpr ReverseRange<TRange>::ReverseRange(Immutable<TRange> range) noexcept
        : range_(range)
    {

    }

    // Non-member functions.
    // =====================

    // ReverseRange.

    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto Front(Immutable<ReverseRange<TRange>> range) noexcept -> decltype(Ranges::Back(range))
    {
        return Ranges::Back(Ranges::Reverse(range));
    }

    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto PopFront(Immutable<ReverseRange<TRange>> range) noexcept -> decltype(ReverseRange(Ranges::PopBack(range)))
    {
        return ReverseRange(Ranges::PopBack(Ranges::Reverse(range)));
    }

    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto IsEmpty(Immutable<ReverseRange<TRange>> range) noexcept -> decltype(Ranges::IsEmpty(range))
    {
        return Ranges::IsEmpty(Ranges::Reverse(range));
    }

    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto Count(Immutable<ReverseRange<TRange>> range) noexcept -> decltype(Ranges::Count(range))
    {
        return Ranges::Count(Ranges::Reverse(range));
    }

    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto Back(Immutable<ReverseRange<TRange>> range) noexcept -> decltype(Ranges::Front(range))
    {
        return Ranges::Front(Ranges::Reverse(range));
    }

    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr auto PopBack(Immutable<ReverseRange<TRange>> range) noexcept -> decltype(ReverseRange(Ranges::PopFront(range)))
    {
        return ReverseRange(Ranges::PopFront(Ranges::Reverse(range)));
    }

    template <Concepts::RandomAccessRange TRange, typename TIndex>
    [[nodiscard]] constexpr auto At(Immutable<ReverseRange<TRange>> range, Immutable<TIndex> index) noexcept -> decltype(Ranges::At(range, index))
    {
        return Ranges::At(Reverse(range), Ranges::Count(range) - index);
    }

    template <Concepts::RandomAccessRange TRange, typename TIndex, typename TCount>
    [[nodiscard]] constexpr auto Slice(Immutable<ReverseRange<TRange>> range, Immutable<TIndex> index, Immutable<TCount> count) noexcept -> decltype(Reverse(Ranges::Slice(range, index, count)))
    {
        return ReverseRange(Ranges::Slice(Reverse(range), Ranges::Count(range) - index, count));
    }

    // Utilities.

    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr ReverseRange<TRange> Reverse(Immutable<TRange> range) noexcept
    {
        return range;
    }

    template <Concepts::BidirectionalRange TRange>
    [[nodiscard]] constexpr TRange Reverse(Immutable<ReverseRange<TRange>> range) noexcept
    {
        return range.range_;
    }
}

// ===========================================================================
