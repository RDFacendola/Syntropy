
/// \file tuple.inl
///
/// \author Raffaele D. Facendola - September 2020.

// ===========================================================================

#include "syntropy/math/math.h"
#include "syntropy/language/support/swap.h"

// ===========================================================================

namespace Syntropy::TuplesADL
{
    /************************************************************************/
    /* TUPLES EXTENSIONS                                                    */
    /************************************************************************/

    /// \brief Type alias for the Get extension functor.
    template <Int TIndex, typename TTuple>
    using GetExtension
        = Tuples::Extensions::Get<TIndex, TTuple>;

    /// \brief Invoke the Get function via extension functor.
    template <Int TIndex, typename TTuple>
    [[nodiscard]] constexpr auto
    InvokeGetExtension(Forwarding<TTuple> rhs)
        noexcept -> decltype(GetExtension<TIndex, TTuple>{}(
            Forward<TTuple>(rhs)))
    {
        return GetExtension<TIndex, TTuple>{}(Forward<TTuple>(rhs));
    }

    /************************************************************************/
    /* TUPLES                                                               */
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

    // In order to concatenate tuples together, two sequences with the same
    // total size are generated.
    // The first sequence is used to associate each element in the resulting
    // sequence with the index of the tuple the element is taken from.
    // The second sequence is used to associate each element in the resulting
    // sequence with the index of the element in the owning tuple.

    // Example: A {a,b,c}, B {d,e}, C {f}
    // First sequence:  000-11-2 AAA-BB-C
    // Second sequence: 012-01-0 abc-de-f

    /// \brief Sequence associating each element to the generating tuple.
    template <typename TSequence, typename... TTuples>
    struct EnumerateTuplesHelper {};

    /// \brief Partial template specialization for sequence-tuples pairs.
    template <Int... TIndex, typename... TTuples>
    struct EnumerateTuplesHelper<Templates::Sequence<TIndex...>, TTuples...>
        : Templates::Alias<
            Templates::SequenceConcatenate<
                Templates::SequenceRepeat<TIndex,
                                          TupleRankOf<TTuples>>...>> {};

    /// \brief Sequence associating each element to the source tuple.
    template <typename... TTuples>
    using EnumerateTuples
        = typename EnumerateTuplesHelper<
            Templates::SequenceFor<TTuples...>, TTuples...>::Type;

    /// \brief Sequence associating each element to the source tuple element.
    template <typename... TTuples>
    using EnumerateTupleElements
        = Templates::SequenceConcatenate<Tuples::SequenceOf<TTuples>...>;
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
        Tuples::Copy(*this, rhs);

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
        Tuples::Move(*this, rhs);

        return *this;
    }

    /************************************************************************/
    /* TUPLES                                                               */
    /************************************************************************/

    template <Int TIndex, IsTupleReference TTuple>
    [[nodiscard]] constexpr decltype(auto)
    Tuples
    ::Get(Forwarding<TTuple> tuple) noexcept
    {
        return TuplesADL::InvokeGet<TIndex>(Forward<TTuple>(tuple));
    }

    template <typename TElement, IsTupleReference TTuple>
    [[nodiscard]] constexpr decltype(auto)
    Tuples
    ::Get(Forwarding<TTuple> tuple) noexcept
    {
        constexpr auto TIndex = ElementIndexOf<TElement, TTuple>;

        return TuplesADL::InvokeGet<TIndex>(Forward<TTuple>(tuple));
    }

    template <typename TFunction, IsTupleReference TTuple>
    constexpr decltype(auto)
    Tuples
    ::Apply(Forwarding<TFunction> function, Forwarding<TTuple> tuple)
    noexcept
    {
        auto apply = [&]<Int... TIndex> (Templates::Sequence<TIndex...>)
        {
            return function(Tuples::Get<TIndex>(Forward<TTuple>(tuple))...);
        };

        return apply(SequenceOf<TTuple>{});
    }

    template <typename TFunction, IsTupleReference TTuple>
    constexpr void
    Tuples
    ::ForEachApply(Forwarding<TFunction> function,
                   Forwarding<TTuple> tuple) noexcept
    {
        auto apply = [&]<Int... TIndex> (Templates::Sequence<TIndex...>)
        {
            (function(Tuples::Get<TIndex>(Forward<TTuple>(tuple))), ...);
        };

        apply(SequenceOf<TTuple>{});
    }

    template <Int TIndex, typename TFunction, IsTupleReference... TTuples>
    constexpr decltype(auto)
    Tuples
    ::ProjectApply(Forwarding<TFunction> function,
                   Forwarding<TTuples>... tuples) noexcept
    {
        return function(Tuples::Get<TIndex>(Forward<TTuples>(tuples))...);
    }

    template <typename TFunction, IsTupleReference... TTuples>
    constexpr void
    Tuples
    ::LockstepApply(Forwarding<TFunction> function,
                    Forwarding<TTuples>... tuples) noexcept
    {
        constexpr auto kMinRank = Math::Min(TupleRankOf<TTuples>...);

        auto apply = [&]<Int... TIndex>(Templates::Sequence<TIndex...>)
        {
            (ProjectApply<TIndex>(Forward<TFunction>(function), tuples...),
                                  ...);
        };

        apply(Templates::MakeSequence<kMinRank>{});
    }

    template <typename TType, IsTupleReference TTuple>
    [[nodiscard]] constexpr TType
    Tuples
    ::MakeFromTuple(Forwarding<TTuple> tuple) noexcept
    {
        auto make = [&]<Int... TIndex>(Templates::Sequence<TIndex...>)
        {
            return TType{ Tuples::Get<TIndex>(Forward<TTuple>(tuple))... };
        };

        return make(SequenceOf<TTuple>{});
    }

    template <IsTuple TTuple, IsTuple UTuple>
    constexpr void
    Tuples
    ::Copy(Mutable<TTuple> destination, Immutable<UTuple> source) noexcept
    {
        static_assert(IsSameRank<TTuple, UTuple>,
                      "Both tuples must have the same rank");

        return PartialCopy(destination, source);
    }

    template <IsTuple TTuple, IsTupleReference UTuple>
    //requires Tuples::IsSameRank<TTuple, UTuple>
    constexpr void
    Tuples
    ::Move(Mutable<TTuple> destination, Forwarding<UTuple> source) noexcept
    {
        static_assert(IsSameRank<TTuple, UTuple>,
                      "Both tuples must have the same rank");

        return PartialMove(destination, Forward<UTuple>(source));
    }

    template <IsTuple TTuple, IsTuple UTuple>
    constexpr void
    Tuples
    ::Swap(Mutable<TTuple> lhs, Mutable<UTuple> rhs) noexcept
    {
        static_assert(IsSameRank<TTuple, UTuple>,
                      "Both tuples must have the same rank");

        auto swap = [&]<Int... TIndex>(Templates::Sequence<TIndex...>)
        {
            (Support::Swap(Tuples::Get<TIndex>(lhs),
                              Tuples::Get<TIndex>(rhs)), ...);
        };

        swap(SequenceOf<TTuple>{});
    }

    template <IsTuple TTuple, IsTupleReference UTuple>
    [[nodiscard]] constexpr TTuple
    Tuples
    ::Exchange(Mutable<TTuple> lhs, Forwarding<UTuple> rhs) noexcept
    {
        static_assert(IsSameRank<TTuple, UTuple>,
                      "Both tuples must have the same rank");

        auto exchange = [&]<Int... TIndex>(Templates::Sequence<TIndex...>)
        {
            return TTuple
            {
                Support::Exchange(Tuples::Get<TIndex>(lhs),
                                  Tuples::Get<TIndex>(Forward(rhs)))...
            };
        };

        return exchange(SequenceOf<TTuple>{});
    }

    template <IsTuple TTuple, IsTuple UTuple>
    constexpr Int
    Tuples
    ::PartialCopy(Mutable<TTuple> destination, Immutable<UTuple> source)
    noexcept
    {
        auto elementwise_copy = [](auto& destination_element,
                                   auto&& source_element)
        {
            destination_element = Copy(source_element);
        };

        Tuples::LockstepApply(elementwise_copy, destination, source);

        return Math::Min(TupleRankOf<TTuple>, TupleRankOf<UTuple>);
    }

    template <IsTuple TTuple, IsTupleReference UTuple>
    constexpr Int
    Tuples
    ::PartialMove(Mutable<TTuple> destination, Forwarding<UTuple> source)
    noexcept
    {
        auto elementwise_move = [](auto& destination_element,
                                   auto&& source_element)
        {
            using SourceType = decltype(source_element);

            destination_element = Forward<SourceType>(source_element);
        };

        Tuples::LockstepApply(elementwise_move,
                               destination,
                               Forward<UTuple>(source));

        return Math::Min(TupleRankOf<TTuple>, TupleRankOf<UTuple>);
    }

    template <IsTuple TTuple, IsTuple UTuple>
    constexpr Int
    Tuples
    ::PartialSwap(Mutable<TTuple> lhs, Mutable<UTuple> rhs) noexcept
    {
        constexpr auto kSwapRank = Math::Min(TupleRankOf<TTuple>, TupleRankOf<UTuple>);

        auto swap = [&]<Int... TIndex>(Templates::Sequence<TIndex...>)
        {
            (Support::Swap(Tuples::Get<TIndex>(lhs),
                           Tuples::Get<TIndex>(rhs)), ...);
        };

        swap(Templates::MakeSequence<kSwapRank>{});

        return kSwapRank;
    }

    template <IsTuple TTuple, IsTuple UTuple>
    [[nodiscard]] constexpr Bool
    Tuples
    ::AreEqual(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept
    {
        auto equal = [&]<Int... TIndex>(Templates::Sequence<TIndex...>)
        {
            return (Comparisons::AreEqual(Tuples::Get<TIndex>(lhs),
                                          Tuples::Get<TIndex>(rhs))
                    && ...);
        };

        if constexpr (IsSameRank<TTuple, UTuple>)
        {
            return equal(SequenceOf<TTuple>{});
        }

        return false;
    }

    template <IsTuple TTuple, IsTuple UTuple>
    [[nodiscard]] constexpr Bool
    Tuples
    ::AreEquivalent(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept
    {
        auto equivalent = [&]<Int... TIndex>(Templates::Sequence<TIndex...>)
        {
            return (Comparisons::AreEquivalent(Tuples::Get<TIndex>(lhs),
                                               Tuples::Get<TIndex>(rhs))
                    && ...);
        };

        if constexpr (IsSameRank<TTuple, UTuple>)
        {
            return equivalent(SequenceOf<TTuple>{});
        }

        return false;
    }

    template <IsTuple TTuple, IsTuple UTuple>
    [[nodiscard]] constexpr Ordering
    Tuples
    ::Compare(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept
    {
        constexpr auto LeftRank = TupleRankOf<TTuple>;
        constexpr auto RightRank = TupleRankOf<UTuple>;
        constexpr auto MinRank = Math::Min<LeftRank, RightRank>;

        auto compare_at = [&]<Int TIndex>(Ordering result)
        {
            if (result == Ordering::kEquivalent)
            {
                return Comparisons::Compare(Tuples::Get<TIndex>(lhs),
                                            Tuples::Get<TIndex>(rhs));
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

    // IsTuple.
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

        using ElementType = TupleElementTypeOf<TIndex,
                                                   Tuple<UElements...>>;

        return static_cast<Immovable<ElementType>>(
                   static_cast<Immutable<BaseType>>(tuple).element_);
    }

    template <Int TIndex, typename... UElements>
    [[nodiscard]] constexpr decltype(auto)
    Get(Movable<Tuple<UElements...>> tuple) noexcept
    {
        using BaseType = Details::TupleBase<TIndex, Tuple<UElements...>>;

        using ElementType = TupleElementTypeOf<TIndex,
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

    template <IsTupleReference... TTuples>
    [[nodiscard]] constexpr auto
    Concatenate(Forwarding<TTuples>... tuples) noexcept
    {
        auto concatenate = [&]<IsTupleReference TTuple,
                               Int... TTupleIndex,
                               Int... TElementIndex>
            (Forwarding<TTuple> tuple,
             Templates::Sequence<TTupleIndex...>,
             Templates::Sequence<TElementIndex...>)
            {
                return MakeTuple(
                           Get<TElementIndex>(
                               Get<TTupleIndex>(Forward<TTuple>(tuple)))
                           ...);
            };

        return concatenate(
            ForwardAsTuple(tuples...),
            Details::EnumerateTuples<TTuples...>{},
            Details::EnumerateTupleElements<TTuples...>{});
    }

    template <IsTupleReference TTuple>
    [[nodiscard]] constexpr auto
    Flatten(Forwarding<TTuple> tuple) noexcept
    {
        auto flatten = [&]<Int... TIndex>(Forwarding<TTuple> tuple,
                                          Templates::Sequence<TIndex...>)
        {
            return Concatenate(Flatten(Get<TIndex>(Forward<TTuple>(tuple)))
                               ...);
        };

        return flatten(Forward<TTuple>(tuple),
                       Tuples::SequenceOf<TTuple>{});
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
        Tuples::Swap(lhs, rhs);
    }

    template <typename... TTypes, typename... UTypes>
    requires (sizeof...(TTypes) == sizeof...(UTypes))
    [[nodiscard]] constexpr Tuple<TTypes...>
    Exchange(Mutable<Tuple<TTypes...>> lhs,
             Forwarding<Tuple<UTypes...>> rhs) noexcept
    {
        return Tuples::Exchange(lhs, Forward<Tuple<UTypes...>>>(rhs));
    }

    // Comparison.
    // ===========

    template <typename... TTypes, typename... UTypes>
    requires (sizeof...(TTypes) == sizeof...(UTypes))
    [[nodiscard]] constexpr Bool
    operator==(Immutable<Tuple<TTypes...>> lhs,
               Immutable<Tuple<UTypes...>> rhs) noexcept
    {
        return Tuples::AreEquivalent(lhs, rhs);
    }

    template <typename... TTypes, typename... UTypes>
    requires (sizeof...(TTypes) == sizeof...(UTypes))
    [[nodiscard]] constexpr Ordering
    operator<=>(Immutable<Tuple<TTypes...>> lhs,
                Immutable<Tuple<UTypes...>> rhs) noexcept
    {
        return Tuples::Compare(lhs, rhs);
    }

}

// ===========================================================================

namespace std
{
    /************************************************************************/
    /* STRUCTURED BINDINGS                                                  */
    /************************************************************************/

    template <Syntropy::IsTuple TTuple>
    struct std::tuple_size<TTuple>
    {
        static constexpr
        std::size_t value = Syntropy::TupleRankOf<TTuple>;
    };

    template <std::size_t TIndex,
              Syntropy::IsTuple TTuple>
    struct std::tuple_element<TIndex, TTuple>
    {
        using type = Syntropy::TupleElementTypeOf<TIndex, TTuple>;
    };

    template <std::size_t TIndex, Syntropy::IsTuple TTuple>
    decltype(auto) get(Syntropy::Immutable<TTuple> tuple)
    {
        return Get<TIndex>(tuple);
    }

    template <std::size_t TIndex, Syntropy::IsTuple TTuple>
    decltype(auto) get(Syntropy::Movable<TTuple> tuple)
    {
        return Get<TIndex>(Syntropy::Move(tuple));
    }
}

// ===========================================================================
