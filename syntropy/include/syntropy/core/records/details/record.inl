
/// \file record.inl
///
/// \author Raffaele D. Facendola - Nov 2020

#pragma once

// ===========================================================================

namespace Syntropy::Records
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Record.
    // =========

    template <Int TIndex, typename TRecord>
    [[nodiscard]] constexpr auto
    Get(Forwarding<TRecord> record) noexcept
        -> decltype(Details::RouteGet<TIndex>(Forward<TRecord>(record)))
    {
        return Details::RouteGet<TIndex>(Forward<TRecord>(record));
    }

    // Element access.
    // ===============

    template <typename TElement, RecordReference TRecord>
    [[nodiscard]] constexpr auto
    Get(Forwarding<TRecord> record) noexcept
        -> decltype(Records::Get<ElementIndexOf<TElement, TRecord>>(
            Forward<TRecord>(record)))
    {
        return Records::Get<ElementIndexOf<TElement, TRecord>>(
            Forward<TRecord>(record));
    }

    // Functional.
    // ===========

    template <typename TFunction, RecordReference TRecord>
    constexpr decltype(auto)
    Apply(Forwarding<TFunction> function, Forwarding<TRecord> record) noexcept
    {
        auto apply = [&]<Int... TIndex> (Templates::Sequence<TIndex...>)
        {
            return function(Records::Get<TIndex>(Forward<TRecord>(record))...);
        };

        return apply(SequenceOf<TRecord>{});
    }

    template <typename TFunction, RecordReference TRecord>
    constexpr void
    ForEachApply(Forwarding<TFunction> function,
                 Forwarding<TRecord> record) noexcept
    {
        auto apply = [&]<Int... TIndex> (Templates::Sequence<TIndex...>)
        {
            (function(Records::Get<TIndex>(Forward<TRecord>(record))), ...);
        };

        apply(SequenceOf<TRecord>{});
    }

    template <Int TIndex, typename TFunction, RecordReference... TRecords>
    constexpr decltype(auto)
    ProjectApply(Forwarding<TFunction> function,
                 Forwarding<TRecords>... records) noexcept
    {
        return function(Records::Get<TIndex>(Forward<TRecords>(records))...);
    }

    template <typename TFunction, RecordReference... TRecords>
    constexpr void
    LockstepApply(Forwarding<TFunction> function,
                  Forwarding<TRecords>... records) noexcept
    {
        constexpr auto kMinRank = Math::Min(RankOf<TRecords>...);

        auto apply = [&]<Int... TIndex>(Templates::Sequence<TIndex...>)
        {
            (ProjectApply<TIndex>(Forward<TFunction>(function), records...),
                                  ...);
        };

        apply(Templates::MakeSequence<kMinRank>{});
    }

    template <typename TType, RecordReference TRecord>
    [[nodiscard]] constexpr TType
    MakeFromRecord(Forwarding<TRecord> record) noexcept
    {
        auto make = [&]<Int... TIndex>(Templates::Sequence<TIndex...>)
        {
            return TType{ Records::Get<TIndex>(Forward<TRecord>(record))... };
        };

        return make(SequenceOf<TRecord>{});
    }

    // Move \ Copy.
    // ============

    template <Record TRecord, Record URecord>
    requires IsSameRank<TRecord, URecord>
    constexpr void
    Copy(Mutable<TRecord> destination, Immutable<URecord> source) noexcept
    {
        return PartialCopy(destination, source);
    }

    template <Record TRecord, Record URecord>
    requires IsSameRank<TRecord, URecord>
    constexpr void
    Move(Mutable<TRecord> destination, Immutable<URecord> source) noexcept
    {
        return PartialMove(destination, source);
    }

    template <Record TRecord, Record URecord>
    requires IsSameRank<TRecord, URecord>
    constexpr void
    Move(Mutable<TRecord> destination, Movable<URecord> source) noexcept
    {
        return PartialMove(destination, Move(source));
    }

    template <Record TRecord, Record URecord>
    constexpr Int
    PartialCopy(Mutable<TRecord> destination, Immutable<URecord> source)
    noexcept
    {
        auto elementwise_copy = [](auto& destination_element,
                                   auto&& source_element)
        {
            destination_element = Copy(source_element);
        };

        Records::LockstepApply(elementwise_copy, destination, source);

        return Math::Min(RankOf<TRecord>, RankOf<URecord>);
    }

    template <Record TRecord, Record URecord>
    constexpr Int
    PartialMove(Mutable<TRecord> destination, Immutable<URecord> source)
    noexcept
    {
        return PartialCopy(destination, source);
    }

    template <Record TRecord, Record URecord>
    constexpr Int
    PartialMove(Mutable<TRecord> destination, Movable<URecord> source)
    noexcept
    {
        auto elementwise_move = [](auto& destination_element,
                                   auto&& source_element)
        {
            destination_element = Move(source_element);
        };

        Records::LockstepApply(elementwise_move, destination, Move(source));

        return Math::Min(RankOf<TRecord>, RankOf<URecord>);
    }

    // Swap.
    // =====

    template <Record TRecord, RecordReference URecord>
    requires Records::IsSameRank<TRecord, URecord>
    [[nodiscard]] constexpr TRecord
    Exchange(Mutable<TRecord> lhs, Forwarding<URecord> rhs) noexcept
    {
        auto exchange = [&]<Int... TIndex>(Templates::Sequence<TIndex...>)
        {
            return TRecord
            {
                Algorithms::Exchange(Records::Get<TIndex>(lhs),
                                     Records::Get<TIndex>(Forward(rhs)))...
            };
        };

        return exchange(SequenceOf<TRecord>{});
    }

    template <Record TRecord>
    constexpr void
    Swap(Mutable<TRecord> lhs, Mutable<TRecord> rhs) noexcept
    {
        auto swap = [&]<Int... TIndex>(Templates::Sequence<TIndex...>)
        {
            (Algorithms::Swap(Records::Get<TIndex>(lhs),
                              Records::Get<TIndex>(rhs)), ...);
        };

        swap(SequenceOf<TRecord>{});
    }

    // Compare.
    // ========

    template <Record TRecord, Record URecord>
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<TRecord> lhs, Immutable<URecord> rhs) noexcept
    {
        auto equal = [&]<Int... TIndex>(Templates::Sequence<TIndex...>)
        {
            return (Algorithms::AreEqual(Records::Get<TIndex>(lhs),
                                         Records::Get<TIndex>(rhs))
                    && ...);
        };

        if constexpr (IsSameRank<TRecord, URecord>)
        {
            return equal(SequenceOf<TRecord>{});
        }

        return false;
    }

    template <Record TRecord, Record URecord>
    [[nodiscard]] constexpr Bool
    AreEquivalent(Immutable<TRecord> lhs, Immutable<URecord> rhs) noexcept
    {
        auto equivalent = [&]<Int... TIndex>(Templates::Sequence<TIndex...>)
        {
            return (Algorithms::AreEquivalent(Records::Get<TIndex>(lhs),
                                              Records::Get<TIndex>(rhs))
                    && ...);
        };

        if constexpr (IsSameRank<TRecord, URecord>)
        {
            return equivalent(SequenceOf<TRecord>{});
        }

        return false;
    }

    template <Record TRecord, Record URecord>
    [[nodiscard]] constexpr Ordering
    Compare(Immutable<TRecord> lhs, Immutable<URecord> rhs) noexcept
    {
        constexpr auto LeftRank = RankOf<TRecord>;
        constexpr auto RightRank = RankOf<URecord>;
        constexpr auto MinRank = Math::Min<LeftRank, RightRank>;

        auto compare_at = [&]<Int TIndex>(Ordering result)
        {
            if (result == Ordering::kEquivalent)
            {
                return Algorithms::Compare(Records::Get<TIndex>(lhs),
                                           Records::Get<TIndex>(rhs));
            }

            return result;
        };

        auto compare = [&]<Int... TIndex>(Templates::Sequence<TIndex...>)
        {
            auto result = Ordering::kEquivalent;

            ((result = compare_at<TIndex>(result)), ...);

            return result;
        };

        if(auto result = compare(Templates::MakeSequence<MinRank>{});
           result != Ordering::kEquivalent)
        {
            return result;
        }

        return (LeftRank < RightRank) ? Ordering::kLess : Ordering::kGreater;
    }

}

// ===========================================================================

namespace std
{
    /************************************************************************/
    /* STRUCTURED BINDINGS                                                  */
    /************************************************************************/

    template <Syntropy::Records::Record TRecord>
    struct std::tuple_size<TRecord>
    {
        static constexpr
        std::size_t value = Syntropy::Records::RankOf<TRecord>;
    };

    template <std::size_t TIndex,
              Syntropy::Records::Record TRecord>
    struct std::tuple_element<TIndex, TRecord>
    {
        using type = Syntropy::Records::ElementTypeOf<TIndex, TRecord>;
    };

    template <std::size_t TIndex, Syntropy::Records::Record TRecord>
    decltype(auto) get(Syntropy::Immutable<TRecord> record)
    {
        return Get<TIndex>(record);
    }

    template <std::size_t TIndex, Syntropy::Records::Record TRecord>
    decltype(auto) get(Syntropy::Movable<TRecord> record)
    {
        return Get<TIndex>(Syntropy::Move(record));
    }
}

// ===========================================================================
