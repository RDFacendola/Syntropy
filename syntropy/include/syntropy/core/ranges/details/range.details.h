
/// \file range_view.details.h
///
/// \author Raffaele D. Facendola - March 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/priority.h"

#include "syntropy/core/ranges/range_extensions.h"

// ===========================================================================

namespace Syntropy::Ranges::Details
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    template <Int TPriority>
    using Priority = Templates::Priority<TPriority>;

    /************************************************************************/
    /* VIEW OF                                                              */
    /************************************************************************/

    /// \brief Invoke the method via a custom extension.
    template <typename TRange>
    auto InvokeViewOf(Forwarding<TRange> range, Priority<2>)
        noexcept ->
        decltype(Extensions::ViewOf<TRange>{}(Forward<TRange>(range)))
    {
        return Extensions::ViewOf<TRange>{}(Forward<TRange>(range));
    }

    /// \brief Invoke the method via member-function.
    template <typename TRange>
    auto InvokeViewOf(Forwarding<TRange> range, Priority<1>)
        noexcept -> decltype(range.GetView())
    {
        return range.GetView();
    }

    /// \brief Invoke the method via non-member function, possibly using ADL.
    template <typename TRange>
    auto InvokeViewOf(Forwarding<TRange> range, Priority<0>)
        noexcept -> decltype(ViewOf(Forward<TRange>(range)))
    {
        return ViewOf(Forward<TRange>(range));
    }

    /// \brief Routes the invocation.
    template <typename TRange>
    auto RouteViewOf(Forwarding<TRange> range)
        noexcept -> decltype(
            InvokeViewOf(Forward<TRange>(range), Priority<2>{}))
    {
        return InvokeViewOf(Forward<TRange>(range), Priority<2>{});
    }

}

// ===========================================================================
