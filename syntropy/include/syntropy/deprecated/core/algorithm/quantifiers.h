
/// \file quantifiers.h
/// \brief This header is part of the Syntropy core module. It contains definitions for quantifiers.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/foundation/range.h"
#include "syntropy/core/algorithm/searching.h"

// ===========================================================================

namespace Syntropy::Algorithms
{
    /************************************************************************/
    /* QUANTIFIERS                                                          */
    /************************************************************************/

    /// \brief Check whether a predicate holds true for at least one element in a range of elements.
    template <typename TRange, typename TPredicate>
    constexpr Bool Any(Reference<TRange> range, TPredicate&& predicate) noexcept;

    /// \brief Check whether a predicate holds false for all the elements in a range of elements.
    template <typename TRange, typename TPredicate>
    constexpr Bool None(Reference<TRange> range, TPredicate&& predicate) noexcept;

    /// \brief Check whether a predicate holds true for all the elements in a range of elements.
    template <typename TRange, typename TPredicate>
    constexpr Bool All(Reference<TRange> range, TPredicate&& predicate) noexcept;
}

// ===========================================================================

namespace Syntropy::Algorithms
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    template <typename TRange, typename TPredicate>
    constexpr Bool Any(Reference<TRange> range, TPredicate&& predicate) noexcept
    {
        return !IsEmpty(FindIf(range, predicate));
    }

    template <typename TRange, typename TPredicate>
    constexpr Bool None(Reference<TRange> range, TPredicate&& predicate) noexcept
    {
        return !Any(range, Forward<TPredicate>(predicate));
    }

    template <typename TRange, typename TPredicate>
    constexpr Bool All(Reference<TRange> range, TPredicate&& predicate) noexcept
    {
        auto not_predicate = [predicate = Move(predicate)](Traits::RangeElement<TRange> element)
        {
            return !predicate(element);
        };

        return !Any(range, not_predicate);
    }

}

// ===========================================================================
