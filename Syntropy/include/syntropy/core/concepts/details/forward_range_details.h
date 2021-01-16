
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
    /************************************************************************/
    /* FORWARD RANGE                                                        */
    /************************************************************************/

    // Based on this amazing post: https://wandbox.org/permlink/AB9uQxO2MymNDDtt

    // FrontRouter.
    // ============

    /// \brief Route the "Front" function across different customization points.
    class FrontRouter
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
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<2>) const noexcept -> decltype(range.GetFront())
        {
            return range.GetFront();
        }

        /// \brief Non-member function (via ADL).
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<1>) const noexcept -> decltype(Front(range))
        {
            // VS2019 bug (possibly) - Cannot define this method outside its declaration.
            return Front(range);
        }

        /// \brief Custom extension.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<0>) const noexcept -> decltype(Ranges::Extensions::Front<TRange>{}(range))
        {
            return Ranges::Extensions::Front<TRange>{}(range);
        }
    };

    // PopFrontRouter.
    // ===============

    /// \brief Route the "PopFront" function across different customization points.
    class PopFrontRouter
    {
    public:

        /// \brief Routes the invocation.
        template <typename TRange>
        auto operator()(Immutable<TRange> range) const noexcept -> decltype((*this)(range))
        {
            return (*this)(range, Syntropy::Templates::kPriority<2>);
        }

    private:

        /// \brief Invokes a member function on the range.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<2>) const noexcept -> decltype(range.PopFront())
        {
            return range.PopFront();
        }

        /// \brief Invokes a non-member function via ADL.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<1>) const noexcept -> decltype(PopFront(range))
        {
            // VS2019 bug (possibly) - Cannot define this method outside its declaration.
            return PopFront(range);
        }

        /// \brief Custom extension.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<0>) const noexcept -> decltype(Ranges::Extensions::PopFront<TRange>{}(range))
        {
            return Ranges::Extensions::PopFront<TRange>{}(range);
        }
    };

    // IsEmptyRouter.
    // ==============

    /// \brief Route the "IsEmpty" function across different customization points.
    class IsEmptyRouter
    {
    public:

        /// \brief Routes the invocation.
        template <typename TRange>
        auto operator()(Immutable<TRange> range) const noexcept -> decltype((*this)(range))
        {
            return (*this)(range, Syntropy::Templates::kPriority<2>);
        }

    private:

        /// \brief Invokes a member function on the range.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<2>) const noexcept -> decltype(range.IsEmpty())
        {
            return range.IsEmpty();
        }

        /// \brief Invokes a non-member function via ADL.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<1>) const noexcept -> decltype(IsEmpty(range))
        {
            // VS2019 bug (possibly) - Cannot define this method outside its declaration.
            return IsEmpty(range);
        }

        /// \brief Custom extension.
        template <typename TRange>
        auto operator()(Immutable<TRange> range, Syntropy::Templates::Priority<0>) const noexcept -> decltype(Ranges::Extensions::IsEmpty<TRange>{}(range))
        {
            return Ranges::Extensions::IsEmpty<TRange>{}(range);
        }

    };

}

// ===========================================================================