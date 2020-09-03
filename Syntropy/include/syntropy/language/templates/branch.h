
/// \file branch.h
/// \brief This header is part of Syntropy language module. It contains control-flow-related template machinery.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"

namespace Syntropy::Templates
{
    /************************************************************************/
    /* CONDITIONAL                                                          */
    /************************************************************************/

    /// \brief Type equal to TTrue if kCondition is true, equal to TFalse otherwise.
    template <Bool kCondition, typename TTrue, typename TFalse>
    using Conditional = std::conditional_t<kCondition, TTrue, TFalse>;

}