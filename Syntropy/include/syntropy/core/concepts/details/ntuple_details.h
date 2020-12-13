
/// \file ntuple_details.h
/// \brief This header is part of Syntropy core module. It contains implementation details for n-tuples.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    template <typename TType>
    struct TupleRankTypeTraits;

    template <Int VIndex, typename TType>
    struct TupleElementTypeTraits;
}

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* N-TUPLE                                                              */
    /************************************************************************/

    /// \brief Rank of a n-tuple.
    template <typename TType>
    inline constexpr Int TupleRank = TupleRankTypeTraits<TType>::kValue;

    /// \brief Type of the VIndex-th element of a n-tuple;
    template <Int VIndex, typename TType>
    using TupleElementType = typename TupleElementTypeTraits<VIndex, TType>::Type;

    // HasTupleElementTypes.
    // =====================

    /// \brief Detect whether TType provides index compile-time access to tuple elements' types.
    template <typename TType, typename TIndex>
    using DetectTupleElementType = decltype(Declval<TupleElementType<TIndex::kValue, TType>>());

    /// \brief Constant equal to true if TType provides compile-time access to the VIndex-th element's type, equal to false otherwise.
    template <typename TType, Int VIndex>
    inline constexpr Bool HasTupleElementType = IsValidExpression<DetectTupleElementType, TType, IntConstant<VIndex>>;

    template <typename TType, Int VRank>
    struct HasTupleElementTypesHelper
    {
        static constexpr Bool kValue = HasTupleElementType<TType, VRank - 1> && HasTupleElementTypesHelper<TType, VRank - 1>::kValue;
    };

    template <typename TType>
    struct HasTupleElementTypesHelper<TType, 0> : True {};

    /// \brief Constant equal to true if TType provides compile-time access to its element types, false otherwise.
    template <typename TType>
    inline constexpr Bool HasTupleElementTypes = HasTupleElementTypesHelper<TType, TupleRank<TType>>::kValue;

    // HasTupleGetters.
    // ===============

    /// \brief Detect whether TType provides compile-time access to the VIndex-th element.
    template <typename TType, typename TIndex>
    using DetectTupleGetter = decltype(Get<TIndex::kValue>(Declval<TType>()));

    /// \brief Constant equal to true if TType provides compile-time access to the VIndex-th element, equal to false otherwise.
    template <typename TType, Int VIndex>
    inline constexpr Bool HasTupleGetter = IsValidExpression<DetectTupleGetter, TType, IntConstant<VIndex>>;

    template <typename TType, Int VRank>
    struct HasTupleGettersHelper
    {
        static constexpr Bool kValue = HasTupleGetter<TType, VRank - 1> && HasTupleGettersHelper<TType, VRank - 1>::kValue;
    };

    template <typename TType>
    struct HasTupleGettersHelper<TType, 0> : True {};

    /// \brief Constant equal to true if TType provides access to all its TupleRank elements, false otherwise.
    template <typename TType>
    inline constexpr Bool HasTupleGetters = HasTupleGettersHelper<TType, TupleRank<TType>>::kValue;

    // SameRank.
    // =========

    /// \brief Helper structure for SameRank trait.
    template <typename TType, typename... TTypes>
    struct SameRankHelper
    {
        static constexpr Bool kValue = ((TupleRank<TType> == TupleRank<TTypes>) && ...);
    };

    /// \brief Specialization for a single tuple.
    template <typename TType>
    struct SameRankHelper<TType> : True {};

    /// \brief Constant equal to true if all TTypes have the same rank, false otherwise.
    template <typename... TTypes>
    inline constexpr Bool SameRank = SameRankHelper<TTypes...>::kValue;

}

// ===========================================================================