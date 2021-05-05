
/// \file tuple.inl
///
/// \author Raffaele D. Facendola - September 2020.

// ===========================================================================

#include "syntropy/math/math.h"
#include "syntropy/language/support/swap.h"

// ===========================================================================

namespace Syntropy::RecordsADL
{
    /************************************************************************/
    /* RANGE EXTENSIONS                                                     */
    /************************************************************************/

    /// \brief Type alias for the Get extension functor.
    template <Int TIndex, typename TRecord>
    using GetExtension
        = Records::Extensions::Get<TIndex, TRecord>;

    /// \brief Invoke the Get function via extension functor.
    template <Int TIndex, typename TRecord>
    [[nodiscard]] constexpr auto
    InvokeGetExtension(Forwarding<TRecord> rhs)
        noexcept -> decltype(GetExtension<TIndex, TRecord>{}(
            Forward<TRecord>(rhs)))
    {
        return GetExtension<TIndex, TRecord>{}(Forward<TRecord>(rhs));
    }

    /************************************************************************/
    /* RECORDS                                                              */
    /************************************************************************/

    /// \brief Invoke the Get function, trying different implementations.
    template <typename TIndex, typename TRange>
    constexpr decltype(auto)
    InvokeGet(Forwarding<TRange> rhs) noexcept
    {
        auto extension = [](auto&& rhs)
            -> decltype(InvokeGetExtension<TIndex>(Forward<TRange>(rhs)))
        {
            return InvokeGetExtension<TIndex>(Forward<TRange>(rhs));
        };

        auto member_function = [](auto&& rhs)
            -> decltype(rhs.template Get<TIndex>())
        {
            return rhs.template Get<TIndex>();
        };

        auto non_member_function = [](auto&& rhs)
            -> decltype(Get<TIndex>(Forward<TRange>(rhs)))
        {
            return Get<TIndex>(Forward<TRange>(rhs));
        };

        return Support::InvokeAny(extension,
                                  member_function,
                                  non_member_function)(rhs);
    }

}

// ===========================================================================

namespace Syntropy::Details
{
    /************************************************************************/
    /* TUPLE                                                                */
    /************************************************************************/

    // Concatenate helpers.
    // ====================

    // In order to concatenate records together, two sequences with the same
    // total size are generated.
    // The first sequence is used to associate each element in the resulting
    // sequence with the index of the record the element is taken from.
    // The second sequence is used to associate each element in the resulting
    // sequence with the index of the element in the owning record.

    // Example: A {a,b,c}, B {d,e}, C {f}
    // First sequence:  000-11-2 AAA-BB-C
    // Second sequence: 012-01-0 abc-de-f

    /// \brief Sequence associating each element to the generating tuple.
    template <typename TSequence, typename... TRecords>
    struct TupleEnumerateRecordsHelper {};

    /// \brief Partial template specialization for sequence-records pairs.
    template <Int... TIndex, typename... TRecords>
    struct TupleEnumerateRecordsHelper<Templates::Sequence<TIndex...>,
                                       TRecords...>
        : Templates::Alias<
            Templates::SequenceConcatenate<
                Templates::SequenceRepeat<TIndex,
                                          Records::RankOf<TRecords>>...>> {};

    /// \brief Sequence associating each element to the source record.
    template <typename... TRecords>
    using TupleEnumerateRecords
        = typename TupleEnumerateRecordsHelper<
            Templates::SequenceFor<TRecords...>, TRecords...>::Type;

    /// \brief Sequence associating each element to the source record element.
    template <typename... TRecords>
    using TupleEnumerateRecordElements
        = Templates::SequenceConcatenate<Records::SequenceOf<TRecords>...>;
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* TUPLE                                                                */
    /************************************************************************/

    template <typename TElement, typename... TElements>
    template <typename UElement>
    requires
        Details::TupleDefaultConstructor<UElement, TElements...>
    constexpr
    Tuple<TElement, TElements...>
    ::Tuple() noexcept
        : Tuple<TElements...>{}
        , element_{}
    {

    }

    template <typename TElement, typename... TElements>
    template <typename... UElements, typename VElements>
    requires
        Details::TupleDirectConstructor<VElements, UElements...>
    constexpr
    Tuple<TElement, TElements...>
    ::Tuple(Forwarding<UElements>... elements) noexcept
        : Tuple(DirectTag{}, Forward<UElements>(elements)...)
    {

    }

    template <typename TElement, typename... TElements>
    template <typename... UElements, typename VElements>
    requires
        Details::TupleCopyConstructor<VElements, UElements...>
    constexpr
    Tuple<TElement, TElements...>
    ::Tuple(Immutable<Tuple<UElements...>> rhs) noexcept
        : Tuple(ElementwiseTag{},
                Templates::SequenceFor<UElements...>{},
                rhs)
    {

    }

    template <typename TElement, typename... TElements>
    template <typename... UElements, typename VElements>
    requires
        Details::TupleMoveConstructor<VElements, UElements...>
    constexpr
    Tuple<TElement, TElements...>
    ::Tuple(Movable<Tuple<UElements...>> rhs) noexcept
        : Tuple(ElementwiseTag{},
                Templates::SequenceFor<UElements...>{},
                Move(rhs))
    {

    }

    template <typename TElement, typename... TElements>
    template <typename UElement, typename... UElements>
    constexpr
    Tuple<TElement, TElements...>
    ::Tuple(DirectTag,
            Forwarding<UElement> element,
            Forwarding<UElements>... elements) noexcept
        : BaseType(Forward<UElements>(elements)...)
        , element_(Forward<UElement>(element))
    {

    }

    template <typename TElement, typename... TElements>
    template <typename TTuple, Int... TSequence>
    constexpr
    Tuple<TElement, TElements...>
    ::Tuple(ElementwiseTag,
            Templates::Sequence<TSequence...>,
            Forwarding<TTuple> tuple) noexcept
        : Tuple(ElementwiseTag{}, Get<TSequence>(Forward<TTuple>(tuple))...)
    {

    }

    template <typename TElement, typename... TElements>
    template <typename... UElements, typename VElements>
    requires
        Details::TupleCopyAssignment<VElements, UElements...>
    constexpr Mutable<Tuple<TElement, TElements...>>
    Tuple<TElement, TElements...>
    ::operator=(Immutable<Tuple<UElements...>> rhs) noexcept
    {
        Records::Copy(*this, rhs);

        return *this;
    }

    template <typename TElement, typename... TElements>
    template <typename... UElements, typename VElements>
    requires
        Details::TupleMoveAssignment<VElements, UElements...>
    constexpr Mutable<Tuple<TElement, TElements...>>
    Tuple<TElement, TElements...>
    ::operator=(Movable<Tuple<UElements...>> rhs) noexcept
    {
        Records::Move(*this, rhs);

        return *this;
    }

    /************************************************************************/
    /* RECORDS                                                              */
    /************************************************************************/

    template <Int TIndex, RecordReference TRecord>
    [[nodiscard]] constexpr decltype(auto)
    Records
    ::Get(Forwarding<TRecord> record) noexcept
    {
        return RecordsADL::InvokeGet<TIndex>(Forward<TRecord>(record));
    }

    template <typename TElement, RecordReference TRecord>
    [[nodiscard]] constexpr decltype(auto)
    Records
    ::Get(Forwarding<TRecord> record) noexcept
    {
        constexpr auto TIndex = ElementIndexOf<TElement, TRecord>;

        return RecordsADL::InvokeGet<TIndex>(Forward<TRecord>(record));
    }

    template <typename TFunction, RecordReference TRecord>
    constexpr decltype(auto)
    Records
    ::Apply(Forwarding<TFunction> function, Forwarding<TRecord> record)
    noexcept
    {
        auto apply = [&]<Int... TIndex> (Templates::Sequence<TIndex...>)
        {
            return function(Records::Get<TIndex>(Forward<TRecord>(record))...);
        };

        return apply(SequenceOf<TRecord>{});
    }

    template <typename TFunction, RecordReference TRecord>
    constexpr void
    Records
    ::ForEachApply(Forwarding<TFunction> function,
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
    Records
    ::ProjectApply(Forwarding<TFunction> function,
                   Forwarding<TRecords>... records) noexcept
    {
        return function(Records::Get<TIndex>(Forward<TRecords>(records))...);
    }

    template <typename TFunction, RecordReference... TRecords>
    constexpr void
    Records
    ::LockstepApply(Forwarding<TFunction> function,
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
    Records
    ::MakeFromRecord(Forwarding<TRecord> record) noexcept
    {
        auto make = [&]<Int... TIndex>(Templates::Sequence<TIndex...>)
        {
            return TType{ Records::Get<TIndex>(Forward<TRecord>(record))... };
        };

        return make(SequenceOf<TRecord>{});
    }

    template <Record TRecord, Record URecord>
    constexpr void
    Records
    ::Copy(Mutable<TRecord> destination, Immutable<URecord> source) noexcept
    {
        static_assert(IsSameRank<TRecord, URecord>,
                      "Both records must have the same rank");

        return PartialCopy(destination, source);
    }

    template <Record TRecord, RecordReference URecord>
    //requires Records::IsSameRank<TRecord, URecord>
    constexpr void
    Records
    ::Move(Mutable<TRecord> destination, Forwarding<URecord> source) noexcept
    {
        static_assert(IsSameRank<TRecord, URecord>,
                      "Both records must have the same rank");

        return PartialMove(destination, Forward<URecord>(source));
    }

    template <Record TRecord, Record URecord>
    constexpr void
    Records
    ::Swap(Mutable<TRecord> lhs, Mutable<URecord> rhs) noexcept
    {
        static_assert(IsSameRank<TRecord, URecord>,
                      "Both records must have the same rank");

        auto swap = [&]<Int... TIndex>(Templates::Sequence<TIndex...>)
        {
            (Support::Swap(Records::Get<TIndex>(lhs),
                              Records::Get<TIndex>(rhs)), ...);
        };

        swap(SequenceOf<TRecord>{});
    }

    template <Record TRecord, RecordReference URecord>
    [[nodiscard]] constexpr TRecord
    Records
    ::Exchange(Mutable<TRecord> lhs, Forwarding<URecord> rhs) noexcept
    {
        static_assert(IsSameRank<TRecord, URecord>,
                      "Both records must have the same rank");

        auto exchange = [&]<Int... TIndex>(Templates::Sequence<TIndex...>)
        {
            return TRecord
            {
                Support::Exchange(Records::Get<TIndex>(lhs),
                                  Records::Get<TIndex>(Forward(rhs)))...
            };
        };

        return exchange(SequenceOf<TRecord>{});
    }

    template <Record TRecord, Record URecord>
    constexpr Int
    Records
    ::PartialCopy(Mutable<TRecord> destination, Immutable<URecord> source)
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

    template <Record TRecord, RecordReference URecord>
    constexpr Int
    Records
    ::PartialMove(Mutable<TRecord> destination, Forwarding<URecord> source)
    noexcept
    {
        auto elementwise_move = [](auto& destination_element,
                                   auto&& source_element)
        {
            using SourceType = decltype(source_element);

            destination_element = Forward<SourceType>(source_element);
        };

        Records::LockstepApply(elementwise_move,
                               destination,
                               Forward<URecord>(source));

        return Math::Min(RankOf<TRecord>, RankOf<URecord>);
    }

    template <Record TRecord, Record URecord>
    constexpr Int
    Records
    ::PartialSwap(Mutable<TRecord> lhs, Mutable<URecord> rhs) noexcept
    {
        constexpr auto kSwapRank = Math::Min(RankOf<TRecord>, RankOf<URecord>);

        auto swap = [&]<Int... TIndex>(Templates::Sequence<TIndex...>)
        {
            (Support::Swap(Records::Get<TIndex>(lhs),
                           Records::Get<TIndex>(rhs)), ...);
        };

        swap(Templates::MakeSequence<kSwapRank>{});

        return kSwapRank;
    }

    template <Record TRecord, Record URecord>
    [[nodiscard]] constexpr Bool
    Records
    ::AreEqual(Immutable<TRecord> lhs, Immutable<URecord> rhs) noexcept
    {
        auto equal = [&]<Int... TIndex>(Templates::Sequence<TIndex...>)
        {
            return (Comparisons::AreEqual(Records::Get<TIndex>(lhs),
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
    Records
    ::AreEquivalent(Immutable<TRecord> lhs, Immutable<URecord> rhs) noexcept
    {
        auto equivalent = [&]<Int... TIndex>(Templates::Sequence<TIndex...>)
        {
            return (Comparisons::AreEquivalent(Records::Get<TIndex>(lhs),
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
    Records
    ::Compare(Immutable<TRecord> lhs, Immutable<URecord> rhs) noexcept
    {
        constexpr auto LeftRank = RankOf<TRecord>;
        constexpr auto RightRank = RankOf<URecord>;
        constexpr auto MinRank = Math::Min<LeftRank, RightRank>;

        auto compare_at = [&]<Int TIndex>(Ordering result)
        {
            if (result == Ordering::kEquivalent)
            {
                return Comparisons::Compare(Records::Get<TIndex>(lhs),
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

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Record.
    // =======

    template <Int TIndex, typename... UElements>
    [[nodiscard]] constexpr decltype(auto)
    Get(Immutable<Tuple<UElements...>> tuple) noexcept
    {
        using BaseType = Details::TupleBase<TIndex, Tuple<UElements...>>;

        return static_cast<Immutable<BaseType>>(tuple).element_;
    }

    template <Int TIndex, typename... UElements>
    [[nodiscard]] constexpr decltype(auto)
    Get(Mutable<Tuple<UElements...>> tuple) noexcept
    {
        using BaseType = Details::TupleBase<TIndex, Tuple<UElements...>>;

        return static_cast<Mutable<BaseType>>(tuple).element_;
    }

    template <Int TIndex, typename... UElements>
    [[nodiscard]] constexpr decltype(auto)
    Get(Immovable<Tuple<UElements...>> tuple) noexcept
    {
        using BaseType = Details::TupleBase<TIndex, Tuple<UElements...>>;

        using ElementType = Records::ElementTypeOf<TIndex,
                                                   Tuple<UElements...>>;

        return static_cast<Immovable<ElementType>>(
                   static_cast<Immutable<BaseType>>(tuple).element_);
    }

    template <Int TIndex, typename... UElements>
    [[nodiscard]] constexpr decltype(auto)
    Get(Movable<Tuple<UElements...>> tuple) noexcept
    {
        using BaseType = Details::TupleBase<TIndex, Tuple<UElements...>>;

        using ElementType = Records::ElementTypeOf<TIndex,
                                                   Tuple<UElements...>>;

        return static_cast<Movable<ElementType>>(
            static_cast<Mutable<BaseType>>(tuple).element_);
    }

    // Utilities.
    // ==========

    template <typename... TElements>
    [[nodiscard]] constexpr Tuple<TElements...>
    MakeTuple(Forwarding<TElements>... elements) noexcept
    {
        return { Forward<TElements>(elements)... };
    }

    template <typename... TElements>
    [[nodiscard]] constexpr Tuple<Mutable<TElements>...>
    Tie(Mutable<TElements>... elements) noexcept
    {
        return Tuple<Mutable<TElements>...>(elements...);
    }

    template <typename... TElements>
    [[nodiscard]] constexpr Tuple<Forwarding<TElements>...>
    ForwardAsTuple(Forwarding<TElements>... elements) noexcept
    {
        return Tuple<Forwarding<TElements>...>(
                   Forward<TElements>(elements)...);
    }

    template <RecordReference... TRecords>
    [[nodiscard]] constexpr auto
    Concatenate(Forwarding<TRecords>... records) noexcept
    {
        auto concatenate = [&]<RecordReference TRecord,
                               Int... TTupleIndex,
                               Int... TElementIndex>
            (Forwarding<TRecord> record,
             Templates::Sequence<TTupleIndex...>,
             Templates::Sequence<TElementIndex...>)
            {
                return MakeTuple(
                           Get<TElementIndex>(
                               Get<TTupleIndex>(Forward<TRecord>(record)))
                           ...);
            };

        return concatenate(
            ForwardAsTuple(records...),
            Details::TupleEnumerateRecords<TRecords...>{},
            Details::TupleEnumerateRecordElements<TRecords...>{});
    }

    template <RecordReference TRecord>
    [[nodiscard]] constexpr auto
    Flatten(Forwarding<TRecord> record) noexcept
    {
        auto flatten = [&]<Int... TIndex>(Forwarding<TRecord> record,
                                          Templates::Sequence<TIndex...>)
        {
            return Concatenate(Flatten(Get<TIndex>(Forward<TRecord>(record)))
                               ...);
        };

        return flatten(Forward<TRecord>(record),
                       Records::SequenceOf<TRecord>{});
    }

    template <typename TElement>
    [[nodiscard]] constexpr decltype(auto)
    Flatten(Forwarding<TElement> element) noexcept
    {
        return MakeTuple(Forward<TElement>(element));
    }

    // Swap.
    // =====

    template <typename... TTypes, typename... UTypes>
    requires (sizeof...(TTypes) == sizeof...(UTypes))
    constexpr void
    Swap(Mutable<Tuple<TTypes...>> lhs,
         Mutable<Tuple<UTypes...>> rhs) noexcept
    {
        Records::Swap(lhs, rhs);
    }

    template <typename... TTypes, typename... UTypes>
    requires (sizeof...(TTypes) == sizeof...(UTypes))
    [[nodiscard]] constexpr Tuple<TTypes...>
    Exchange(Mutable<Tuple<TTypes...>> lhs,
             Forwarding<Tuple<UTypes...>> rhs) noexcept
    {
        return Records::Exchange(lhs, Forward<Tuple<UTypes...>>>(rhs));
    }

    // Comparison.
    // ===========

    template <typename... TTypes, typename... UTypes>
    requires (sizeof...(TTypes) == sizeof...(UTypes))
    [[nodiscard]] constexpr Bool
    operator==(Immutable<Tuple<TTypes...>> lhs,
               Immutable<Tuple<UTypes...>> rhs) noexcept
    {
        return Records::AreEquivalent(lhs, rhs);
    }

    template <typename... TTypes, typename... UTypes>
    requires (sizeof...(TTypes) == sizeof...(UTypes))
    [[nodiscard]] constexpr Ordering
    operator<=>(Immutable<Tuple<TTypes...>> lhs,
                Immutable<Tuple<UTypes...>> rhs) noexcept
    {
        return Records::Compare(lhs, rhs);
    }

}

// ===========================================================================

namespace std
{
    /************************************************************************/
    /* STRUCTURED BINDINGS                                                  */
    /************************************************************************/

    template <Syntropy::Record TRecord>
    struct std::tuple_size<TRecord>
    {
        static constexpr
        std::size_t value = Syntropy::Records::RankOf<TRecord>;
    };

    template <std::size_t TIndex,
              Syntropy::Record TRecord>
    struct std::tuple_element<TIndex, TRecord>
    {
        using type = Syntropy::Records::ElementTypeOf<TIndex, TRecord>;
    };

    template <std::size_t TIndex, Syntropy::Record TRecord>
    decltype(auto) get(Syntropy::Immutable<TRecord> record)
    {
        return Get<TIndex>(record);
    }

    template <std::size_t TIndex, Syntropy::Record TRecord>
    decltype(auto) get(Syntropy::Movable<TRecord> record)
    {
        return Get<TIndex>(Syntropy::Move(record));
    }
}

// ===========================================================================
