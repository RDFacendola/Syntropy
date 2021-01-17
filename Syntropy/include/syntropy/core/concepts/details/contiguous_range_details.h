
/// \file contiguous_range_details.h
/// \brief This header is part of the Syntropy core module. It contains implementation details about contiguous ranges.
///
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/priority.h"

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* CONTIGUOUS RANGE EXTENSIONS                                          */
    /************************************************************************/

    // Functors to extend ranges support to custom types.

    /// \brief Access range's element storage.
    template <typename TType>
    struct Data;
}

// ===========================================================================

namespace Syntropy::Ranges::Details
{
    // Based on this amazing post: https://wandbox.org/permlink/AB9uQxO2MymNDDtt

    /************************************************************************/
    /* DATA ROUTER                                                          */
    /************************************************************************/

    template <typename TRange>
    void Data(Immutable<TRange>) noexcept;

    /// \brief Route the "Data" function across different customization points.
    struct DataRouter
    {
        /// \brief Custom extension.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<2>) const noexcept -> decltype(Ranges::Extensions::Data<TRange>{}(range));

        /// \brief Member-function.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<1>) const noexcept -> decltype(range.GetData());

        /// \brief Non-member function (via ADL).
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<0>) const noexcept -> decltype(Data(range));

        /// \brief Routes the invocation.
        template <typename TRange>
        auto operator()(Immutable<TRange> range) const noexcept -> decltype((*this)(range, Syntropy::Templates::kPriority<2>));
    };

}

// ===========================================================================

namespace Syntropy::Ranges::Details
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // DataRouter.
    // ===========

    template <typename TRange>
    auto DataRouter::operator()(Immutable<TRange> range, Syntropy::Templates::Priority<2>) const noexcept -> decltype(Ranges::Extensions::Data<TRange>{}(range))
    {
        return Ranges::Extensions::Data<TRange>{}(range);
    }

    template <typename TRange>
    auto DataRouter::operator()(Immutable<TRange> range, Syntropy::Templates::Priority<1>) const noexcept -> decltype(range.GetData())
    {
        return range.GetData();
    }

    template <typename TRange>
    auto DataRouter::operator()(Immutable<TRange> range, Syntropy::Templates::Priority<0>) const noexcept -> decltype(Data(range))
    {
        return Data(range);
    }

    template <typename TRange>
    auto DataRouter::operator()(Immutable<TRange> range) const noexcept -> decltype((*this)(range, Syntropy::Templates::kPriority<2>))
    {
        return (*this)(range, Syntropy::Templates::kPriority<2>);
    }

}

// ===========================================================================