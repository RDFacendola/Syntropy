
/// \file declarations.h
/// \brief This header is part of Syntropy language module. It contains language utility definitions.
///
/// This header contains mostly aliases to the standard traits, eventually modified to suite Syntropy needs and to enforce code style consistency.
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