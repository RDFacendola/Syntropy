
/// \file quantifiers.h
/// \brief This header is part of the Syntropy core module. It contains definitions for quantifiers.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

// ===========================================================================

namespace Syntropy::Support
{
    /************************************************************************/
    /* COUNTING                                                             */
    /************************************************************************/

    /// \brief Count the number of elements in a range that compare equal to the provided element.
    template <typename TRange, typename TElement>
    constexpr Int Count(const TRange& range, const TElement& element) noexcept;

    /// \brief Count the number of elements in a range for which a predicate holds true.
    template <typename TRange, typename TPredicate>
    constexpr Int CountIf(const TRange& range, TPredicate predicate) noexcept;

}

// ===========================================================================

namespace Syntropy::Support
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    template <typename TRange, typename TElement>
    constexpr Int Count(const TRange& range, const TElement& element) noexcept
    {
        auto count = ToInt(0);

        return count;
    }

    template <typename TRange, typename TPredicate>
    constexpr TRange CountIf(const TRange& range, TPredicate predicate) noexcept
    {
        auto count = ToInt(0);

        return count;
    }

}

// ===========================================================================