
/// \file foundation.h
/// \brief This header is part of Syntropy language module. It contains language foundation definitions.
///
/// \author Raffaele D. Facendola - August  2020

#pragma once

#include "syntropy/language/type_traits/manipulation.h"

namespace Syntropy::Traits
{
    /************************************************************************/
    /* DECLVAL                                                              */
    /************************************************************************/

    /// \brief Convert TType as a reference type, without calling any constructor.
    /// \remarks this function shall never be evaluated as it has no definition.
    template <typename TType>
    Traits::AddRValueReference<TType> Declval() noexcept;
}