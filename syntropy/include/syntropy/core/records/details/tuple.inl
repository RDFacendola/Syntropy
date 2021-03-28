
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

    template <Int TIndex, typename... TElements>
    [[nodiscard]] constexpr
    Immutable<Templates::PackElementOf<TIndex, TElements...>>
    Get(Immutable<Tuple<TElements...>> tuple) noexcept
    {
        using TupleType = Tuple<TElements...>;

        using TupleBaseType = Details::TupleBase<TIndex, TupleType>;

        return static_cast<Immutable<TupleBaseType>>(tuple).element_;
    }

    template <Int TIndex, typename... TElements>
    [[nodiscard]] constexpr
    Mutable<Templates::PackElementOf<TIndex, TElements...>>
    Get(Mutable<Tuple<TElements...>> tuple) noexcept
    {
        using TupleType = Tuple<TElements...>;

        using TupleBaseType = Details::TupleBase<TIndex, TupleType>;

        return static_cast<Mutable<TupleBaseType>>(tuple).element_;
    }

    template <Int TIndex, typename... TElements>
    [[nodiscard]] constexpr
    Immovable<Templates::PackElementOf<TIndex, TElements...>>
    Get(Immovable<Tuple<TElements...>> tuple) noexcept
    {
        using TupleType = Tuple<TElements...>;

        using TupleBaseType = Details::TupleBase<TIndex, TupleType>;

        using ElementTypeList = Templates::TypeList<TElements...>;

        using ElementType = Templates::ElementOf<TIndex, ElementTypeList>;

        return static_cast<Immovable<ElementType>>(
                   static_cast<Immutable<TupleBaseType>>(tuple).element_);
    }

    template <Int TIndex, typename... TElements>
    [[nodiscard]] constexpr
    Movable<Templates::PackElementOf<TIndex, TElements...>>
    Get(Movable<Tuple<TElements...>> tuple) noexcept
    {
        using TupleType = Tuple<TElements...>;

        using TupleBaseType = Details::TupleBase<TIndex, TupleType>;

        using ElementTypeList = Templates::TypeList<TElements...>;

        using ElementType = Templates::ElementOf<TIndex, ElementTypeList>;

        return static_cast<Movable<ElementType>>(
            static_cast<Mutable<TupleBaseType>>(tuple).element_);
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

    template <typename... TElements>
    constexpr void
    Swap(Mutable<Tuple<TElements...>> lhs,
         Mutable<Tuple<TElements...>> rhs) noexcept
    {
        return Records::Swap(lhs, rhs);
    }

    template <typename... TElements, typename... UElements>
    requires (sizeof...(TElements) == sizeof...(UElements))
    [[nodiscard]] constexpr Tuple<TElements...>
    Exchange(Mutable<Tuple<TElements...>> lhs,
             Forwarding<Tuple<UElements...>> rhs) noexcept
    {
        return Records::Exchange(lhs, Forward<Tuple<UElements...>>(rhs));
    }

    // Comparison.
    // ===========

    template <typename... TElements, typename... UElements>
    [[nodiscard]] constexpr Bool
    operator==(Immutable<Tuple<TElements...>> lhs,
               Immutable<Tuple<UElements...>> rhs) noexcept
    {
        return Records::AreEquivalent(lhs, rhs);
    }

    template <typename... TElements, typename... UElements>
    [[nodiscard]] constexpr Ordering
    operator<=>(Immutable<Tuple<TElements...>> lhs,
                Immutable<Tuple<UElements...>> rhs) noexcept
    {
        return Records::Compare(lhs, rhs);
    }

    template <typename... TElements, typename... UElements>
    requires (sizeof...(TElements) == sizeof...(UElements))
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<Tuple<TElements...>> lhs,
             Immutable<Tuple<UElements...>> rhs) noexcept
    {
        return Records::AreEqual(lhs, rhs);
    }

    template <typename... TElements, typename... UElements>
    requires (sizeof...(TElements) == sizeof...(UElements))
    [[nodiscard]] constexpr Bool
    AreEquivalent(Immutable<Tuple<TElements...>> lhs,
                  Immutable<Tuple<UElements...>> rhs) noexcept
    {
        return Records::AreEquivalent(lhs, rhs);
    }

    template <typename... TElements, typename... UElements>
    requires (sizeof...(TElements) == sizeof...(UElements))
    [[nodiscard]] constexpr Ordering
    Compare(Immutable<Tuple<TElements...>> lhs,
            Immutable<Tuple<UElements...>> rhs) noexcept
    {
        return Records::Compare(lhs, rhs);
    }

}

// ===========================================================================

#include "record.details.inl"

// ===========================================================================
