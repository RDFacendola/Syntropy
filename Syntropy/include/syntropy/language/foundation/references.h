
/// \file references.h
/// \brief This header is part of Syntropy language module. It contains definitions for reference types.
/// 
/// \author Raffaele D. Facendola - Dic 2020

#pragma once

#include <cstdint>
#include <cstddef>

#include "syntropy/language/templates/type_traits.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* REFERENCE TYPES                                                      */
    /************************************************************************/

    /// \brief Lvalue reference to an immutable instance of type TType.
    template <typename TType>
    using Immutable = const TType&;

    /// \brief LValue reference to a mutable instance of type TType.
    template <typename TType>
    using Mutable = TType&;

    /// \brief RValue reference to an immutable instance of type TType.
    template <typename TType>
    using Immovable = const TType&&;

    /// \brief RValue reference to an mutable instance of type TType.
    template <typename TType>
    using Movable = TType&&;
}
