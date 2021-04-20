
/// \file compare_extensions.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for compare-related customization point
///        objects.
///
/// \author Raffaele D. Facendola - March 2021

#pragma once

// ===========================================================================

namespace Syntropy::Comparisons::Extensions
{
    /************************************************************************/
    /* COMPARE EXTENSIONS                                                   */
    /************************************************************************/

    /// \brief Check whether two objects are equal.
    template <typename TType, typename UType, typename = void>
    struct AreEqual;

    /// \brief Check whether two objects are equivalent.
    template <typename TType, typename UType, typename = void>
    struct AreEquivalent;

    /// \brief Compare two objects.
    template <typename TType, typename UType, typename = void>
    struct Compare;

}

// ===========================================================================
