
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

    template <typename TElement, ForwardingRecord TRecord>
    [[nodiscard]] constexpr auto
    Get(Forwarding<TRecord> record) noexcept
        -> decltype(Get<ElementIndexOf<
            TElement,
            Templates::QualifiedOf<TRecord>>>(
                Forward<TRecord>(record)))
    {

    }

    // Functional.
    // ===========

    template <typename TFunction, ForwardingRecord TRecord>
    constexpr decltype(auto)
    Apply(Forwarding<TFunction> function, Forwarding<TRecord> record) noexcept
    {
        auto apply = [&]<Int... TIndex>
            (Templates::Sequence<TIndex...>)
            {
                return function(Records::Get<TIndex>(
                    Forward<TRecord>(record))...);
            };

        return apply(EnumerationSequenceOf<TRecord>{});
    }

    template <typename TFunction, ForwardingRecord TRecord>
    constexpr void
    ForEachApply(Forwarding<TFunction> function,
                 Forwarding<TRecord> record) noexcept
    {
        auto for_each_apply = [&]<Int... TIndex>
            (Templates::Sequence<TIndex...>)
            {
                (function(Records::Get<TIndex>(Forward<TRecord>(record))),
                                               ...);
            };

        for_each_apply(EnumerationSequenceOf<TRecord>{});
    }

    template <Int TIndex,
              typename TFunction,
              ForwardingRecord... TRecords>
    constexpr decltype(auto)
    ProjectApply(Forwarding<TFunction> function,
                 Forwarding<TRecords>... records) noexcept
    {
        return function(Records::Get<TIndex>(Forward<TRecords>(records))...);
    }

    template <typename TFunction, ForwardingRecord... TRecords>
    constexpr void
    LockstepApply(Forwarding<TFunction> function,
                  Forwarding<TRecords>... records) noexcept
    {
        constexpr auto kMinRank = Math::Min(RankOf<TRecords>...);

        auto lockstep_apply = [&]<Int... TIndex>
            (Templates::Sequence<TIndex...>)
            {
                (ProjectApply<TIndex>(Forward<TFunction>(function),
                                      records...), ...);
            };

        lockstep_apply(Templates::MakeSequence<kMinRank>{});
    }

    template <typename TType, ForwardingRecord TRecord>
    [[nodiscard]] constexpr TType
    MakeFromRecord(Forwarding<TRecord> record) noexcept
    {
        auto make_from_record = [&]<Int... TIndex>
            (Templates::Sequence<TIndex...>)
            {
                return TType(Records::Get<TIndex>(Forward<TRecord>(record))
                             ...);
            };

        return make_from_record(EnumerationSequenceOf<TRecord>{});
    }

    // Swap.
    // =====

    template <Record TRecord, ForwardingRecord URecord>
    requires IsSameRank<TRecord, Templates::UnqualifiedOf<URecord>>
    [[nodiscard]] constexpr TRecord
    Exchange(Mutable<TRecord> lhs, Forwarding<URecord> rhs) noexcept
    {
        auto exchange = [&]<Int... TIndex>(
            Templates::Sequence<TIndex...>) mutable
        {
            return TRecord{
                Algorithms::Exchange(Records::Get<TIndex>(lhs),
                                    Records::Get<TIndex>(Forward(rhs)))... };
        };

        return exchange(EnumerationSequenceOf<TRecord>{});
    }

    // Compare.
    // ========

    template <Record TRecord, Record URecord>
    requires IsSameRank<TRecord, URecord>
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<TRecord> lhs, Immutable<URecord> rhs) noexcept
    {
        auto are_equal = [&]<Int... TIndex>(
            Templates::Sequence<TIndex...>)
        {
            return (Algorithms::AreEqual(Records::Get<TIndex>(lhs),
                                        Records::Get<TIndex>(rhs)) && ...);
        };

        return are_equal(lhs, rhs);
    }

    template <Record TRecord, Record URecord>
    requires IsSameRank<TRecord, URecord>
    [[nodiscard]] constexpr Bool
    AreEquivalent(Immutable<TRecord> lhs, Immutable<URecord> rhs) noexcept
    {
        auto are_equivalent = [&]<Int... TIndex>(
            Templates::Sequence<TIndex...>)
        {
            return (Algorithms::AreEquivalent(
                Records::Get<TIndex>(lhs),
                Records::Get<TIndex>(rhs)) && ...);
        };

        return are_equal(lhs, rhs);
    }

    template <Record TRecord, Record URecord>
    requires IsSameRank<TRecord, URecord>
    [[nodiscard]] constexpr Ordering
    Compare(Immutable<TRecord> lhs, Immutable<URecord> rhs) noexcept
    {
        auto compare = [&]<Int TIndex>(
            Ordering result, Templates::IntConstant<TIndex>)
        {
            return (result == Ordering::kEquivalent)
                ? Algorithms::Compare(Records::Get<TIndex>(lhs),
                                     Records::Get<TIndex>(rhs))
                : result;
        };

        auto lockstep_compare = [&]<Int... TIndex>(
            Templates::Sequence<TIndex...>)
        {
            auto result = Ordering::kEquivalent;

            ((result = compare(
                result,
                Templates::IntConstant<TIndex>{})), ...);

            return result;
        };

        return lockstep_compare(
            EnumerationSequenceOf<TRecord>{});
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
