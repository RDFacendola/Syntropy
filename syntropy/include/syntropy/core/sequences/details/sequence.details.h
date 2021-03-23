
/// \file ntuple.details.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/templates/templates.h"

// ===========================================================================

namespace Syntropy::Sequences::Templates
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    template <typename TType>
    struct RankTypeTraits;

    template <Int VIndex, typename TType>
    struct ElementTypeTraits;
}

// ===========================================================================

namespace Syntropy::Sequences::Templates::Details
{
    /************************************************************************/
    /* N-TUPLE                                                              */
    /************************************************************************/

    /// \brief Rank of a n-tuple.
    template <
        typename TType,
        typename UType = Syntropy::Templates::UnqualifiedOf<TType>>
    inline constexpr Int
    Rank = RankTypeTraits<UType>::kValue;

    /// \brief Type of the VIndex-th element of a n-tuple;
    template <
        Int VIndex,
        typename TType,
        typename UType = Syntropy::Templates::UnqualifiedOf<TType>>
    using ElementType = typename ElementTypeTraits<VIndex, UType>::Type;

    // HasElementTypes.
    // =====================

    /// \brief Detect whether TType provides index compile-time access to
    ///        tuple elements' types.
    template <typename TType, typename TIndex>
    using DetectElementType = decltype(
        Syntropy::Templates::Declval<ElementType<TIndex::kValue, TType>>());

    /// \brief Constant equal to true if TType provides compile-time access
    ///        to the VIndex-th element's type, equal to false otherwise.
    template <typename TType, Int VIndex>
    inline constexpr Bool HasTupleElementType
        = Syntropy::Templates::IsValidExpression<
            DetectElementType,
            TType,
            Syntropy::Templates::IntConstant<VIndex>>;

    template <typename TType, Int VRank>
    struct HasElementTypesHelper
    {
        static constexpr Bool kValue
             = HasTupleElementType<TType, VRank - 1>
            && HasElementTypesHelper<TType, VRank - 1>::kValue;
    };

    template <typename TType>
    struct HasElementTypesHelper<TType, 0> : Syntropy::Templates::True {};

    /// \brief Constant equal to true if TType provides compile-time access to
    ///        its element types, false otherwise.
    template <typename TType>
    inline constexpr Bool HasElementTypes
        = HasElementTypesHelper<TType, Rank<TType>>::kValue;

    // HasGetters.
    // ===============

    /// \brief Detect whether TType provides compile-time access to the
    ///        VIndex-th element.
    template <typename TType, typename TIndex>
    using DetectTupleGetter
        = decltype(Get<Syntropy::Templates::Evaluate<TIndex>>(
              Syntropy::Templates::Declval<TType>()));

    /// \brief Constant equal to true if TType provides compile-time access to
    ///        the VIndex-th element, equal to false otherwise.
    template <typename TType, Int VIndex>
    inline constexpr Bool HasTupleGetter
        = Syntropy::Templates::IsValidExpression<
            DetectTupleGetter,
            TType,
            Syntropy::Templates::IntConstant<VIndex>>;

    template <typename TType, Int VRank>
    struct HasGettersHelper
    {
        static constexpr Bool kValue
             = HasTupleGetter<TType, VRank - 1>
            && HasGettersHelper<TType, VRank - 1>::kValue;
    };

    template <typename TType>
    struct HasGettersHelper<TType, 0> : Syntropy::Templates::True {};

    /// \brief Constant equal to true if TType provides access to all its
    ///        TupleRank elements, false otherwise.
    template <typename TType>
    inline constexpr Bool HasGetters
        = HasGettersHelper<TType, Rank<TType>>::kValue;

    // SameRank.
    // =========

    /// \brief Helper structure for SameRank trait.
    template <typename TType, typename... TTypes>
    struct SameRankHelper
    {
        static constexpr Bool kValue = ((Rank<TType> == Rank<TTypes>) && ...);
    };

    /// \brief Specialization for a single tuple.
    template <typename TType>
    struct SameRankHelper<TType> : Syntropy::Templates::True {};

    /// \brief Constant equal to true if all TTypes have the same rank,
    ///        false otherwise.
    template <typename... TTypes>
    inline constexpr Bool SameRank = SameRankHelper<TTypes...>::kValue;

}

// ===========================================================================
