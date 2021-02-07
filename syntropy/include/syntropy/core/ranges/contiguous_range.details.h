
/// \file contiguous_range.details.h
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
    /* DATA                                                                 */
    /************************************************************************/

    /// \brief Invoke the method via a custom extension.
    template <typename TRange>
    inline auto
    InvokeData(Immutable<TRange> range, Syntropy::Templates::Priority<2>)
       noexcept -> decltype(Ranges::Extensions::Data<TRange>{}(range))
    {
        return Ranges::Extensions::Data<TRange>{}(range);
    }

    /// \brief Invoke the method via member-function.
    template <typename TRange>
    inline auto
    InvokeData(Immutable<TRange> range, Syntropy::Templates::Priority<1>)
       noexcept -> decltype(range.GetData())
    {
        return range.GetData();
    }

    /// \brief Invoke the method via non-member function, possibly using ADL.
    template <typename TRange>
    inline auto
    InvokeData(Immutable<TRange> range, Syntropy::Templates::Priority<0>)
       noexcept -> decltype(Data(range))
    {
        return Data(range);
    }

    /// \brief Routes the invocation.
    template <typename TRange>
    inline auto
    RouteData(Immutable<TRange> range)
        noexcept -> decltype(InvokeData(range,
                                        Syntropy::Templates::kPriority<2>))
    {
        return InvokeData(range, Syntropy::Templates::kPriority<2>);
    }
}

// ===========================================================================
