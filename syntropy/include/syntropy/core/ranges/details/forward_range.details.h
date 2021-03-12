
/// \file forward_range_.details.h
///
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/priority.h"

#include "syntropy/core/ranges/range_extensions.h"

// ===========================================================================

namespace Syntropy::Ranges::Details
{
    /************************************************************************/
    /* RANGE ITERATOR                                                       */
    /************************************************************************/

    /// \brief Wraps a range and adapt it for iteration via range-based
    ///        for loop.
    /// \author Raffaele D. Facendola - December 2020.
    template <typename TRange>
    class RangeIterator
    {
    public:

        /// \brief Create an empty range.
        constexpr RangeIterator() noexcept = default;

        /// \brief Wrap a range for iteration.
        constexpr RangeIterator(Immutable<TRange> range) noexcept;

        /// \brief Access the front element.
        [[nodiscard]] constexpr decltype(auto) operator*() const noexcept;

        /// \brief Move to the next element.
        constexpr Mutable<RangeIterator> operator++() noexcept;

        /// \brief Check whether two iterators are equal.
        [[nodiscard]] constexpr
        Bool operator==(Immutable<RangeIterator> other) const noexcept;

    private:

        /// \brief Iterable range.
        TRange range_;
    };

    /************************************************************************/
    /* FRONT                                                                */
    /************************************************************************/

    /// \brief Invoke the method via a custom extension.
    template <typename TRange>
    auto InvokeFront(Immutable<TRange> range, Syntropy::Templates::Priority<2>)
        noexcept ->
        decltype(Syntropy::Ranges::Extensions::Front<TRange>{}(range))
    {
        return Syntropy::Ranges::Extensions::Front<TRange>{}(range);
    }

    /// \brief Invoke the method via member-function.
    template <typename TRange>
    auto InvokeFront(Immutable<TRange> range, Syntropy::Templates::Priority<1>)
        noexcept -> decltype(range.GetFront())
    {
        return range.GetFront();
    }

    /// \brief Invoke the method via non-member function, possibly using ADL.
    template <typename TRange>
    auto InvokeFront(Immutable<TRange> range, Syntropy::Templates::Priority<0>)
        noexcept -> decltype(Front(range))
    {
        return Front(range);
    }

    /// \brief Routes the invocation.
    template <typename TRange>
    auto RouteFront(Immutable<TRange> range)
        -> decltype(InvokeFront(range, Syntropy::Templates::kPriority<2>))
    {
        return InvokeFront(range, Syntropy::Templates::kPriority<2>);
    }

    /************************************************************************/
    /* POP FRONT                                                            */
    /************************************************************************/

    /// \brief Invoke the method via a custom extension.
    template <typename TRange>
    auto InvokePopFront(Immutable<TRange> range,
                        Syntropy::Templates::Priority<2>)
        noexcept -> decltype(Ranges::Extensions::PopFront<TRange>{}(range))
    {
        return Ranges::Extensions::PopFront<TRange>{}(range);
    }

    template <typename TRange>
    auto InvokePopFront(Immutable<TRange> range,
                        Syntropy::Templates::Priority<1>)
        noexcept -> decltype(range.PopFront())
    {
        return range.PopFront();
    }

    template <typename TRange>
    auto InvokePopFront(Immutable<TRange> range,
                        Syntropy::Templates::Priority<0>)
        noexcept -> decltype(PopFront(range))
    {
        return PopFront(range);
    }

    template <typename TRange>
    auto RoutePopFront(Immutable<TRange> range)
        noexcept -> decltype(InvokePopFront(range,
                                            Syntropy::Templates::kPriority<2>))
    {
        return InvokePopFront(range, Syntropy::Templates::kPriority<2>);
    }

    /************************************************************************/
    /* IS EMPTY                                                             */
    /************************************************************************/

    /// \brief Invoke the method via a custom extension.
    template <typename TRange>
    inline auto InvokeIsEmpty(Immutable<TRange> range,
                              Syntropy::Templates::Priority<2>)
      noexcept -> decltype(Ranges::Extensions::IsEmpty<TRange>{}(range))
    {
        return Ranges::Extensions::IsEmpty<TRange>{}(range);
    }

    /// \brief Invokes a member function on the range.
    template <typename TRange>
    inline auto InvokeIsEmpty(Immutable<TRange> range,
                              Syntropy::Templates::Priority<1>)
      noexcept -> decltype(range.IsEmpty())
    {
        return range.IsEmpty();
    }

    /// \brief Invokes a non-member function via ADL.
    template <typename TRange>
    inline auto InvokeIsEmpty(Immutable<TRange> range,
                              Syntropy::Templates::Priority<0>)
      noexcept -> decltype(IsEmpty(range))
    {
        return IsEmpty(range);
    }

    /// \brief Routes the invocation.
    template <typename TRange>
    inline auto RouteIsEmpty(Immutable<TRange> range)
        noexcept -> decltype(InvokeIsEmpty(range,
                                           Syntropy::Templates::kPriority<2>))
    {
        return InvokeIsEmpty(range, Syntropy::Templates::kPriority<2>);
    }

}

// ===========================================================================

#include "forward_range.details.inl"

// ===========================================================================
