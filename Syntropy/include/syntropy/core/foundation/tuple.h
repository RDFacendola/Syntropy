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
    struct TupleT;

    /// \brief Recursive tuple definition.
    template <typename TElement, typename... TElements>
    struct TupleT<TElement, TElements...> : private TupleT<TElements...>
    {
        template <Int VIndex, typename... TElements>
        friend constexpr MutableRef<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(MutableRef<TupleT<TElements...>> tuple) noexcept;

        template <Int VIndex, typename... TElements>
        friend constexpr MoveRef<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(MoveRef<TupleT<TElements...>> tuple) noexcept;

        template <Int VIndex, typename... TElements>
        friend constexpr Ref<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(Ref<TupleT<TElements...>> tuple) noexcept;

        template <Int VIndex, typename... TElements>
        friend constexpr ConstMoveRef<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(ConstMoveRef<TupleT<TElements...>> tuple) noexcept;

    public:

        /// \brief Type of the base class.
        using BaseClass = TupleT<TElements...>;

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
        constexpr TupleT() noexcept
            : BaseClass{}
            , element_{}
        {

        }

        /// \brief Tuple direct constructor. Enabled if all elements are copy-constructible.
        template<typename UElement = TElement, Details::EnableIfTupleDirectConstructor<ArgumentList<UElement, TElements...>> = nullptr>
        explicit (Details::ExplicitIfTupleDirectConstructor<UElement, TElements...>)
        constexpr TupleT(Ref<TElement> element, Ref<TElements>... elements) noexcept
            : TupleT(ElementwiseTag{}, element, elements...)
        {

        }

        /// \brief Tuple converting constructor. Enabled if all tuple elements are copy-constructible.
        template<typename UElement, typename... UElements, Details::EnableIfTupleConvertingConstructor<ElementList, UElement, UElements...> = nullptr>
        explicit (Details::ExplicitIfTupleConvertingConstructor<ElementList, UElement, UElements...>)
        constexpr TupleT(ForwardRef<UElement> element, ForwardRef<UElements>... elements) noexcept
            : TupleT(ElementwiseTag{}, Forward<UElement>(element), Forward<UElements>(elements)...)
        {

        }

        /// \brief Tuple converting copy constructor. Enabled if all tuple elements are copy-constructible.
        template<typename UElement, typename... UElements, Details::EnableIfTupleConvertingCopyConstructor<ElementList, UElement, UElements...> = nullptr>
        explicit (Details::ExplicitIfTupleConvertingCopyConstructor<ElementList, UElement, UElements...>)
        constexpr TupleT(Ref<TupleT<UElement, UElements...>> rhs) noexcept
            : TupleT(UnwindTag{}, Templates::SequenceFor<UElement, UElements...>, rhs)
        {

        }

        /// \brief Tuple converting move constructor. Enabled if all tuple elements are move-constructible.
        template<typename UElement, typename... UElements, Details::EnableIfTupleConvertingMoveConstructor<ElementList, UElement, UElements...> = nullptr>
        explicit (Details::ExplicitIfTupleConvertingMoveConstructor<ElementList, UElement, UElements...>)
        constexpr TupleT(MoveRef<TupleT<UElement, UElements...>> rhs) noexcept
            : TupleT(UnwindTag{}, Templates::SequenceFor<UElement, UElements...>, Move(rhs))
        {

        }

        /// \brief Construct a tuple forwarding explicit arguments.
        template<typename UElement, typename... UElements>
        constexpr TupleT(ElementwiseTag, ForwardRef<UElement> element, ForwardRef<UElements>... elements) noexcept;

        /// \brief Construct a tuple unwinding another tuple elements.
        template<typename TTuple, Int... VIndexes>
        constexpr TupleT(UnwindTag, Templates::Sequence<VIndexes...>, ForwardRef<TTuple> tuple) noexcept;

        /// \brief Fallback case for when no assignment operator could be found.
        constexpr MutableRef<TupleT> operator=(volatile Ref<TupleT>) = delete;

        /// \brief Copy-assignment operator.
        template <typename TSelf = TupleT, typename TSelfList = ElementList, Details::EnableIfTupleCopyAssignment<TSelfList> = nullptr>
        constexpr MutableRef<TupleT> operator=(Templates::Identity<Ref<TSelf>> rhs) noexcept;

        /// \brief Move-assignment operator.
        template <typename TSelf = TupleT, typename TSelfList = ElementList, Details::EnableIfTupleMoveAssignment<TSelfList> = nullptr>
        constexpr MutableRef<TupleT> operator=(Templates::Identity<MoveRef<TSelf>> rhs) noexcept;

        /// \brief Tuple converting copy-assignment operator.
        template <typename... UElements, typename TSelfList = ElementList, Details::EnableIfTupleConvertingCopyAssignment<TSelfList, ArgumentList<UElements...>> = nullptr>
        constexpr MutableRef<TupleT> operator=(Ref<TupleT<UElements...>> rhs) noexcept;

        /// \brief Tuple converting move-assignment operator.
        template <typename... UElements, typename TSelfList = ElementList, Details::EnableIfTupleConvertingMoveAssignment<TSelfList, ArgumentList<UElements...>> = nullptr>
        constexpr MutableRef<TupleT> operator=(MoveRef<TupleT<UElements...>> rhs) noexcept;

        /// \brief Default copy-constructor.
        constexpr TupleT(Ref<TupleT> other) = default;

        /// \brief Default move-constructor.
        constexpr TupleT(MoveRef<TupleT> other) = default;

        /// \brief Swap this tuple with rhs by means of element-wise Swap.
        constexpr MutableRef<TupleT> Swap(MutableRef<TupleT> rhs) noexcept;

        /// \brief Head element.
        TElement element_;
    };

    /// \brief Empty tuple.
    /// \author Raffaele D. Facendola - September 2020.
    template <>
    struct TupleT<>
    {
        /// \brief Default constructor.
        constexpr TupleT() noexcept = default;

        /// \brief Default copy constructor.
        constexpr TupleT(Ref<TupleT>) noexcept = default;

        /// \brief Default copy-assignment.
        constexpr TupleT& operator=(Ref<TupleT>) noexcept = default;

        /// \brief Swap this tuple with rhs by means of element-wise Swap.
        constexpr TupleT& Swap(MutableRef<TupleT> rhs) noexcept;
    };

    /// \brief Deduction rule.
    template <typename... TElements>
    TupleT(TElements...) ->TupleT<TElements...>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Comparison.
    // ===========

    /// \brief Compare two tuples.
    /// \return Returns true if each element in lhs compares equal to the corresponding element in rhs, returns false otherwise.
    template <typename... TElements, typename... UElements>
    constexpr Bool operator==(Ref<TupleT<TElements...>> lhs, Ref<TupleT<UElements...>> rhs) noexcept;

    // Tuple element access.
    // =====================

    /// \brief Access the VIndex-th element in a tuple.
    /// \remarks The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TElements>
    constexpr MutableRef<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(MutableRef<TupleT<TElements...>> tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple.
    /// \remarks The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TElements>
    constexpr MoveRef<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(MoveRef<TupleT<TElements...>> tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple.
    /// \remarks The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TElements>
    constexpr Ref<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(Ref<TupleT<TElements...>> tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple.
    /// \remarks The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TElements>
    constexpr ConstMoveRef<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(ConstMoveRef<TupleT<TElements...>> tuple) noexcept;

    /// \brief Access an element of a tuple by type.
    /// \remarks The program is ill-formed unless the tuple has exactly one occurrence of TElement.
    template <typename TElement, typename... TElements>
    constexpr MutableRef<TElement> Get(MutableRef<TupleT<TElements...>> tuple) noexcept;

    /// \brief Access an element of a tuple by type.
    /// \remarks The program is ill-formed unless the tuple has exactly one occurrence of TElement.
    template <typename TElement, typename... TElements>
    constexpr MoveRef<TElement> Get(MoveRef<TupleT<TElements...>> tuple) noexcept;

    /// \brief Access an element of a tuple by type.
    /// \remarks The program is ill-formed unless the tuple has exactly one occurrence of TElement.
    template <typename TElement, typename... TElements>
    constexpr Ref<TElement> Get(Ref<TupleT<TElements...>> tuple) noexcept;

    /// \brief Access an element of a tuple by type.
    /// \remarks The program is ill-formed unless the tuple has exactly one occurrence of TElement.
    template <typename TElement, typename... TElements>
    constexpr ConstMoveRef<TElement> Get(ConstMoveRef<TupleT<TElements...>> tuple) noexcept;

    // Construction utilities.
    // =======================

    /// \brief Create a tuple instance, deducing template types from arguments.
    template <typename... TElements>
    constexpr TupleT<TElements...> MakeTuple(ForwardRef<TElements>... elements) noexcept;

    /// \brief Create a tuple of lvalue references to provided arguments.
    template <typename... TElements>
    constexpr TupleT<MutableRef<TElements>...> Tie(MutableRef<TElements>... elements) noexcept;

    /// \brief Create a tuple of the perfectly-forwarded elements provided.
    template <typename... TElements>
    constexpr TupleT<ForwardRef<TElements>...> ForwardAsTuple(ForwardRef<TElements>... elements) noexcept;

    // Swap.
    // =====

    /// \brief Swap two tuples.
    template <typename... TElements>
    constexpr void Swap(MutableRef<TupleT<TElements...>> lhs, MutableRef<TupleT<TElements...>> rhs) noexcept;

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
    inline constexpr Int Rank<TupleT<TElements...>> = sizeof...(TElements);
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // TupleT.
    // ======

    template <typename TElement, typename... TElements>
    template <typename UElement, typename... UElements>
    constexpr TupleT<TElement, TElements...>::TupleT(ElementwiseTag, ForwardRef<UElement> element, ForwardRef<UElements>... elements) noexcept
        : BaseClass(Forward<UElements>(elements)...)
        , element_(Forward<UElement>(element))
    {

    }

    template <typename TElement, typename... TElements>
    template<typename TTuple, Int... VIndexes>
    constexpr TupleT<TElement, TElements...>::TupleT(UnwindTag, Templates::Sequence<VIndexes...>, ForwardRef<TTuple> tuple) noexcept
        : TupleT(ElementwiseTag, Get<VIndexes>(Forward<TTuple>(tuple))...)
    {

    }

    template <typename TElement, typename... TElements>
    template <typename TSelf, typename TSelfList, Details::EnableIfTupleCopyAssignment<TSelfList>>
    constexpr MutableRef<TupleT<TElement, TElements...>> TupleT<TElement, TElements...>::operator=(Templates::Identity<Ref<TSelf>> rhs) noexcept
    {
        LockstepApply([&rhs](auto& lhs_element, const auto& rhs_element){ lhs_element = rhs_element; }, *this, rhs);

        return *this;
    }

    template <typename TElement, typename... TElements>
    template <typename TSelf, typename TSelfList, Details::EnableIfTupleMoveAssignment<TSelfList>>
    constexpr MutableRef<TupleT<TElement, TElements...>> TupleT<TElement, TElements...>::operator=(Templates::Identity<MoveRef<TSelf>> rhs) noexcept
    {
        LockstepApply([&rhs](auto& lhs_element, auto&& rhs_element) { lhs_element = Move(rhs_element); }, *this, rhs);

        return *this;
    }

    template <typename TElement, typename... TElements>
    template <typename... UElements, typename TSelfList, Details::EnableIfTupleConvertingCopyAssignment<TSelfList, Templates::TypeList<UElements...>>>
    constexpr MutableRef<TupleT<TElement, TElements...>> TupleT<TElement, TElements...>::operator=(Ref<TupleT<UElements...>> rhs) noexcept
    {
        LockstepApply([&rhs](auto& lhs_element, const auto& rhs_element) { lhs_element = rhs_element; }, *this, rhs);

        return *this;
    }

    template <typename TElement, typename... TElements>
    template <typename... UElements, typename TSelfList, Details::EnableIfTupleConvertingMoveAssignment<TSelfList, Templates::TypeList<UElements...>>>
    constexpr MutableRef<TupleT<TElement, TElements...>> TupleT<TElement, TElements...>::operator=(MoveRef<TupleT<UElements...>> rhs) noexcept
    {
        LockstepApply([&rhs](auto& lhs_element, auto&& rhs_element) { lhs_element = Move(rhs_element); }, *this, rhs);

        return *this;
    }

    template <typename TElement, typename... TElements>
    constexpr MutableRef<TupleT<TElement, TElements...>> TupleT<TElement, TElements...>::Swap(MutableRef<TupleT> rhs) noexcept
    {
        Syntropy::Swap(element_, rhs.element_);

        static_cast<MutableRef<BaseClass>>(*this).Swap(static_cast<MutableRef<BaseClass>>(rhs));

        return *this;
    }

    constexpr TupleT<>& TupleT<>::Swap(TupleT<>& rhs) noexcept
    {
        return *this;
    }

    // Non-member functions.
    // =====================

    // Comparison.

    template <typename... TElements, typename... UElements>
    constexpr Bool operator==(Ref<TupleT<TElements...>> lhs, Ref<TupleT<UElements...>> rhs) noexcept
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
    constexpr MutableRef<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(MutableRef<TupleT<TElements...>> tuple) noexcept
    {
        using TTupleBase = Details::TupleBase<VIndex, TupleT<TElements...>>;

        return static_cast<MutableRef<TTupleBase>>(tuple).element_;
    }

    template <Int VIndex, typename... TElements>
    constexpr MoveRef<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(MoveRef<TupleT<TElements...>> tuple) noexcept
    {
        using TTupleBase = Details::TupleBase<VIndex, TupleT<TElements...>>;
        using TElement = Templates::TupleElement<VIndex, TupleT<TElements...>>;

        return static_cast<MoveRef<TElement>>(static_cast<MutableRef<TTupleBase>>(tuple).element_);
    }

    template <Int VIndex, typename... TElements>
    constexpr Ref<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(Ref<TupleT<TElements...>> tuple) noexcept
    {
        using TTupleBase = Details::TupleBase<VIndex, TupleT<TElements...>>;

        return static_cast<Ref<TTupleBase>>(tuple).element_;
    }

    template <Int VIndex, typename... TElements>
    constexpr ConstMoveRef<Templates::TypeListElement<VIndex, Templates::TypeList<TElements...>>> Get(ConstMoveRef<TupleT<TElements...>> tuple) noexcept
    {
        using TTupleBase = Details::TupleBase<VIndex, TupleT<TElements...>>;
        using TElement = Templates::TupleElement<VIndex, TupleT<TElements...>>;

        return static_cast<ConstMoveRef<TElement>>(static_cast<Ref<TTupleBase>>(tuple).element_);
    }

    template <typename TElement, typename... TElements>
    constexpr MutableRef<TElement> Get(MutableRef<TupleT<TElements...>> tuple) noexcept
    {
        constexpr auto kIndex = Templates::TypeListIndex<TElement, Templates::TypeList<TElements...>>;

        return Get<kIndex>(tuple);
    }

    template <typename TElement, typename... TElements>
    constexpr MoveRef<TElement> Get(MoveRef<TupleT<TElements...>> tuple) noexcept
    {
        constexpr auto kIndex = Templates::TypeListIndex<TElement, Templates::TypeList<TElements...>>;

        return Get<kIndex>(Move(tuple));
    }

    template <typename TElement, typename... TElements>
    constexpr Ref<TElement> Get(Ref<TupleT<TElements...>> tuple) noexcept
    {
        constexpr auto kIndex = Templates::TypeListIndex<TElement, Templates::TypeList<TElements...>>;

        return Get<kIndex>(tuple);
    }

    template <typename TElement, typename... TElements>
    constexpr ConstMoveRef<TElement> Get(ConstMoveRef<TupleT<TElements...>> tuple) noexcept
    {
        constexpr auto kIndex = Templates::TypeListIndex<TElement, Templates::TypeList<TElements...>>;

        return Get<kIndex>(Move(tuple));
    }

    // Construction utilities.

    template <typename... TElements>
    constexpr TupleT<TElements...> MakeTuple(ForwardRef<TElements>... elements) noexcept
    {
        return { Forward<TElements>(elements)... };
    }

    template <typename... TElements>
    constexpr TupleT<MutableRef<TElements>...> Tie(MutableRef<TElements>... elements) noexcept
    {
        return TupleT<TElements&...>(elements...);
    }

    template <typename... TElements>
    constexpr TupleT<ForwardRef<TElements>...> ForwardAsTuple(ForwardRef<TElements>... elements) noexcept
    {
        return TupleT<TElements&&...>(Forward<TElements>(elements)...);
    }

    // Swap.

    template <typename... TElements>
    constexpr void Swap(MutableRef<TupleT<TElements...>> lhs, MutableRef<TupleT<TElements...>> rhs) noexcept
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

    /// \brief Size of a Syntropy::TupleT.
    template <typename... TElements>
    struct std::tuple_size<Syntropy::TupleT<TElements...>>
    {
        static constexpr std::size_t value = sizeof...(TElements);
    };

    /// \brief Type of the VIndex-th element of a Syntropy::TupleT.
    template <std::size_t VIndex, typename... TElements>
    struct std::tuple_element<VIndex, Syntropy::TupleT<TElements...>>
    {
        using type = Syntropy::Templates::TypeListElement<VIndex, Syntropy::Templates::TypeList<TElements...>>;
    };

    /// \brief Get the VIndex-th element of a tuple.
    template <std::size_t VIndex, typename... TElements>
    decltype(auto) get(Syntropy::Ref<Syntropy::TupleT<TElements...>> tuple)
    {
        return Syntropy::Get<VIndex>(tuple);
    }

    /// \brief Get the VIndex-th element of a tuple.
    template <std::size_t VIndex, typename... TElements>
    decltype(auto) get(Syntropy::MoveRef<Syntropy::TupleT<TElements...>> tuple)
    {
        return Syntropy::Get<VIndex>(Syntropy::Move(tuple));
    }
}

// ===========================================================================
