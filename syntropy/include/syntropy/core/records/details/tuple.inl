
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
    template <typename UElement, typename... UElements>
    constexpr Tuple<TElement, TElements...>
    ::Tuple(ElementwiseTag,
            Forwarding<UElement> element,
            Forwarding<UElements>... elements) noexcept
        : BaseClass(Forward<UElements>(elements)...)
        , element_(Forward<UElement>(element))
    {

    }

    template <typename TElement, typename... TElements>
    template<Records::RecordReference TTuple, Int... VIndexes>
    constexpr Tuple<TElement, TElements...>
    ::Tuple(UnwindTag,
            Templates::Sequence<VIndexes...>,
            Forwarding<TTuple> tuple) noexcept
        : Tuple(ElementwiseTag{}, Get<VIndexes>(Forward<TTuple>(tuple))...)
    {

    }

    template <typename TElement, typename... TElements>
    template <typename TSelf,
              typename TSelfList,
              Details::EnableIfTupleCopyAssignment<TSelfList>>
    constexpr
    Mutable<Tuple<TElement, TElements...>> Tuple<TElement, TElements...>
    ::operator=(Templates::ExactOf<Immutable<TSelf>> rhs) noexcept
    {
        Records::LockstepApply([](auto& lhs_element, const auto& rhs_element)
        {
            lhs_element = rhs_element;
        }, *this, rhs);

        return *this;
    }

    template <typename TElement, typename... TElements>
    template <typename... UElements,
              typename TSelfList,
              Details::EnableIfTupleConvertingCopyAssignment<
                TSelfList,
                Templates::TypeList<UElements...>>>
    constexpr
    Mutable<Tuple<TElement, TElements...>> Tuple<TElement, TElements...>
    ::operator=(Immutable<Tuple<UElements...>> rhs) noexcept
    {
        Records::LockstepApply([](auto& lhs_element, const auto& rhs_element)
        {
            lhs_element = rhs_element;
        }, *this, rhs);

        return *this;
    }

    template <typename TElement, typename... TElements>
    template <typename TSelf,
              typename TSelfList,
              Details::EnableIfTupleMoveAssignment<TSelfList>>
    constexpr Mutable<Tuple<TElement, TElements...>>
    Tuple<TElement, TElements...>
    ::operator=(Templates::ExactOf<Movable<TSelf>> rhs) noexcept
    {
        Records::LockstepApply([](auto& lhs_element, auto&& rhs_element)
        {
            lhs_element = Move(rhs_element);
        }, *this, rhs);

        return *this;
    }

    template <typename TElement, typename... TElements>
    template <typename... UElements,
              typename TSelfList,
              Details::EnableIfTupleConvertingMoveAssignment<
                  TSelfList,
                  Templates::TypeList<UElements...>>>
    constexpr Mutable<Tuple<TElement, TElements...>>
    Tuple<TElement, TElements...>
    ::operator=(Movable<Tuple<UElements...>> rhs) noexcept
    {
        Records::LockstepApply([](auto& lhs_element, auto&& rhs_element)
        {
            lhs_element = Move(rhs_element);
        }, *this, rhs);

        return *this;
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Record.
    // =======

    template <Int TIndex, IsTuple TTuple>
    [[nodiscard]] constexpr Immutable<Templates::ElementOf<TIndex, TTuple>>
    Get(Immutable<TTuple> tuple) noexcept
    {
        using BaseType = Details::TupleBase<TIndex, TTuple>;

        return static_cast<Immutable<BaseType>>(tuple).element_;
    }

    template <Int TIndex, IsTuple TTuple>
    [[nodiscard]] constexpr Mutable<Templates::ElementOf<TIndex, TTuple>>
    Get(Mutable<TTuple> tuple) noexcept
    {
        using BaseType = Details::TupleBase<TIndex, TTuple>;

        return static_cast<Mutable<BaseType>>(tuple).element_;
    }

    template <Int TIndex, IsTuple TTuple>
    [[nodiscard]] constexpr Immovable<Templates::ElementOf<TIndex, TTuple>>
    Get(Immovable<TTuple> tuple) noexcept
    {
        using BaseType = Details::TupleBase<TIndex, TTuple>;

        using ElementType = ElementOf<TIndex, TTuple>;

        return static_cast<Immovable<ElementType>>(
                   static_cast<Immutable<BaseType>>(tuple).element_);
    }

    template <Int TIndex, IsTuple TTuple>
    [[nodiscard]] constexpr Movable<Templates::ElementOf<TIndex, TTuple>>
    Get(Movable<TTuple> tuple) noexcept
    {
        using BaseType = Details::TupleBase<TIndex, TTuple>;

        using ElementType = ElementOf<TIndex, TTuple>;

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
    Concatenate(Forwarding<TRecords>... tuples) noexcept
        -> decltype(Details::Concatenate(Forward<TRecords>(tuples)...))
    {
        return Details::Concatenate(Forward<TRecords>(tuples)...);
    }

    template <Records::RecordReference TTuple>
    [[nodiscard]] constexpr auto
    Flatten(Forwarding<TTuple> tuple) noexcept
        -> decltype(Details::Flatten(Forward<TTuple>(tuple)))
    {
        return Details::Flatten(Forward<TTuple>(tuple));
    }

    // Swap.
    // =====

    template <IsTuple TTuple>
    constexpr void
    Swap(Mutable<TTuple> lhs, Mutable<TTuple> rhs) noexcept
    {
        return Records::Swap(lhs, rhs);
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

    template <IsTuple TTuple, IsTuple UTuple>
    requires Records::IsSameRank<TTuple, UTuple>
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept
    {
        return Records::AreEqual(lhs, rhs);
    }

    template <IsTuple TTuple, IsTuple UTuple>
    requires Records::IsSameRank<TTuple, UTuple>
    [[nodiscard]] constexpr Bool
    AreEquivalent(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept
    {
        return Records::AreEquivalent(lhs, rhs);
    }

    template <IsTuple TTuple, IsTuple UTuple>
    requires Records::IsSameRank<TTuple, UTuple>
    [[nodiscard]] constexpr Ordering
    Compare(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept
    {
        return Records::Compare(lhs, rhs);
    }

}

// ===========================================================================

#include "record.details.inl"

// ===========================================================================
