/// \file tuple.h
/// \brief This header is part of Syntropy core module. It contains definitions for tuples.
///
/// \author Raffaele D. Facendola - Sep 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/templates/sequence.h"
#include "syntropy/language/support/swap.h"

#include "syntropy/core/foundation/details/tuple_details.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* TUPLE                                                                */
    /************************************************************************/

    // !IMPORTANT! In VS2019 apparently "explicit" keyword is not considered when 
    //             constructors are not declared and defined at the same time.

     /// \brief Represents a fixed-size collection of heterogeneous elements.
     /// \author Raffaele D. Facendola - September 2020.
    template <typename... TElements>
    struct Tuple;

    /// \brief Recursive tuple definition.
    template <typename TElement, typename... TElements>
    struct Tuple<TElement, TElements...> : private Tuple<TElements...>
    {
        template <Int VIndex, typename... TElements>
        friend constexpr MutableRef<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(MutableRef<Tuple<TElements...>> tuple) noexcept;

        template <Int VIndex, typename... TElements>
        friend constexpr MoveRef<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(MoveRef<Tuple<TElements...>> tuple) noexcept;

        template <Int VIndex, typename... TElements>
        friend constexpr Ref<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(Ref<Tuple<TElements...>> tuple) noexcept;

        template <Int VIndex, typename... TElements>
        friend constexpr ConstMoveRef<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(ConstMoveRef<Tuple<TElements...>> tuple) noexcept;

    public:

        /// \brief Type of the base class.
        using BaseClass = Tuple<TElements...>;

        /// \brief Types of tuple elements.
        using ElementList = Templates::TypeList<TElement, TElements...>;

        /// \brief Types of arguments.
        template <typename... UElements>
        using ArgumentList = Templates::TypeList<UElements...>;

        /// \brief Tag type used to construct a tuple element-wise.
        struct ElementwiseTag {};

        /// \brief Tag type used to construct a tuple by unwinding another tuple.
        struct UnwindTag {};

        /// \brief Tuple default constructor. Enabled if all elements are default-constructible.
        template<typename UElement = TElement, Details::EnableIfTupleDefaultConstructor<ArgumentList<UElement, TElements...>> = nullptr>
        explicit (Details::ExplicitIfTupleDefaultConstructor<UElement, TElements...>)
        constexpr Tuple() noexcept
            : BaseClass{}
            , element_{}
        {

        }

        /// \brief Tuple direct constructor. Enabled if all elements are copy-constructible.
        template<typename UElement = TElement, Details::EnableIfTupleDirectConstructor<ArgumentList<UElement, TElements...>> = nullptr>
        explicit (Details::ExplicitIfTupleDirectConstructor<UElement, TElements...>)
        constexpr Tuple(Ref<TElement> element, Ref<TElements>... elements) noexcept
            : Tuple(ElementwiseTag{}, element, elements...)
        {

        }

        /// \brief Tuple converting constructor. Enabled if all tuple elements are copy-constructible.
        template<typename UElement, typename... UElements, Details::EnableIfTupleConvertingConstructor<ElementList, UElement, UElements...> = nullptr>
        explicit (Details::ExplicitIfTupleConvertingConstructor<ElementList, UElement, UElements...>)
        constexpr Tuple(ForwardRef<UElement> element, ForwardRef<UElements>... elements) noexcept
            : Tuple(ElementwiseTag{}, Forward<UElement>(element), Forward<UElements>(elements)...)
        {

        }

        /// \brief Tuple converting copy constructor. Enabled if all tuple elements are copy-constructible.
        template<typename UElement, typename... UElements, Details::EnableIfTupleConvertingCopyConstructor<ElementList, UElement, UElements...> = nullptr>
        explicit (Details::ExplicitIfTupleConvertingCopyConstructor<ElementList, UElement, UElements...>)
        constexpr Tuple(Ref<Tuple<UElement, UElements...>> rhs) noexcept
            : Tuple(UnwindTag{}, Templates::SequenceFor<UElement, UElements...>, rhs)
        {

        }

        /// \brief Tuple converting move constructor. Enabled if all tuple elements are move-constructible.
        template<typename UElement, typename... UElements, Details::EnableIfTupleConvertingMoveConstructor<ElementList, UElement, UElements...> = nullptr>
        explicit (Details::ExplicitIfTupleConvertingMoveConstructor<ElementList, UElement, UElements...>)
        constexpr Tuple(MoveRef<Tuple<UElement, UElements...>> rhs) noexcept
            : Tuple(UnwindTag{}, Templates::SequenceFor<UElement, UElements...>, Move(rhs))
        {

        }

        /// \brief Construct a tuple forwarding explicit arguments.
        template<typename UElement, typename... UElements>
        constexpr Tuple(ElementwiseTag, ForwardRef<UElement> element, ForwardRef<UElements>... elements) noexcept;

        /// \brief Construct a tuple unwinding another tuple elements.
        template<typename TTuple, Int... VIndexes>
        constexpr Tuple(UnwindTag, Templates::Sequence<VIndexes...>, ForwardRef<TTuple> tuple) noexcept;

        /// \brief Fallback case for when no assignment operator could be found.
        constexpr MutableRef<Tuple> operator=(volatile Ref<Tuple>) = delete;

        /// \brief Copy-assignment operator.
        template <typename TSelf = Tuple, typename TSelfList = ElementList, Details::EnableIfTupleCopyAssignment<TSelfList> = nullptr>
        constexpr MutableRef<Tuple> operator=(Templates::Identity<Ref<TSelf>> rhs) noexcept;

        /// \brief Move-assignment operator.
        template <typename TSelf = Tuple, typename TSelfList = ElementList, Details::EnableIfTupleMoveAssignment<TSelfList> = nullptr>
        constexpr MutableRef<Tuple> operator=(Templates::Identity<MoveRef<TSelf>> rhs) noexcept;

        /// \brief Tuple converting copy-assignment operator.
        template <typename... UElements, typename TSelfList = ElementList, Details::EnableIfTupleConvertingCopyAssignment<TSelfList, ArgumentList<UElements...>> = nullptr>
        constexpr MutableRef<Tuple> operator=(Ref<Tuple<UElements...>> rhs) noexcept;

        /// \brief Tuple converting move-assignment operator.
        template <typename... UElements, typename TSelfList = ElementList, Details::EnableIfTupleConvertingMoveAssignment<TSelfList, ArgumentList<UElements...>> = nullptr>
        constexpr MutableRef<Tuple> operator=(MoveRef<Tuple<UElements...>> rhs) noexcept;

        /// \brief Default copy-constructor.
        constexpr Tuple(Ref<Tuple> other) = default;

        /// \brief Default move-constructor.
        constexpr Tuple(MoveRef<Tuple> other) = default;

        /// \brief Swap this tuple with rhs by means of element-wise Swap.
        constexpr MutableRef<Tuple> Swap(MutableRef<Tuple> rhs) noexcept;

        /// \brief Head element.
        TElement element_;
    };

    /// \brief Empty tuple.
    /// \author Raffaele D. Facendola - September 2020.
    template <>
    struct Tuple<>
    {
        /// \brief Default constructor.
        constexpr Tuple() noexcept = default;

        /// \brief Default copy constructor.
        constexpr Tuple(Ref<Tuple>) noexcept = default;

        /// \brief Default copy-assignment.
        constexpr Tuple& operator=(Ref<Tuple>) noexcept = default;

        /// \brief Swap this tuple with rhs by means of element-wise Swap.
        constexpr Tuple& Swap(MutableRef<Tuple> rhs) noexcept;
    };

    /// \brief Deduction rule.
    template <typename... TElements>
    Tuple(TElements...) ->Tuple<TElements...>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Comparison.
    // ===========

    /// \brief Compare two tuples.
    /// \return Returns true if each element in lhs compares equal to the corresponding element in rhs, returns false otherwise.
    template <typename... TElements, typename... UElements>
    constexpr Bool operator==(Ref<Tuple<TElements...>> lhs, Ref<Tuple<UElements...>> rhs) noexcept;

    // Tuple element access.
    // =====================

    /// \brief Access the VIndex-th element in a tuple.
    /// \remarks The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TElements>
    constexpr MutableRef<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(MutableRef<Tuple<TElements...>> tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple.
    /// \remarks The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TElements>
    constexpr MoveRef<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(MoveRef<Tuple<TElements...>> tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple.
    /// \remarks The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TElements>
    constexpr Ref<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(Ref<Tuple<TElements...>> tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple.
    /// \remarks The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TElements>
    constexpr ConstMoveRef<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(ConstMoveRef<Tuple<TElements...>> tuple) noexcept;

    /// \brief Access an element of a tuple by type.
    /// \remarks The program is ill-formed unless the tuple has exactly one occurrence of TElement.
    template <typename TElement, typename... TElements>
    constexpr MutableRef<TElement> Get(MutableRef<Tuple<TElements...>> tuple) noexcept;

    /// \brief Access an element of a tuple by type.
    /// \remarks The program is ill-formed unless the tuple has exactly one occurrence of TElement.
    template <typename TElement, typename... TElements>
    constexpr MoveRef<TElement> Get(MoveRef<Tuple<TElements...>> tuple) noexcept;

    /// \brief Access an element of a tuple by type.
    /// \remarks The program is ill-formed unless the tuple has exactly one occurrence of TElement.
    template <typename TElement, typename... TElements>
    constexpr Ref<TElement> Get(Ref<Tuple<TElements...>> tuple) noexcept;

    /// \brief Access an element of a tuple by type.
    /// \remarks The program is ill-formed unless the tuple has exactly one occurrence of TElement.
    template <typename TElement, typename... TElements>
    constexpr ConstMoveRef<TElement> Get(ConstMoveRef<Tuple<TElements...>> tuple) noexcept;

    // Construction utilities.
    // =======================

    /// \brief Create a tuple instance, deducing template types from arguments.
    template <typename... TElements>
    constexpr Tuple<TElements...> MakeTuple(ForwardRef<TElements>... elements) noexcept;

    /// \brief Create a tuple of lvalue references to provided arguments.
    template <typename... TElements>
    constexpr Tuple<MutableRef<TElements>...> Tie(MutableRef<TElements>... elements) noexcept;

    /// \brief Create a tuple of the perfectly-forwarded elements provided.
    template <typename... TElements>
    constexpr Tuple<ForwardRef<TElements>...> ForwardAsTuple(ForwardRef<TElements>... elements) noexcept;

    // Swap.
    // =====

    /// \brief Swap two tuples.
    template <typename... TElements>
    constexpr void Swap(MutableRef<Tuple<TElements...>> lhs, MutableRef<Tuple<TElements...>> rhs) noexcept;

    // Functional.
    // ===========

    /// \brief Invoke a callable object with arguments provided as tuple.
    template <typename TCallable, typename TArguments>
    constexpr decltype(auto) Apply(ForwardRef<TCallable> callable, ForwardRef<TArguments> arguments) noexcept;

    /// \brief Apply a function passing the VIndex-th element of each tuple-like object as function arguments.
    /// \remarks Tuple elements are accessed by means of Get<Int>(tuple) function, which is expected to be defined for each type in TTuples.
    template <Int VIndex, typename TFunction, typename... TTuples>
    constexpr decltype(auto) LockstepApplyAt(ForwardRef<TFunction> function, ForwardRef<TTuples>... tuples) noexcept;

    /// \brief Apply a function to all argument list generated by projecting the i-th element of all provided tuples, in the same order, for each index i.
    /// \remarks Tuple elements are accessed by means of Get<Int>(tuple) function, which is expected to be defined for each type in TTuples.
    template <typename TFunction, typename TTuple, typename... TTuples>
    constexpr void LockstepApply(ForwardRef<TFunction> function, ForwardRef<TTuple> tuple, ForwardRef<TTuples>... tuples) noexcept;

}

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* TYPE TRAITS                                                          */
    /************************************************************************/

    /// \brief Provides indexed access to tuple elements' types.
    template <Int VIndex, typename TTuple>
    using TupleElement = TypeListElement<VIndex, typename TTuple::TElementList>;

    /// \brief Partial template specialization for tuples.
    template <typename... TElements>
    inline constexpr Int Rank<Tuple<TElements...>> = sizeof...(TElements);
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Tuple.
    // ======

    template <typename TElement, typename... TElements>
    template <typename UElement, typename... UElements>
    constexpr Tuple<TElement, TElements...>::Tuple(ElementwiseTag, ForwardRef<UElement> element, ForwardRef<UElements>... elements) noexcept
        : BaseClass(Forward<UElements>(elements)...)
        , element_(Forward<UElement>(element))
    {

    }

    template <typename TElement, typename... TElements>
    template<typename TTuple, Int... VIndexes>
    constexpr Tuple<TElement, TElements...>::Tuple(UnwindTag, Templates::Sequence<VIndexes...>, ForwardRef<TTuple> tuple) noexcept
        : Tuple(ElementwiseTag, Get<VIndexes>(Forward<TTuple>(tuple))...)
    {

    }

    template <typename TElement, typename... TElements>
    template <typename TSelf, typename TSelfList, Details::EnableIfTupleCopyAssignment<TSelfList>>
    constexpr MutableRef<Tuple<TElement, TElements...>> Tuple<TElement, TElements...>::operator=(Templates::Identity<Ref<TSelf>> rhs) noexcept
    {
        LockstepApply([&rhs](auto& lhs_element, const auto& rhs_element){ lhs_element = rhs_element; }, *this, rhs);

        return *this;
    }

    template <typename TElement, typename... TElements>
    template <typename TSelf, typename TSelfList, Details::EnableIfTupleMoveAssignment<TSelfList>>
    constexpr MutableRef<Tuple<TElement, TElements...>> Tuple<TElement, TElements...>::operator=(Templates::Identity<MoveRef<TSelf>> rhs) noexcept
    {
        LockstepApply([&rhs](auto& lhs_element, auto&& rhs_element) { lhs_element = Move(rhs_element); }, *this, rhs);

        return *this;
    }

    template <typename TElement, typename... TElements>
    template <typename... UElements, typename TSelfList, Details::EnableIfTupleConvertingCopyAssignment<TSelfList, Templates::TypeList<UElements...>>>
    constexpr MutableRef<Tuple<TElement, TElements...>> Tuple<TElement, TElements...>::operator=(Ref<Tuple<UElements...>> rhs) noexcept
    {
        LockstepApply([&rhs](auto& lhs_element, const auto& rhs_element) { lhs_element = rhs_element; }, *this, rhs);

        return *this;
    }

    template <typename TElement, typename... TElements>
    template <typename... UElements, typename TSelfList, Details::EnableIfTupleConvertingMoveAssignment<TSelfList, Templates::TypeList<UElements...>>>
    constexpr MutableRef<Tuple<TElement, TElements...>> Tuple<TElement, TElements...>::operator=(MoveRef<Tuple<UElements...>> rhs) noexcept
    {
        LockstepApply([&rhs](auto& lhs_element, auto&& rhs_element) { lhs_element = Move(rhs_element); }, *this, rhs);

        return *this;
    }

    template <typename TElement, typename... TElements>
    constexpr MutableRef<Tuple<TElement, TElements...>> Tuple<TElement, TElements...>::Swap(MutableRef<Tuple> rhs) noexcept
    {
        Syntropy::Swap(element_, rhs.element_);

        static_cast<MutableRef<BaseClass>>(*this).Swap(static_cast<MutableRef<BaseClass>>(rhs));

        return *this;
    }

    constexpr Tuple<>& Tuple<>::Swap(Tuple<>& rhs) noexcept
    {
        return *this;
    }

    // Non-member functions.
    // =====================

    // Comparison.

    template <typename... TElements, typename... UElements>
    constexpr Bool operator==(Ref<Tuple<TElements...>> lhs, Ref<Tuple<UElements...>> rhs) noexcept
    {
        auto result = true;

        LockstepApply([&result](const auto& lhs_element, const auto& rhs_element)
        {
            result &= (lhs_element == rhs_element);
        }, lhs, rhs);

        return result;
    }

    // Tuple element access.

    template <Int VIndex, typename... TElements>
    constexpr MutableRef<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(MutableRef<Tuple<TElements...>> tuple) noexcept
    {
        using TTupleBase = Details::TupleBase<VIndex, Tuple<TElements...>>;

        return static_cast<MutableRef<TTupleBase>>(tuple).element_;
    }

    template <Int VIndex, typename... TElements>
    constexpr MoveRef<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(MoveRef<Tuple<TElements...>> tuple) noexcept
    {
        using TTupleBase = Details::TupleBase<VIndex, Tuple<TElements...>>;
        using TElement = Templates::TupleElement<VIndex, Tuple<TElements...>>;

        return static_cast<MoveRef<TElement>>(static_cast<MutableRef<TTupleBase>>(tuple).element_);
    }

    template <Int VIndex, typename... TElements>
    constexpr Ref<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(Ref<Tuple<TElements...>> tuple) noexcept
    {
        using TTupleBase = Details::TupleBase<VIndex, Tuple<TElements...>>;

        return static_cast<Ref<TTupleBase>>(tuple).element_;
    }

    template <Int VIndex, typename... TElements>
    constexpr ConstMoveRef<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(ConstMoveRef<Tuple<TElements...>> tuple) noexcept
    {
        using TTupleBase = Details::TupleBase<VIndex, Tuple<TElements...>>;
        using TElement = Templates::TupleElement<VIndex, Tuple<TElements...>>;

        return static_cast<ConstMoveRef<TElement>>(static_cast<Ref<TTupleBase>>(tuple).element_);
    }

    template <typename TElement, typename... TElements>
    constexpr MutableRef<TElement> Get(MutableRef<Tuple<TElements...>> tuple) noexcept
    {
        constexpr auto kIndex = Templates::TypeListIndex<TElement, Templates::TypeList<TElements...>>;

        return Get<kIndex>(tuple);
    }

    template <typename TElement, typename... TElements>
    constexpr MoveRef<TElement> Get(MoveRef<Tuple<TElements...>> tuple) noexcept
    {
        constexpr auto kIndex = Templates::TypeListIndex<TElement, Templates::TypeList<TElements...>>;

        return Get<kIndex>(Move(tuple));
    }

    template <typename TElement, typename... TElements>
    constexpr Ref<TElement> Get(Ref<Tuple<TElements...>> tuple) noexcept
    {
        constexpr auto kIndex = Templates::TypeListIndex<TElement, Templates::TypeList<TElements...>>;

        return Get<kIndex>(tuple);
    }

    template <typename TElement, typename... TElements>
    constexpr ConstMoveRef<TElement> Get(ConstMoveRef<Tuple<TElements...>> tuple) noexcept
    {
        constexpr auto kIndex = Templates::TypeListIndex<TElement, Templates::TypeList<TElements...>>;

        return Get<kIndex>(Move(tuple));
    }

    // Construction utilities.

    template <typename... TElements>
    constexpr Tuple<TElements...> MakeTuple(ForwardRef<TElements>... elements) noexcept
    {
        return { Forward<TElements>(elements)... };
    }

    template <typename... TElements>
    constexpr Tuple<MutableRef<TElements>...> Tie(MutableRef<TElements>... elements) noexcept
    {
        return Tuple<MutableRef<TElements>...>(elements...);
    }

    template <typename... TElements>
    constexpr Tuple<ForwardRef<TElements>...> ForwardAsTuple(ForwardRef<TElements>... elements) noexcept
    {
        return Tuple<ForwardRef<TElements>...>(Forward<TElements>(elements)...);
    }

    // Swap.

    template <typename... TElements>
    constexpr void Swap(MutableRef<Tuple<TElements...>> lhs, MutableRef<Tuple<TElements...>> rhs) noexcept
    {
        lhs.Swap(rhs);
    }

    // Functional.

    template <typename TCallable, typename TArguments>
    constexpr decltype(auto) Apply(ForwardRef<TCallable> callable, ForwardRef<TArguments> arguments) noexcept
    {
        return Details::Apply(Forward<TCallable>(callable), Forward<TArguments>(arguments), Templates::MakeSequence<Templates::Rank<Templates::RemoveConstReference<TArguments>>>{});
    }

    template <Int VIndex, typename TFunction, typename... TTuples>
    constexpr decltype(auto) LockstepApplyAt(ForwardRef<TFunction> function, ForwardRef<TTuples>... tuples) noexcept
    {
        return function(Get<VIndex>(Forward<TTuples>(tuples))...);
    }

    template <typename TFunction, typename TTuple, typename... TTuples>
    constexpr void LockstepApply(ForwardRef<TFunction> function, ForwardRef<TTuple> tuple, ForwardRef<TTuples>... tuples) noexcept
    {
        static_assert(Templates::SameRank<Templates::Decay<TTuple>, Templates::Decay<TTuples>...>, "Tuples must have the same rank.");

        using TSequence = Templates::MakeSequence<Templates::Rank<Templates::Decay<TTuple>>>;

        Details::LockstepApply(TSequence{}, Forward<TFunction>(function), Forward<TTuple>(tuple), Forward<TTuples>(tuples)...);
    }


}

// ===========================================================================

namespace std
{
    /************************************************************************/
    /* STRUCTURED BINDINGS                                                  */
    /************************************************************************/

    /// \brief Size of a Syntropy::Tuple.
    template <typename... TElements>
    struct std::tuple_size<Syntropy::Tuple<TElements...>>
    {
        static constexpr std::size_t value = sizeof...(TElements);
    };

    /// \brief Type of the VIndex-th element of a Syntropy::Tuple.
    template <std::size_t VIndex, typename... TElements>
    struct std::tuple_element<VIndex, Syntropy::Tuple<TElements...>>
    {
        using type = Syntropy::Templates::TypeListElement<VIndex, Syntropy::Templates::TypeList<TElements...>>;
    };

    /// \brief Get the VIndex-th element of a tuple.
    template <std::size_t VIndex, typename... TElements>
    decltype(auto) get(Syntropy::Ref<Syntropy::Tuple<TElements...>> tuple)
    {
        return Syntropy::Get<VIndex>(tuple);
    }

    /// \brief Get the VIndex-th element of a tuple.
    template <std::size_t VIndex, typename... TElements>
    decltype(auto) get(Syntropy::MoveRef<Syntropy::Tuple<TElements...>> tuple)
    {
        return Syntropy::Get<VIndex>(Syntropy::Move(tuple));
    }
}

// ===========================================================================
