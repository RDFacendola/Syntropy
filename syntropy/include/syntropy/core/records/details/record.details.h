
/// \file record.details.h
///
/// \author Raffaele D. Facendola - 2020.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/templates/templates.h"

// ===========================================================================

namespace Syntropy::Records::Details
{
    /************************************************************************/
    /* TYPE TRAITS                                                          */
    /************************************************************************/

    /// \brief Number of elements in a record.
    template <typename TRecord,
              template <typename> typename TRankTrait>
    inline constexpr
    Int RankOf
        = TRankTrait<Templates::UnqualifiedOf<TRecord>>::kValue;

    /// \brief True if two records have the same rank, false otherwise.
    template <typename TRecord,
              typename URecord,
              template <typename> typename TRankTrait>
    inline constexpr
    Bool IsSameRank
        = (RankOf<TRecord, TRankTrait> == RankOf<URecord, TRankTrait>);

    /// \brief Type of a record element.
    template <Int TIndex,
              typename TRecord,
              template <Int, typename> typename TElementTypeTrait>
    using ElementTypeOf
        = typename TElementTypeTrait<TIndex,
                                     Templates::UnqualifiedOf<TRecord>>::Type;

    /// \brief Generates a sequence that can be used to enumerate all
    ///        elements in a record.
    template <typename TRecord,
              template <typename> typename TRankTrait>
    using SequenceOf
        = Templates::MakeSequence<RankOf<TRecord, TRankTrait>>;

    /// \brief Helper function for ElementTypeListOf.
    template <template <Int, typename> typename TElementTypeTrait,
              typename TRecord,
              Int... TIndex>
    auto
    ElementTypeListOfHelper(Templates::Sequence<TIndex...>) noexcept
        -> Templates::TypeList<ElementTypeOf<TIndex,
                                             TRecord,
                                             TElementTypeTrait>...>;

    /// \brief List of types of a record's elements.
    template <typename TRecord,
              template <typename> typename TRankTrait,
              template <Int, typename> typename TElementTypeTrait>
    using
    ElementTypeListOf = decltype(
        ElementTypeListOfHelper<TElementTypeTrait,TRecord>(
            SequenceOf<TRecord, TRankTrait>{}));

    /// \brief Index of the first element with type TElement in a record.
    template <typename TElement,
              typename TRecord,
              template <typename> typename TRankTrait,
              template <Int, typename> typename TElementTypeTrait>
    inline constexpr Int
    ElementIndexOf
        = Templates::ElementIndexOf<TElement,
                                    ElementTypeListOf<TRecord,
                                                      TRankTrait,
                                                      TElementTypeTrait>>;

}

// ===========================================================================
