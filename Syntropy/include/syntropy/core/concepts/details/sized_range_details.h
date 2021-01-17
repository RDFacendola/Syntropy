
/// \file sized_range_details.h
/// \brief This header is part of the Syntropy core module. It contains implementation details about sized ranges.
///
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/priority.h"

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* SIZED RANGE EXTENSIONS                                               */
    /************************************************************************/

    /// \brief Get range's elements count.
    template <typename TType>
    struct Count;
}

// ===========================================================================

namespace Syntropy::Ranges::Details
{
    // Based on this amazing post: https://wandbox.org/permlink/AB9uQxO2MymNDDtt

    /************************************************************************/
    /* COUNT                                                                */
    /************************************************************************/

    /// \brief Invoke the method via a custom extension.
    template <typename TRange>
    inline auto InvokeCount(Immutable<TRange> range, Syntropy::Templates::Priority<2>) noexcept -> decltype(Ranges::Extensions::Count<TRange>{}(range))
    {
        return Ranges::Extensions::Count<TRange>{}(range);
    }

    /// \brief Invoke the method via member-function.
    template <typename TRange>
    inline auto InvokeCount(Immutable<TRange> range, Syntropy::Templates::Priority<1>) noexcept -> decltype(range.GetCount())
    {
        return range.GetCount();
    }

    /// \brief Invoke the method via non-member function, possibly using ADL.
    template <typename TRange>
    inline auto InvokeCount(Immutable<TRange> range, Syntropy::Templates::Priority<0>) noexcept -> decltype(Count(range))
    {
        return Count(range);
    }

    /// \brief Routes the invocation.
    template <typename TRange>
    inline auto RouteCount(Immutable<TRange> range) noexcept -> decltype(InvokeCount(range, Syntropy::Templates::kPriority<2>))
    {
        return InvokeCount(range, Syntropy::Templates::kPriority<2>);
    }

}

// ===========================================================================