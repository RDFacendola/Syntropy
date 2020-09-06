
/// \file capabilities.h
/// \brief This header is part of Syntropy language module. It contains type capabilities query.
///
/// \author Raffaele D. Facendola - August  2020

#pragma once

#include "syntropy/language/support/declarations.h"
#include "syntropy/language/traits/details/capabilities.h"

namespace Syntropy::Traits
{
    /************************************************************************/
    /* COMPARISON                                                           */
    /************************************************************************/

    /// \brief Check an instance of TType can be compared equal to an instance of T1Type.
    template <typename T0Type, typename T1Type>
    using HasEqualityComparison = decltype(Declval<T0Type>() == Declval<T1Type>());

    /// \brief Check an instance of TType can be compared equal to an instance of T1Type.
    template <typename T0Type, typename T1Type>
    using HasInequalityComparison = decltype(Declval<T0Type>() != Declval<T1Type>());

    /************************************************************************/
    /* SWAP                                                                 */
    /************************************************************************/

    /// \brief Check whether exists an ADL-aware function Swap for TType.
    template <typename TType>
    using HasSwap = Details::HasSwapDetector::Detect<TType>;

}