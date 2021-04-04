
    /// \file tuple.inl
///
/// \author Raffaele D. Facendola - Sep 2020.

#pragma once

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
        : Tuple<TElements...>(Forward<UElements>(elements)...)
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
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Record.
    // =======

    template <Int TIndex, IsTuple TTuple>
    [[nodiscard]] constexpr Immutable<Records::ElementTypeOf<TIndex, TTuple>>
    Get(Immutable<TTuple> tuple) noexcept
    {
        using BaseType = Details::TupleBase<TIndex, TTuple>;

        return static_cast<Immutable<BaseType>>(tuple).element_;
    }

    template <Int TIndex, IsTuple TTuple>
    [[nodiscard]] constexpr Mutable<Records::ElementTypeOf<TIndex, TTuple>>
    Get(Mutable<TTuple> tuple) noexcept
    {
        using BaseType = Details::TupleBase<TIndex, TTuple>;

        return static_cast<Mutable<BaseType>>(tuple).element_;
    }

    template <Int TIndex, IsTuple TTuple>
    [[nodiscard]] constexpr Immovable<Records::ElementTypeOf<TIndex, TTuple>>
    Get(Immovable<TTuple> tuple) noexcept
    {
        using BaseType = Details::TupleBase<TIndex, TTuple>;

        using ElementType = Records::ElementTypeOf<TIndex, TTuple>;

        return static_cast<Immovable<ElementType>>(
                   static_cast<Immutable<BaseType>>(tuple).element_);
    }

    template <Int TIndex, IsTuple TTuple>
    [[nodiscard]] constexpr Movable<Records::ElementTypeOf<TIndex, TTuple>>
    Get(Movable<TTuple> tuple) noexcept
    {
        using BaseType = Details::TupleBase<TIndex, TTuple>;

        using ElementType = Records::ElementTypeOf<TIndex, TTuple>;

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

    template <Records::RecordReference... TRecords>
    [[nodiscard]] constexpr auto
    Concatenate(Forwarding<TRecords>... records) noexcept
    {
        auto concatenate = [&]<Records::RecordReference TRecord,
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

    template <Records::RecordReference TRecord>
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

    template <IsTuple TTuple, IsTuple UTuple>
    requires Records::IsSameRank<TTuple, UTuple>
    constexpr void
    Swap(Mutable<TTuple> lhs, Mutable<UTuple> rhs) noexcept
    {
        Records::Swap(lhs, rhs);
    }

    template <IsTuple TTuple, IsTuple UTuple>
    requires Records::IsSameRank<TTuple, UTuple>
    [[nodiscard]] constexpr TTuple
    Exchange(Mutable<TTuple> lhs, Forwarding<UTuple> rhs) noexcept
    {
        return Records::Exchange(lhs, Forward<UTuple>>(rhs));
    }

    // Comparison.
    // ===========

    template <IsTuple TTuple, IsTuple UTuple>
    requires Records::IsSameRank<TTuple, UTuple>
    [[nodiscard]] constexpr Bool
    operator==(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept
    {
        return Records::AreEquivalent(lhs, rhs);
    }

    template <IsTuple TTuple, IsTuple UTuple>
    requires Records::IsSameRank<TTuple, UTuple>
    [[nodiscard]] constexpr Ordering
    operator<=>(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept
    {
        return Records::Compare(lhs, rhs);
    }

}

// ===========================================================================

#include "record.details.inl"

// ===========================================================================
