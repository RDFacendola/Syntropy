
/// \file sized_range.details.h
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
    /* COUNT                                                                */
    /************************************************************************/

    /// \brief Invoke the method via a custom extension.
    template <typename TRangeView>
    inline auto
    InvokeCount(Immutable<TRangeView> range_view, Syntropy::Templates::Priority<2>)
        noexcept -> decltype(Ranges::Extensions::Count<TRangeView>{}(range_view))
    {
        return Ranges::Extensions::Count<TRangeView>{}(range_view);
    }

    /// \brief Invoke the method via member-function.
    template <typename TRangeView>
    inline auto
    InvokeCount(Immutable<TRangeView> range_view, Syntropy::Templates::Priority<1>)
        noexcept -> decltype(range_view.GetCount())
    {
        return range_view.GetCount();
    }

    /// \brief Invoke the method via non-member function, possibly using ADL.
    template <typename TRangeView>
    inline auto
    InvokeCount(Immutable<TRangeView> range_view, Syntropy::Templates::Priority<0>)
        noexcept -> decltype(Count(range_view))
    {
        return Count(range_view);
    }

    /// \brief Routes the invocation.
    template <typename TRangeView>
    inline auto
    RouteCount(Immutable<TRangeView> range_view)
        noexcept -> decltype(InvokeCount(range_view,
                                         Syntropy::Templates::kPriority<2>))
    {
        return InvokeCount(range_view, Syntropy::Templates::kPriority<2>);
    }

}

// ===========================================================================
