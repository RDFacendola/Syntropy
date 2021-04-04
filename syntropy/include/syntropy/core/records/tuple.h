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
#include "syntropy/core/algorithms/compare.h"

#include "syntropy/core/records/record.h"

#include <iostream>

// ===========================================================================

#include "details/tuple.details.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* TUPLE                                                                */
    /************************************************************************/

     /// \brief Represents a fixed-size collection of heterogeneous elements.
     /// \author Raffaele D. Facendola - September 2020.
    template <typename... TElements>
    class Tuple;

    /// \brief Concept for template arguments that bind to tuples only.
    template <typename TTuple>
    concept IsTuple
        = Templates::IsTemplateSpecializationOf<TTuple, Tuple>;

    /// \brief Recursive tuple definition.
    template <typename TElement, typename... TElements>
    class Tuple<TElement, TElements...> : private Tuple<TElements...>
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

        /// \brief Type of the tuple itself.
        using SelfType = Tuple<TElement, TElements...>;

        /// \brief Element types.
        using ElementTypes = Templates::TypeList<TElement, TElements...>;

    public:

        /// \brief Default constructor.
        template<typename UElement = TElement>
        requires
            Details::TupleDefaultConstructor<UElement, TElements...>
        explicit
            (Details::TupleExplicitDefaultConstructor<UElement, TElements...>)
        constexpr
        Tuple() noexcept;

        /// \brief Direct constructor.
        template<typename... UElements, typename VElements = ElementTypes>
        requires
            Details::TupleDirectConstructor<VElements, UElements...>
        explicit
            (Details::TupleExplicitDirectConstructor<VElements, UElements...>)
        constexpr
        Tuple(Forwarding<UElements>... elements) noexcept;

        /// \brief Converting copy constructor.
        template<typename... UElements, typename VElements = ElementTypes>
        requires
            Details::TupleCopyConstructor<VElements, UElements...>
        explicit
            (Details::TupleExplicitCopyConstructor<VElements, UElements...>)
        constexpr
        Tuple(Immutable<Tuple<UElements...>> rhs) noexcept;

        /// \brief Converting move constructor.
        template<typename... UElements, typename VElements = ElementTypes>
        requires
            Details::TupleMoveConstructor<VElements, UElements...>
        explicit
            (Details::TupleExplicitMoveConstructor<VElements, UElements...>)
        constexpr
        Tuple(Movable<Tuple<UElements...>> rhs) noexcept;

        /// \brief Converting copy assignment operator.
        template <typename... UElements, typename VElements = ElementTypes>
        requires
            Details::TupleCopyAssignment<VElements, UElements...>
        constexpr Mutable<Tuple>
        operator=(Immutable<Tuple<UElements...>> rhs) noexcept;

        /// \brief Converting move assignment operator.
        template <typename... UElements, typename VElements = ElementTypes>
        requires
            Details::TupleMoveAssignment<VElements, UElements...>
        constexpr Mutable<Tuple>
        operator=(Movable<Tuple<UElements...>> rhs) noexcept;

    private:

        /// \brief Tag type used to construct a tuple directly.
        struct DirectTag {};

        /// \brief Tag type used to element-wise-construct a tuple.
        struct ElementwiseTag {};

        /// \brief Construct a tuple forwarding explicit arguments.
        template<typename UElement, typename... UElements>
        constexpr
        Tuple(DirectTag,
              Forwarding<UElement> element,
              Forwarding<UElements>... elements) noexcept;

        /// \brief Construct a tuple unwinding another tuple elements.
        template<typename TTuple, Int... TSequence>
        constexpr
        Tuple(ElementwiseTag,
              Templates::Sequence<TSequence...>,
              Forwarding<TTuple> tuple) noexcept;

        /// \brief Head element.
        TElement element_;

    };

    /// \brief Empty tuple.
    /// \author Raffaele D. Facendola - September 2020.
    template <>
    class Tuple<>
    {
    public:

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
    Concatenate(Forwarding<TRecords>... records) noexcept;

    /// \brief Flatten a tuple recursively.
    template <Records::RecordReference TRecord>
    [[nodiscard]] constexpr auto
    Flatten(Forwarding<TRecord> record) noexcept;

    /// \brief Flatten a tuple recursively.
    template <typename TElement>
    [[nodiscard]] constexpr auto
    Flatten(Forwarding<TElement> element) noexcept;

    // Copy.
    // =====

    /// \brief Member-wise copy a tuple to another one with the same rank.
    template <IsTuple TTuple, IsTuple UTuple>
    requires Records::IsSameRank<TTuple, UTuple>
    constexpr void
    Copy(Mutable<TTuple> destination, Immutable<UTuple> source) noexcept;

    /// \brief Member-wise move a tuple to another one with the same rank.
    template <IsTuple TTuple, IsTuple UTuple>
    requires Records::IsSameRank<TTuple, UTuple>
    constexpr void
    Move(Mutable<TTuple> destination, Immutable<UTuple> source) noexcept;

    /// \brief Member-wise move a tuple to another one with the same rank.
    template <IsTuple TTuple, IsTuple UTuple>
    requires Records::IsSameRank<TTuple, UTuple>
    constexpr void
    Move(Mutable<TTuple> destination, Movable<UTuple> source) noexcept;

    /// \brief Member-wise copy a tuple to another one, until either is
    ///        exhausted.
    /// \return Returns the number of copied elements.
    template <IsTuple TTuple, IsTuple UTuple>
    constexpr Int
    PartialCopy(Mutable<TTuple> destination, Immutable<UTuple> source)
    noexcept;

    /// \brief Member-wise move a tuple to another one, until either is
    ///        exhausted.
    /// \return Returns the number of copied elements.
    template <IsTuple TTuple, IsTuple UTuple>
    constexpr Int
    PartialMove(Mutable<TTuple> destination, Immutable<UTuple> source)
    noexcept;

    /// \brief Member-wise move a tuple to another one, until either is
    ///        exhausted.
    /// \return Returns the number of copied elements.
    template <IsTuple TTuple, IsTuple UTuple>
    constexpr Int
    PartialMove(Mutable<TTuple> destination, Movable<UTuple> source)
    noexcept;

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
        : Templates::IntConstant<TTuple::kCount> {};

    /// \brief Partial template specialization for tuples.
    template <Int TIndex, IsTuple TTuple>
    struct Records::ElementTypeTrait<TIndex, TTuple>
        : Templates::Alias<
            Templates::ElementTypeOf<TIndex,
                                     typename TTuple::ElementTypes>> {};

}

// ===========================================================================

#include "details/tuple.inl"

// ===========================================================================
