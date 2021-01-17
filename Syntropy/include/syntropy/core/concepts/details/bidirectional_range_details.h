
/// \file bidirectional_range_details.h
/// \brief This header is part of the Syntropy core module. It contains implementation details about bidirectional ranges.
///
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/priority.h"

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* BIDIRECTIONAL RANGE EXTENSIONS                                       */
    /************************************************************************/

    // Functors to extend ranges support to custom types.

    /// \brief Access range's last element.
    template <typename TType>
    struct Back;

    /// \brief Discard range's last element and return the resulting range.
    template <typename TType>
    struct PopBack;
}

// ===========================================================================

namespace Syntropy::Ranges::Details
{
    // Based on this amazing post: https://wandbox.org/permlink/AB9uQxO2MymNDDtt

    /************************************************************************/
    /* BACK                                                                 */
    /************************************************************************/

    /// \brief Custom extension.
    template <typename TRange>
    auto InvokeBack(Immutable<TRange> range, Syntropy::Templates::Priority<2>) noexcept -> decltype(Ranges::Extensions::Back<TRange>{}(range))
    {
        return Ranges::Extensions::Back<TRange>{}(range);
    }

    /// \brief Member-function.
    template <typename TRange>
    auto InvokeBack(Immutable<TRange> range, Syntropy::Templates::Priority<1>) noexcept -> decltype(range.GetBack())
    {
        return range.GetBack();
    }

    /// \brief Non-member function (via ADL).
    template <typename TRange>
    auto InvokeBack(Immutable<TRange> range, Syntropy::Templates::Priority<0>) noexcept -> decltype(Back(range))
    {
        return Back(range);
    }

    /// \brief Routes the invocation.
    template <typename TRange>
    auto RouteBack(Immutable<TRange> range) noexcept -> decltype(InvokeBack(range, Syntropy::Templates::kPriority<2>))
    {
        return InvokeBack(range, Syntropy::Templates::kPriority<2>);
    }

    /************************************************************************/
    /* POP BACK                                                             */
    /************************************************************************/

    /// \brief Custom extension.
    template <typename TRange>
    auto InvokePopBack(Immutable<TRange> range, Syntropy::Templates::Priority<2>) noexcept -> decltype(Ranges::Extensions::PopBack<TRange>{}(range))
    {
        return Ranges::Extensions::PopBack<TRange>{}(range);
    }

    /// \brief Member-function.
    template <typename TRange>
    auto InvokePopBack(Immutable<TRange> range, Syntropy::Templates::Priority<1>) noexcept -> decltype(range.PopBack())
    {
        return range.PopBack();
    }

    /// \brief Non-member function (via ADL).
    template <typename TRange>
    auto InvokePopBack(Immutable<TRange> range, Syntropy::Templates::Priority<0>) noexcept -> decltype(PopBack(range))
    {
        return PopBack(range);
    }

    /// \brief Routes the invocation.
    template <typename TRange>
    auto RoutePopBack(Immutable<TRange> range) noexcept -> decltype(InvokePopBack(range, Syntropy::Templates::kPriority<2>))
    {
        return InvokePopBack(range, Syntropy::Templates::kPriority<2>);
    }

}

// ===========================================================================
