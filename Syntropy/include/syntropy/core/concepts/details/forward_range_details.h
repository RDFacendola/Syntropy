
/// \file forward_range_details.h
/// \brief This header is part of the Syntropy core module. It contains implementation details about forward ranges.
///
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/priority.h"

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* FORWARD RANGE EXTENSIONS                                             */
    /************************************************************************/

    // Functors to extend ranges support to custom types.

    /// \brief Access range's first element.
    template <typename TType>
    struct Front;

    /// \brief Discard range's first element and return the resulting range.
    template <typename TType>
    struct PopFront;

    /// \brief Check whether a range is empty.
    template <typename TType>
    struct IsEmpty;
}

// ===========================================================================

namespace Syntropy::Ranges::Details
{
    // Based on this amazing post: https://wandbox.org/permlink/AB9uQxO2MymNDDtt

    /************************************************************************/
    /* FRONT ROUTER                                                         */
    /************************************************************************/

    template <typename TRange>
    void Front(Immutable<TRange>) noexcept;

    /// \brief Route the "Front" function across different customization points.
    struct FrontRouter
    {
        /// \brief Custom extension.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<2>) const noexcept -> decltype(Ranges::Extensions::Front<TRange>{}(range));

        /// \brief Member-function.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<1>) const noexcept -> decltype(range.GetFront());

        /// \brief Non-member function (via ADL).
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<0>) const noexcept -> decltype(Front(range));

        /// \brief Routes the invocation.
        template <typename TRange>
        auto operator()(Immutable<TRange> range) const noexcept -> decltype((*this)(range, Syntropy::Templates::kPriority<2>));
    };

    /************************************************************************/
    /* POP FRONT ROUTER                                                     */
    /************************************************************************/

    template <typename TRange>
    void PopFront(Immutable<TRange>) noexcept;

    /// \brief Route the "PopFront" function across different customization points.
    struct PopFrontRouter
    {
        /// \brief Custom extension.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<2>) const noexcept -> decltype(Ranges::Extensions::PopFront<TRange>{}(range));

        /// \brief Invokes a member function on the range.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<1>) const noexcept -> decltype(range.PopFront());

        /// \brief Invokes a non-member function via ADL.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<0>) const noexcept -> decltype(PopFront(range));

        /// \brief Routes the invocation.
        template <typename TRange>
        auto operator()(Immutable<TRange> range) const noexcept -> decltype((*this)(range, Syntropy::Templates::kPriority<2>));
    };

    /************************************************************************/
    /* IS EMPTY ROUTER                                                      */
    /************************************************************************/

    template <typename TRange>
    void IsEmpty(Immutable<TRange>) noexcept;

    /// \brief Route the "IsEmpty" function across different customization points.
    struct IsEmptyRouter
    {
        /// \brief Custom extension.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<2>) const noexcept -> decltype(Ranges::Extensions::IsEmpty<TRange>{}(range));

        /// \brief Invokes a member function on the range.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<1>) const noexcept -> decltype(range.IsEmpty());

        /// \brief Invokes a non-member function via ADL.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<0>) const noexcept -> decltype(IsEmpty(range));

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

    // FrontRouter.
    // ============

    template <typename TRange>
    inline auto FrontRouter::operator()(Immutable<TRange> range, Syntropy::Templates::Priority<2>) const noexcept -> decltype(Ranges::Extensions::Front<TRange>{}(range))
    {
        return Ranges::Extensions::Front<TRange>{}(range);
    }

    template <typename TRange>
    inline auto FrontRouter::operator()(Immutable<TRange> range, Syntropy::Templates::Priority<1>) const noexcept -> decltype(range.GetFront())
    {
        return range.GetFront();
    }

    template <typename TRange>
    inline auto FrontRouter::operator()(Immutable<TRange> range, Syntropy::Templates::Priority<0>) const noexcept -> decltype(Front(range))
    {
        return Front(range);
    }

    template <typename TRange>
    inline auto FrontRouter::operator()(Immutable<TRange> range) const noexcept -> decltype((*this)(range, Syntropy::Templates::kPriority<2>))
    {
        return (*this)(range, Syntropy::Templates::kPriority<2>);
    }

    // PopFrontRouter.
    // ===============

    template <typename TRange>
    inline auto PopFrontRouter::operator()(Immutable<TRange> range, Syntropy::Templates::Priority<2>) const noexcept -> decltype(Ranges::Extensions::PopFront<TRange>{}(range))
    {
        return Ranges::Extensions::PopFront<TRange>{}(range);
    }

    template <typename TRange>
    inline auto PopFrontRouter::operator()(Immutable<TRange> range, Syntropy::Templates::Priority<1>) const noexcept -> decltype(range.PopFront())
    {
        return range.PopFront();
    }

    template <typename TRange>
    inline auto PopFrontRouter::operator()(Immutable<TRange> range, Syntropy::Templates::Priority<0>) const noexcept -> decltype(PopFront(range))
    {
        return PopFront(range);
    }

    template <typename TRange>
    inline auto PopFrontRouter::operator()(Immutable<TRange> range) const noexcept -> decltype((*this)(range, Syntropy::Templates::kPriority<2>))
    {
        return (*this)(range, Syntropy::Templates::kPriority<2>);
    }

    // IsEmptyRouter.
    // ==============

    template <typename TRange>
    inline auto IsEmptyRouter::operator()(Immutable<TRange> range, Syntropy::Templates::Priority<2>) const noexcept -> decltype(Ranges::Extensions::IsEmpty<TRange>{}(range))
    {
        return Ranges::Extensions::IsEmpty<TRange>{}(range);
    }

    template <typename TRange>
    inline auto IsEmptyRouter::operator()(Immutable<TRange> range, Syntropy::Templates::Priority<1>) const noexcept -> decltype(range.IsEmpty())
    {
        return range.IsEmpty();
    }

    template <typename TRange>
    inline auto IsEmptyRouter::operator()(Immutable<TRange> range, Syntropy::Templates::Priority<0>) const noexcept -> decltype(IsEmpty(range))
    {
        return IsEmpty(range);
    }

    template <typename TRange>
    inline auto IsEmptyRouter::operator()(Immutable<TRange> range) const noexcept -> decltype((*this)(range, Syntropy::Templates::kPriority<2>))
    {
        return (*this)(range, Syntropy::Templates::kPriority<2>);
    }

}

// ===========================================================================
