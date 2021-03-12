
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
    // Based on this amazing post:
    // https://wandbox.org/permlink/AB9uQxO2MymNDDtt

    /************************************************************************/
    /* AT                                                                   */
    /************************************************************************/

    /// \brief Invoke the method via a custom extension.
    template <typename TRange, typename TIndex>
    inline auto
    InvokeAt(
        Immutable<TRange> range,
        Immutable<TIndex> index,
        Syntropy::Templates::Priority<3>)
            noexcept -> decltype(Ranges::Extensions::At<TRange>{}(range,
                                                                  index))
    {
        return Ranges::Extensions::At<TRange>{}(range, index);
    }

    /// \brief Invoke the method via member-operator.
    template <typename TRange, typename TIndex>
    inline auto
    InvokeAt(Immutable<TRange> range,
             Immutable<TIndex> index,
             Syntropy::Templates::Priority<2>)
                noexcept -> decltype(range[index])
    {
        return range[index];
    }

    /// \brief Invoke the method via member-function.
    template <typename TRange, typename TIndex>
    inline auto
    InvokeAt(Immutable<TRange> range,
             Immutable<TIndex> index,
             Syntropy::Templates::Priority<1>)
                noexcept -> decltype(range.At(index))
    {
        return range.At(index);
    }

    /// \brief Invoke the method via non-member function, possibly using ADL.
    template <typename TRange, typename TIndex>
    inline auto
    InvokeAt(Immutable<TRange> range,
             Immutable<TIndex> index,
             Syntropy::Templates::Priority<0>)
                noexcept -> decltype(At(range, index))
    {
        return At(range, index);
    }

    /// \brief Routes the invocation.
    template <typename TRange, typename TIndex>
    inline auto
    RouteAt(Immutable<TRange> range, Immutable<TIndex> index)
        noexcept -> decltype(
            InvokeAt(range, index, Syntropy::Templates::kPriority<3>))
    {
        return InvokeAt(range, index, Syntropy::Templates::kPriority<3>);
    }

    /************************************************************************/
    /* SLICE                                                                */
    /************************************************************************/

    /// \brief Invoke the method via a custom extension.
    template <typename TRange, typename TIndex, typename TCount>
    inline auto
    InvokeSlice(
        Immutable<TRange> range,
        Immutable<TIndex> index,
        Immutable<TCount> count,
        Syntropy::Templates::Priority<2>)
            noexcept -> decltype(
                Ranges::Extensions::Slice<TRange>{}(range, index, count))
    {
        return Ranges::Extensions::Slice<TRange>{}(range, index, count);
    }

    /// \brief Invoke the method via member-function.
    template <typename TRange, typename TIndex, typename TCount>
    inline auto
    InvokeSlice(Immutable<TRange> range,
                Immutable<TIndex> index,
                Immutable<TCount> count,
                Syntropy::Templates::Priority<1>)
                    noexcept -> decltype(range.Slice(index, count))
    {
        return range.Slice(index, count);
    }

    /// \brief Invoke the method via non-member function, possibly using ADL.
    template <typename TRange, typename TIndex, typename TCount>
    inline auto
    InvokeSlice(
        Immutable<TRange> range,
        Immutable<TIndex> index,
        Immutable<TCount> count,
        Syntropy::Templates::Priority<0>)
            noexcept -> decltype(Slice(range, index, count))
    {
        return Slice(range, index, count);
    }

    /// \brief Routes the invocation.
    template <typename TRange, typename TIndex, typename TCount>
    inline auto
    RouteSlice(Immutable<TRange> range,
               Immutable<TIndex> index,
               Immutable<TCount> count)
                   noexcept -> decltype(
                       InvokeSlice(range,
                                   index,
                                   count,
                                   Syntropy::Templates::kPriority<2>))
    {
        return InvokeSlice(range,
                           index,
                           count,
                           Syntropy::Templates::kPriority<2>);
    }

}

// ===========================================================================
