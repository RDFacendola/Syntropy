
/// \file record.inl
///
/// \author Raffaele D. Facendola - Nov 2020

#pragma once

// ===========================================================================

namespace Syntropy::Sequences
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Sequence.
    // =========

    template <Int TIndex, typename TSequence>
    [[nodiscard]] constexpr auto
    Get(Forwarding<TSequence> sequence) noexcept
        -> decltype(Details::RouteGet<TIndex>(Forward<TSequence>(sequence)))
    {
        return Details::RouteGet<TIndex>(Forward<TSequence>(sequence));
    }






    // Functional.
    // ===========

    template <typename TFunction, Concepts::ForwardingSequence TSequence>
    constexpr decltype(auto)
    Apply(Forwarding<TFunction> function, Forwarding<TSequence> ntuple) noexcept
    {
        using namespace Templates;

        auto apply = [&]<Int... TIndex>
            (Syntropy::Templates::Sequence<TIndex...>)
            {
                return function(Get<TIndex>(Forward<TSequence>(ntuple))...);
            };

        return apply(Templates::SequenceEnumerationOf<TSequence>{});
    }

    template <typename TFunction, Concepts::ForwardingSequence TSequence>
    constexpr void
    ForEachApply(Forwarding<TFunction> function,
                 Forwarding<TSequence> ntuple) noexcept
    {
        using namespace Templates;

        auto for_each_apply = [&]<Int... TIndex>
            (Syntropy::Templates::Sequence<TIndex...>)
            {
                (function(Get<TIndex>(Forward<TSequence>(ntuple))), ...);
            };

        for_each_apply(Templates::SequenceEnumerationOf<TSequence>{});
    }

    template <Int TIndex,
              typename TFunction,
              Concepts::ForwardingSequence... TSequences>
    constexpr decltype(auto)
    ProjectApply(Forwarding<TFunction> function,
                 Forwarding<TSequences>... tuples) noexcept
    {
        using namespace Templates;

        return function(Get<TIndex>(Forward<TSequences>(tuples))...);
    }

    template <typename TFunction, Concepts::ForwardingSequence... TSequences>
    constexpr void
    LockstepApply(Forwarding<TFunction> function,
                  Forwarding<TSequences>... tuples) noexcept
    {
        using namespace Templates;

        constexpr auto kMinRank = Math::Min(SequenceRankOf<TSequences>...);

        auto lockstep_apply = [&]<Int... TIndex>
            (Syntropy::Templates::Sequence<TIndex...>)
            {
                (ProjectApply<TIndex>(Forward<TFunction>(function),
                                      tuples...), ...);
            };

        lockstep_apply(Syntropy::Templates::MakeSequence<kMinRank>{});
    }

    template <typename TType, Concepts::ForwardingSequence TSequence>
    [[nodiscard]] constexpr TType
    MakeFromTuple(Forwarding<TSequence> tuple) noexcept
    {
        using namespace Templates;

        auto make_from_tuple = [&]<Int... TIndex>
            (Syntropy::Templates::Sequence<TIndex...>)
            {
                return TType(Get<TIndex>(Forward<TSequence>(tuple))...);
            };

        return make_from_tuple(Templates::SequenceEnumerationOf<TSequence>{});
    }

}

// ===========================================================================

namespace Syntropy::Algorithm::Extensions
{
    /************************************************************************/
    /* SWAP EXTENSIONS                                                      */
    /************************************************************************/

    template <Sequences::Concepts::Sequence TSequence>
    constexpr void
    Swap<TSequence>::
    operator()(Mutable<TSequence> lhs, Mutable<TSequence> rhs)
    const noexcept
    {
        auto swap = [&]<Int... TIndex>
            (Syntropy::Templates::Sequence<TIndex...>)
            {
                (Algorithm::Swap(Sequences::Get<TIndex>(lhs),
                                 Sequences::Get<TIndex>(rhs)), ...);
            };

        swap(Sequences::Templates::SequenceEnumerationOf<TSequence>{});
    }

    template <Sequences::Concepts::Sequence TSequence,
              Sequences::Concepts::Sequence USequence>
    requires Sequences::Templates::SequenceSameRank<TSequence, USequence>
    constexpr TSequence
    Exchange<TSequence, USequence>::
    operator()(Mutable<TSequence> lhs, Immutable<USequence> rhs)
    const noexcept
    {
        auto exchange = [&]<Int... TIndex>(
            Syntropy::Templates::Sequence<TIndex...>)
        {
            return TSequence{
                Algorithm::Exchange(Sequences::Get<TIndex>(lhs),
                                    Sequences::Get<TIndex>(rhs))... };
        };

        return exchange(Sequences::Templates::SequenceEnumerationOf<TSequence>{});
    }

    template <Syntropy::Sequences::Concepts::Sequence TSequence,
              Syntropy::Sequences::Concepts::Sequence USequence>
    requires Sequences::Templates::SequenceSameRank<TSequence, USequence>
    constexpr TSequence
    Exchange<TSequence, USequence>::
    operator()(Mutable<TSequence> lhs, Movable<USequence> rhs)
    const noexcept
    {
        auto exchange = [&]<Int... TIndex>(
            Syntropy::Templates::Sequence<TIndex...>) mutable
        {
            return TSequence{
                Algorithm::Exchange(Sequences::Get<TIndex>(lhs),
                                    Sequences::Get<TIndex>(Move(rhs)))... };
        };

        return exchange(Sequences::Templates::SequenceEnumerationOf<TSequence>{});
    }

    /************************************************************************/
    /* COMPARE EXTENSIONS                                                   */
    /************************************************************************/

    template <Sequences::Concepts::Sequence TSequence,
              Sequences::Concepts::Sequence USequence>
    requires Sequences::Templates::SequenceSameRank<TSequence, USequence>
    [[nodiscard]] constexpr Bool
    AreEqual<TSequence, USequence>::
    operator()(Immutable<TSequence> lhs, Immutable<USequence> rhs)
    const noexcept
    {
        auto are_equal = [&]<Int... TIndex>(
            Syntropy::Templates::Sequence<TIndex...>)
        {
            return (Algorithm::AreEqual(Sequences::Get<TIndex>(lhs),
                                        Sequences::Get<TIndex>(rhs)) && ...);
        };

        return are_equal(lhs, rhs);
    }

    template <Sequences::Concepts::Sequence TSequence,
              Sequences::Concepts::Sequence USequence>
    requires Sequences::Templates::SequenceSameRank<TSequence, USequence>
    [[nodiscard]] constexpr Bool
    AreEquivalent<TSequence, USequence>::
    operator()(Immutable<TSequence> lhs, Immutable<USequence> rhs)
    const noexcept
    {
        auto are_equivalent = [&]<Int... TIndex>(
            Syntropy::Templates::Sequence<TIndex...>)
        {
            return (Algorithm::AreEquivalent(
                Sequences::Get<TIndex>(lhs),
                Sequences::Get<TIndex>(rhs)) && ...);
        };

        return are_equal(lhs, rhs);
    }

    template <Sequences::Concepts::Sequence TSequence,
              Sequences::Concepts::Sequence USequence>
    requires Sequences::Templates::SequenceSameRank<TSequence, USequence>
    [[nodiscard]] constexpr Ordering
    Compare<TSequence, USequence>::
    operator()(Immutable<TSequence> lhs, Immutable<USequence> rhs)
    const noexcept
    {
        auto compare = [&]<Int TIndex>(
            Ordering result, Syntropy::Templates::IntConstant<TIndex>)
        {
            return (result == Ordering::kEquivalent)
                ? Algorithm::Compare(Sequences::Get<TIndex>(lhs),
                                     Sequences::Get<TIndex>(rhs))
                : result;
        };

        auto lockstep_compare = [&]<Int... TIndex>(
            Syntropy::Templates::Sequence<TIndex...>)
        {
            auto result = Ordering::kEquivalent;

            ((result = compare(
                result,
                Syntropy::Templates::IntConstant<TIndex>{})), ...);

            return result;
        };

        return lockstep_compare(Sequences::Templates::SequenceEnumerationOf<TSequence>{});
    }

}

// ===========================================================================

namespace std
{
    /************************************************************************/
    /* STRUCTURED BINDINGS                                                  */
    /************************************************************************/

    template <Syntropy::Sequences::Concepts::Sequence TSequence>
    struct std::tuple_size<TSequence>
    {
        static constexpr std::size_t value
            = Syntropy::Sequences::Templates::SequenceRankOf<TSequence>;
    };

    template <std::size_t TIndex,
              Syntropy::Sequences::Concepts::Sequence TSequence>
    struct std::tuple_element<TIndex, TSequence>
    {
        using type
            = Syntropy::Sequences::Templates::SequenceElementTypeOf<TIndex, TSequence>;
    };

    template <std::size_t TIndex,
              Syntropy::Sequences::Concepts::Sequence TSequence>
    decltype(auto) get(Syntropy::Immutable<TSequence> tuple)
    {
        return Get<TIndex>(tuple);
    }

    template <std::size_t TIndex,
              Syntropy::Sequences::Concepts::Sequence TSequence>
    decltype(auto) get(Syntropy::Movable<TSequence> tuple)
    {
        return Get<TIndex>(Syntropy::Move(tuple));
    }
}

// ===========================================================================
