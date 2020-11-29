
/// \file functional.h
/// \brief This header is part of the Syntropy language module. It contains definitions for functional-style templates.
///
/// \author Raffaele D. Facendola - November 2020

#pragma once

#include "syntropy/language/templates/type_traits.h"

#include "syntropy/language/foundation/details/functional_details.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* FUNCTIONAL                                                           */
    /************************************************************************/

    /// \brief Invoke a callable object with provided arguments.
    template <typename TCallable, typename... TArguments>
    constexpr Templates::InvokeResult<TCallable, TArguments...> Invoke(TCallable&& callable, TArguments&&... arguments) noexcept;

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    /// \brief Invoke a callable object with provided arguments.
    template <typename TCallable, typename... TArguments>
    constexpr Templates::InvokeResult<TCallable, TArguments...> Invoke(TCallable&& callable, TArguments&&... arguments) noexcept
    {
        return Details::Invoke(Forward<TCallable>(callable), Forward<TArguments>(arguments)...);
    }
}

// ===========================================================================
