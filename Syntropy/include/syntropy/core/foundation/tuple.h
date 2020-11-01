
/// \file tuple.h
/// \brief This header is part of Syntropy core module. It contains definitions for tuples.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <cstdint>
#include <utility>
#include <type_traits>
#include <tuple>

#include "syntropy/language/foundation.h"

namespace Syntropy
{
    /************************************************************************/
    /* TUPLE <T...>                                                         */
    /************************************************************************/

    /// \brief Type alias for a tuple.
    template <typename... TElements>
    using Tuple = std::tuple<TElements...>;

}
