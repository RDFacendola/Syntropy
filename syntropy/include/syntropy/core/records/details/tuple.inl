
    /// \file tuple.inl
///
/// \author Raffaele D. Facendola - Sep 2020.

#pragma once

// ===========================================================================

namespace Syntropy::Records
{
    /************************************************************************/
    /* TUPLE                                                                */
    /************************************************************************/

    template <typename TElement, typename... TElements>
    template <typename UElement, typename... UElements>
    constexpr Tuple<TElement, TElements...>
    ::Tuple(ElementwiseTag,            Forwarding<UElement> element,
            Forwarding<UElements>... elements) noexcept
        : BaseClass(Forward<UElements>(elements)...)
        , element_(Forward<UElement>(element))
    {

    }

    template <typename TElement, typename... TElements>
    template<Concepts::ForwardingRecord TTuple, Int... VIndexes>
    constexpr Tuple<TElement, TElements...>
    ::Tuple(UnwindTag,
            Syntropy::Templates::Sequence<VIndexes...>,
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
    ::operator=(Syntropy::Templates::ExactOf<Immutable<TSelf>> rhs) noexcept
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
                Syntropy::Templates::TypeList<UElements...>>>
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
    ::operator=(Syntropy::Templates::ExactOf<Movable<TSelf>> rhs) noexcept
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
                Syntropy::Templates::TypeList<UElements...>>>
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

    // Comparison.
    // ===========

    template <typename... TElements, typename... UElements>
    [[nodiscard]] constexpr Bool
    operator==(Immutable<Tuple<TElements...>> lhs,
               Immutable<Tuple<UElements...>> rhs) noexcept
    {
        using namespace Records;

        return AreEquivalent(lhs, rhs);
    }

    template <typename... TElements, typename... UElements>
    [[nodiscard]] constexpr Ordering
    operator<=>(Immutable<Tuple<TElements...>> lhs,
                Immutable<Tuple<UElements...>> rhs) noexcept
    {
        using namespace Records;

        return Compare(lhs, rhs);
    }

    // Record.
    // =======

    template <Int VIndex, typename... TElements>
    [[nodiscard]] constexpr
    Immutable<Templates::ElementTypeOf<VIndex, Tuple<TElements...>>>
    Get(Immutable<Tuple<TElements...>> tuple) noexcept
    {
        using TTupleBase = Details::TupleBase<VIndex, Tuple<TElements...>>;

        return static_cast<Immutable<TTupleBase>>(tuple).element_;
    }

    template <Int VIndex, typename... TElements>
    [[nodiscard]] constexpr
    Mutable<Templates::ElementTypeOf<VIndex, Tuple<TElements...>>>
    Get(Mutable<Tuple<TElements...>> tuple) noexcept
    {
        using TTupleBase = Details::TupleBase<VIndex, Tuple<TElements...>>;

        return static_cast<Mutable<TTupleBase>>(tuple).element_;
    }

    template <Int VIndex, typename... TElements>
    [[nodiscard]] constexpr
    Immovable<Templates::ElementTypeOf<VIndex, Tuple<TElements...>>>
    Get(Immovable<Tuple<TElements...>> tuple) noexcept
    {
        using TTupleBase = Details::TupleBase<VIndex, Tuple<TElements...>>;
        using TElement = Templates::ElementTypeOf<VIndex, Tuple<TElements...>>;

        return static_cast<Immovable<TElement>>(
                   static_cast<Immutable<TTupleBase>>(tuple).element_);
    }

    template <Int VIndex, typename... TElements>
    [[nodiscard]] constexpr
    Movable<Templates::ElementTypeOf<VIndex, Tuple<TElements...>>>
    Get(Movable<Tuple<TElements...>> tuple) noexcept
    {
        using TTupleBase = Details::TupleBase<VIndex, Tuple<TElements...>>;
        using TElement = Templates::ElementTypeOf<VIndex, Tuple<TElements...>>;

        return static_cast<Movable<TElement>>(
            static_cast<Mutable<TTupleBase>>(tuple).element_);
    }

    template <typename TElement, typename... TElements>
    [[nodiscard]] constexpr Immutable<TElement>
    Get(Immutable<Tuple<TElements...>> tuple) noexcept
    {
        constexpr auto kIndex = Syntropy::Templates::IndexOf<
            TElement,
            Syntropy::Templates::TypeList<TElements...>>;

        return Get<kIndex>(tuple);
    }

    template <typename TElement, typename... TElements>
    [[nodiscard]] constexpr Mutable<TElement>
    Get(Mutable<Tuple<TElements...>> tuple) noexcept
    {
        constexpr auto kIndex = Syntropy::Templates::IndexOf<
            TElement,
            Syntropy::Templates::TypeList<TElements...>>;

        return Get<kIndex>(tuple);
    }

    template <typename TElement, typename... TElements>
    [[nodiscard]] constexpr Immovable<TElement>
    Get(Immovable<Tuple<TElements...>> tuple) noexcept
    {
        constexpr auto kIndex = Syntropy::Templates::IndexOf<
            TElement,
            Syntropy::Templates::TypeList<TElements...>>;

        return Get<kIndex>(Move(tuple));
    }

    template <typename TElement, typename... TElements>
    [[nodiscard]] constexpr Movable<TElement>
    Get(Movable<Tuple<TElements...>> tuple) noexcept
    {
        constexpr auto kIndex = Syntropy::Templates::IndexOf<
            TElement,
            Syntropy::Templates::TypeList<TElements...>>;

        return Get<kIndex>(Move(tuple));
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

    template <Concepts::ForwardingRecord... TRecords>
    [[nodiscard]] constexpr decltype(auto)
    Concatenate(Forwarding<TRecords>... tuples) noexcept
    {
        return Details::Concatenate(Forward<TRecords>(tuples)...);
    }

    template <Concepts::ForwardingRecord TTuple>
    [[nodiscard]] constexpr decltype(auto)
    Flatten(Forwarding<TTuple> tuple) noexcept
    {
        return Details::Flatten(Forward<TTuple>(tuple));
    }

    // Swap.
    // =====

    template <typename... TElements, typename... UElements>
    requires (sizeof...(TElements) == sizeof...(UElements))
    constexpr void
    Swap(Mutable<Tuple<TElements...>> lhs,
         Mutable<Tuple<UElements...>> rhs) noexcept
    {
        Records::Swap(lhs, rhs);
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
