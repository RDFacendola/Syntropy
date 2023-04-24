
/// \file searching.h
/// \brief This header is part of the Syntropy core module. It contains definitions for searching functions.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/experimental/core/foundation/range.h"

// ===========================================================================

namespace Syntropy::Support
{
    /************************************************************************/
    /* FIND                                                                 */
    /************************************************************************/

    /// \brief Reduce range from the front until element compares equal to the front element in the range or the range is exhausted.
    /// \return Returns the reduced range starting from the first occurrence of element in the range or an empty range if no occurrence was found.
    template <Templates::ForwardRangeT TRange, typename TElement>
    constexpr TRange Find(const TRange& range, const TElement& element) noexcept;

    /// \brief Reduce range from the front until the predicate holds true for the front element or the range is exhausted.
    /// \return Returns the reduced range starting from the first occurrence of an element in the range for which predicate holds true or an empty range if no such occurrence was found.
    template <Templates::ForwardRangeT TRange, typename TPredicate>
    constexpr TRange FindIf(const TRange& range, TPredicate predicate) noexcept;

    /// \brief Reduce range from the back until element compares equal to the rear element in the range or the range is exhausted.
    /// \return Returns the reduced range ending with the last occurrence of element in the range or an empty range if no such occurrence was found.
    template <Templates::BidirectionalRangeT TRange, typename TElement>
    constexpr TRange FindReverse(const TRange& range, const TElement& element) noexcept;

    /// \brief Reduce range from the back until the predicate holds true for the rear element or the range is exhausted.
    /// \return Returns the reduced range ending with the last occurrence of an element in the range for which predicate holds true or an empty range if no such occurrence was found.
    template <Templates::BidirectionalRangeT TRange, typename TPredicate>
    constexpr TRange FindIfReverse(const TRange& range, TPredicate&& predicate) noexcept;
}

// ===========================================================================

namespace Syntropy::Support
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Find.
    // =====

    template <Templates::ForwardRangeT TRange, typename TElement>
    constexpr TRange Find(const TRange& range, const TElement& element) noexcept
    {
        auto result = range;

        for (; !IsEmpty(result) && (Front(result) != element); result = PopFront(result));

        return result;
    }

    template <Templates::ForwardRangeT TRange, typename TPredicate>
    constexpr TRange FindIf(const TRange& range, TPredicate predicate) noexcept
    {
        auto result = range;

        for (; !IsEmpty(result) && !predicate(ReadOnly(Front(result))); result = PopFront(result));

        return result;
    }

    template <Templates::BidirectionalRangeT TRange, typename TElement>
    constexpr TRange FindReverse(const TRange& range, const TElement& element) noexcept
    {
        return Reverse(Find(Reverse(range), element));
    }

    template <Templates::BidirectionalRangeT TRange, typename TPredicate>
    constexpr TRange FindIfReverse(const TRange& range, TPredicate&& predicate) noexcept
    {
        return Reverse(FindIf(Reverse(range), Forward<TPredicate>(predicate)));
    }
}

// ===========================================================================