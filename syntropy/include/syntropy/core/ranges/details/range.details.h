
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
    /* VIEW OF                                                              */
    /************************************************************************/

    /// \brief Invoke the method via a custom extension.
    template <typename TRangeView>
    auto InvokeViewOf(Forwarding<TRangeView> range_view,
                      Syntropy::Templates::Priority<2>)
        noexcept ->
        decltype(Syntropy::Ranges::Extensions::ViewOf<TRangeView>{}(
            Forward<TRangeView>(range_view)))
    {
        return Syntropy::Ranges::Extensions::ViewOf<TRangeView>{}(
            Forward<TRangeView>(range_view));
    }

    /// \brief Invoke the method via member-function.
    template <typename TRangeView>
    auto InvokeViewOf(Forwarding<TRangeView> range_view,
                      Syntropy::Templates::Priority<1>)
        noexcept -> decltype(range_view.GetView())
    {
        return range_view.GetView();
    }

    /// \brief Invoke the method via non-member function, possibly using ADL.
    template <typename TRangeView>
    auto InvokeViewOf(Forwarding<TRangeView> range_view,
                      Syntropy::Templates::Priority<0>)
        noexcept -> decltype(ViewOf(Forward<TRangeView>(range_view)))
    {
        return ViewOf(Forward<TRangeView>(range_view));
    }

    /// \brief Routes the invocation.
    template <typename TRangeView>
    auto RouteViewOf(Forwarding<TRangeView> range_view)
        -> decltype(InvokeFront(Forward<TRangeView>(range_view),
                                Syntropy::Templates::kPriority<2>))
    {
        return InvokeViewOf(Forward<TRangeView>(range_view),
                            Syntropy::Templates::kPriority<2>);
    }

}

// ===========================================================================
