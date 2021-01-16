
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

    // Functors to extend ranges support to custom types.

    /// \brief Get range's elements count.
    template <typename TType>
    struct Count;
}

// ===========================================================================

namespace Syntropy::Ranges::Details
{
    /************************************************************************/
    /* SIZED RANGE                                                          */
    /************************************************************************/

    // Based on this amazing post: https://wandbox.org/permlink/AB9uQxO2MymNDDtt

    // CountRouter.
    // ============

    /// \brief Route the "Count" function across different customization points.
    class CountRouter
    {
    public:

        /// \brief Routes the invocation.
        template <typename TRange>
        auto operator()(Immutable<TRange> range) const noexcept -> decltype((*this)(range))
        {
            return (*this)(range, Syntropy::Templates::kPriority<2>);
        }

    private:

        /// \brief Member-function.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<2>) const noexcept -> decltype(range.GetCount())
        {
            return range.GetCount();
        }

        /// \brief Non-member function (via ADL).
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<1>) const noexcept -> decltype(Count(range))
        {
            // VS2019 bug (possibly) - Cannot define this method outside its declaration.
            return Count(range);
        }

        /// \brief Custom extension.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<0>) const noexcept -> decltype(Ranges::Extensions::Count<TRange>{}(range))
        {
            return Ranges::Extensions::Count<TRange>{}(range);
        }
    };

}

// ===========================================================================