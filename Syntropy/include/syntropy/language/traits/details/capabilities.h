
/// \file capabilities.h
/// \brief This header is part of Syntropy language module. It contains detail implementation for capabilities-related type traits.
///
/// \author Raffaele D. Facendola - August  2020

#pragma once

#include "syntropy/language/support/declarations.h"

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* HAS SWAP DETECTOR                                                    */
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

}