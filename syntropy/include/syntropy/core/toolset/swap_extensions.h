
/// \file swap_extensions.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for swap-related customization point
///        objects.
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

// ===========================================================================

namespace Syntropy::Algorithms::Extensions
{
    /************************************************************************/
    /* SWAP EXTENSIONS                                                      */
    /************************************************************************/

    /// \brief Swap two elements and return the old value of the first.
    template <typename TType, typename UType, typename = void>
    struct Exchange;

    /// \brief Swap two elements.
    template <typename TType, typename = void>
    struct Swap;

}

// ===========================================================================
