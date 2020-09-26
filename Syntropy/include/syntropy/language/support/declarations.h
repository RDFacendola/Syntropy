
/// \file declarations.h
/// \brief This header is part of Syntropy language module. It contains definitions for declaration-related functionalities.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/templates/manipulation.h"

namespace Syntropy
{
    /************************************************************************/
    /* DECLVAL                                                              */
    /************************************************************************/

    /// \brief Convert TType as a reference type, without calling any constructor.
    /// \remarks this function shall never be evaluated as it has no definition.
    template <typename TType>
    Templates::AddRValueReference<TType> Declval() noexcept;

}