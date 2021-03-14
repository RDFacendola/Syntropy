
/// \file bidirectional_range.details.h
///
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/priority.h"

#include "syntropy/core/ranges/range_extensions.h"

// ===========================================================================

namespace Syntropy::Ranges::Details
{
    // Based on this amazing post:
    // https://wandbox.org/permlink/AB9uQxO2MymNDDtt

    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    template <Int TPriority>
    using Priority = Syntropy::Templates::Priority<TPriority>;

    template <typename TRangeView>
    using ExtensionBack = Ranges::Extensions::Back<TRangeView>;

    template <typename TRangeView>
    using ExtensionPopBack = Ranges::Extensions::PopBack<TRangeView>;

    /************************************************************************/
    /* BACK                                                                 */
    /************************************************************************/

    /// \brief Invoke the method via a custom extension.
    template <typename TRangeView>
    auto InvokeBack(Immutable<TRangeView> range_view, Priority<2>)
        noexcept -> decltype(ExtensionBack<TRangeView>{}(range_view))
    {
        return ExtensionBack<TRangeView>{}(range_view);
    }

    /// \brief Invoke the method via member-function.
    template <typename TRangeView>
    auto InvokeBack(Immutable<TRangeView> range_view, Priority<1>)
        noexcept -> decltype(range_view.GetBack())
    {
        return range_view.GetBack();
    }

    /// \brief Invoke the method via non-member function, possibly using ADL.
    template <typename TRangeView>
    auto InvokeBack(Immutable<TRangeView> range_view, Priority<0>)
        noexcept -> decltype(Back(range_view))
    {
        return Back(range_view);
    }

    /// \brief Routes the invocation.
    template <typename TRangeView>
    auto RouteBack(Immutable<TRangeView> range_view)
        noexcept -> decltype(InvokeBack(range_view, Priority<2>{}))
    {
        return InvokeBack(range_view, Priority<2>{});
    }

    /************************************************************************/
    /* POP BACK                                                             */
    /************************************************************************/

    /// \brief Invoke the method via a custom extension.
    template <typename TRangeView>
    auto InvokePopBack(Immutable<TRangeView> range_view, Priority<2>)
       noexcept -> decltype(ExtensionPopBack<TRangeView>{}(range_view))
    {
        return ExtensionPopBack<TRangeView>{}(range_view);
    }

    /// \brief Invoke the method via member-function.
    template <typename TRangeView>
    auto InvokePopBack(Immutable<TRangeView> range_view, Priority<1>)
        noexcept -> decltype(range_view.PopBack())
    {
        return range_view.PopBack();
    }

    /// \brief Invoke the method via non-member function, possibly using ADL.
    template <typename TRangeView>
    auto InvokePopBack(Immutable<TRangeView> range_view, Priority<0>)
       noexcept -> decltype(PopBack(range_view))
    {
        return PopBack(range_view);
    }

    /// \brief Routes the invocation.
    template <typename TRangeView>
    auto RoutePopBack(Immutable<TRangeView> range_view)
        noexcept -> decltype(InvokePopBack(range_view, Priority<2>{}))
    {
        return InvokePopBack(range_view, Priority<2>{});
    }

}

// ===========================================================================
