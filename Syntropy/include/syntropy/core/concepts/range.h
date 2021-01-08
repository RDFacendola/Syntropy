
/// \file ranges.h
/// \brief This header is part of the Syntropy core module. It contains base definitions for ranges.
///
/// \author Raffaele D. Facendola - Nov 2020
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* RANGE TRAITS                                                         */
    /************************************************************************/

    /// \brief Exposes a member type Type equal to a reference to an element in a range TRange.
    template <typename TRange>
    struct RangeElementReferenceTypeTraits;

    /// \brief Exposes a member type Type equal to a pointer to an element in a range TRange.
    template <typename TRange>
    struct RangeElementPointerTypeTraits
    {
        using Type = Templates::AddPointer<Templates::RemoveReference<typename RangeElementReferenceTypeTraits<Templates::RemoveConstReference<TRange>>>>;
    };

    /// \brief Exposes a member type Type equal to the type of the number of elements in a range TRange.
    template <typename TRange>
    struct RangeElementCountTypeTraits
    {
        using Type = Int;
    };

    /// \brief Type of a reference to an element in a range TRange.
    template <typename TRange, typename URange = Templates::RemoveConstReference<TRange>>
    using RangeElementReference = typename RangeElementReferenceTypeTraits<URange>::Type;

    /// \brief Type of a pointer to an element in a range TRange.
    template <typename TRange, typename URange = Templates::RemoveConstReference<TRange>>
    using RangeElementPointer = typename RangeElementPointerTypeTraits<URange>::Type;

    /// \brief Type of a number of elements in a range TRange.
    template <typename TRange, typename URange = Templates::RemoveConstReference<TRange>>
    using RangeElementCount = typename RangeElementCountTypeTraits<URange>::Type;
}

// ===========================================================================

namespace Syntropy::Concepts
{
    /************************************************************************/
    /* RANGE                                                                */
    /************************************************************************/

    /// \brief Base concept for ranges.
    /// \author Raffaele D. Facendola - November 2020.
    template <typename TRange>
    concept Range = requires()
    {
         /// \brief Trait used to determine the reference type of an element inside the range.
         typename Templates::RangeElementReferenceTypeTraits<TRange>::Type;
 
         /// \brief Trait used to determine the pointer type of an element inside the range.
         typename Templates::RangeElementPointerTypeTraits<TRange>::Type;
 
         /// \brief Trait used to determine the type of the cardinality of the range.
         typename Templates::RangeElementCountTypeTraits<TRange>::Type;
    };

}

// ===========================================================================