
/// \file ranges.h
/// \brief This header is part of the Syntropy core module. It contains definitions for ranges and basic ranges functionalities.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/support/support.h"

// ===========================================================================

namespace Syntropy::Traits
{
    /************************************************************************/
    /* TRAITS                                                               */
    /************************************************************************/

    /// \brief Type of a range element.
    template <typename TRange>
    using RangeElement = decltype(Front(Declval<TRange>()));

}

// ===========================================================================

namespace Syntropy::Algorithm
{
    /************************************************************************/
    /* RANGE                                                                */
    /************************************************************************/

    /// \brief Apply a function to each element in the range.
    template <typename TRange, typename TFunction>
    constexpr void ForEach(TRange&& range, TFunction function) noexcept;

}

// ===========================================================================

namespace Syntropy::Algorithm
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Ranges.
    // =======

    template <typename TRange, typename TFunction>
    constexpr void ForEach(TRange&& range, TFunction function) noexcept
    {
        for (auto&& element : range)
        {
            function(element);
        }
    }

}

// ===========================================================================