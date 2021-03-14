
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
    /* RANGE VIEW ITERATOR                                                  */
    /************************************************************************/

    /// \brief Wraps a range view and adapt it for iteration via range-based
    ///        for loop.
    /// \author Raffaele D. Facendola - December 2020.
    template <typename TRangeView>
    class RangeViewIterator
    {
    public:

        /// \brief Create an empty range view iterator.
        constexpr RangeViewIterator() noexcept = default;

        /// \brief Wrap a range view for iteration.
        constexpr RangeViewIterator(Immutable<TRangeView> range_view) noexcept;

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

    /************************************************************************/
    /* FRONT                                                                */
    /************************************************************************/

    /// \brief Invoke the method via a custom extension.
    template <typename TRangeView>
    auto InvokeFront(Immutable<TRangeView> range_view,
                     Syntropy::Templates::Priority<2>)
        noexcept ->
        decltype(Syntropy::Ranges::Extensions::Front<TRangeView>{}(range_view))
    {
        return Syntropy::Ranges::Extensions::Front<TRangeView>{}(range_view);
    }

    /// \brief Invoke the method via member-function.
    template <typename TRangeView>
    auto InvokeFront(Immutable<TRangeView> range_view,
                     Syntropy::Templates::Priority<1>)
        noexcept -> decltype(range_view.GetFront())
    {
        return range_view.GetFront();
    }

    /// \brief Invoke the method via non-member function, possibly using ADL.
    template <typename TRangeView>
    auto InvokeFront(Immutable<TRangeView> range_view,
                     Syntropy::Templates::Priority<0>)
        noexcept -> decltype(Front(range_view))
    {
        return Front(range_view);
    }

    /// \brief Routes the invocation.
    template <typename TRangeView>
    auto RouteFront(Immutable<TRangeView> range_view)
        -> decltype(InvokeFront(range_view, Syntropy::Templates::kPriority<2>))
    {
        return InvokeFront(range_view, Syntropy::Templates::kPriority<2>);
    }

    /************************************************************************/
    /* POP FRONT                                                            */
    /************************************************************************/

    /// \brief Invoke the method via a custom extension.
    template <typename TRangeView>
    auto InvokePopFront(Immutable<TRangeView> range_view,
                        Syntropy::Templates::Priority<2>)
        noexcept ->
            decltype(Ranges::Extensions::PopFront<TRangeView>{}(range_view))
    {
        return Ranges::Extensions::PopFront<TRangeView>{}(range_view);
    }

    template <typename TRangeView>
    auto InvokePopFront(Immutable<TRangeView> range_view,
                        Syntropy::Templates::Priority<1>)
        noexcept -> decltype(range_view.PopFront())
    {
        return range_view.PopFront();
    }

    template <typename TRangeView>
    auto InvokePopFront(Immutable<TRangeView> range_view,
                        Syntropy::Templates::Priority<0>)
        noexcept -> decltype(PopFront(range_view))
    {
        return PopFront(range_view);
    }

    template <typename TRangeView>
    auto RoutePopFront(Immutable<TRangeView> range_view)
        noexcept -> decltype(InvokePopFront(range_view,
                                            Syntropy::Templates::kPriority<2>))
    {
        return InvokePopFront(range_view, Syntropy::Templates::kPriority<2>);
    }

    /************************************************************************/
    /* IS EMPTY                                                             */
    /************************************************************************/

    /// \brief Invoke the method via a custom extension.
    template <typename TRangeView>
    inline auto InvokeIsEmpty(Immutable<TRangeView> range_view,
                              Syntropy::Templates::Priority<2>)
      noexcept ->
        decltype(Ranges::Extensions::IsEmpty<TRangeView>{}(range_view))
    {
        return Ranges::Extensions::IsEmpty<TRangeView>{}(range_view);
    }

    /// \brief Invokes a member function on the range view.
    template <typename TRangeView>
    inline auto InvokeIsEmpty(Immutable<TRangeView> range_view,
                              Syntropy::Templates::Priority<1>)
      noexcept -> decltype(range_view.IsEmpty())
    {
        return range_view.IsEmpty();
    }

    /// \brief Invokes a non-member function via ADL.
    template <typename TRangeView>
    inline auto InvokeIsEmpty(Immutable<TRangeView> range_view,
                              Syntropy::Templates::Priority<0>)
      noexcept -> decltype(IsEmpty(range_view))
    {
        return IsEmpty(range_view);
    }

    /// \brief Routes the invocation.
    template <typename TRangeView>
    inline auto RouteIsEmpty(Immutable<TRangeView> range_view)
        noexcept -> decltype(InvokeIsEmpty(range_view,
                                           Syntropy::Templates::kPriority<2>))
    {
        return InvokeIsEmpty(range_view, Syntropy::Templates::kPriority<2>);
    }

}

// ===========================================================================

#include "forward_range.details.inl"

// ===========================================================================
