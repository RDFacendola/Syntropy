
/// \file ntuple.h
/// \brief This header is part of the Syntropy core module. It contains definitions for N-tuples concepts and related functionalities.
///
/// \author Raffaele D. Facendola - Nov 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/support/compare.h"
#include "syntropy/language/support/swap.h"
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
    concept NTuple = requires
    {
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
    inline constexpr Bool SameRank = Details::SameRank<RemoveConstReference<TNTuples>...>;

    /// \brief Generates a sequence that can be used to enumerate all elements in a given tuple.
    template <Concepts::NTuple TTuple>
    using TupleSequenceFor = MakeSequence<TupleRank<RemoveConstReference<TTuple>>>;
}

// ===========================================================================

namespace Syntropy::Tuples
{
    /************************************************************************/
    /* TUPLES                                                               */
    /************************************************************************/

    /// \brief Check whether lhs and rhs are equal.
    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    constexpr Bool AreEqual(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept;

    /// \brief Check whether lhs and rhs are equivalent.
    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    constexpr Bool AreEquivalent(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept;

    /// \brief Compare two n-tuples lexicographically.
    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    constexpr Ordering Compare(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept;

    /// \brief Member-wise swap two tuples.
    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    requires ( Templates::TupleRank<TTuple> == Templates::TupleRank<UTuple>)
    constexpr void Swap(Mutable<TTuple> lhs, Mutable<UTuple> rhs) noexcept;

    /// \brief Swap lhs with rhs and return the old value of lhs.
    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    requires (Templates::TupleRank<TTuple> == Templates::TupleRank<UTuple>)
    constexpr TTuple Exchange(Mutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept;

    /// \brief Swap lhs with rhs and return the old value of lhs.
    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    requires (Templates::TupleRank<TTuple> == Templates::TupleRank<UTuple>)
    constexpr TTuple Exchange(Mutable<TTuple> lhs, Movable<UTuple> rhs) noexcept;

    /// \brief Invoke a function with arguments provided in form of n-tuple.
    template <typename TFunction, typename TTuple>
    constexpr decltype(auto) Apply(Forwarding<TFunction> function, Forwarding<TTuple> ntuple) noexcept;

    /// \brief Invoke a function for each element in a n-tuple.
    template <typename TFunction, typename TTuple>
    constexpr void ForEachApply(Forwarding<TFunction> function, Forwarding<TTuple> ntuple) noexcept;

    /// \brief Apply a function to all argument list generated by projecting the i-th element of all provided tuples, in the same order, for each index i.
    /// \remarks Tuple elements are accessed by means of Get<Int>(tuple) function, which is expected to be defined for each type in TTuples.
    template <typename TFunction, typename TTuple, typename... TTuples>
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

namespace Syntropy::Tuples
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Tuples.
    // =======

    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    constexpr Bool AreEqual(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept
    {
        // In some implementations comparing two instances for identity is much faster than comparing them for equality.
        // This function is left as a customization point for those implementations.

        return AreEquivalent(lhs, rhs);
    }

    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    constexpr Bool AreEquivalent(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept
    {
        using namespace Templates;

        // Early out if the two tuples have different ranks.

        if constexpr (TupleRank<TTuple> != TupleRank<UTuple>)
        {
            return false;
        }
        else
        {
            return Compare(lhs, rhs) == Ordering::kEquivalent;
        }
    }

    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    constexpr Ordering Compare(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept
    {
        using namespace Templates;

        // Lexicographic compare between two tuple elements.

        auto lexicographic_compare = [&lhs, &rhs]<Int VIndex>(Ordering compare_result, IntConstant<VIndex>)
        {
            return (compare_result == Ordering::kEquivalent) ? (Get<VIndex>(lhs) <=> Get<VIndex>(rhs)) : compare_result;
        };

        // Lexicographic compare between two same-rank tuples.

        auto lockstep_lexicographic_compare = [&lexicographic_compare]<Int... VIndex>(Sequence<VIndex...>) mutable
        {
            auto compare_result = Ordering::kEquivalent;

            ((compare_result = lexicographic_compare(compare_result, IntConstant<VIndex>{})), ...);

            return compare_result;
        };

        // 1) Same-rank comparison.

        if constexpr (TupleRank<TTuple> == TupleRank<UTuple>)
        {
            return lockstep_lexicographic_compare(TupleSequenceFor<TTuple>{});
        }
        
        // 2) Left-to-right comparison.

        if constexpr (TupleRank<TTuple> < TupleRank<UTuple>)
        {
            if (auto compare_result = lockstep_lexicographic_compare(TupleSequenceFor<TTuple>{}); compare_result != Ordering::kEquivalent)
            {
                return compare_result;
            }

            return Ordering::kLess;
        }

        // 3) Right-to-left comparison.

        if constexpr (TupleRank<TTuple> > TupleRank<UTuple>)
        {
            if (auto compare_result = lockstep_lexicographic_compare(TupleSequenceFor<UTuple>{}); compare_result != Ordering::kEquivalent)
            {
                return compare_result;
            }

            return Ordering::kGreater;
        }
    }

    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    requires (Templates::TupleRank<TTuple> == Templates::TupleRank<UTuple>)
    constexpr void Swap(Mutable<TTuple> lhs, Mutable<UTuple> rhs) noexcept
    {
        using namespace Templates;

        auto memberwise_swap = [&lhs, &rhs]<Int... VIndex>(Sequence<VIndex...>)
        {
            (Swap(Get<VIndex>(lhs), Get<VIndex>(rhs)), ...);
        };

        return memberwise_swap(TupleSequenceFor<TTuple>{});
    }

    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    requires (Templates::TupleRank<TTuple> == Templates::TupleRank<UTuple>)
    constexpr TTuple Exchange(Mutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept
    {
        using namespace Templates;

        auto memberwise_exchange = [&lhs, &rhs]<Int... VIndex>(Sequence<VIndex...>)
        {
            using Syntropy::Exchange;

            return TTuple{ Exchange(Get<VIndex>(lhs), Get<VIndex>(rhs))... };
        };

        return memberwise_exchange(TupleSequenceFor<TTuple>{});
    }

    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    requires (Templates::TupleRank<TTuple> == Templates::TupleRank<UTuple>)
    constexpr TTuple Exchange(Mutable<TTuple> lhs, Movable<UTuple> rhs) noexcept
    {
        using namespace Templates;

        auto memberwise_exchange = [&lhs, &rhs]<Int... VIndex>(Sequence<VIndex...>) mutable
        {
            using Syntropy::Exchange;

            return TTuple{ Exchange(Get<VIndex>(lhs), Get<VIndex>(Move(rhs)))... };
        };

        return memberwise_exchange(TupleSequenceFor<TTuple>{});
    }

    template <typename TFunction, typename TTuple>
    constexpr decltype(auto) Apply(Forwarding<TFunction> function, Forwarding<TTuple> ntuple) noexcept
    {
        using namespace Templates;

        auto apply = [&function, &ntuple]<Int... VIndex>(Sequence<VIndex...>)
        {
            return function(Get<VIndex>(Forward<TTuple>(ntuple))...);
        };

        return apply(TupleSequenceFor<RemoveConstReference<TTuple>>{});
    }

    template <typename TFunction, typename TTuple>
    constexpr void ForEachApply(Forwarding<TFunction> function, Forwarding<TTuple> ntuple) noexcept
    {
        using namespace Templates;

        auto for_each_apply = [&function, &ntuple]<Int... VIndex>(Sequence<VIndex...>)
        {
            (function(Get<VIndex>(Forward<TTuple>(ntuple))), ...);
        };

        for_each_apply(TupleSequenceFor<RemoveConstReference<TTuple>>{});
    }

    template <typename TFunction, typename TTuple, typename... TTuples>
    constexpr void LockstepApply(Forwarding<TFunction> function, Forwarding<TTuple> tuple, Forwarding<TTuples>... tuples) noexcept
    {
        using namespace Templates;

        auto lockstep_apply = [&function, &tuple, &tuples...]<Int... VIndex>(Sequence<VIndex...>)
        {
            (function(Get<VIndex>(Forward<TTuple>(tuple)), Get<VIndex>(Forward<TTuples>(tuples))...), ...);
        };

        lockstep_apply(TupleSequenceFor<RemoveConstReference<TTuple>>{});
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