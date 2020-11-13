
/// \file concepts_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of fundamental concepts definitions.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/templates/traits.h"

// ===========================================================================

namespace Syntropy::Concepts::Details
{
    /************************************************************************/
    /* CONCEPTS                                                             */
    /************************************************************************/

    /// \brief Concept satisfied only if TType and UType are exactly the same (and vice-versa).
    template <typename TType, typename UType>
    concept SameAs = Templates::IsSame<TType, UType> && Templates::IsSame<UType, TType>;

    /// \brief Concept satisfied only if TType is convertible to UType.
    template <typename TType, typename UType>
    concept ConvertibleTo = Templates::IsConvertible<TType, UType> && requires(Templates::AddRValueReference<TType>(&function)())
    {
        static_cast<UType>(function());
    };

}

// ===========================================================================
