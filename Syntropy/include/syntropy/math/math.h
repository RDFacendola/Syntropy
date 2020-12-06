
/// \file math.h
/// \brief This header is part of the Syntropy math module. It contains generic mathematical functions.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/type_traits.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* MATH                                                                 */
    /************************************************************************/

    /// \brief Exposes arithmetic functions.
    /// \author Raffaele D. Facendola - May 2020.
    namespace Math
    {
        /// \brief Get the minimum element.
        template <typename TValue, typename... TValues>
        constexpr Templates::CommonType<TValue, TValues...> Min(Immutable<TValue> first, Immutable<TValues>... rest) noexcept;

        /// \brief Get the maximum element.
        template <typename TValue, typename... TValues>
        constexpr Templates::CommonType<TValue, TValues...> Max(Immutable<TValue> first, Immutable<TValues>... rest) noexcept;
    }

}

// ===========================================================================


namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Math.
    // =====

    template <typename TValue, typename... TValues>
    constexpr Templates::CommonType<TValue, TValues...> Math::Min(Immutable<TValue> first, Immutable<TValues>... rest) noexcept
    {
        auto min = Templates::CommonType<TValue, TValues...>{ first };

        auto fold = [&min]<typename UValue>(Immutable<UValue> value)
        {
            min = (min < value) ? min : value;
        };

        (fold(rest), ...);

        return min;
    }

    template <typename TValue, typename... TValues>
    constexpr Templates::CommonType<TValue, TValues...> Math::Max(Immutable<TValue> first, Immutable<TValues>... rest) noexcept
    {
        auto max = Templates::CommonType<TValue, TValues...>{ first };

        auto fold = [&max]<typename UValue>(Immutable<UValue> value)
        {
            max = (max > value) ? max : value;
        };

        (fold(rest), ...);

        return max;
    }
}

// ===========================================================================
