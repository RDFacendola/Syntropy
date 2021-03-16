
/// \file random_access_range.details.h
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

    /************************************************************************/
    /* AT                                                                   */
    /************************************************************************/

    /// \brief Invoke the method via a custom extension.
    template <typename TRangeView, typename TIndex>
    inline auto
    InvokeAt(Immutable<TRangeView> range_view,
             Immutable<TIndex> index,
             Priority<3>)
                noexcept -> decltype(
                    Extensions::At<TRangeView>{}(range_view, index))
    {
        return Extensions::At<TRangeView>{}(range_view, index);
    }

    /// \brief Invoke the method via member-operator.
    template <typename TRangeView, typename TIndex>
    inline auto
    InvokeAt(Immutable<TRangeView> range_view,
             Immutable<TIndex> index,
             Priority<2>)
                noexcept -> decltype(range_view[index])
    {
        return range_view[index];
    }

    /// \brief Invoke the method via member-function.
    template <typename TRangeView, typename TIndex>
    inline auto
    InvokeAt(Immutable<TRangeView> range_view,
             Immutable<TIndex> index,
             Priority<1>)
                noexcept -> decltype(range_view.At(index))
    {
        return range_view.At(index);
    }

    /// \brief Invoke the method via non-member function, possibly using ADL.
    template <typename TRangeView, typename TIndex>
    inline auto
    InvokeAt(Immutable<TRangeView> range_view,
             Immutable<TIndex> index,
             Priority<0>)
                noexcept -> decltype(At(range_view, index))
    {
        return At(range_view, index);
    }

    /// \brief Routes the invocation.
    template <typename TRangeView, typename TIndex>
    inline auto
    RouteAt(Immutable<TRangeView> range_view, Immutable<TIndex> index)
        noexcept -> decltype(
            InvokeAt(range_view, index, Syntropy::Templates::kPriority<3>))
    {
        return InvokeAt(range_view, index, Syntropy::Templates::kPriority<3>);
    }

    /************************************************************************/
    /* SLICE                                                                */
    /************************************************************************/

    /// \brief Invoke the method via a custom extension.
    template <typename TRangeView, typename TIndex, typename TCount>
    inline auto
    InvokeSlice(Immutable<TRangeView> range_view,
                Immutable<TIndex> index,
                Immutable<TCount> count,
                Priority<2>)
                    noexcept -> decltype(
                Extensions::Slice<TRangeView>{}(range_view, index, count))
    {
        return Extensions::Slice<TRangeView>{}(range_view, index, count);
    }

    /// \brief Invoke the method via member-function.
    template <typename TRangeView, typename TIndex, typename TCount>
    inline auto
    InvokeSlice(Immutable<TRangeView> range_view,
                Immutable<TIndex> index,
                Immutable<TCount> count,
                Priority<1>)
                    noexcept -> decltype(range_view.Slice(index, count))
    {
        return range_view.Slice(index, count);
    }

    /// \brief Invoke the method via non-member function, possibly using ADL.
    template <typename TRangeView, typename TIndex, typename TCount>
    inline auto
    InvokeSlice(
        Immutable<TRangeView> range_view,
        Immutable<TIndex> index,
        Immutable<TCount> count,
        Priority<0>)
            noexcept -> decltype(Slice(range_view, index, count))
    {
        return Slice(range_view, index, count);
    }

    /// \brief Routes the invocation.
    template <typename TRangeView, typename TIndex, typename TCount>
    inline auto
    RouteSlice(Immutable<TRangeView> range_view,
               Immutable<TIndex> index,
               Immutable<TCount> count)
                   noexcept -> decltype(
                       InvokeSlice(range_view, index, count, Priority<2>{}))
    {
        return InvokeSlice(range_view,
                           index,
                           count,
                           Priority<2>{});
    }

}

// ===========================================================================
