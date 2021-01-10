
/// \file ranges.h
/// \brief This header is part of the Syntropy core module. It contains definitions for base ranges.
///
/// \author Raffaele D. Facendola - Nov 2020
/// \author Raffaele D. Facendola - Jan 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"

// ===========================================================================

namespace Syntropy::Ranges::Templates
{
    /************************************************************************/
    /* RANGE                                                                */
    /************************************************************************/

    /// \brief Exposes a member type Type equal to a reference to an element in a range TRange.
    template <typename TRange>
    struct ElementReferenceTypeTraits;

    /// \brief Exposes a member type Type equal to a pointer to an element in a range TRange.
    template <typename TRange>
    struct ElementPointerTypeTraits
    {
        using URange = Syntropy::Templates::RemoveConstReference<TRange>;

        using Type = Syntropy::Templates::AddPointer<Syntropy::Templates::RemoveReference<typename ElementReferenceTypeTraits<URange>>>;
    };

    /// \brief Exposes a member type Type equal to the type of the number of elements in a range TRange.
    template <typename TRange>
    struct ElementCountTypeTraits
    {
        using Type = Int;
    };

    /// \brief Type of a reference to an element in a range TRange.
    template <typename TRange, typename URange = Syntropy::Templates::RemoveConstReference<TRange>>
    using ElementReference = typename ElementReferenceTypeTraits<URange>::Type;

    /// \brief Type of a pointer to an element in a range TRange.
    template <typename TRange, typename URange = Syntropy::Templates::RemoveConstReference<TRange>>
    using ElementPointer = typename ElementPointerTypeTraits<URange>::Type;

    /// \brief Type of a number of elements in a range TRange.
    template <typename TRange, typename URange = Syntropy::Templates::RemoveConstReference<TRange>>
    using ElementCount = typename ElementCountTypeTraits<URange>::Type;
}

// ===========================================================================

namespace Syntropy::Ranges::Concepts
{
    /************************************************************************/
    /* RANGE                                                                */
    /************************************************************************/

    template <typename TCardinality>
    concept RangeCardinality = requires(Immutable<TCardinality> lhs, Immutable<TCardinality> rhs)
    {
        /// \brief Range cardinality type shall be closed under addition.
        { lhs + rhs } -> Syntropy::Concepts::SameAs<TCardinality>;

        /// \brief Range cardinality type shall be closed under subtraction.
        { lhs - rhs } -> Syntropy::Concepts::SameAs<TCardinality>;
    };
}

// ===========================================================================
