/// \file tuple.h
/// \brief This header is part of Syntropy core module. It contains definitions for tuples.
///
/// \author Raffaele D. Facendola - Sep 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/templates/sequence.h"

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
    template <typename... TTypes>
    struct TupleT;

    /// \brief Recursive tuple definition.
    template <typename TType, typename... TTypes>
    struct TupleT<TType, TTypes...> : private TupleT<TTypes...>
    {
        template <Int VIndex, typename... TTypes>
        friend constexpr MutableRef<Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>> Get(MutableRef<TupleT<TTypes...>> tuple) noexcept;

        template <Int VIndex, typename... TTypes>
        friend constexpr MoveRef<Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>> Get(MoveRef<TupleT<TTypes...>> tuple) noexcept;

        template <Int VIndex, typename... TTypes>
        friend constexpr Ref<Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>> Get(Ref<TupleT<TTypes...>> tuple) noexcept;

        template <Int VIndex, typename... TTypes>
        friend constexpr ConstMoveRef<Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>> Get(ConstMoveRef<TupleT<TTypes...>> tuple) noexcept;

    public:

        /// \brief Type of the base class.
        using TBaseClass = TupleT<TTypes...>;

        /// \brief Types of tuple elements.
        using TTypeList = Syntropy::Templates::TypeList<TType, TTypes...>;

        /// \brief Tag type used to construct a tuple element-wise.
        struct ElementwiseConstructor {};

        /// \brief Tag type used to construct a tuple by unwinding another tuple.
        struct UnwindConstructor {};

        /// \brief Tuple default constructor. Enabled if all elements are default-constructible.
        template<typename UType = TType, Details::EnableIfTupleDefaultConstructor<Templates::TypeList<UType, TTypes...>> = nullptr>
        explicit (Details::ExplicitIfTupleDefaultConstructor<UType, TTypes...>)
        constexpr TupleT() noexcept
            : TBaseClass{}
            , element_{}
        {

        }

        /// \brief Tuple direct constructor. Enabled if all elements are copy-constructible.
        template<typename UType = TType, Details::EnableIfTupleDirectConstructor<Templates::TypeList<UType, TTypes...>> = nullptr>
        explicit (Details::ExplicitIfTupleDirectConstructor<UType, TTypes...>)
        constexpr TupleT(Ref<TType> element, Ref<TTypes>... elements) noexcept
            : TupleT(ElementwiseConstructor{}, element, elements...)
        {

        }

        /// \brief Tuple converting constructor. Enabled if all tuple elements are copy-constructible.
        template<typename UType, typename... UTypes, Details::EnableIfTupleConvertingConstructor<TTypeList, UType, UTypes...> = nullptr>
        explicit (Details::ExplicitIfTupleConvertingConstructor<TTypeList, UType, UTypes...>)
        constexpr TupleT(ForwardRef<UType> element, ForwardRef<UTypes>... elements) noexcept
            : TupleT(ElementwiseConstructor{}, Forward<UType>(element), Forward<UTypes>(elements)...)
        {

        }

        /// \brief Tuple converting copy constructor. Enabled if all tuple elements are copy-constructible.
        template<typename UType, typename... UTypes, Details::EnableIfTupleConvertingCopyConstructor<TTypeList, UType, UTypes...> = nullptr>
        explicit (Details::ExplicitIfTupleConvertingCopyConstructor<TTypeList, UType, UTypes...>)
        constexpr TupleT(Ref<TupleT<UType, UTypes...>> rhs) noexcept
            : TupleT(UnwindConstructor{}, Templates::SequenceFor<UType, UTypes...>, rhs)
        {

        }

        /// \brief Tuple converting move constructor. Enabled if all tuple elements are move-constructible.
        template<typename UType, typename... UTypes, Details::EnableIfTupleConvertingMoveConstructor<TTypeList, UType, UTypes...> = nullptr>
        explicit (Details::ExplicitIfTupleConvertingMoveConstructor<TTypeList, UType, UTypes...>)
        constexpr TupleT(MoveRef<TupleT<UType, UTypes...>> rhs) noexcept
            : TupleT(UnwindConstructor{}, Templates::SequenceFor<UType, UTypes...>, Move(rhs))
        {

        }

        /// \brief Swap this tuple with rhs by means of element-wise Swap.
        constexpr MutableRef<TupleT> Swap(MutableRef<TupleT> rhs) noexcept;

        /// \brief Fallback case for when no assignment operator could be found.
        constexpr MutableRef<TupleT> operator=(volatile Ref<TupleT>) = delete;

        /// \brief Copy-assignment operator.
        template <typename TSelf = TupleT, typename TSelfList = TTypeList, Details::EnableIfTupleCopyAssignment<TSelfList> = nullptr>
        constexpr MutableRef<TupleT> operator=(Templates::Identity<Ref<TSelf>> rhs) noexcept;

        /// \brief Move-assignment operator.
        template <typename TSelf = TupleT, typename TSelfList = TTypeList, Details::EnableIfTupleMoveAssignment<TSelfList> = nullptr>
        constexpr MutableRef<TupleT> operator=(Templates::Identity<MoveRef<TSelf>> rhs) noexcept;

        /// \brief Tuple converting copy-assignment operator.
        template <typename... UTypes, typename TSelfList = TTypeList, Details::EnableIfTupleConvertingCopyAssignment<TSelfList, Templates::TypeList<UTypes...>> = nullptr>
        constexpr MutableRef<TupleT> operator=(Ref<TupleT<UTypes...>> rhs) noexcept;

        /// \brief Tuple converting move-assignment operator.
        template <typename... UTypes, typename TSelfList = TTypeList, Details::EnableIfTupleConvertingMoveAssignment<TSelfList, Templates::TypeList<UTypes...>> = nullptr>
        constexpr MutableRef<TupleT> operator=(MoveRef<TupleT<UTypes...>> rhs) noexcept;

        /// \brief Construct a tuple forwarding explicit arguments.
        template<typename UType, typename... UTypes>
        constexpr TupleT(ElementwiseConstructor, ForwardRef<UType> element, ForwardRef<UTypes>... elements) noexcept;

        /// \brief Construct a tuple unwinding another tuple elements.
        template<typename TTuple, Int... VIndexes>
        constexpr TupleT(UnwindConstructor, Templates::Sequence<VIndexes...>, ForwardRef<TTuple> tuple) noexcept;

        /// \brief Default copy-constructor.
        constexpr TupleT(Ref<TupleT> other) = default;

        /// \brief Default move-constructor.
        constexpr TupleT(MoveRef<TupleT> other) = default;

        /// \brief Head element.
        TType element_;
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
    template <typename... TTypes>
    TupleT(TTypes...) ->TupleT<TTypes...>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Compare two tuples.
    /// \return Returns true if each element in lhs compares equal to the corresponding element in rhs, returns false otherwise.
    template <typename... TTypes, typename... UTypes>
    constexpr Bool operator==(Ref<TupleT<TTypes...>> lhs, Ref<TupleT<UTypes...>> rhs) noexcept;

    /// \brief Access the VIndex-th element in a tuple. The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TTypes>
    constexpr MutableRef<Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>> Get(MutableRef<TupleT<TTypes...>> tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple. The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TTypes>
    constexpr MoveRef<Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>> Get(MoveRef<TupleT<TTypes...>> tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple. The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TTypes>
    constexpr Ref<Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>> Get(Ref<TupleT<TTypes...>> tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple. The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TTypes>
    constexpr ConstMoveRef<Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>> Get(ConstMoveRef<TupleT<TTypes...>> tuple) noexcept;

    /// \brief Access an element of a tuple by type. The program is ill-formed unless the tuple has exactly one occurrence of TType.
    template <typename TType, typename... TTypes>
    constexpr MutableRef<TType> Get(MutableRef<TupleT<TTypes...>> tuple) noexcept;

    /// \brief Access an element of a tuple by type. The program is ill-formed unless the tuple has exactly one occurrence of TType.
    template <typename TType, typename... TTypes>
    constexpr MoveRef<TType> Get(MoveRef<TupleT<TTypes...>> tuple) noexcept;

    /// \brief Access an element of a tuple by type. The program is ill-formed unless the tuple has exactly one occurrence of TType.
    template <typename TType, typename... TTypes>
    constexpr Ref<TType> Get(Ref<TupleT<TTypes...>> tuple) noexcept;

    /// \brief Access an element of a tuple by type. The program is ill-formed unless the tuple has exactly one occurrence of TType.
    template <typename TType, typename... TTypes>
    constexpr ConstMoveRef<TType> Get(ConstMoveRef<TupleT<TTypes...>> tuple) noexcept;

    /// \brief Create a tuple instance, deducing template types from arguments.
    template <typename... TTypes>
    constexpr TupleT<TTypes...> MakeTuple(ForwardRef<TTypes>... elements) noexcept;

    /// \brief Create a tuple of lvalue references to provided arguments.
    template <typename... TElements>
    constexpr TupleT<MutableRef<TElements>...> Tie(MutableRef<TElements>... elements) noexcept;

    /// \brief Create a tuple of the perfectly-forwarded elements provided.
    template <typename... TElements>
    constexpr TupleT<ForwardRef<TElements>...> ForwardAsTuple(ForwardRef<TElements>... elements) noexcept;

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

    /// \brief Swap two tuples.
    template <typename... TTypes>
    constexpr void Swap(MutableRef<TupleT<TTypes...>> lhs, MutableRef<TupleT<TTypes...>> rhs) noexcept;
}

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* TYPE TRAITS                                                          */
    /************************************************************************/

    /// \brief Provides indexed access to tuple elements' types.
    template <Int VIndex, typename TTuple>
    using TupleElement = TypeListElement<VIndex, typename TTuple::TTypeList>;

    /// \brief Partial template specialization for tuples.
    template <typename... TTypes>
    inline constexpr Int Rank<TupleT<TTypes...>> = sizeof...(TTypes);
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // TupleT.
    // ======

    template <typename TType, typename... TTypes>
    template <typename UType, typename... UTypes>
    constexpr TupleT<TType, TTypes...>::TupleT(ElementwiseConstructor, ForwardRef<UType> element, ForwardRef<UTypes>... elements) noexcept
        : TBaseClass(Forward<UTypes>(elements)...)
        , element_(Forward<UType>(element))
    {

    }

    template <typename TType, typename... TTypes>
    template<typename TTuple, Int... VIndexes>
    constexpr TupleT<TType, TTypes...>::TupleT(UnwindConstructor, Templates::Sequence<VIndexes...>, ForwardRef<TTuple> tuple) noexcept
        : TupleT(ElementwiseConstructor, Get<VIndexes>(Forward<TTuple>(tuple))...)
    {

    }

    template <typename TType, typename... TTypes>
    template <typename TSelf, typename TSelfList, Details::EnableIfTupleCopyAssignment<TSelfList>>
    constexpr MutableRef<TupleT<TType, TTypes...>> TupleT<TType, TTypes...>::operator=(Templates::Identity<Ref<TSelf>> rhs) noexcept
    {
        LockstepApply([&rhs](auto& lhs_element, const auto& rhs_element){ lhs_element = rhs_element; }, *this, rhs);

        return *this;
    }

    template <typename TType, typename... TTypes>
    template <typename TSelf, typename TSelfList, Details::EnableIfTupleMoveAssignment<TSelfList>>
    constexpr MutableRef<TupleT<TType, TTypes...>> TupleT<TType, TTypes...>::operator=(Templates::Identity<MoveRef<TSelf>> rhs) noexcept
    {
        LockstepApply([&rhs](auto& lhs_element, auto&& rhs_element) { lhs_element = Move(rhs_element); }, *this, rhs);

        return *this;
    }

    template <typename TType, typename... TTypes>
    template <typename... UTypes, typename TSelfList, Details::EnableIfTupleConvertingCopyAssignment<TSelfList, Templates::TypeList<UTypes...>>>
    constexpr MutableRef<TupleT<TType, TTypes...>> TupleT<TType, TTypes...>::operator=(Ref<TupleT<UTypes...>> rhs) noexcept
    {
        LockstepApply([&rhs](auto& lhs_element, const auto& rhs_element) { lhs_element = rhs_element; }, *this, rhs);

        return *this;
    }

    template <typename TType, typename... TTypes>
    template <typename... UTypes, typename TSelfList, Details::EnableIfTupleConvertingMoveAssignment<TSelfList, Templates::TypeList<UTypes...>>>
    constexpr MutableRef<TupleT<TType, TTypes...>> TupleT<TType, TTypes...>::operator=(MoveRef<TupleT<UTypes...>> rhs) noexcept
    {
        LockstepApply([&rhs](auto& lhs_element, auto&& rhs_element) { lhs_element = Move(rhs_element); }, *this, rhs);

        return *this;
    }

    template <typename TType, typename... TTypes>
    constexpr MutableRef<TupleT<TType, TTypes...>> TupleT<TType, TTypes...>::Swap(MutableRef<TupleT> rhs) noexcept
    {
        Syntropy::Swap(element_, rhs.element_);

        static_cast<TBaseClass&>(*this).Swap(static_cast<TBaseClass&>(rhs));

        return *this;
    }

    constexpr TupleT<>& TupleT<>::Swap(TupleT<>& rhs) noexcept
    {
        return *this;
    }

    // 
    // Non-member functions.
    // =====================

    template <typename... TTypes, typename... UTypes>
    constexpr Bool operator==(Ref<TupleT<TTypes...>> lhs, Ref<TupleT<UTypes...>> rhs) noexcept
    {
        auto result = true;

        LockstepApply([&result](const auto& lhs_element, const auto& rhs_element)
        {
            result &= (lhs_element == rhs_element);
        }, lhs, rhs);

        return result;
    }

    template <Int VIndex, typename... TTypes>
    constexpr MutableRef<Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>> Get(MutableRef<TupleT<TTypes...>> tuple) noexcept
    {
        using TTupleBase = Details::TupleBase<VIndex, TupleT<TTypes...>>;

        return static_cast<MutableRef<TTupleBase>>(tuple).element_;
    }

    template <Int VIndex, typename... TTypes>
    constexpr MoveRef<Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>> Get(MoveRef<TupleT<TTypes...>> tuple) noexcept
    {
        using TTupleBase = Details::TupleBase<VIndex, TupleT<TTypes...>>;
        using TElement = Templates::TupleElement<VIndex, TupleT<TTypes...>>;

        return static_cast<MoveRef<TElement>>(static_cast<MutableRef<TTupleBase>>(tuple).element_);
    }

    template <Int VIndex, typename... TTypes>
    constexpr Ref<Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>> Get(Ref<TupleT<TTypes...>> tuple) noexcept
    {
        using TTupleBase = Details::TupleBase<VIndex, TupleT<TTypes...>>;

        return static_cast<Ref<TTupleBase>>(tuple).element_;
    }

    template <Int VIndex, typename... TTypes>
    constexpr ConstMoveRef<Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>> Get(ConstMoveRef<TupleT<TTypes...>> tuple) noexcept
    {
        using TTupleBase = Details::TupleBase<VIndex, TupleT<TTypes...>>;
        using TElement = Templates::TupleElement<VIndex, TupleT<TTypes...>>;

        return static_cast<ConstMoveRef<TElement>>(static_cast<Ref<TTupleBase>>(tuple).element_);
    }

    template <typename TType, typename... TTypes>
    constexpr MutableRef<TType> Get(MutableRef<TupleT<TTypes...>> tuple) noexcept
    {
        constexpr auto kIndex = Templates::TypeListIndex<TType, Templates::TypeList<TTypes...>>;

        return Get<kIndex>(tuple);
    }

    template <typename TType, typename... TTypes>
    constexpr MoveRef<TType> Get(MoveRef<TupleT<TTypes...>> tuple) noexcept
    {
        constexpr auto kIndex = Templates::TypeListIndex<TType, Templates::TypeList<TTypes...>>;

        return Get<kIndex>(Move(tuple));
    }

    template <typename TType, typename... TTypes>
    constexpr Ref<TType> Get(Ref<TupleT<TTypes...>> tuple) noexcept
    {
        constexpr auto kIndex = Templates::TypeListIndex<TType, Templates::TypeList<TTypes...>>;

        return Get<kIndex>(tuple);
    }

    template <typename TType, typename... TTypes>
    constexpr ConstMoveRef<TType> Get(ConstMoveRef<TupleT<TTypes...>> tuple) noexcept
    {
        constexpr auto kIndex = Templates::TypeListIndex<TType, Templates::TypeList<TTypes...>>;

        return Get<kIndex>(Move(tuple));
    }

    template <typename... TTypes>
    constexpr TupleT<TTypes...> MakeTuple(ForwardRef<TTypes>... elements) noexcept
    {
        return { Forward<TTypes>(elements)... };
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

    template <typename... TTypes>
    constexpr void Swap(MutableRef<TupleT<TTypes...>> lhs, MutableRef<TupleT<TTypes...>> rhs) noexcept
    {
        lhs.Swap(rhs);
    }
}

// ===========================================================================

namespace std
{
    /************************************************************************/
    /* STRUCTURED BINDINGS                                                  */
    /************************************************************************/

    /// \brief Size of a Syntropy::TupleT.
    template <typename... TTypes>
    struct std::tuple_size<Syntropy::TupleT<TTypes...>>
    {
        static constexpr std::size_t value = sizeof...(TTypes);
    };

    /// \brief Type of the VIndex-th element of a Syntropy::TupleT.
    template <std::size_t VIndex, typename... TTypes>
    struct std::tuple_element<VIndex, Syntropy::TupleT<TTypes...>>
    {
        using type = Syntropy::Templates::TypeListElement<VIndex, Syntropy::Templates::TypeList<TTypes...>>;
    };

    /// \brief Get the VIndex-th element of a tuple.
    template <std::size_t VIndex, typename... TTypes>
    decltype(auto) get(Syntropy::Ref<Syntropy::TupleT<TTypes...>> tuple)
    {
        return Syntropy::Get<VIndex>(tuple);
    }

    /// \brief Get the VIndex-th element of a tuple.
    template <std::size_t VIndex, typename... TTypes>
    decltype(auto) get(Syntropy::MoveRef<Syntropy::TupleT<TTypes...>> tuple)
    {
        return Syntropy::Get<VIndex>(Syntropy::Move(tuple));
    }
}

// ===========================================================================