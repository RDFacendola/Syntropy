
/// \file numbers.h
///
/// \brief This header is part of the Syntropy math module.
///        It contains base number definitions.
///
/// \author Raffaele D. Facendola - April 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

// ===========================================================================

namespace Syntropy::Math
{
    /************************************************************************/
    /* MATH                                                                 */
    /************************************************************************/

    /// \brief Get the positive infinity value.
    [[nodiscard]] constexpr Float
    Infinity() noexcept;

    /// \brief Get the not-a-number value.
    [[nodiscard]] constexpr Float
    NotANumber() noexcept;

}

// ===========================================================================

#include "details/numbers.inl"

// ===========================================================================
