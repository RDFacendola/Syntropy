
/// \file forward_range_details.h
/// \brief This header is part of the Syntropy core module. It contains implementation details about forward ranges.
///
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/priority.h"

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* FORWARD RANGE EXTENSIONS                                             */
    /************************************************************************/

    // Functors to extend ranges support to custom types.

    /// \brief Access range's first element.
    template <typename TType>
    struct Front;

    /// \brief Discard range's first element and return the resulting range.
    template <typename TType>
    struct PopFront;

    /// \brief Check whether a range is empty.
    template <typename TType>
    struct IsEmpty;
}

// ===========================================================================

namespace Syntropy::Ranges::Details
{
    // Based on this amazing post: https://wandbox.org/permlink/AB9uQxO2MymNDDtt

    /************************************************************************/
    /* FRONT                                                                */
    /************************************************************************/

    /// \brief Custom extension.
    template <typename TRange>
    auto InvokeFront(Immutable<TRange> range, Syntropy::Templates::Priority<2>) noexcept -> decltype(Syntropy::Ranges::Extensions::Front<TRange>{}(range))
    {
        return Syntropy::Ranges::Extensions::Front<TRange>{}(range);
    }

    /// \brief Member-function.
    template <typename TRange>
    auto InvokeFront(Immutable<TRange> range, Syntropy::Templates::Priority<1>) noexcept -> decltype(range.GetFront())
    {
        return range.GetFront();
    }

    /// \brief Non-member function (via ADL).
    template <typename TRange>
    auto InvokeFront(Immutable<TRange> range, Syntropy::Templates::Priority<0>) noexcept -> decltype(Front(range))
    {
        return Front(range);
    }

    /// \brief Routes the invocation.
    template <typename TRange>
    auto RouteFront(Immutable<TRange> range) -> decltype(InvokeFront(range, Syntropy::Templates::kPriority<2>))
    {
        return InvokeFront(range, Syntropy::Templates::kPriority<2>);
    }

    /************************************************************************/
    /* POP FRONT                                                            */
    /************************************************************************/

    template <typename TRange>
    auto InvokePopFront(Immutable<TRange> range, Syntropy::Templates::Priority<2>) noexcept -> decltype(Ranges::Extensions::PopFront<TRange>{}(range))
    {
        return Ranges::Extensions::PopFront<TRange>{}(range);
    }

    template <typename TRange>
    auto InvokePopFront(Immutable<TRange> range, Syntropy::Templates::Priority<1>) noexcept -> decltype(range.PopFront())
    {
        return range.PopFront();
    }

    template <typename TRange>
    auto InvokePopFront(Immutable<TRange> range, Syntropy::Templates::Priority<0>) noexcept -> decltype(PopFront(range))
    {
        return PopFront(range);
    }

    template <typename TRange>
    auto RoutePopFront(Immutable<TRange> range) noexcept -> decltype(InvokePopFront(range, Syntropy::Templates::kPriority<2>))
    {
        return InvokePopFront(range, Syntropy::Templates::kPriority<2>);
    }

    /************************************************************************/
    /* IS EMPTY                                                             */
    /************************************************************************/

    /// \brief Custom extension.
    template <typename TRange>
    inline auto InvokeIsEmpty(Immutable<TRange> range, Syntropy::Templates::Priority<2>) noexcept -> decltype(Ranges::Extensions::IsEmpty<TRange>{}(range))
    {
        return Ranges::Extensions::IsEmpty<TRange>{}(range);
    }

    /// \brief Invokes a member function on the range.
    template <typename TRange>
    inline auto InvokeIsEmpty(Immutable<TRange> range, Syntropy::Templates::Priority<1>) noexcept -> decltype(range.IsEmpty())
    {
        return range.IsEmpty();
    }

    /// \brief Invokes a non-member function via ADL.
    template <typename TRange>
    inline auto InvokeIsEmpty(Immutable<TRange> range, Syntropy::Templates::Priority<0>) noexcept -> decltype(IsEmpty(range))
    {
        return IsEmpty(range);
    }

    /// \brief Routes the invocation.
    template <typename TRange>
    inline auto RouteIsEmpty(Immutable<TRange> range) noexcept -> decltype(InvokeIsEmpty(range, Syntropy::Templates::kPriority<2>))
    {
        return InvokeIsEmpty(range, Syntropy::Templates::kPriority<2>);
    }

}

// ===========================================================================
