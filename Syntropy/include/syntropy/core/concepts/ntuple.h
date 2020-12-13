
/// \file ntuple.h
/// \brief This header is part of the Syntropy core module. It contains definitions for N-tuples concepts and related functionalities.
///
/// \author Raffaele D. Facendola - Nov 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/support/compare.h"
#include "syntropy/language/templates/concepts.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/templates/sequence.h"

#include "syntropy/core/concepts/details/ntuple_details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* N-TUPLE                                                              */
    /************************************************************************/
    
    /// \brief Exposes a member integer kValue equal to the rank of the n-tuple.
    template <typename TType>
    struct TupleRankTypeTraits;

    /// \brief Provides indexed access to n-tuple elements' types.
    template <Int VIndex, typename TType>
    struct TupleElementTypeTraits;

    /// \brief Rank of a n-tuple.
    template <typename TType>
    inline constexpr Int TupleRank = TupleRankTypeTraits<TType>::kValue;

    /// \brief Type of the VIndex-th element of a n-tuple.
    template <Int VIndex, typename TType>
    using TupleElementType = typename TupleElementTypeTraits<VIndex, TType>::Type;

    /// \brief Constant equal to true if TType provides compile-time access to its element types, false otherwise.
    template <typename TType>
    inline constexpr Bool HasTupleElementTypes = Details::HasTupleElementTypes<TType>;

    /// \brief Constant equal to true if TType provides access to all its TupleRank elements, false otherwise.
    template <typename TType>
    inline constexpr Bool HasTupleGetters = Details::HasTupleGetters<TType>;
}

// ===========================================================================

namespace Syntropy::Concepts
{
    /************************************************************************/
    /* N-TUPLE                                                              */
    /************************************************************************/

    /// \brief Concept for types that behave as tuples, providing indexed compile-time access to its elements.
    template <typename TType>
    concept NTuple = requires(Immutable<TType> ntuple)
    {
        /// \brief Type trait used to determine tuple rank.
        typename Templates::TupleRankTypeTraits<TType>;

        /// \brief Rank of the tuple.
        { Templates::TupleRankTypeTraits<TType>::kValue } -> Integral;
    }
    && Templates::HasTupleElementTypes<TType>
    && Templates::HasTupleGetters<TType>;
}

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* N-TUPLE                                                              */
    /************************************************************************/

    /// \brief Constant equal to true if all TNTuples have the same rank, false otherwise.
    template <Concepts::NTuple... TNTuples>
    inline constexpr Bool SameRank = Details::SameRank<TNTuples...>;

    /// \brief Generates a sequence that can be used to enumerate all elements in a given tuple.
    template <Concepts::NTuple TTuple>
    using TupleSequenceFor = MakeSequence<TupleRank<RemoveConstReference<TTuple>>>;
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Check whether lhs and rhs are equal.
    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    requires Templates::SameRank<TTuple, UTuple>
    constexpr Bool AreEqual(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept;

    /// \brief Check whether lhs and rhs are equivalent.
    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    requires Templates::SameRank<TTuple, UTuple>
    constexpr Bool AreEquivalent(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept;

    /// \brief Compare two n-tuples lexicographically.
    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    requires Templates::SameRank<TTuple, UTuple>
    constexpr Ordering Compare(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept;

    /// \brief Member-wise swap two n-tuples.
    template <Concepts::NTuple TTuple>
    constexpr void Swap(Mutable<TTuple> lhs, Mutable<TTuple> rhs) noexcept;

    /// \brief Invoke a function with arguments provided in form of n-tuple.
    template <typename TFunction, Concepts::NTuple TTuple>
    constexpr decltype(auto) Apply(Forwarding<TFunction> function, Forwarding<TTuple> ntuple) noexcept;

    /// \brief Invoke a function for each element in a n-tuple.
    template <typename TFunction, Concepts::NTuple TTuple>
    constexpr void ForEachApply(Forwarding<TFunction> function, Forwarding<TTuple> ntuple) noexcept;

    /// \brief Apply a function to all argument list generated by projecting the i-th element of all provided tuples, in the same order, for each index i.
    /// \remarks Tuple elements are accessed by means of Get<Int>(tuple) function, which is expected to be defined for each type in TTuples.
    template <typename TFunction, Concepts::NTuple TTuple, Concepts::NTuple... TTuples>
    requires Templates::SameRank<TTuple, TTuples...>
    constexpr void LockstepApply(Forwarding<TFunction> function, Forwarding<TTuple> tuple, Forwarding<TTuples>... tuples) noexcept;
}

// ===========================================================================

namespace std
{
    /************************************************************************/
    /* STRUCTURED BINDINGS                                                  */
    /************************************************************************/

    /// \brief Size of a NTuple.
    template <Syntropy::Concepts::NTuple TTuple>
    struct std::tuple_size<TTuple>;

    /// \brief Type of the VIndex-th element of a Syntropy::Tuple.
    template <std::size_t VIndex, Syntropy::Concepts::NTuple TTuple>
    struct std::tuple_element<VIndex, TTuple>;

    /// \brief Get the VIndex-th element of a tuple.
    template <std::size_t VIndex, Syntropy::Concepts::NTuple TTuple>
    decltype(auto) get(Syntropy::Immutable<TTuple> tuple);

    /// \brief Get the VIndex-th element of a tuple.
    template <std::size_t VIndex, Syntropy::Concepts::NTuple TTuple>
    decltype(auto) get(Syntropy::Movable<TTuple> tuple);
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    requires Templates::SameRank<TTuple, UTuple>
    constexpr Bool AreEqual(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept
    {
        // In some implementations comparing two instances for identity is much faster than comparing them for equality.
        // This function is left as a customization point for those implementations.

        return AreEquivalent(lhs, rhs);
    }

    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    requires Templates::SameRank<TTuple, UTuple>
    constexpr Bool AreEquivalent(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept
    {
        auto result = true;

        auto are_equivalent = [&result](const auto& lhs_element, const auto& rhs_element)
        {
            result &= (lhs_element == rhs_element);
        };

        LockstepApply(are_equivalent, lhs, rhs);

        return result;
    }

    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    requires Templates::SameRank<TTuple, UTuple>
    constexpr Ordering Compare(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept
    {
        auto result = Ordering::Equivalent;

        auto compare = [&result](const auto& lhs_element, const auto& rhs_element)
        {
            if (result == Ordering::Equivalent)
            {
                result = (lhs_element) <=> (rhs_element);
            }
        };

        LockstepApply(compare, lhs, rhs);

        return result;
    }

    template <Concepts::NTuple TTuple>
    constexpr void Swap(Mutable<TTuple> lhs, Mutable<TTuple> rhs) noexcept
    {
        auto memberwise_swap = [](auto& lhs_element, auto& rhs_element)
        {
            Swap(lhs_element, rhs_element);
        };

        LockstepApply(memberwise_swap, lhs, rhs);
    }

    template <typename TFunction, Concepts::NTuple TTuple>
    constexpr decltype(auto) Apply(Forwarding<TFunction> function, Forwarding<TTuple> ntuple) noexcept
    {
        using namespace Templates;

        auto apply = [&function, &ntuple]<Int... VIndex>(Sequence<VIndex...>) mutable
        {
            return function(Get<VIndex>(Forward<TTuple>(ntuple))...);
        };

        return apply(TupleSequenceFor<TTuple>{});
    }

    template <typename TFunction, Concepts::NTuple TTuple>
    constexpr void ForEachApply(Forwarding<TFunction> function, Forwarding<TTuple> ntuple) noexcept
    {
        using namespace Templates;

        auto for_each_apply = [&function, &ntuple]<Int... VIndex>(Sequence<VIndex...>) mutable
        {
            (function(Get<VIndex>(Forward<TTuple>(ntuple))), ...);
        };

        for_each_apply(TupleSequenceFor<TTuple>{});
    }

    template <typename TFunction, Concepts::NTuple TTuple, Concepts::NTuple... TTuples>
    requires Templates::SameRank<TTuple, TTuples...>
    constexpr void LockstepApply(Forwarding<TFunction> function, Forwarding<TTuple> tuple, Forwarding<TTuples>... tuples) noexcept
    {
        using namespace Templates;

        auto lockstep_apply = [&function, &tuple, &tuples...]<Int... VIndex>(Sequence<VIndex...>) mutable
        {
            (function(Get<VIndex>(Forward<TTuple>(tuple)), Get<VIndex>(Forward<TTuples>(tuples))...), ...);
        };

        lockstep_apply(TupleSequenceFor<TTuple>{});
    }

}

// ===========================================================================

namespace std
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Structured bindings.
    // ====================

    template <Syntropy::Concepts::NTuple TTuple>
    struct std::tuple_size<TTuple>
    {
        static constexpr std::size_t value = Syntropy::Templates::TupleRank<TTuple>;
    };

    template <std::size_t VIndex, Syntropy::Concepts::NTuple TTuple>
    struct std::tuple_element<VIndex, TTuple>
    {
        using type = Syntropy::Templates::TupleElementType<VIndex, TTuple>;
    };

    template <std::size_t VIndex, Syntropy::Concepts::NTuple TTuple>
    decltype(auto) get(Syntropy::Immutable<TTuple> tuple)
    {
        return Get<VIndex>(tuple);
    }

    template <std::size_t VIndex, Syntropy::Concepts::NTuple TTuple>
    decltype(auto) get(Syntropy::Movable<TTuple> tuple)
    {
        return Get<VIndex>(Syntropy::Move(tuple));
    }
}

// ===========================================================================