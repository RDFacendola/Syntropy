/// \file tuple.h
/// \brief This header is part of Syntropy core module. It contains definitions for tuples.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/templates/type_list.h"

#include "syntropy/language/foundation.h"
#include "syntropy/language/templates/templates.h"
#include "syntropy/language/templates/sequences.h"

#include "syntropy/core/algorithm/algorithm.h"

#include "syntropy/experimental/core/foundation/details/tuple_details.h"

// ===========================================================================

namespace Syntropy::Experimental::Templates
{
    /************************************************************************/
    /* TUPLE ELEMENT LIST                                                   */
    /************************************************************************/

    /// \brief Type alias equal to a type list of all elements in TTuple.
    template <typename TTuple>
    using TupleElementList = Details::TupleElementList<Syntropy::Templates::RemoveConstReference<TTuple>>;

    /************************************************************************/
    /* TUPLE ELEMENT                                                        */
    /************************************************************************/

    /// \brief Provides indexed access to tuple elements' types.
    template <Int VIndex, typename TTuple>
    using TupleElement = Details::TupleElement<VIndex, Syntropy::Templates::RemoveConstReference<TTuple>>;

    /************************************************************************/
    /* TUPLE POP FRONT                                                      */
    /************************************************************************/

    /// \brief Discards the first VCount elements in a tuple and provides a type alias equal to a tuple with the remaining elements.
    template <Int VCount, typename TTuple>
    using TuplePopFront = Details::TuplePopFront<VCount, Syntropy::Templates::RemoveConstReference<TTuple>>;

    /************************************************************************/
    /* TUPLE SIZE                                                           */
    /************************************************************************/

    /// \brief Constant equal to the size (rank) of a tuple.
    template <typename TTuple>
    inline constexpr Int TupleSize = Details::TupleSize<Syntropy::Templates::RemoveConstReference<TTuple>>;
}

// ===========================================================================

namespace Syntropy::Experimental
{
    /************************************************************************/
    /* TUPLE <ELEMENTS...>                                                  */
    /************************************************************************/

    // !IMPORTANT! In VS2019 apparently "explicit" keyword is not considered when 
    //             constructors are not declared and defined at the same time.

     /// \brief Represents a fixed-size collection of heterogeneous elements.
     /// \author Raffaele D. Facendola - September 2020.
    template <typename... TTypes>
    struct Tuple;

    /// \brief Recursive tuple definition.
    template <typename TType, typename... TTypes>
    struct Tuple<TType, TTypes...> : private Tuple<TTypes...>
    {
        template <Int VIndex, typename... TTypes>
        friend constexpr Templates::TupleElement<VIndex, Tuple<TTypes...>>& Get(Tuple<TTypes...>& tuple) noexcept;

        template <Int VIndex, typename... TTypes>
        friend constexpr Templates::TupleElement<VIndex, Tuple<TTypes...>>&& Get(Tuple<TTypes...>&& tuple) noexcept;

        template <Int VIndex, typename... TTypes>
        friend constexpr const Templates::TupleElement<VIndex, Tuple<TTypes...>>& Get(const Tuple<TTypes...>& tuple) noexcept;

        template <Int VIndex, typename... TTypes>
        friend constexpr const Templates::TupleElement<VIndex, Tuple<TTypes...>>&& Get(const Tuple<TTypes...>&& tuple) noexcept;

    public:

        /// \brief Type of the base class.
        using TBaseClass = Tuple<TTypes...>;

        /// \brief Types of tuple elements.
        using TTypeList = Syntropy::Templates::TypeList<TType, TTypes...>;

        /// \brief Tag type used to construct a tuple element-wise.
        struct ElementwiseConstructor {};

        /// \brief Tag type used to construct a tuple by unwinding another tuple.
        struct UnwindConstructor {};

        /// \brief Tuple default constructor. Enabled if all elements are default-constructible.
        template<typename UType = TType, Details::EnableIfTupleDefaultConstructor<Syntropy::Templates::TypeList<UType, TTypes...>> = nullptr>
        explicit (Details::ExplicitIfTupleDefaultConstructor<UType, TTypes...>)
        constexpr Tuple() noexcept
            : TBaseClass{}
            , element_{}
        {

        }

        /// \brief Tuple direct constructor. Enabled if all elements are copy-constructible.
        template<typename UType = TType, Details::EnableIfTupleDirectConstructor<Syntropy::Templates::TypeList<UType, TTypes...>> = nullptr>
        explicit (Details::ExplicitIfTupleDirectConstructor<UType, TTypes...>)
        constexpr Tuple(const TType& element, const TTypes&... elements) noexcept
            : Tuple(ElementwiseConstructor{}, Forward<TType>(element), Forward<TTypes>(elements)...)
        {

        }

        /// \brief Tuple converting constructor. Enabled if all tuple elements are copy-constructible.
        template<typename UType, typename... UTypes, Details::EnableIfTupleConvertingConstructor<TTypeList, UType, UTypes...> = nullptr>
        explicit (Details::ExplicitIfTupleConvertingConstructor<TTypeList, UType, UTypes...>)
        constexpr Tuple(UType&& element, UTypes&&... elements) noexcept
            : Tuple(ElementwiseConstructor{}, Forward<UType>(element), Forward<UTypes>(elements)...)
        {

        }

        /// \brief Tuple converting copy constructor. Enabled if all tuple elements are copy-constructible.
        template<typename UType, typename... UTypes, Details::EnableIfTupleConvertingCopyConstructor<TTypeList, UType, UTypes...> = nullptr>
        explicit (Details::ExplicitIfTupleConvertingCopyConstructor<TTypeList, UType, UTypes...>)
        constexpr Tuple(const Tuple<UType, UTypes...>& rhs) noexcept
            : Tuple(UnwindConstructor{}, Syntropy::Templates::IntegerSequenceFor<UType, UTypes...>, rhs)
        {

        }

        /// \brief Tuple converting copy constructor. Enabled if all tuple elements are copy-constructible.
        template<typename UType, typename... UTypes, Details::EnableIfTupleConvertingMoveConstructor<TTypeList, UType, UTypes...> = nullptr>
        explicit (Details::ExplicitIfTupleConvertingMoveConstructor<TTypeList, UType, UTypes...>)
        constexpr Tuple(Tuple<UType, UTypes...>&& rhs) noexcept
            : Tuple(UnwindConstructor{}, Syntropy::Templates::IntegerSequenceFor<UType, UTypes...>, Move(rhs))
        {

        }

        /// \brief Swap this tuple with rhs by means of element-wise Swap.
        constexpr Tuple& Swap(Tuple& rhs) noexcept;

        /// \brief Fallback case for when no assignment operator could be found.
        constexpr Tuple& operator=(const volatile Tuple&) = delete;

        /// \brief Copy-assignment operator.
        template <typename TSelf = Tuple, typename TSelfList = TTypeList, Details::EnableIfTupleCopyAssignment<TSelfList> = nullptr>
        constexpr Tuple& operator=(Syntropy::Templates::Identity<const TSelf&> rhs) noexcept;

        /// \brief Move-assignment operator.
        template <typename TSelf = Tuple, typename TSelfList = TTypeList, Details::EnableIfTupleMoveAssignment<TSelfList> = nullptr>
        constexpr Tuple& operator=(Syntropy::Templates::Identity<TSelf&&> rhs) noexcept;

        /// \brief Tuple converting copy-assignment operator.
        template <typename... UTypes, typename TSelfList = TTypeList, Details::EnableIfTupleConvertingCopyAssignment<TSelfList, Syntropy::Templates::TypeList<UTypes...>> = nullptr>
        constexpr Tuple& operator=(const Tuple<UTypes...>& rhs) noexcept;

        /// \brief Tuple converting move-assignment operator.
        template <typename... UTypes, typename TSelfList = TTypeList, Details::EnableIfTupleConvertingMoveAssignment<TSelfList, Syntropy::Templates::TypeList<UTypes...>> = nullptr>
        constexpr Tuple& operator=(Tuple<UTypes...>&& rhs) noexcept;

        /// \brief Construct a tuple forwarding explicit arguments.
        template<typename UType, typename... UTypes>
        constexpr Tuple(ElementwiseConstructor, UType&& element, UTypes&&... elements) noexcept;

        /// \brief Construct a tuple unwinding another tuple elements.
        template<typename TTuple, Int... VIndexes>
        constexpr Tuple(UnwindConstructor, Syntropy::Templates::IntegerSequence<VIndexes...>, TTuple&& tuple) noexcept;

        /// \brief Default copy-constructor.
        constexpr Tuple(const Tuple& other) = default;

        /// \brief Default move-constructor.
        constexpr Tuple(Tuple&& other) = default;

        /// \brief Head element.
        TType element_;
    };

    /// \brief Empty tuple.
    /// \author Raffaele D. Facendola - September 2020.
    template <>
    struct Tuple<>
    {
        /// \brief Default constructor.
        constexpr Tuple() noexcept = default;

        /// \brief Default copy constructor.
        constexpr Tuple(const Tuple&) noexcept = default;

        /// \brief Default copy-assignment.
        constexpr Tuple& operator=(const Tuple&) noexcept = default;

        /// \brief Swap this tuple with rhs by means of element-wise Swap.
        constexpr Tuple& Swap(Tuple& rhs) noexcept;
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Compare two tuples.
    /// \return Returns true if each element in lhs compares equal to the corresponding element in rhs, returns false otherwise.
    template <typename... TTypes, typename... UTypes>
    constexpr Bool operator==(const Tuple<TTypes...>& lhs, const Tuple<UTypes...>& rhs) noexcept;

    /// \brief Access the VIndex-th element in a tuple. The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TTypes>
    constexpr Templates::TupleElement<VIndex, Tuple<TTypes...>>& Get(Tuple<TTypes...>& tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple. The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TTypes>
    constexpr Templates::TupleElement<VIndex, Tuple<TTypes...>>&& Get(Tuple<TTypes...>&& tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple. The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TTypes>
    constexpr const Templates::TupleElement<VIndex, Tuple<TTypes...>>& Get(const Tuple<TTypes...>& tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple. The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TTypes>
    constexpr const Templates::TupleElement<VIndex, Tuple<TTypes...>>&& Get(const Tuple<TTypes...>&& tuple) noexcept;

    /// \brief Access an element of a tuple by type. The program is ill-formed unless the tuple has exactly one occurrence of TType.
    template <typename TType, typename... TTypes>
    constexpr TType& Get(Tuple<TTypes...>& tuple) noexcept;

    /// \brief Access an element of a tuple by type. The program is ill-formed unless the tuple has exactly one occurrence of TType.
    template <typename TType, typename... TTypes>
    constexpr TType&& Get(Tuple<TTypes...>&& tuple) noexcept;

    /// \brief Access an element of a tuple by type. The program is ill-formed unless the tuple has exactly one occurrence of TType.
    template <typename TType, typename... TTypes>
    constexpr const TType& Get(const Tuple<TTypes...>& tuple) noexcept;

    /// \brief Access an element of a tuple by type. The program is ill-formed unless the tuple has exactly one occurrence of TType.
    template <typename TType, typename... TTypes>
    constexpr const TType&& Get(const Tuple<TTypes...>&& tuple) noexcept;

    /// \brief Swap two tuples by means of element-wise Swap.
    template <typename... TTypes>
    constexpr void Swap(Tuple<TTypes...>& lhs, Tuple<TTypes...>& rhs) noexcept;

    /// \brief Create a tuple instance, deducing template types from arguments.
    template <typename... TTypes>
    constexpr Tuple<TTypes...> MakeTuple(TTypes&&... elements) noexcept;

    /// \brief Create a tuple of lvalue references to provided arguments.
    template <typename... TElements>
    constexpr Tuple<TElements&...> Tie(TElements&... elements) noexcept;

    /// \brief Create a tuple of the perfectly-forwarded elements provided.
    template <typename... TElements>
    constexpr Tuple<TElements&&...> ForwardAsTuple(TElements&&... elements) noexcept;

    /// \brief Project the VIndex-th element of the provided tuples, in the same order, and apply a function to the argument list generated this way.
    template <Int VIndex, typename TFunction, typename... TTuples>
    constexpr decltype(auto) ProjectionApply(TFunction&& function, TTuples&&... tuples) noexcept;

    /// \brief Apply a function to all argument list generated by projecting the i-th element of all provided tuples, in the same order, for each index i.
    template <typename TFunction, typename TTuple, typename... TTuples>
    constexpr void LockstepApply(TFunction&& function, TTuple&& tuple, TTuples&&... tuples) noexcept;

}

// ===========================================================================

namespace Syntropy::Experimental
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Tuple.
    // ======

    template <typename TType, typename... TTypes>
    template <typename UType, typename... UTypes>
    constexpr Tuple<TType, TTypes...>::Tuple(ElementwiseConstructor, UType&& element, UTypes&&... elements) noexcept
        : TBaseClass(Forward<UTypes>(elements)...)
        , element_(Forward<UType>(element))
    {

    }

    template <typename TType, typename... TTypes>
    template<typename TTuple, Int... VIndexes>
    constexpr Tuple<TType, TTypes...>::Tuple(UnwindConstructor, Syntropy::Templates::IntegerSequence<VIndexes...>, TTuple&& tuple) noexcept
        : Tuple(ElementwiseConstructor, Get<VIndexes>(Forward<TTuple>(tuple))...)
    {

    }

    template <typename TType, typename... TTypes>
    template <typename TSelf, typename TSelfList, Details::EnableIfTupleCopyAssignment<TSelfList>>
    constexpr Tuple<TType, TTypes...>& Tuple<TType, TTypes...>::operator=(Syntropy::Templates::Identity<const TSelf&> rhs) noexcept
    {
        LockstepApply([&rhs](auto& lhs_element, const auto& rhs_element){ lhs_element = rhs_element; }, *this, rhs);

        return *this;
    }

    template <typename TType, typename... TTypes>
    template <typename TSelf, typename TSelfList, Details::EnableIfTupleMoveAssignment<TSelfList>>
    constexpr Tuple<TType, TTypes...>& Tuple<TType, TTypes...>::operator=(Syntropy::Templates::Identity<TSelf&&> rhs) noexcept
    {
        LockstepApply([&rhs](auto& lhs_element, auto&& rhs_element) { lhs_element = Move(rhs_element); }, *this, rhs);

        return *this;
    }

    template <typename TType, typename... TTypes>
    template <typename... UTypes, typename TSelfList, Details::EnableIfTupleConvertingCopyAssignment<TSelfList, Syntropy::Templates::TypeList<UTypes...>>>
    constexpr Tuple<TType, TTypes...>& Tuple<TType, TTypes...>::operator=(const Tuple<UTypes...>& rhs) noexcept
    {
        LockstepApply([&rhs](auto& lhs_element, const auto& rhs_element) { lhs_element = rhs_element; }, *this, rhs);

        return *this;
    }

    template <typename TType, typename... TTypes>
    template <typename... UTypes, typename TSelfList, Details::EnableIfTupleConvertingMoveAssignment<TSelfList, Syntropy::Templates::TypeList<UTypes...>>>
    constexpr Tuple<TType, TTypes...>& Tuple<TType, TTypes...>::operator=(Tuple<UTypes...>&& rhs) noexcept
    {
        LockstepApply([&rhs](auto& lhs_element, auto&& rhs_element) { lhs_element = Move(rhs_element); }, *this, rhs);

        return *this;
    }

    template <typename TType, typename... TTypes>
    constexpr Tuple<TType, TTypes...>& Tuple<TType, TTypes...>::Swap(Tuple<TType, TTypes...>& rhs) noexcept
    {
        using Syntropy::Algorithm::Swap;

        Swap(element_, rhs.element_);

        static_cast<TBaseClass&>(*this).Swap(static_cast<TBaseClass&>(rhs));

        return *this;
    }

    constexpr Tuple<>& Tuple<>::Swap(Tuple<>& rhs) noexcept
    {
        return *this;
    }

    // 
    // Non-member functions.
    // =====================

    template <typename... TTypes, typename... UTypes>
    constexpr Bool operator==(const Tuple<TTypes...>& lhs, const Tuple<UTypes...>& rhs) noexcept
    {
        auto result = true;

        LockstepApply([&result](const auto& lhs_element, const auto& rhs_element)
        {
            result &= (lhs_element == rhs_element);
        }, lhs, rhs);

        return result;
    }

    template <Int VIndex, typename... TTypes>
    constexpr Templates::TupleElement<VIndex, Tuple<TTypes...>>& Get(Tuple<TTypes...>& tuple) noexcept
    {
        using TTuple = Templates::TuplePopFront<VIndex, Tuple<TTypes...>>;

        return static_cast<TTuple&>(tuple).element_;
    }

    template <Int VIndex, typename... TTypes>
    constexpr Templates::TupleElement<VIndex, Tuple<TTypes...>>&& Get(Tuple<TTypes...>&& tuple) noexcept
    {
        using TTuple = Templates::TuplePopFront<VIndex, Tuple<TTypes...>>;
        using TElement = Templates::TupleElement<VIndex, Tuple<TTypes...>>;

        return static_cast<TElement&&>(static_cast<TTuple&>(tuple).element_);
    }

    template <Int VIndex, typename... TTypes>
    constexpr const Templates::TupleElement<VIndex, Tuple<TTypes...>>& Get(const Tuple<TTypes...>& tuple) noexcept
    {
        using TTuple = Templates::TuplePopFront<VIndex, Tuple<TTypes...>>;

        return static_cast<const TTuple&>(tuple).element_;
    }

    template <Int VIndex, typename... TTypes>
    constexpr const Templates::TupleElement<VIndex, Tuple<TTypes...>>&& Get(const Tuple<TTypes...>&& tuple) noexcept
    {
        using TTuple = Templates::TuplePopFront<VIndex, Tuple<TTypes...>>;
        using TElement = Templates::TupleElement<VIndex, Tuple<TTypes...>>;

        return static_cast<const TElement&&>(static_cast<const TTuple&>(tuple).element_);
    }

    template <typename TType, typename... TTypes>
    constexpr TType& Get(Tuple<TTypes...>& tuple) noexcept
    {
        constexpr auto kIndex = Syntropy::Templates::TypeListIndex<TType, Syntropy::Templates::TypeList<TTypes...>>;

        return Get<kIndex>(tuple);
    }

    template <typename TType, typename... TTypes>
    constexpr TType&& Get(Tuple<TTypes...>&& tuple) noexcept
    {
        constexpr auto kIndex = Syntropy::Templates::TypeListIndex<TType, Syntropy::Templates::TypeList<TTypes...>>;

        return Get<kIndex>(Move(tuple));
    }

    template <typename TType, typename... TTypes>
    constexpr const TType& Get(const Tuple<TTypes...>& tuple) noexcept
    {
        constexpr auto kIndex = Syntropy::Templates::TypeListIndex<TType, Syntropy::Templates::TypeList<TTypes...>>;

        return Get<kIndex>(tuple);
    }

    template <typename TType, typename... TTypes>
    constexpr const TType&& Get(const Tuple<TTypes...>&& tuple) noexcept
    {
        constexpr auto kIndex = Syntropy::Templates::TypeListIndex<TType, Syntropy::Templates::TypeList<TTypes...>>;

        return Get<kIndex>(Move(tuple));
    }

    template <typename... TTypes>
    constexpr void Swap(Tuple<TTypes...>& lhs, Tuple<TTypes...>& rhs) noexcept
    {
        lhs.Swap(rhs);
    }

    template <typename... TTypes>
    constexpr Tuple<TTypes...> MakeTuple(TTypes&&... elements) noexcept
    {
        return { Forward<TTypes>(elements)... };
    }

    template <typename... TElements>
    constexpr Tuple<TElements&...> Tie(TElements&... elements) noexcept
    {
        return Tuple<TElements&...>(elements...);
    }

    template <typename... TElements>
    constexpr Tuple<TElements&&...> ForwardAsTuple(TElements&&... elements) noexcept
    {
        return Tuple<TElements&&...>(Forward<TElements>(elements)...);
    }

    template <Int VIndex, typename TFunction, typename... TTuples>
    constexpr decltype(auto) ProjectionApply(TFunction&& function, TTuples&&... tuples) noexcept
    {
        return function(Get<VIndex>(Forward<TTuples>(tuples))...);
    }

     template <typename TFunction, typename TTuple, typename... TTuples>
     constexpr void LockstepApply(TFunction&& function, TTuple&& tuple, TTuples&&... tuples) noexcept
     {
          static_assert(((Templates::TupleSize<TTuple> == Templates::TupleSize<TTuples>) && ...), "Tuples must have the same size.");
  
          using TSequence = Syntropy::Templates::MakeIntegerSequence<Templates::TupleSize<TTuple>>;

          Details::LockstepApply(TSequence{}, Forward<TFunction>(function), Forward<TTuple>(tuple), Forward<TTuples>(tuples)...);
     }

}

// ===========================================================================