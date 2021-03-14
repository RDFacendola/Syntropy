
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
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    template <Int TPriority>
    using Priority = Syntropy::Templates::Priority<TPriority>;

    template <typename TRangeView>
    using ExtensionData = Ranges::Extensions::Data<TRangeView>;

    /************************************************************************/
    /* DATA                                                                 */
    /************************************************************************/

    /// \brief Invoke the method via a custom extension.
    template <typename TRangeView>
    inline auto
    InvokeData(Immutable<TRangeView> range_view, Priority<2>)
       noexcept -> decltype(ExtensionData<TRangeView>{}(range_view))
    {
        return ExtensionData<TRangeView>{}(range_view);
    }

    /// \brief Invoke the method via member-function.
    template <typename TRangeView>
    inline auto
    InvokeData(Immutable<TRangeView> range_view, Priority<1>)
       noexcept -> decltype(range_view.GetData())
    {
        return range_view.GetData();
    }

    /// \brief Invoke the method via non-member function, possibly using ADL.
    template <typename TRangeView>
    inline auto
    InvokeData(Immutable<TRangeView> range_view, Priority<0>)
       noexcept -> decltype(Data(range_view))
    {
        return Data(range_view);
    }

    /// \brief Routes the invocation.
    template <typename TRangeView>
    inline auto
    RouteData(Immutable<TRangeView> range_view)
        noexcept -> decltype(InvokeData(range_view, Priority<2>{}))
    {
        return InvokeData(range_view, Priority<2>{});
    }
}

// ===========================================================================
