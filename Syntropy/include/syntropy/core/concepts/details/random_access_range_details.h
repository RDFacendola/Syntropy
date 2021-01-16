
/// \file random_access_range_details.h
/// \brief This header is part of the Syntropy core module. It contains implementation details about random-access ranges.
///
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/priority.h"

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* RANDOM ACCESS RANGE EXTENSIONS                                       */
    /************************************************************************/

    // Functors to extend ranges support to custom types.

    /// \brief Access range's element by index.
    template <typename TType>
    struct At;

    /// \brief Obtain a view to a sub-range.
    template <typename TType>
    struct Slice;
}

// ===========================================================================

namespace Syntropy::Ranges::Details
{
    /************************************************************************/
    /* RANDOM ACCESS RANGE                                                  */
    /************************************************************************/

    // Based on this amazing post: https://wandbox.org/permlink/AB9uQxO2MymNDDtt

    // AtRouter.
    // =========

    /// \brief Route the "At" function across different customization points.
    class AtRouter
    {
    public:

        /// \brief Routes the invocation.
        template <typename TRange, typename TIndex>
        auto operator()(Immutable<TRange> range, Immutable<TIndex> index) const noexcept -> decltype((*this)(range, index))
        {
            return (*this)(range, index, Syntropy::Templates::kPriority<3>);
        }

    private:

        /// \brief Member-operator.
        template <typename TRange, typename TIndex>
        auto operator()(Immutable<TRange> range, Immutable<TIndex> index, Syntropy::Templates::Priority<3>) const noexcept -> decltype(range[index])
        {
            return range[index];
        }

        /// \brief Member-function.
        template <typename TRange, typename TIndex>
        auto operator()(Immutable<TRange> range, Immutable<TIndex> index, Syntropy::Templates::Priority<2>) const noexcept -> decltype(range.At(index))
        {
            return range.At(index);
        }

        /// \brief Non-member function (via ADL).
        template <typename TRange, typename TIndex>
        auto operator()(Immutable<TRange> range, Immutable<TIndex> index, Syntropy::Templates::Priority<1>) const noexcept -> decltype(At(range, index))
        {
            // VS2019 bug (possibly) - Cannot define this method outside its declaration.
            return At(range, index);
        }

        /// \brief Custom extension.
        template <typename TRange, typename TIndex>
        auto operator()(Immutable<TRange> range, Immutable<TIndex> index, Syntropy::Templates::Priority<0>) const noexcept -> decltype(Ranges::Extensions::At<TRange>{}(range, index))
        {
            return Ranges::Extensions::At<TRange>{}(range, index);
        }
    };

    // SliceRouter.
    // ============

    /// \brief Route the "Slice" function across different customization points.
    class SliceRouter
    {
    public:

        /// \brief Routes the invocation.
        template <typename TRange, typename TIndex, typename TCount>
        auto operator()(Immutable<TRange> range, Immutable<TIndex> index, Immutable<TCount> count) const noexcept -> decltype((*this)(range, index, count))
        {
            return (*this)(range, index, count, Syntropy::Templates::kPriority<2>);
        }

    private:

        /// \brief Member-function.
        template <typename TRange, typename TIndex, typename TCount>
        auto operator()(Immutable<TRange> range, Immutable<TIndex> index, Immutable<TCount> count, Syntropy::Templates::Priority<2>) const noexcept -> decltype(range.Slice(index, count))
        {
            return range.Slice(index, count);
        }

        /// \brief Non-member function (via ADL).
        template <typename TRange, typename TIndex, typename TCount>
        auto operator()(Immutable<TRange> range, Immutable<TIndex> index, Immutable<TCount> count, Syntropy::Templates::Priority<1>) const noexcept -> decltype(Slice(range, index, count))
        {
            // VS2019 bug (possibly) - Cannot define this method outside its declaration.
            return Slice(range, index, count);
        }

        /// \brief Custom extension.
        template <typename TRange, typename TIndex, typename TCount>
        auto operator()(Immutable<TRange> range, Immutable<TIndex> index, Immutable<TCount> count, Syntropy::Templates::Priority<0>) const noexcept -> decltype(Ranges::Extensions::Slice<TRange>{}(range, index, count))
        {
            return Ranges::Extensions::Slice<TRange>{}(range, index, count);
        }
    };

}

// ===========================================================================