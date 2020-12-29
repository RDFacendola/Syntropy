
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

#include "syntropy/math/math.h"

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
    template <typename TType, typename UType = Templates::RemoveConstReference<TType>>
    inline constexpr Int TupleRank = TupleRankTypeTraits<UType>::kValue;

    /// \brief Type of the VIndex-th element of a n-tuple.
    template <Int VIndex, typename TType, typename UType = Templates::RemoveConstReference<TType>>
    using TupleElementType = typename TupleElementTypeTraits<VIndex, UType>::Type;

    /// \brief Constant equal to true if TType provides compile-time access to its element types, false otherwise.
    template <typename TType, typename UType = Templates::RemoveConstReference<TType>>
    inline constexpr Bool HasTupleElementTypes = Details::HasTupleElementTypes<UType>;

    /// \brief Constant equal to true if TType provides access to all its TupleRank elements, false otherwise.
    template <typename TType, typename UType = Templates::RemoveConstReference<TType>>
    inline constexpr Bool HasTupleGetters = Details::HasTupleGetters<UType>;
}

// ===========================================================================

namespace Syntropy::Concepts
{
    /************************************************************************/
    /* N-TUPLE                                                              */
    /************************************************************************/

    /// \brief Concept for types that behave as tuples, providing indexed compile-time access to its elements.
    template <typename TType, typename UType = Templates::RemoveConstReference<TType>>
    concept NTuple = requires
    {
        /// \brief Rank of the tuple.
        { Templates::TupleRankTypeTraits<UType>::kValue } -> Integral;

    }
    && Templates::HasTupleElementTypes<TType>
    && Templates::HasTupleGetters<TType>;

    /// \brief Concept for reference types that behave as tuple.
    /// \remarks This concept is especially useful when working with forwarding references to tuple types,
    ///          where adding a reference would cause no type traits to be found.
    template <typename TType>
    concept NTupleReference = NTuple<Templates::Decay<TType>>;
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
    template <Concepts::NTupleReference TTuple>
    using TupleSequenceFor = MakeSequence<TupleRank<TTuple>>;
}

// ===========================================================================

namespace Syntropy::Tuples
{
    /************************************************************************/
    /* TUPLES                                                               */
    /************************************************************************/

    // Comparison.
    // ===========

    /// \brief Check whether lhs and rhs are equal.
    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    [[nodiscard]] constexpr Bool AreEqual(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept;

    /// \brief Check whether lhs and rhs are equivalent.
    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    [[nodiscard]] constexpr Bool AreEquivalent(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept;

    /// \brief Compare two n-tuples lexicographically.
    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    [[nodiscard]] constexpr Ordering Compare(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept;

    // Swap.
    // =====

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

    // Functional.
    // ===========

    /// \brief Invoke a function with arguments provided in form of n-tuple.
    template <typename TFunction, Concepts::NTupleReference TTuple>
    constexpr decltype(auto) Apply(Forwarding<TFunction> function, Forwarding<TTuple> ntuple) noexcept;

    /// \brief Invoke a function to each element in a n-tuple individually.
    template <typename TFunction, Concepts::NTupleReference TTuple>
    constexpr void ForEachApply(Forwarding<TFunction> function, Forwarding<TTuple> ntuple) noexcept;

    /// \brief Invoke a function to the VIndex-th element of each provided tuple at once.
    template <Int VIndex, typename TFunction, Concepts::NTupleReference... TTuples>
    constexpr decltype(auto) ProjectApply(Forwarding<TFunction> function, Forwarding<TTuples>... tuples) noexcept;

    /// \brief Invoke a function to each argument list generated by projecting the i-th element of all the provided tuples at once,
    ///        for each index up to the minimum rank among those tuples.
    template <typename TFunction, Concepts::NTupleReference... TTuples>
    constexpr void LockstepApply(Forwarding<TFunction> function, Forwarding<TTuples>... tuples) noexcept;

    /// \brief Create a new instance of type TType using TTuple as constructor arguments.
    template <typename TType, Concepts::NTupleReference TTuple>
    [[nodiscard]] constexpr TType MakeFromTuple(Forwarding<TTuple> tuple) noexcept;
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

    // Comparison.

    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    [[nodiscard]] constexpr Bool AreEqual(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept
    {
        // In some implementations comparing two instances for identity is much faster than comparing them for equality.
        // This function is left as a customization point for those implementations.

        return AreEquivalent(lhs, rhs);
    }

    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    [[nodiscard]] constexpr Bool AreEquivalent(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept
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
    [[nodiscard]] constexpr Ordering Compare(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept
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

    // Swap.

    template <Concepts::NTuple TTuple, Concepts::NTuple UTuple>
    requires (Templates::TupleRank<TTuple> == Templates::TupleRank<UTuple>)
    constexpr void Swap(Mutable<TTuple> lhs, Mutable<UTuple> rhs) noexcept
    {
        using namespace Templates;

        auto memberwise_swap = [&]<Int... VIndex>(Sequence<VIndex...>)
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

        auto memberwise_exchange = [&]<Int... VIndex>(Sequence<VIndex...>)
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

        auto memberwise_exchange = [&]<Int... VIndex>(Sequence<VIndex...>) mutable
        {
            using Syntropy::Exchange;

            return TTuple{ Exchange(Get<VIndex>(lhs), Get<VIndex>(Move(rhs)))... };
        };

        return memberwise_exchange(TupleSequenceFor<TTuple>{});
    }

    // Functional.

    template <typename TFunction, Concepts::NTupleReference TTuple>
    constexpr decltype(auto) Apply(Forwarding<TFunction> function, Forwarding<TTuple> ntuple) noexcept
    {
        using namespace Templates;

        auto apply = [&]<Int... VIndex>(Sequence<VIndex...>)
        {
            return function(Get<VIndex>(Forward<TTuple>(ntuple))...);
        };

        return apply(TupleSequenceFor<TTuple>{});
    }

    template <typename TFunction, Concepts::NTupleReference TTuple>
    constexpr void ForEachApply(Forwarding<TFunction> function, Forwarding<TTuple> ntuple) noexcept
    {
        using namespace Templates;

        auto for_each_apply = [&]<Int... VIndex>(Sequence<VIndex...>)
        {
            (function(Get<VIndex>(Forward<TTuple>(ntuple))), ...);
        };

        for_each_apply(TupleSequenceFor<TTuple>{});
    }

    template <Int VIndex, typename TFunction, Concepts::NTupleReference... TTuples>
    constexpr decltype(auto) ProjectApply(Forwarding<TFunction> function, Forwarding<TTuples>... tuples) noexcept
    {
        using namespace Templates;

        return function(Get<VIndex>(Forward<TTuples>(tuples))...);
    }

    template <typename TFunction, Concepts::NTupleReference... TTuples>
    constexpr void LockstepApply(Forwarding<TFunction> function, Forwarding<TTuples>... tuples) noexcept
    {
        using namespace Templates;

        constexpr auto kMinRank = Math::Min(TupleRank<TTuples>...);

        auto lockstep_apply = [&]<Int... VIndex>(Sequence<VIndex...>)
        {
            (ProjectApply<VIndex>(Forward<TFunction>(function), tuples...), ...);
        };

        lockstep_apply(MakeSequence<kMinRank>{});
    }

    template <typename TType, Concepts::NTupleReference TTuple>
    [[nodiscard]] constexpr TType MakeFromTuple(Forwarding<TTuple> tuple) noexcept
    {
        using namespace Templates;

        auto make_from_tuple = [&]<Int... VIndex>(Sequence<VIndex...>)
        {
            return TType(Get<VIndex>(Forward<TTuple>(tuple))...);
        };

        return make_from_tuple(Templates::TupleSequenceFor<TTuple>{});
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