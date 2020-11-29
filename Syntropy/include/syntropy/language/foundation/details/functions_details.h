
/// \file functions_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details for fundamental functions.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <functional>

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/type_traits.h"

// ===========================================================================

namespace Syntropy::Details
{
    /************************************************************************/
    /* FUNCTIONAL                                                           */
    /************************************************************************/

    template <typename TCallable, typename... TArguments>
    constexpr Templates::InvokeResult<TCallable, TArguments...> Invoke(TCallable&& callable, TArguments&&... arguments) noexcept
    {
        return std::invoke(Forward<TCallable>(callable), Forward<TArguments>(arguments)...);
    }

}

// ===========================================================================
