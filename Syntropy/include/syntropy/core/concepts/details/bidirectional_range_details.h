
/// \file bidirectional_range_details.h
/// \brief This header is part of the Syntropy core module. It contains implementation details about bidirectional ranges.
///
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/priority.h"

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* BIDIRECTIONAL RANGE EXTENSIONS                                       */
    /************************************************************************/

    // Functors to extend ranges support to custom types.

    /// \brief Access range's last element.
    template <typename TType>
    struct Back;

    /// \brief Discard range's last element and return the resulting range.
    template <typename TType>
    struct PopBack;
}

// ===========================================================================

namespace Syntropy::Ranges::Details
{
    /************************************************************************/
    /* BIDIRECTIONAL RANGE                                                  */
    /************************************************************************/

    // Based on this amazing post: https://wandbox.org/permlink/AB9uQxO2MymNDDtt

    // BackRouter.
    // ===========

    /// \brief Route the "Back" function across different customization points.
    class BackRouter
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
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<2>) const noexcept -> decltype(range.GetBack())
        {
            return range.GetBack();
        }

        /// \brief Non-member function (via ADL).
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<1>) const noexcept -> decltype(Back(range))
        {
            // VS2019 bug (possibly) - Cannot define this method outside its declaration.
            return Back(range);
        }

        /// \brief Custom extension.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<0>) const noexcept -> decltype(Ranges::Extensions::Back<TRange>{}(range))
        {
            return Ranges::Extensions::Back<TRange>{}(range);
        }
    };

    // PopBackRouter.
    // ==============

    /// \brief Route the "PopBack" function across different customization points.
    class PopBackRouter
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
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<2>) const noexcept -> decltype(range.PopBack())
        {
            return range.PopBack();
        }

        /// \brief Non-member function (via ADL).
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<1>) const noexcept -> decltype(PopBack(range))
        {
            // VS2019 bug (possibly) - Cannot define this method outside its declaration.
            return PopBack(range);
        }

        /// \brief Custom extension.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<0>) const noexcept -> decltype(Ranges::Extensions::PopBack<TRange>{}(range))
        {
            return Ranges::Extensions::PopBack<TRange>{}(range);
        }
    };

}

// ===========================================================================