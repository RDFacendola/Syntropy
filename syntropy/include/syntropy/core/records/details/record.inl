
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

    // Functional.
    // ===========

    template <typename TFunction, Concepts::ForwardingRecord TRecord>
    constexpr decltype(auto)
    Apply(Forwarding<TFunction> function, Forwarding<TRecord> record) noexcept
    {
        auto apply = [&]<Int... TIndex>
            (Syntropy::Templates::Sequence<TIndex...>)
            {
                return function(Records::Get<TIndex>(
                    Forward<TRecord>(record))...);
            };

        return apply(Templates::RecordEnumerationOf<TRecord>{});
    }

    template <typename TFunction, Concepts::ForwardingRecord TRecord>
    constexpr void
    ForEachApply(Forwarding<TFunction> function,
                 Forwarding<TRecord> record) noexcept
    {
        auto for_each_apply = [&]<Int... TIndex>
            (Syntropy::Templates::Sequence<TIndex...>)
            {
                (function(Records::Get<TIndex>(
                    Forward<TRecord>(record))), ...);
            };

        for_each_apply(Templates::RecordEnumerationOf<TRecord>{});
    }

    template <Int TIndex,
              typename TFunction,
              Concepts::ForwardingRecord... TRecords>
    constexpr decltype(auto)
    ProjectApply(Forwarding<TFunction> function,
                 Forwarding<TRecords>... records) noexcept
    {
        return function(Records::Get<TIndex>(Forward<TRecords>(records))...);
    }

    template <typename TFunction, Concepts::ForwardingRecord... TRecords>
    constexpr void
    LockstepApply(Forwarding<TFunction> function,
                  Forwarding<TRecords>... records) noexcept
    {
        constexpr auto kMinRank
            = Math::Min(Templates::RecordRankOf<TRecords>...);

        auto lockstep_apply = [&]<Int... TIndex>
            (Syntropy::Templates::Sequence<TIndex...>)
            {
                (ProjectApply<TIndex>(Forward<TFunction>(function),
                                      records...), ...);
            };

        lockstep_apply(Syntropy::Templates::MakeSequence<kMinRank>{});
    }

    template <typename TType, Concepts::ForwardingRecord TRecord>
    [[nodiscard]] constexpr TType
    MakeFromRecord(Forwarding<TRecord> record) noexcept
    {
        auto make_from_record = [&]<Int... TIndex>
            (Syntropy::Templates::Sequence<TIndex...>)
            {
                return TType(
                    Records::Get<TIndex>(Forward<TRecord>(record))...);
            };

        return make_from_record(Templates::RecordEnumerationOf<TRecord>{});
    }

}

// ===========================================================================

namespace Syntropy::Algorithm::Extensions
{
    /************************************************************************/
    /* SWAP EXTENSIONS                                                      */
    /************************************************************************/

    template <Records::Concepts::Record TRecord>
    constexpr void
    Swap<TRecord>::
    operator()(Mutable<TRecord> lhs, Mutable<TRecord> rhs)
    const noexcept
    {
        auto swap = [&]<Int... TIndex>
            (Syntropy::Templates::Sequence<TIndex...>)
            {
                (Algorithm::Swap(Records::Get<TIndex>(lhs),
                                 Records::Get<TIndex>(rhs)), ...);
            };

        swap(Records::Templates::RecordEnumerationOf<TRecord>{});
    }

    template <Records::Concepts::Record TRecord,
              Records::Concepts::Record URecord>
    requires Records::Templates::RecordSameRank<TRecord, URecord>
    constexpr TRecord
    Exchange<TRecord, URecord>::
    operator()(Mutable<TRecord> lhs, Immutable<URecord> rhs)
    const noexcept
    {
        auto exchange = [&]<Int... TIndex>(
            Syntropy::Templates::Sequence<TIndex...>)
        {
            return TRecord{
                Algorithm::Exchange(Records::Get<TIndex>(lhs),
                                    Records::Get<TIndex>(rhs))... };
        };

        return exchange(Records::Templates::RecordEnumerationOf<TRecord>{});
    }

    template <Syntropy::Records::Concepts::Record TRecord,
              Syntropy::Records::Concepts::Record URecord>
    requires Records::Templates::RecordSameRank<TRecord, URecord>
    constexpr TRecord
    Exchange<TRecord, URecord>::
    operator()(Mutable<TRecord> lhs, Movable<URecord> rhs)
    const noexcept
    {
        auto exchange = [&]<Int... TIndex>(
            Syntropy::Templates::Sequence<TIndex...>) mutable
        {
            return TRecord{
                Algorithm::Exchange(Records::Get<TIndex>(lhs),
                                    Records::Get<TIndex>(Move(rhs)))... };
        };

        return exchange(Records::Templates::RecordEnumerationOf<TRecord>{});
    }

    /************************************************************************/
    /* COMPARE EXTENSIONS                                                   */
    /************************************************************************/

    template <Records::Concepts::Record TRecord,
              Records::Concepts::Record URecord>
    requires Records::Templates::RecordSameRank<TRecord, URecord>
    [[nodiscard]] constexpr Bool
    AreEqual<TRecord, URecord>::
    operator()(Immutable<TRecord> lhs, Immutable<URecord> rhs)
    const noexcept
    {
        auto are_equal = [&]<Int... TIndex>(
            Syntropy::Templates::Sequence<TIndex...>)
        {
            return (Algorithm::AreEqual(Records::Get<TIndex>(lhs),
                                        Records::Get<TIndex>(rhs)) && ...);
        };

        return are_equal(lhs, rhs);
    }

    template <Records::Concepts::Record TRecord,
              Records::Concepts::Record URecord>
    requires Records::Templates::RecordSameRank<TRecord, URecord>
    [[nodiscard]] constexpr Bool
    AreEquivalent<TRecord, URecord>::
    operator()(Immutable<TRecord> lhs, Immutable<URecord> rhs)
    const noexcept
    {
        auto are_equivalent = [&]<Int... TIndex>(
            Syntropy::Templates::Sequence<TIndex...>)
        {
            return (Algorithm::AreEquivalent(
                Records::Get<TIndex>(lhs),
                Records::Get<TIndex>(rhs)) && ...);
        };

        return are_equal(lhs, rhs);
    }

    template <Records::Concepts::Record TRecord,
              Records::Concepts::Record URecord>
    requires Records::Templates::RecordSameRank<TRecord, URecord>
    [[nodiscard]] constexpr Ordering
    Compare<TRecord, URecord>::
    operator()(Immutable<TRecord> lhs, Immutable<URecord> rhs)
    const noexcept
    {
        auto compare = [&]<Int TIndex>(
            Ordering result, Syntropy::Templates::IntConstant<TIndex>)
        {
            return (result == Ordering::kEquivalent)
                ? Algorithm::Compare(Records::Get<TIndex>(lhs),
                                     Records::Get<TIndex>(rhs))
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

        return lockstep_compare(
            Records::Templates::RecordEnumerationOf<TRecord>{});
    }

}

// ===========================================================================

namespace std
{
    /************************************************************************/
    /* STRUCTURED BINDINGS                                                  */
    /************************************************************************/

    template <Syntropy::Records::Concepts::Record TRecord>
    struct std::tuple_size<TRecord>
    {
        static constexpr std::size_t value
            = Syntropy::Records::Templates::RecordRankOf<TRecord>;
    };

    template <std::size_t TIndex,
              Syntropy::Records::Concepts::Record TRecord>
    struct std::tuple_element<TIndex, TRecord>
    {
        using type
            = Syntropy::Records::Templates::RecordElementTypeOf<TIndex, TRecord>;
    };

    template <std::size_t TIndex,
              Syntropy::Records::Concepts::Record TRecord>
    decltype(auto) get(Syntropy::Immutable<TRecord> record)
    {
        return Get<TIndex>(record);
    }

    template <std::size_t TIndex,
              Syntropy::Records::Concepts::Record TRecord>
    decltype(auto) get(Syntropy::Movable<TRecord> record)
    {
        return Get<TIndex>(Syntropy::Move(record));
    }
}

// ===========================================================================
