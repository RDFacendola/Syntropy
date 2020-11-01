
/// \file capabilities.h
/// \brief This header is part of Syntropy language module. It contains type capabilities query.
///
/// \author Raffaele D. Facendola - August  2020

#pragma once

#include "syntropy/language/templates/details/capabilities_details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* HAS EQUALITY \ INEQUALITY                                            */
    /************************************************************************/

    /// \brief Check an instance of TType can be compared equal to an instance of UType.
    template <typename TType, typename UType>
    using HasEqualityComparison = Details::HasEqualityComparison<TType, UType>;

    /// \brief Check an instance of TType can be compared equal to an instance of UType.
    template <typename TType, typename UType>
    using HasInequalityComparison = Details::HasInequalityComparison<TType, UType>;

    /************************************************************************/
    /* HAS SWAP                                                             */
    /************************************************************************/

    /// \brief Check whether exists an ADL-aware function Swap for TType.
    template <typename TType>
    using HasSwap = Details::HasSwap<TType>;

}

// ===========================================================================
