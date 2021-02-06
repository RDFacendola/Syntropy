/// \file tuple.h
///
/// \brief This header is part of Syntropy core module.
///        It contains definitions for tuples.
///
/// \author Raffaele D. Facendola - Sep 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/templates/concepts.h"
#include "syntropy/language/templates/sequence.h"
#include "syntropy/language/support/compare.h"

#include "syntropy/core/concepts/ntuple.h"
#include "syntropy/core/foundation/details/tuple_details.h"

// ===========================================================================

namespace Syntropy::Tuples
{
    /************************************************************************/
    /* TUPLE                                                                */
    /************************************************************************/

    // !IMPORTANT! In VS2019 apparently "explicit" keyword is not
    //             considered when
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
        friend constexpr
        Immutable<Templates::ElementType<VIndex, Tuple<TElements...>>>
        Get(Immutable<Tuple<TElements...>> tuple) noexcept;

        template <Int VIndex, typename... TElements>
        friend constexpr
        Mutable<Templates::ElementType<VIndex, Tuple<TElements...>>>
        Get(Mutable<Tuple<TElements...>> tuple) noexcept;

        template <Int VIndex, typename... TElements>
        friend constexpr
        Immovable<Templates::ElementType<VIndex, Tuple<TElements...>>>
        Get(Immovable<Tuple<TElements...>> tuple) noexcept;

        template <Int VIndex, typename... TElements>
        friend constexpr
        Movable<Templates::ElementType<VIndex, Tuple<TElements...>>>
        Get(Movable<Tuple<TElements...>> tuple) noexcept;

    public:

        /// \brief Type of the base class.
        using BaseClass = Tuple<TElements...>;

        /// \brief Types of tuple elements.
        using ElementList = Syntropy::Templates::TypeList<TElement,
                                                          TElements...>;

        /// \brief Types of arguments.
        template <typename... UElements>
        using ArgumentList = Syntropy::Templates::TypeList<UElements...>;

        /// \brief Tag type used to construct a tuple element-wise.
        struct ElementwiseTag {};

        /// \brief Tag type used to construct a tuple by unwinding
        ///        another tuple.
        struct UnwindTag {};

        /// \brief Tuple default constructor. Enabled if all elements
        ///        are default-constructible.
        template<
            typename UElement = TElement,
            Details::EnableIfTupleDefaultConstructor<
                ArgumentList<UElement, TElements...>> = nullptr>
        explicit (Details::ExplicitIfTupleDefaultConstructor
            <UElement, TElements...>)
        constexpr Tuple() noexcept
            : BaseClass{}
            , element_{}
        {

        }

        /// \brief Tuple direct constructor. Enabled if all elements are
        ///        copy-constructible.
        template<
            typename UElement = TElement,
            Details::EnableIfTupleDirectConstructor<
                ArgumentList<UElement, TElements...>> = nullptr>
        constexpr explicit (Details::ExplicitIfTupleDirectConstructor
            <UElement, TElements...>)
        Tuple(Immutable<TElement> element,
              Immutable<TElements>... elements) noexcept
            : Tuple(ElementwiseTag{}, element, elements...)
        {

        }

        /// \brief Tuple converting constructor. Enabled if all tuple elements
        ///        are copy-constructible.
        template<
            typename UElement,
            typename... UElements,
            Details::EnableIfTupleConvertingConstructor
                <ElementList, UElement, UElements...> = nullptr>
        constexpr explicit (Details::ExplicitIfTupleConvertingConstructor
            < ElementList, UElement, UElements...>)
        Tuple(Forwarding<UElement> element,
              Forwarding<UElements>... elements) noexcept
            : Tuple(ElementwiseTag{},
                    Forward<UElement>(element),
                    Forward<UElements>(elements)...)
        {

        }

        /// \brief Tuple converting copy constructor. Enabled if all tuple
        ///        elements are copy-constructible.
        template<
            typename UElement,
            typename... UElements,
            Details::EnableIfTupleConvertingCopyConstructor
                <ElementList, UElement, UElements...> = nullptr>
        explicit (Details::ExplicitIfTupleConvertingCopyConstructor
            <ElementList, UElement, UElements...>)
        constexpr Tuple(Immutable<Tuple<UElement, UElements...>> rhs) noexcept
            : Tuple(UnwindTag{},
                    Syntropy::Templates::SequenceFor<UElement, UElements...>,
                    rhs)
        {

        }

        /// \brief Tuple converting move constructor. Enabled if all tuple
        ///        elements are move-constructible.
        template<
            typename UElement,
            typename... UElements,
            Details::EnableIfTupleConvertingMoveConstructor
                <ElementList, UElement, UElements...> = nullptr>
        constexpr explicit (Details::ExplicitIfTupleConvertingMoveConstructor
            <ElementList, UElement, UElements...>)
         Tuple(Movable<Tuple<UElement, UElements...>> rhs) noexcept
            : Tuple(UnwindTag{},
                    Syntropy::Templates::SequenceFor<UElement, UElements...>,
                    Move(rhs))
        {

        }

        /// \brief Construct a tuple forwarding explicit arguments.
        template<typename UElement, typename... UElements>
        constexpr
        Tuple(ElementwiseTag,
              Forwarding<UElement> element,
              Forwarding<UElements>... elements) noexcept;

        /// \brief Construct a tuple unwinding another tuple elements.
        template<Concepts::NTupleReference TTuple, Int... VIndexes>
        constexpr
        Tuple(UnwindTag,
              Syntropy::Templates::Sequence<VIndexes...>,
              Forwarding<TTuple> tuple) noexcept;

        /// \brief Copy-assignment operator.
        template <
            typename TSelf = Tuple,
            typename TSelfList = ElementList,
            Details::EnableIfTupleCopyAssignment<TSelfList> = nullptr>
        constexpr
        Mutable<Tuple> operator=(
            Syntropy::Templates::Identity<Immutable<TSelf>> rhs) noexcept;

        /// \brief Move-assignment operator.
        template <
            typename TSelf = Tuple,
            typename TSelfList = ElementList,
            Details::EnableIfTupleMoveAssignment<TSelfList> = nullptr>
        constexpr Mutable<Tuple>
        operator=(Syntropy::Templates::Identity<Movable<TSelf>> rhs) noexcept;

        /// \brief Tuple converting copy-assignment operator.
        template <
            typename... UElements,
            typename TSelfList = ElementList,
            Details::EnableIfTupleConvertingCopyAssignment
                <TSelfList, ArgumentList<UElements...>> = nullptr>
        constexpr Mutable<Tuple>
            operator=(Immutable<Tuple<UElements...>> rhs) noexcept;

        /// \brief Tuple converting move-assignment operator.
        template <
            typename... UElements,
            typename TSelfList = ElementList,
            Details::EnableIfTupleConvertingMoveAssignment
                <TSelfList, ArgumentList<UElements...>> = nullptr>
        constexpr Mutable<Tuple>
            operator=(Movable<Tuple<UElements...>> rhs) noexcept;

        /// \brief Default copy-constructor.
        constexpr
        Tuple(Immutable<Tuple> rhs) noexcept  = default;

        /// \brief Default move-constructor.
        constexpr
        Tuple(Movable<Tuple> rhs) noexcept  = default;

        /// \brief Head element.
        TElement element_;
    };

    /// \brief Empty tuple.
    /// \author Raffaele D. Facendola - September 2020.
    template <>
    struct Tuple<>
    {
        /// \brief Default constructor.
        constexpr
        Tuple() noexcept = default;

        /// \brief Default copy constructor.
        constexpr
        Tuple(Immutable<Tuple>) noexcept = default;

        /// \brief Default copy-assignment.
        constexpr Tuple&
        operator=(Immutable<Tuple>) noexcept = default;
    };

    /// \brief Deduction rule.
    template <typename... TElements>
    Tuple(TElements...) -> Tuple<TElements...>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Comparison.
    // ===========

    /// \brief Check whether two tuples compare equal.
    ///
    /// \return Returns true if each element in lhs compares equal to the
    ///         corresponding element in rhs, returns false otherwise.
    template <typename... TElements, typename... UElements>
    [[nodiscard]] constexpr Bool
    operator==(Immutable<Tuple<TElements...>> lhs,
               Immutable<Tuple<UElements...>> rhs) noexcept;

    /// \brief Compare two tuples lexicographically.
    template <typename... TElements, typename... UElements>
    [[nodiscard]] constexpr Ordering
    operator<=>(Immutable<Tuple<TElements...>> lhs,
                Immutable<Tuple<UElements...>> rhs) noexcept;

    // NTuple.
    // =======

    /// \brief Access the VIndex-th element in a tuple.
    ///
    /// \remarks The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TElements>
    [[nodiscard]] constexpr
    Immutable<Templates::ElementType<VIndex, Tuple<TElements...>>>
    Get(Immutable<Tuple<TElements...>> tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple.
    ///
    /// \remarks The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TElements>
    [[nodiscard]] constexpr
    Mutable<Templates::ElementType<VIndex, Tuple<TElements...>>>
    Get(Mutable<Tuple<TElements...>> tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple.
    ///
    /// \remarks The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TElements>
    [[nodiscard]] constexpr
    Immovable<Templates::ElementType<VIndex, Tuple<TElements...>>>
    Get(Immovable<Tuple<TElements...>> tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple.
    ///
    /// \remarks The program is ill-formed if no such element exists.
    template <Int VIndex, typename... TElements>
    [[nodiscard]] constexpr
    Movable<Templates::ElementType<VIndex, Tuple<TElements...>>>
    Get(Movable<Tuple<TElements...>> tuple) noexcept;

    /// \brief Access an element of a tuple by type.
    ///
    /// \remarks The program is ill-formed unless the tuple has exactly one
    ///          occurrence of TElement.
    template <typename TElement, typename... TElements>
    [[nodiscard]] constexpr Immutable<TElement>
    Get(Immutable<Tuple<TElements...>> tuple) noexcept;

    /// \brief Access an element of a tuple by type.
    ///
    /// \remarks The program is ill-formed unless the tuple has exactly one
    ///          occurrence of TElement.
    template <typename TElement, typename... TElements>
    [[nodiscard]] constexpr Mutable<TElement>
    Get(Mutable<Tuple<TElements...>> tuple) noexcept;

    /// \brief Access an element of a tuple by type.
    /// \remarks The program is ill-formed unless the tuple has exactly one
    ///          occurrence of TElement.
    template <typename TElement, typename... TElements>
    [[nodiscard]] constexpr Immovable<TElement>
    Get(Immovable<Tuple<TElements...>> tuple) noexcept;

    /// \brief Access an element of a tuple by type.
    /// \remarks The program is ill-formed unless the tuple has exactly one
    ///          occurrence of TElement.
    template <typename TElement, typename... TElements>
    [[nodiscard]] constexpr Movable<TElement>
    Get(Movable<Tuple<TElements...>> tuple) noexcept;

    // Utilities.
    // ==========

    /// \brief Create a tuple instance, deducing template types from arguments.
    template <typename... TElements>
    [[nodiscard]] constexpr Tuple<TElements...>
    MakeTuple(Forwarding<TElements>... elements) noexcept;

    /// \brief Create a tuple of lvalue references to provided arguments.
    template <typename... TElements>
    [[nodiscard]] constexpr Tuple<Mutable<TElements>...>
    Tie(Mutable<TElements>... elements) noexcept;

    /// \brief Create a tuple of the perfectly-forwarded elements provided.
    template <typename... TElements>
    [[nodiscard]] constexpr Tuple<Forwarding<TElements>...>
    ForwardAsTuple(Forwarding<TElements>... elements) noexcept;

    /// \brief Concatenate a set of tuples.
    template <Concepts::NTupleReference... TTuples>
    [[nodiscard]] constexpr decltype(auto)
    Concatenate(Forwarding<TTuples>... tuples) noexcept;

    /// \brief Flatten a tuple recursively.
    template <Concepts::NTupleReference TTuple>
    [[nodiscard]] constexpr decltype(auto)
    Flatten(Forwarding<TTuple> tuple) noexcept;

    // Swap.
    // =====

    /// \brief Member-wise swap two tuples.
    template <typename... TElements, typename... UElements>
    requires (sizeof...(TElements) == sizeof...(UElements))
    constexpr void
    Swap(Mutable<Tuple<TElements...>> lhs,
         Mutable<Tuple<UElements...>> rhs) noexcept;

    /// \brief Swap lhs with rhs and return the old value of lhs.
    template <typename... TElements, typename... UElements>
    requires (sizeof...(TElements) == sizeof...(UElements))
    [[nodiscard]] constexpr Tuple<TElements...>
    Exchange(Mutable<Tuple<TElements...>> lhs,
             Immutable<Tuple<UElements...>> rhs) noexcept;

    /// \brief Swap lhs with rhs and return the old value of lhs.
    template <typename... TElements, typename... UElements>
    requires (sizeof...(TElements) == sizeof...(UElements))
    [[nodiscard]] constexpr Tuple<TElements...>
    Exchange(Mutable<Tuple<TElements...>> lhs,
             Movable<Tuple<UElements...>> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy::Tuples::Templates
{
    /************************************************************************/
    /* TYPE TRAITS                                                          */
    /************************************************************************/

    /// \brief Partial template specialization for tuples.
    template <Int VIndex, typename... TElements>
    struct ElementTypeTraits<VIndex, Tuples::Tuple<TElements...>>
        : Syntropy::Templates::Alias<
            Syntropy::Templates::TypeListElement<
                VIndex,
                Syntropy::Templates::TypeList<TElements...>>> {};

    /// \brief Partial template specialization for tuples.
    template <typename... TElements>
    struct RankTypeTraits<Tuples::Tuple<TElements...>>
        : Syntropy::Templates::IntConstant<sizeof...(TElements)> {};
}

// ===========================================================================

#include "tuple.inl"

// ===========================================================================
