
/// \file searching.h
/// \brief This header is part of the Syntropy core module. It contains definitions for searching functions.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/experimental/core/foundation/range.h"

// ===========================================================================

namespace Syntropy::Algorithm
{
    /************************************************************************/
    /* SEARCHING                                                             */
    /************************************************************************/

    /// \brief Reduce range until element becomes the first element in the range or the range is exhausted.
    /// \return Returns the reduced range starting from the first occurrence of the element in the range or an empty range if no occurrence was found.
    template <typename TRange, typename TElement>
    constexpr TRange Find(const TRange& range, const TElement& element) noexcept;

    /// \brief Count the number of elements in a range for which a predicate holds true.
    template <typename TRange, typename TPredicate>
    constexpr TRange FindIf(const TRange& range, TPredicate predicate) noexcept;

}

// ===========================================================================

namespace Syntropy::Algorithm
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    template <typename TRange, typename TElement>
    constexpr TRange Find(const TRange& range, const TElement& element) noexcept
    {
        auto result = range;

        for (; result && (Front(result) != element); result = PopFront(result));

        return result;
    }

    template <typename TRange, typename TPredicate>
    constexpr TRange FindIf(const TRange& range, TPredicate predicate) noexcept
    {
        auto result = range;

        for (; result && !predicate(ReadOnly(Front(result))); result = PopFront(result));

        return result;
    }

}

// ===========================================================================