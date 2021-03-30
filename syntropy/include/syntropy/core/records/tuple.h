/// \file tuple.h
///
/// \brief This header is part of Syntropy core module.
///        It contains definitions for tuples.
///
/// \author Raffaele D. Facendola - September 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/templates/concepts.h"
#include "syntropy/language/templates/sequence.h"
#include "syntropy/core/algorithms/compare.h"

#include "syntropy/core/records/record.h"

// ===========================================================================

#include "details/tuple.details.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* TUPLE                                                                */
    /************************************************************************/

    // !IMPORTANT! In VS2019 apparently "explicit" keyword is not
    //             considered when constructors are not declared and defined at
    //             the same time.

     /// \brief Represents a fixed-size collection of heterogeneous elements.
     /// \author Raffaele D. Facendola - September 2020.
    template <typename... TElements>
    struct Tuple;

    /************************************************************************/
    /* CONCEPT                                                              */
    /************************************************************************/

    /// \brief Concept for template arguments that bind to tuples only.
    template <typename TTuple>
    concept IsTuple = Templates::IsTemplateSpecializationOf<TTuple, Tuple>;

    /************************************************************************/
    /* TUPLE                                                                */
    /************************************************************************/

    /// \brief Recursive tuple definition.
    template <typename TElement, typename... TElements>
    struct Tuple<TElement, TElements...> : private Tuple<TElements...>
    {
        template <Int TIndex, IsTuple TTuple>
        friend constexpr Immutable<Records::ElementTypeOf<TIndex, TTuple>>
        Get(Immutable<TTuple> tuple) noexcept;

        template <Int TIndex, IsTuple TTuple>
        friend constexpr Mutable<Records::ElementTypeOf<TIndex, TTuple>>
        Get(Mutable<TTuple> tuple) noexcept;

        template <Int TIndex, IsTuple TTuple>
        friend constexpr Immovable<Records::ElementTypeOf<TIndex, TTuple>>
        Get(Immovable<TTuple> tuple) noexcept;

        template <Int TIndex, IsTuple TTuple>
        friend constexpr Movable<Records::ElementTypeOf<TIndex, TTuple>>
        Get(Movable<TTuple> tuple) noexcept;

    public:

        /// \brief Number of elements in the tuple.
        static constexpr Int
        kRank = sizeof...(TElements) + 1;

        /// \brief Type of the base class.
        using BaseClass = Tuple<TElements...>;

        /// \brief Type of the tuple itself.
        using SelfType = Tuple<TElement, TElements...>;

        /// \brief Element types.
        using ElementTypes = Templates::TypeList<TElement, TElements...>;

        /// \brief Tag type used to construct a tuple element-wise.
        struct ElementwiseTag {};

        /// \brief Tag type used to construct a tuple by unwinding
        ///        another tuple.
        struct UnwindTag {};

        /// \brief Tuple default constructor. Enabled if all elements
        ///        are default-constructible.
        template<typename UElement = TElement>
        requires Details::TupleDefaultConstructor<UElement, TElements...>
        explicit (Details::ExplicitIfTupleDefaultConstructor<UElement, TElements...>)
        constexpr Tuple() noexcept
            : BaseClass{}
            , element_{}
        {

        }

        /// \brief Tuple direct constructor. Enabled if all elements are
        ///        copy-constructible.
        template<typename UElement = TElement>
        requires Details::TupleDirectConstructor<UElement, TElements...>
        constexpr explicit (Details::ExplicitIfTupleDirectConstructor<UElement, TElements...>)
        Tuple(Immutable<TElement> element,
              Immutable<TElements>... elements) noexcept
            : Tuple(ElementwiseTag{}, element, elements...)
        {

        }

        /// \brief Tuple converting constructor. Enabled if all tuple elements
        ///        are copy-constructible.
        template<typename UElement,
                 typename... UElements,
                 Details::EnableIfTupleConvertingConstructor<ElementTypes, UElement, UElements...> = nullptr>
        constexpr explicit (Details::ExplicitIfTupleConvertingConstructor<ElementTypes, UElement, UElements...>)
        Tuple(Forwarding<UElement> element,
              Forwarding<UElements>... elements) noexcept
            : Tuple(ElementwiseTag{},
                    Forward<UElement>(element),
                    Forward<UElements>(elements)...)
        {

        }

        /// \brief Tuple converting copy constructor. Enabled if all tuple
        ///        elements are copy-constructible.
        template<typename UElement,
                 typename... UElements,
                 Details::EnableIfTupleConvertingCopyConstructor<ElementTypes, UElement, UElements...> = nullptr>
        explicit (Details::ExplicitIfTupleConvertingCopyConstructor<ElementTypes, UElement, UElements...>)
        constexpr Tuple(Immutable<Tuple<UElement, UElements...>> rhs) noexcept
            : Tuple(UnwindTag{},
                    Templates::SequenceFor<UElement, UElements...>{},
                    rhs)
        {

        }

        /// \brief Tuple converting move constructor. Enabled if all tuple
        ///        elements are move-constructible.
        template<typename UElement,
                 typename... UElements,
                 Details::EnableIfTupleConvertingMoveConstructor<ElementTypes, UElement, UElements...> = nullptr>
        constexpr explicit (Details::ExplicitIfTupleConvertingMoveConstructor<ElementTypes, UElement, UElements...>)
         Tuple(Movable<Tuple<UElement, UElements...>> rhs) noexcept
            : Tuple(UnwindTag{},
                    Templates::SequenceFor<UElement, UElements...>{},
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
        template<Records::RecordReference TTuple, Int... VIndexes>
        constexpr
        Tuple(UnwindTag,
              Templates::Sequence<VIndexes...>,
              Forwarding<TTuple> tuple) noexcept;

        /// \brief Copy-assignment operator.
        template <typename TSelf = Tuple,
                  typename TSelfList = ElementTypes,
                  Details::EnableIfTupleCopyAssignment<TSelfList> = nullptr>
        constexpr Mutable<Tuple>
        operator=(Templates::ExactOf<Immutable<TSelf>> rhs) noexcept;

        /// \brief Move-assignment operator.
        template <typename TSelf = Tuple,
                  typename TSelfList = ElementTypes,
                  Details::EnableIfTupleMoveAssignment<TSelfList> = nullptr>
        constexpr Mutable<Tuple>
        operator=(Templates::ExactOf<Movable<TSelf>> rhs) noexcept;

        /// \brief Tuple converting copy-assignment operator.
        template <typename... UElements,
                  typename TSelfList = ElementTypes,
                  Details::EnableIfTupleConvertingCopyAssignment<TSelfList, Templates::TypeList<UElements...>> = nullptr>
        constexpr Mutable<Tuple>
        operator=(Immutable<Tuple<UElements...>> rhs) noexcept;

        /// \brief Tuple converting move-assignment operator.
        template <typename... UElements,
                  typename TSelfList = ElementTypes,
                  Details::EnableIfTupleConvertingMoveAssignment<TSelfList, Templates::TypeList<UElements...>> = nullptr>
        constexpr Mutable<Tuple>
        operator=(Movable<Tuple<UElements...>> rhs) noexcept;

        /// \brief Default copy-constructor.
        constexpr
        Tuple(Immutable<Tuple> rhs) noexcept = default;

        /// \brief Default move-constructor.
        constexpr
        Tuple(Movable<Tuple> rhs) noexcept = default;

        /// \brief Head element.
        TElement element_;
    };

    /// \brief Empty tuple.
    /// \author Raffaele D. Facendola - September 2020.
    template <>
    struct Tuple<>
    {
        /// \brief Number of elements in the tuple.
        static constexpr Int
        kRank = 0;

        /// \brief Type of the tuple itself.
        using SelfType = Tuple<>;

        /// \brief Element types.
        using ElementTypes = Templates::TypeList<>;

        /// \brief Default constructor.
        constexpr
        Tuple() noexcept = default;

        /// \brief Default copy constructor.
        constexpr
        Tuple(Immutable<Tuple>) noexcept = default;

        /// \brief Default copy-assignment.
        constexpr Mutable<Tuple>
        operator=(Immutable<Tuple>) noexcept = default;
    };

    /// \brief Deduction rule.
    template <typename... TElements>
    Tuple(TElements...) -> Tuple<TElements...>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Element access.
    // ===============

    /// \brief Access a tuple element by index.
    ///
    /// \remarks Ill-formed if no such element exists.
    template <Int TIndex, IsTuple TTuple>
    [[nodiscard]] constexpr Immutable<Records::ElementTypeOf<TIndex, TTuple>>
    Get(Immutable<TTuple> tuple) noexcept;

    /// \brief Access a tuple element by index.
    ///
    /// \remarks Ill-formed if no such element exists.
    template <Int TIndex, IsTuple TTuple>
    [[nodiscard]] constexpr Mutable<Records::ElementTypeOf<TIndex, TTuple>>
    Get(Mutable<TTuple> tuple) noexcept;

    /// \brief Access a tuple element by index.
    ///
    /// \remarks Ill-formed if no such element exists.
    template <Int TIndex, IsTuple TTuple>
    [[nodiscard]] constexpr Immovable<Records::ElementTypeOf<TIndex, TTuple>>
    Get(Immovable<TTuple> tuple) noexcept;

    /// \brief Access a tuple element by index.
    ///
    /// \remarks Ill-formed if no such element exists.
    template <Int TIndex, IsTuple TTuple>
    [[nodiscard]] constexpr Movable<Records::ElementTypeOf<TIndex, TTuple>>
    Get(Movable<TTuple> tuple) noexcept;

    // Utilities.
    // ==========

    /// \brief Create a tuple deducing template types from arguments.
    template <typename... TElements>
    [[nodiscard]] constexpr Tuple<TElements...>
    MakeTuple(Forwarding<TElements>... elements) noexcept;

    /// \brief Create a tuple of references to the provided arguments.
    template <typename... TElements>
    [[nodiscard]] constexpr Tuple<Mutable<TElements>...>
    Tie(Mutable<TElements>... elements) noexcept;

    /// \brief Create a tuple of perfectly-forwarded elements.
    template <typename... TElements>
    [[nodiscard]] constexpr Tuple<Forwarding<TElements>...>
    ForwardAsTuple(Forwarding<TElements>... elements) noexcept;

    /// \brief Concatenate a set of tuples.
    template <Records::RecordReference... TRecords>
    [[nodiscard]] constexpr auto
    Concatenate(Forwarding<TRecords>... tuples) noexcept
        -> decltype(Details::Concatenate(Forward<TRecords>(tuples)...));

    /// \brief Flatten a tuple recursively.
    template <Records::RecordReference TTuple>
    [[nodiscard]] constexpr auto
    Flatten(Forwarding<TTuple> tuple) noexcept
        -> decltype(Details::Flatten(Forward<TTuple>(tuple)));

    // Swap.
    // =====

    /// \brief Member-wise swap two tuples.
    template <IsTuple TTuple>
    constexpr void
    Swap(Mutable<TTuple> lhs, Mutable<TTuple> rhs) noexcept;

    /// \brief Swap lhs with rhs and return the old value of lhs.
    template <IsTuple TTuple, IsTuple UTuple>
    requires Records::IsSameRank<TTuple, UTuple>
    [[nodiscard]] constexpr TTuple
    Exchange(Mutable<TTuple> lhs, Forwarding<UTuple> rhs) noexcept;

    // Comparison.
    // ===========

    /// \brief Check whether two tuples are member-wise equivalent.
    /// \brief Equivalence doesn't imply equality.
    template <IsTuple TTuple, IsTuple UTuple>
    requires Records::IsSameRank<TTuple, UTuple>
    [[nodiscard]] constexpr Bool
    operator==(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept;

    /// \brief Member-wise compare two tuples.
    template <IsTuple TTuple, IsTuple UTuple>
    requires Records::IsSameRank<TTuple, UTuple>
    [[nodiscard]] constexpr Ordering
    operator<=>(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept;

    /// \brief Check whether two tuples are member-wise equal.
    /// \remarks Equality implies equivalence.
    template <IsTuple TTuple, IsTuple UTuple>
    requires Records::IsSameRank<TTuple, UTuple>
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept;

    /// \brief Check whether two tuples are member-wise equivalent.
    /// \remarks Equivalence doesn't imply equality.
    template <IsTuple TTuple, IsTuple UTuple>
    requires Records::IsSameRank<TTuple, UTuple>
    [[nodiscard]] constexpr Bool
    AreEquivalent(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept;

    /// \brief Member-wise compare two tuples.
    template <IsTuple TTuple, IsTuple UTuple>
    requires Records::IsSameRank<TTuple, UTuple>
    [[nodiscard]] constexpr Ordering
    Compare(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept;

    /************************************************************************/
    /* TYPE TRAITS                                                          */
    /************************************************************************/

    /// \brief Partial template specialization for tuples.
    template <IsTuple TTuple>
    struct Records::RankTrait<TTuple>
    {
        static constexpr Int
        kValue = TTuple::kCount;
    };

    /// \brief Partial template specialization for tuples.
    template <Int TIndex, IsTuple TTuple>
    struct Records::ElementTypeTrait<TIndex, TTuple>
    {
        using
        Type = Templates::ElementOf<TIndex, typename TTuple::ElementTypes>;
    };

}

// ===========================================================================

#include "details/tuple.inl"

// ===========================================================================
