
/// \file concepts.h
/// \brief This header is part of the Syntropy language module. It contains definitions for fundamental concepts.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/experimental/language/concepts/details/concepts_details.h"

// ===========================================================================

namespace Syntropy::Concepts
{
    /************************************************************************/
    /* CONCEPTS                                                             */
    /************************************************************************/

    /// \brief Concept satisfied only if TType and UType are exactly the same (and vice-versa).
    template <typename TType, typename UType>
    concept SameAs = Details::SameAs<TType, UType>;

    /// \brief Concept satisfied only if TType is convertible to UType.
    template <typename TType, typename UType>
    concept ConvertibleTo = Details::ConvertibleTo<TType, UType>;
}

// ===========================================================================