
/// \file capabilities_details.h
/// \brief This header is part of Syntropy language module. It contains detail implementation for capabilities-related type traits.
///
/// \author Raffaele D. Facendola - August  2020

#pragma once

#include "syntropy/language/support/declarations.h"

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* HAS EQUALITY \ INEQUALITY                                            */
    /************************************************************************/

    /// \brief Check an instance of TType can be compared equal to an instance of UType.
    template <typename TType, typename UType>
    using HasEqualityComparison = decltype(Declval<TType>() == Declval<UType>());

    /// \brief Check an instance of TType can be compared equal to an instance of UType.
    template <typename TType, typename UType>
    using HasInequalityComparison = decltype(Declval<TType>() != Declval<UType>());

    /************************************************************************/
    /* HAS SWAP                                                             */
    /************************************************************************/

    /// \brief Self-contained namespace used to determine whether a type has an ADL-aware Swap implementation.
    namespace HasSwapDetector
    {
        // Undefined, triggers ADL.
        void Swap();

        /// \brief Check whether exists an ADL-aware function Swap for TType.
        template <typename TType>
        using Detect = decltype(Swap(Declval<TType&>(), Declval<TType&>));
    }

    /// \brief Check whether exists an ADL-aware function Swap for TType.
    template <typename TType>
    using HasSwap = HasSwapDetector::Detect<TType>;

}

// ===========================================================================
