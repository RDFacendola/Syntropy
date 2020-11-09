/// \file tuple.h
/// \brief This header is part of Syntropy language module. It contains definitions for tuples.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/traits.h"
#include "syntropy/language/templates/functional.h"

#include "syntropy/language/support/details/tuple_details.h"

#include "syntropy/language/support/support.h"

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
        friend constexpr Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>& Get(TupleT<TTypes...>& tuple) noexcept;

        template <Int VIndex, typename... TTypes>
        friend constexpr Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>&& Get(TupleT<TTypes...>&& tuple) noexcept;

        template <Int VIndex, typename... TTypes>
        friend constexpr const Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>& Get(const TupleT<TTypes...>& tuple) noexcept;

        template <Int VIndex, typename... TTypes>
        friend constexpr const Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>&& Get(const TupleT<TTypes...>&& tuple) noexcept;

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
        constexpr TupleT(const TType& element, const TTypes&... elements) noexcept
            : TupleT(ElementwiseConstructor{}, Forward<TType>(element), Forward<TTypes>(elements)...)
        {

        }

        /// \brief Tuple converting constructor. Enabled if all tuple elements are copy-constructible.
        template<typename UType, typename... UTypes, Details::EnableIfTupleConvertingConstructor<TTypeList, UType, UTypes...> = nullptr>
        explicit (Details::ExplicitIfTupleConvertingConstructor<TTypeList, UType, UTypes...>)
        constexpr TupleT(UType&& element, UTypes&&... elements) noexcept
            : TupleT(ElementwiseConstructor{}, Forward<UType>(element), Forward<UTypes>(elements)...)
        {

        }

        /// \brief Tuple converting copy constructor. Enabled if all tuple elements are copy-constructible.
        template<typename UType, typename... UTypes, Details::EnableIfTupleConvertingCopyConstructor<TTypeList, UType, UTypes...> = nullptr>
        explicit (Details::ExplicitIfTupleConvertingCopyConstructor<TTypeList, UType, UTypes...>)
        constexpr TupleT(const TupleT<UType, UTypes...>& rhs) noexcept
            : TupleT(UnwindConstructor{}, Templates::SequenceFor<UType, UTypes...>, rhs)
        {

        }

        /// \brief Tuple converting copy constructor. Enabled if all tuple elements are copy-constructible.
        template<typename UType, typename... UTypes, Details::EnableIfTupleConvertingMoveConstructor<TTypeList, UType, UTypes...> = nullptr>
        explicit (Details::ExplicitIfTupleConvertingMoveConstructor<TTypeList, UType, UTypes...>)
        constexpr TupleT(TupleT<UType, UTypes...>&& rhs) noexcept
            : TupleT(UnwindConstructor{}, Templates::SequenceFor<UType, UTypes...>, Move(rhs))
        {

        }

        /// \brief Swap this tuple with rhs by means of element-wise Swap.
        constexpr TupleT& Swap(TupleT& rhs) noexcept;

        /// \brief Fallback case for when no assignment operator could be found.
        constexpr TupleT& operator=(const volatile TupleT&) = delete;

        /// \brief Copy-assignment operator.
        template <typename TSelf = TupleT, typename TSelfList = TTypeList, Details::EnableIfTupleCopyAssignment<TSelfList> = nullptr>
        constexpr TupleT& operator=(Templates::Identity<const TSelf&> rhs) noexcept;

        /// \brief Move-assignment operator.
        template <typename TSelf = TupleT, typename TSelfList = TTypeList, Details::EnableIfTupleMoveAssignment<TSelfList> = nullptr>
        constexpr TupleT& operator=(Templates::Identity<TSelf&&> rhs) noexcept;

        /// \brief Tuple converting copy-assignment operator.
        template <typename... UTypes, typename TSelfList = TTypeList, Details::EnableIfTupleConvertingCopyAssignment<TSelfList, Templates::TypeList<UTypes...>> = nullptr>
        constexpr TupleT& operator=(const TupleT<UTypes...>& rhs) noexcept;

        /// \brief Tuple converting move-assignment operator.
        template <typename... UTypes, typename TSelfList = TTypeList, Details::EnableIfTupleConvertingMoveAssignment<TSelfList, Templates::TypeList<UTypes...>> = nullptr>
        constexpr TupleT& operator=(TupleT<UTypes...>&& rhs) noexcept;

        /// \brief Construct a tuple forwarding explicit arguments.
        template<typename UType, typename... UTypes>
        constexpr TupleT(ElementwiseConstructor, UType&& element, UTypes&&... elements) noexcept;

        /// \brief Construct a tuple unwinding another tuple elements.
        template<typename TTuple, Int... VIndexes>
        constexpr TupleT(UnwindConstructor, Templates::Sequence<VIndexes...>, TTuple&& tuple) noexcept;

        /// \brief Default copy-constructor.
        constexpr TupleT(const TupleT& other) = default;

        /// \brief Default move-constructor.
        constexpr TupleT(TupleT&& other) = default;

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
        constexpr TupleT(const TupleT&) noexcept = default;

        /// \brief Default copy-assignment.
        constexpr TupleT& operator=(const TupleT&) noexcept = default;

        /// \brief Swap this tuple with rhs by means of element-wise Swap.
        constexpr TupleT& Swap(TupleT& rhs) noexcept;
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
    constexpr Bool operator==(const TupleT<TTypes...>& lhs, const TupleT<UTypes...>& rhs) noexcept;

    /// \brief Access the VIndex-th element in a tuple. The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TTypes>
    constexpr Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>& Get(TupleT<TTypes...>& tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple. The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TTypes>
    constexpr Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>&& Get(TupleT<TTypes...>&& tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple. The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TTypes>
    constexpr const Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>& Get(const TupleT<TTypes...>& tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple. The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TTypes>
    constexpr const Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>&& Get(const TupleT<TTypes...>&& tuple) noexcept;

    /// \brief Access an element of a tuple by type. The program is ill-formed unless the tuple has exactly one occurrence of TType.
    template <typename TType, typename... TTypes>
    constexpr TType& Get(TupleT<TTypes...>& tuple) noexcept;

    /// \brief Access an element of a tuple by type. The program is ill-formed unless the tuple has exactly one occurrence of TType.
    template <typename TType, typename... TTypes>
    constexpr TType&& Get(TupleT<TTypes...>&& tuple) noexcept;

    /// \brief Access an element of a tuple by type. The program is ill-formed unless the tuple has exactly one occurrence of TType.
    template <typename TType, typename... TTypes>
    constexpr const TType& Get(const TupleT<TTypes...>& tuple) noexcept;

    /// \brief Access an element of a tuple by type. The program is ill-formed unless the tuple has exactly one occurrence of TType.
    template <typename TType, typename... TTypes>
    constexpr const TType&& Get(const TupleT<TTypes...>&& tuple) noexcept;

    /// \brief Create a tuple instance, deducing template types from arguments.
    template <typename... TTypes>
    constexpr TupleT<TTypes...> MakeTuple(TTypes&&... elements) noexcept;

    /// \brief Create a tuple of lvalue references to provided arguments.
    template <typename... TElements>
    constexpr TupleT<TElements&...> Tie(TElements&... elements) noexcept;

    /// \brief Create a tuple of the perfectly-forwarded elements provided.
    template <typename... TElements>
    constexpr TupleT<TElements&&...> ForwardAsTuple(TElements&&... elements) noexcept;

    /// \brief Swap two tuples
    template <typename... TTypes>
    constexpr void Swap(TupleT<TTypes...>& lhs, TupleT<TTypes...>& rhs) noexcept;
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
    constexpr TupleT<TType, TTypes...>::TupleT(ElementwiseConstructor, UType&& element, UTypes&&... elements) noexcept
        : TBaseClass(Forward<UTypes>(elements)...)
        , element_(Forward<UType>(element))
    {

    }

    template <typename TType, typename... TTypes>
    template<typename TTuple, Int... VIndexes>
    constexpr TupleT<TType, TTypes...>::TupleT(UnwindConstructor, Templates::Sequence<VIndexes...>, TTuple&& tuple) noexcept
        : TupleT(ElementwiseConstructor, Get<VIndexes>(Forward<TTuple>(tuple))...)
    {

    }

    template <typename TType, typename... TTypes>
    template <typename TSelf, typename TSelfList, Details::EnableIfTupleCopyAssignment<TSelfList>>
    constexpr TupleT<TType, TTypes...>& TupleT<TType, TTypes...>::operator=(Templates::Identity<const TSelf&> rhs) noexcept
    {
        Templates::ApplyLockstep([&rhs](auto& lhs_element, const auto& rhs_element){ lhs_element = rhs_element; }, *this, rhs);

        return *this;
    }

    template <typename TType, typename... TTypes>
    template <typename TSelf, typename TSelfList, Details::EnableIfTupleMoveAssignment<TSelfList>>
    constexpr TupleT<TType, TTypes...>& TupleT<TType, TTypes...>::operator=(Templates::Identity<TSelf&&> rhs) noexcept
    {
        Templates::ApplyLockstep([&rhs](auto& lhs_element, auto&& rhs_element) { lhs_element = Move(rhs_element); }, *this, rhs);

        return *this;
    }

    template <typename TType, typename... TTypes>
    template <typename... UTypes, typename TSelfList, Details::EnableIfTupleConvertingCopyAssignment<TSelfList, Templates::TypeList<UTypes...>>>
    constexpr TupleT<TType, TTypes...>& TupleT<TType, TTypes...>::operator=(const TupleT<UTypes...>& rhs) noexcept
    {
        Templates::ApplyLockstep([&rhs](auto& lhs_element, const auto& rhs_element) { lhs_element = rhs_element; }, *this, rhs);

        return *this;
    }

    template <typename TType, typename... TTypes>
    template <typename... UTypes, typename TSelfList, Details::EnableIfTupleConvertingMoveAssignment<TSelfList, Templates::TypeList<UTypes...>>>
    constexpr TupleT<TType, TTypes...>& TupleT<TType, TTypes...>::operator=(TupleT<UTypes...>&& rhs) noexcept
    {
        Templates::ApplyLockstep([&rhs](auto& lhs_element, auto&& rhs_element) { lhs_element = Move(rhs_element); }, *this, rhs);

        return *this;
    }

    template <typename TType, typename... TTypes>
    constexpr TupleT<TType, TTypes...>& TupleT<TType, TTypes...>::Swap(TupleT<TType, TTypes...>& rhs) noexcept
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
    constexpr Bool operator==(const TupleT<TTypes...>& lhs, const TupleT<UTypes...>& rhs) noexcept
    {
        auto result = true;

        Templates::ApplyLockstep([&result](const auto& lhs_element, const auto& rhs_element)
        {
            result &= (lhs_element == rhs_element);
        }, lhs, rhs);

        return result;
    }

    template <Int VIndex, typename... TTypes>
    constexpr Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>& Get(TupleT<TTypes...>& tuple) noexcept
    {
        using TTupleBase = Details::TupleBase<VIndex, TupleT<TTypes...>>;

        return static_cast<TTupleBase&>(tuple).element_;
    }

    template <Int VIndex, typename... TTypes>
    constexpr Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>&& Get(TupleT<TTypes...>&& tuple) noexcept
    {
        using TTupleBase = Details::TupleBase<VIndex, TupleT<TTypes...>>;
        using TElement = Templates::TupleElement<VIndex, TupleT<TTypes...>>;

        return static_cast<TElement&&>(static_cast<TTupleBase&>(tuple).element_);
    }

    template <Int VIndex, typename... TTypes>
    constexpr const Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>& Get(const TupleT<TTypes...>& tuple) noexcept
    {
        using TTupleBase = Details::TupleBase<VIndex, TupleT<TTypes...>>;

        return static_cast<const TTupleBase&>(tuple).element_;
    }

    template <Int VIndex, typename... TTypes>
    constexpr const Templates::TypeListElement<VIndex, Templates::TypeList<TTypes...>>&& Get(const TupleT<TTypes...>&& tuple) noexcept
    {
        using TTupleBase = Details::TupleBase<VIndex, TupleT<TTypes...>>;
        using TElement = Templates::TupleElement<VIndex, TupleT<TTypes...>>;

        return static_cast<const TElement&&>(static_cast<const TTupleBase&>(tuple).element_);
    }

    template <typename TType, typename... TTypes>
    constexpr TType& Get(TupleT<TTypes...>& tuple) noexcept
    {
        constexpr auto kIndex = Templates::TypeListIndex<TType, Templates::TypeList<TTypes...>>;

        return Get<kIndex>(tuple);
    }

    template <typename TType, typename... TTypes>
    constexpr TType&& Get(TupleT<TTypes...>&& tuple) noexcept
    {
        constexpr auto kIndex = Templates::TypeListIndex<TType, Templates::TypeList<TTypes...>>;

        return Get<kIndex>(Move(tuple));
    }

    template <typename TType, typename... TTypes>
    constexpr const TType& Get(const TupleT<TTypes...>& tuple) noexcept
    {
        constexpr auto kIndex = Templates::TypeListIndex<TType, Templates::TypeList<TTypes...>>;

        return Get<kIndex>(tuple);
    }

    template <typename TType, typename... TTypes>
    constexpr const TType&& Get(const TupleT<TTypes...>&& tuple) noexcept
    {
        constexpr auto kIndex = Templates::TypeListIndex<TType, Templates::TypeList<TTypes...>>;

        return Get<kIndex>(Move(tuple));
    }

    template <typename... TTypes>
    constexpr TupleT<TTypes...> MakeTuple(TTypes&&... elements) noexcept
    {
        return { Forward<TTypes>(elements)... };
    }

    template <typename... TElements>
    constexpr TupleT<TElements&...> Tie(TElements&... elements) noexcept
    {
        return TupleT<TElements&...>(elements...);
    }

    template <typename... TElements>
    constexpr TupleT<TElements&&...> ForwardAsTuple(TElements&&... elements) noexcept
    {
        return TupleT<TElements&&...>(Forward<TElements>(elements)...);
    }

    template <typename... TTypes>
    constexpr void Swap(TupleT<TTypes...>& lhs, TupleT<TTypes...>& rhs) noexcept
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
    decltype(auto) get(const Syntropy::TupleT<TTypes...>& tuple)
    {
        return Syntropy::Get<VIndex>(tuple);
    }

    /// \brief Get the VIndex-th element of a tuple.
    template <std::size_t VIndex, typename... TTypes>
    decltype(auto) get(Syntropy::TupleT<TTypes...>&& tuple)
    {
        return Syntropy::Get<VIndex>(std::move(tuple));
    }
}

// ===========================================================================