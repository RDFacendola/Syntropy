
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
    // Based on this amazing post: https://wandbox.org/permlink/AB9uQxO2MymNDDtt

    /************************************************************************/
    /* BACK ROUTER                                                          */
    /************************************************************************/

    template <typename TRange>
    void Back(Immutable<TRange>) noexcept;

    /// \brief Route the "Back" function across different customization points.
    struct BackRouter
    {
        /// \brief Custom extension.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<2>) const noexcept -> decltype(Ranges::Extensions::Back<TRange>{}(range));

        /// \brief Member-function.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<1>) const noexcept -> decltype(range.GetBack());

        /// \brief Non-member function (via ADL).
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<0>) const noexcept -> decltype(Back(range));

        /// \brief Routes the invocation.
        template <typename TRange>
        auto operator()(Immutable<TRange> range) const noexcept -> decltype((*this)(range, Syntropy::Templates::kPriority<2>));
    };

    /************************************************************************/
    /* POP BACK ROUTER                                                      */
    /************************************************************************/

    template <typename TRange>
    void PopBack(Immutable<TRange>) noexcept;

    /// \brief Route the "PopBack" function across different customization points.
    struct PopBackRouter
    {
        /// \brief Custom extension.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<2>) const noexcept -> decltype(Ranges::Extensions::PopBack<TRange>{}(range));

        /// \brief Member-function.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<1>) const noexcept -> decltype(range.PopBack());

        /// \brief Non-member function (via ADL).
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<0>) const noexcept -> decltype(PopBack(range));

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

    // BackRouter.
    // ===========

    template <typename TRange>
    inline auto BackRouter::operator()(Immutable<TRange> range, Syntropy::Templates::Priority<2>) const noexcept -> decltype(Ranges::Extensions::Back<TRange>{}(range))
    {
        return Ranges::Extensions::Back<TRange>{}(range);
    }

    template <typename TRange>
    inline auto BackRouter::operator()(Immutable<TRange> range, Syntropy::Templates::Priority<1>) const noexcept -> decltype(range.GetBack())
    {
        return range.GetBack();
    }

    template <typename TRange>
    inline auto BackRouter::operator()(Immutable<TRange> range, Syntropy::Templates::Priority<0>) const noexcept -> decltype(Back(range))
    {
        return Back(range);
    }

    template <typename TRange>
    inline auto BackRouter::operator()(Immutable<TRange> range) const noexcept -> decltype((*this)(range, Syntropy::Templates::kPriority<2>))
    {
        return (*this)(range, Syntropy::Templates::kPriority<2>);
    }

    // PopBackRouter.
    // ==============

    template <typename TRange>
    inline auto PopBackRouter::operator()(Immutable<TRange> range, Syntropy::Templates::Priority<2>) const noexcept -> decltype(Ranges::Extensions::PopBack<TRange>{}(range))
    {
        return Ranges::Extensions::PopBack<TRange>{}(range);
    }

    template <typename TRange>
    inline auto PopBackRouter::operator()(Immutable<TRange> range, Syntropy::Templates::Priority<1>) const noexcept -> decltype(range.PopBack())
    {
        return range.PopBack();
    }

    template <typename TRange>
    inline auto PopBackRouter::operator()(Immutable<TRange> range, Syntropy::Templates::Priority<0>) const noexcept -> decltype(PopBack(range))
    {
        return PopBack(range);
    }

    template <typename TRange>
    inline auto PopBackRouter::operator()(Immutable<TRange> range) const noexcept -> decltype((*this)(range, Syntropy::Templates::kPriority<2>))
    {
        return (*this)(range, Syntropy::Templates::kPriority<2>);
    }

}



// ===========================================================================