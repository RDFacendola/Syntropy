
/// \file capabilities.h
/// \brief This header is part of Syntropy language module. It contains type capabilities query.
///
/// Definitions here are candidate for conversion to upcoming C++20 concepts.
/// 
/// \author Raffaele D. Facendola - August  2020

#pragma once

#include "syntropy/language/support/declarations.h"
#include "syntropy/language/type_traits/details/capabilities.h"

namespace Syntropy::Traits
{
    /************************************************************************/
    /* COMPARISON                                                           */
    /************************************************************************/

    /// \brief Check an instance of TType can be compared equal to an instance of UType.
    template <typename TType, typename UType>
    using HasEqualityComparison = decltype(Declval<TType>() == Declval<UType>());

    /// \brief Check an instance of TType can be compared equal to an instance of UType.
    template <typename TType, typename UType>
    using HasInequalityComparison = decltype(Declval<TType>() != Declval<UType>());

    /************************************************************************/
    /* SWAP                                                                 */
    /************************************************************************/

    /// \brief Check whether exists an ADL-aware function Swap for TType.
    template <typename TType>
    using HasSwap = Details::HasSwapDetector::Detect<TType>;

}