
/// \file range.details.h
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
    template <typename TRange>
    auto InvokeViewOf(Forwarding<TRange> range,
                      Syntropy::Templates::Priority<2>)
        noexcept ->
        decltype(Syntropy::Ranges::Extensions::ViewOf<TRange>{}(
            Forward<TRange>(range)))
    {
        return Syntropy::Ranges::Extensions::ViewOf<TRange>{}(
            Forward<TRange>(range));
    }

    /// \brief Invoke the method via member-function.
    template <typename TRange>
    auto InvokeViewOf(Forwarding<TRange> range,
                      Syntropy::Templates::Priority<1>)
        noexcept -> decltype(range.GetView())
    {
        return range.GetView();
    }

    /// \brief Invoke the method via non-member function, possibly using ADL.
    template <typename TRange>
    auto InvokeViewOf(Forwarding<TRange> range,
                      Syntropy::Templates::Priority<0>)
        noexcept -> decltype(ViewOf(Forward<TRange>(range)))
    {
        return ViewOf(Forward<TRange>(range));
    }

    /// \brief Routes the invocation.
    template <typename TRange>
    auto RouteViewOf(Forwarding<TRange> range)
        -> decltype(InvokeFront(Forward<TRange>(range),
                                Syntropy::Templates::kPriority<2>))
    {
        return InvokeViewOf(Forward<TRange>(range),
                            Syntropy::Templates::kPriority<2>);
    }

}

// ===========================================================================
