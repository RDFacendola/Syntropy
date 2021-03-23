
/// \file ntuple.inl
///
/// \author Raffaele D. Facendola - Nov 2020

#pragma once

// ===========================================================================

namespace Syntropy::Sequences
{
    /************************************************************************/
    /* TUPLES                                                               */
    /************************************************************************/

    // Comparison.
    // ===========

    template <Concepts::Sequence TTuple, Concepts::Sequence UTuple>
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept
    {
        // In some implementations comparing two instances for identity
        // is much faster than comparing them for equality.
        // This function is left as a customization point for those
        // implementations.

        return AreEquivalent(lhs, rhs);
    }

    template <Concepts::Sequence TTuple, Concepts::Sequence UTuple>
    [[nodiscard]] constexpr Bool
    AreEquivalent(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept
    {
        using namespace Templates;

        // Early out if the two tuples have different ranks.

        if constexpr (Rank<TTuple> != Rank<UTuple>)
        {
            return false;
        }
        else
        {
            return Compare(lhs, rhs) == Ordering::kEquivalent;
        }
    }

    template <Concepts::Sequence TTuple, Concepts::Sequence UTuple>
    [[nodiscard]] constexpr Ordering
    Compare(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept
    {
        using namespace Templates;

        // Lexicographic compare between two tuple elements.

        auto lexicographic_compare = [&lhs, &rhs]<Int VIndex>
            (Ordering compare_result,
             Syntropy::Templates::IntConstant<VIndex>)
            {
                return (compare_result == Ordering::kEquivalent)
                    ? (Get<VIndex>(lhs) <=> Get<VIndex>(rhs))
                    : compare_result;
            };

        // Lexicographic compare between two same-rank tuples.

        auto lockstep_lexicographic_compare
            = [&lexicographic_compare]<Int... VIndex>
                (Syntropy::Templates::Sequence<VIndex...>) mutable
        {
            auto compare_result = Ordering::kEquivalent;

            ((compare_result = lexicographic_compare(
                compare_result,
                Syntropy::Templates::IntConstant<VIndex>{})), ...);

            return compare_result;
        };

        // 1) Same-rank comparison.

        if constexpr (Rank<TTuple> == Rank<UTuple>)
        {
            return lockstep_lexicographic_compare(TupleSequenceFor<TTuple>{});
        }

        // 2) Left-to-right comparison.

        if constexpr (Rank<TTuple> < Rank<UTuple>)
        {
            if (auto compare_result
                = lockstep_lexicographic_compare(
                    TupleSequenceFor<TTuple>{});
                compare_result != Ordering::kEquivalent)
            {
                return compare_result;
            }

            return Ordering::kLess;
        }

        // 3) Right-to-left comparison.

        if constexpr (Rank<TTuple> > Rank<UTuple>)
        {
            if (auto compare_result
                = lockstep_lexicographic_compare(
                    TupleSequenceFor<UTuple>{});
                compare_result != Ordering::kEquivalent)
            {
                return compare_result;
            }

            return Ordering::kGreater;
        }
    }

    // Swap.
    // =====

    template <Concepts::Sequence TTuple, Concepts::Sequence UTuple>
    requires (Templates::Rank<TTuple> == Templates::Rank<UTuple>)
    constexpr void
    Swap(Mutable<TTuple> lhs, Mutable<UTuple> rhs) noexcept
    {
        using namespace Templates;

        auto memberwise_swap = [&]<Int... VIndex>
            (Syntropy::Templates::Sequence<VIndex...>)
            {
                (Swap(Get<VIndex>(lhs), Get<VIndex>(rhs)), ...);
            };

        return memberwise_swap(TupleSequenceFor<TTuple>{});
    }

    template <Concepts::Sequence TTuple, Concepts::Sequence UTuple>
    requires (Templates::Rank<TTuple> == Templates::Rank<UTuple>)
    constexpr TTuple
    Exchange(Mutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept
    {
        using namespace Templates;

        auto memberwise_exchange = [&]<Int... VIndex>(
            Syntropy::Templates::Sequence<VIndex...>)
        {
            return TTuple{ Algorithm::Exchange(Get<VIndex>(lhs),
                                               Get<VIndex>(rhs))... };
        };

        return memberwise_exchange(TupleSequenceFor<TTuple>{});
    }

    template <Concepts::Sequence TTuple, Concepts::Sequence UTuple>
    requires (Templates::Rank<TTuple> == Templates::Rank<UTuple>)
    constexpr TTuple
    Exchange(Mutable<TTuple> lhs, Movable<UTuple> rhs) noexcept
    {
        using namespace Templates;

        auto memberwise_exchange = [&]<Int... VIndex>(
            Syntropy::Templates::Sequence<VIndex...>) mutable
        {
            return TTuple{ Algorithm::Exchange(Get<VIndex>(lhs),
                                               Get<VIndex>(Move(rhs)))... };
        };

        return memberwise_exchange(TupleSequenceFor<TTuple>{});
    }

    // Functional.
    // ===========

    template <typename TFunction, Concepts::SequenceReference TTuple>
    constexpr decltype(auto)
    Apply(Forwarding<TFunction> function, Forwarding<TTuple> ntuple) noexcept
    {
        using namespace Templates;

        auto apply = [&]<Int... VIndex>
            (Syntropy::Templates::Sequence<VIndex...>)
            {
                return function(Get<VIndex>(Forward<TTuple>(ntuple))...);
            };

        return apply(TupleSequenceFor<TTuple>{});
    }

    template <typename TFunction, Concepts::SequenceReference TTuple>
    constexpr void
    ForEachApply(Forwarding<TFunction> function,
                 Forwarding<TTuple> ntuple) noexcept
    {
        using namespace Templates;

        auto for_each_apply = [&]<Int... VIndex>
            (Syntropy::Templates::Sequence<VIndex...>)
            {
                (function(Get<VIndex>(Forward<TTuple>(ntuple))), ...);
            };

        for_each_apply(TupleSequenceFor<TTuple>{});
    }

    template <Int VIndex,
              typename TFunction,
              Concepts::SequenceReference... TSequences>
    constexpr decltype(auto)
    ProjectApply(Forwarding<TFunction> function,
                 Forwarding<TSequences>... tuples) noexcept
    {
        using namespace Templates;

        return function(Get<VIndex>(Forward<TSequences>(tuples))...);
    }

    template <typename TFunction, Concepts::SequenceReference... TSequences>
    constexpr void
    LockstepApply(Forwarding<TFunction> function,
                  Forwarding<TSequences>... tuples) noexcept
    {
        using namespace Templates;

        constexpr auto kMinRank = Math::Min(Rank<TSequences>...);

        auto lockstep_apply = [&]<Int... VIndex>
            (Syntropy::Templates::Sequence<VIndex...>)
            {
                (ProjectApply<VIndex>(Forward<TFunction>(function),
                                      tuples...), ...);
            };

        lockstep_apply(Syntropy::Templates::MakeSequence<kMinRank>{});
    }

    template <typename TType, Concepts::SequenceReference TTuple>
    [[nodiscard]] constexpr TType
    MakeFromTuple(Forwarding<TTuple> tuple) noexcept
    {
        using namespace Templates;

        auto make_from_tuple = [&]<Int... VIndex>
            (Syntropy::Templates::Sequence<VIndex...>)
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
    /* STRUCTURED BINDINGS                                                  */
    /************************************************************************/

    template <Syntropy::Sequences::Concepts::Sequence TTuple>
    struct std::tuple_size<TTuple>
    {
        static constexpr std::size_t value
            = Syntropy::Sequences::Templates::Rank<TTuple>;
    };

    template <std::size_t VIndex, Syntropy::Sequences::Concepts::Sequence TTuple>
    struct std::tuple_element<VIndex, TTuple>
    {
        using type = Syntropy::Sequences::Templates::ElementType<VIndex, TTuple>;
    };

    template <std::size_t VIndex, Syntropy::Sequences::Concepts::Sequence TTuple>
    decltype(auto) get(Syntropy::Immutable<TTuple> tuple)
    {
        return Get<VIndex>(tuple);
    }

    template <std::size_t VIndex, Syntropy::Sequences::Concepts::Sequence TTuple>
    decltype(auto) get(Syntropy::Movable<TTuple> tuple)
    {
        return Get<VIndex>(Syntropy::Move(tuple));
    }
}

// ===========================================================================
