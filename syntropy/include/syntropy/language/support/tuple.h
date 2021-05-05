/// \file tuple.h
///
/// \brief This header is part of Syntropy core module.
///        It contains definitions for tuples.
///
/// \author Raffaele D. Facendola - September 2020.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/templates/concepts.h"
#include "syntropy/core/comparisons/compare.h"

// ===========================================================================

#include "details/tuple.details.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* TUPLES TYPE TRAITS                                                   */
    /************************************************************************/

    /// \brief Number of elements in a tuple.
    template <typename TTuple>
    struct TupleRank
    {
        // : Syntropy::Templates::IntType<rank>
    };

    /// \brief Type of an element in a tuple, by index.
    template <Int TIndex, typename TTuple>
    struct TupleElementType
    {
        // : Syntropy::Templates::Alias<element type>;
    };

    /// \brief Concept for a tuple whose elements can be accessed at
    ///        compile-time by index.
    template <typename TTuple>
    concept IsTuple = requires
    {
        /// \brief Rank of the tuple.
        { TupleRank<TTuple>::kValue } -> Templates::IsIntegral;
    };

    /// \brief Concept for a reference to a tuple.
    template <typename TTuple>
    concept IsTupleReference
        = IsTuple<Templates::UnqualifiedOf<TTuple>>;

    /// \brief Tag type associated to tuples.
    struct TupleTag{};

    /// \brief Number of elements in a tuple.
    template <IsTupleReference TTuple>
    inline constexpr
    Int TupleRankOf
        = TupleRank<Templates::UnqualifiedOf<TTuple>>::kValue;

    /// \brief Type of a tuple element.
    ///
    /// \remarks Ill-formed if exceeding tuple bounds.
    template <Int TIndex, IsTupleReference TTuple>
    using TupleElementTypeOf
        = typename TupleElementType<TIndex,
                                    Templates::UnqualifiedOf<TTuple>>::Type;

    /// \brief True if two tuples have the same rank, false otherwise.
    template <IsTupleReference TTuple, IsTupleReference UTuple>
    inline constexpr
    Bool TupleIsSameRank
        = (TupleRankOf<TTuple> == TupleRankOf<UTuple>);

    /// \brief Generates a sequence that can be used to enumerate all
    ///        elements in a tuple.
    template <IsTupleReference TTuple>
    using TupleSequenceOf
        = Templates::MakeSequence<TupleRankOf<TTuple>>;

    /// \brief List of types of a tuple's elements.
    template <IsTupleReference TTuple>
    using TupleElementTypeListOf
        = Details::TupleElementTypeListOf<TTuple,
                                          TupleSequenceOf<TTuple>,
                                          TupleElementType>;

    /************************************************************************/
    /* TUPLE                                                                */
    /************************************************************************/

     /// \brief Represents a fixed-size collection of heterogeneous elements.
     /// \author Raffaele D. Facendola - September 2020.
    template <typename... TElements>
    class Tuple;

    /// \brief Recursive tuple definition.
    template <typename TElement, typename... TElements>
    class Tuple<TElement, TElements...> : private Tuple<TElements...>
    {
        template <Int TIndex, typename... UElements>
        friend constexpr decltype(auto)
        Get(Immutable<Tuple<UElements...>> tuple) noexcept;

        template <Int TIndex, typename... UElements>
        friend constexpr decltype(auto)
        Get(Mutable<Tuple<UElements...>> tuple) noexcept;

        template <Int TIndex, typename... UElements>
        friend constexpr decltype(auto)
        Get(Immovable<Tuple<UElements...>> tuple) noexcept;

        template <Int TIndex, typename... UElements>
        friend constexpr decltype(auto)
        Get(Movable<Tuple<UElements...>> tuple) noexcept;

    public:

        /// \brief Number of elements in the tuple.
        static constexpr Int
        kRank = sizeof...(TElements) + 1;

        /// \brief Type of the tuple itself.
        using SelfType = Tuple<TElement, TElements...>;

        /// \brief Type of the tuple base class.
        using BaseType = Tuple<TElements...>;

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

        /// \brief Default destructor.
        ~Tuple() noexcept = default;

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

        /// \brief Construct a tuple from another tuple.
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

        /// \brief Default destructor.
        ~Tuple() noexcept = default;

    };

    /// \brief Deduction rule.
    template <typename... TElements>
    Tuple(TElements...) -> Tuple<TElements...>;

    /************************************************************************/
    /* TUPLES EXTENSIONS                                                    */
    /************************************************************************/

    namespace Tuples::Extensions
    {
        /// \brief Access a tuple element by index.
        template <Int TIndex, typename TTuple, typename = void>
        struct Get;
    }

    /************************************************************************/
    /* TUPLES                                                               */
    /************************************************************************/

    namespace Tuples
    {
        /// \brief Index of the first element with type TElement in a tuple.
        ///
        /// \remarks Ill-formed if no such element exist.
        template <typename TElement, IsTupleReference TTuple>
        inline constexpr
        Int ElementIndexOf
            = Templates::ElementIndexOf<TElement, TupleElementTypeListOf<TTuple>>;

        /// \brief Access a tuple element by index.
        ///
        /// \remarks Ill-formed if the index exceeds tuple's bounds.
        template <Int TIndex, IsTupleReference TTuple>
        [[nodiscard]] constexpr decltype(auto)
        Get(Forwarding<TTuple> tuple) noexcept;

        /// \brief Access the first element of type TElement in a tuple.
        ///
        /// \remarks Ill-formed if no such element exist.
        template <typename TElement, IsTupleReference TTuple>
        [[nodiscard]] constexpr decltype(auto)
        Get(Forwarding<TTuple> tuple) noexcept;

        /// \brief Invoke a function with arguments provided in form of tuple.
        template <typename TFunction, IsTupleReference TTuple>
        constexpr decltype(auto)
        Apply(Forwarding<TFunction> function, Forwarding<TTuple> tuple)
        noexcept;

        /// \brief Apply a function to each tuple elements individually.
        template <typename TFunction, IsTupleReference TTuple>
        constexpr void
        ForEachApply(Forwarding<TFunction> function,
                     Forwarding<TTuple> tuple) noexcept;

        /// \brief Apply a function to the index-th element of each tuple at
        ///        once.
        template <Int TIndex, typename TFunction, IsTupleReference... TTuples>
        constexpr decltype(auto)
        ProjectApply(Forwarding<TFunction> function,
                     Forwarding<TTuples>... tuples) noexcept;

        /// \brief Apply a function to each argument list generated by
        ///        projecting the i-th tuples elements, for each index up to
        ///        the minimum rank among those tuples.
        template <typename TFunction, IsTupleReference... TTuples>
        constexpr void
        LockstepApply(Forwarding<TFunction> function,
                      Forwarding<TTuples>... tuples) noexcept;

        /// \brief Create a new instance of TType using TTuple as constructor
        ///        arguments.
        template <typename TType, IsTupleReference TTuple>
        [[nodiscard]] constexpr TType
        MakeFromTuple(Forwarding<TTuple> tuple) noexcept;

        /// \brief Member-wise copy a tuple to another one with the same rank.
        template <IsTuple TTuple, IsTuple UTuple>
        constexpr void
        Copy(Mutable<TTuple> destination, Immutable<UTuple> source)
        noexcept;

        /// \brief Member-wise move a tuple to another one with the same rank.
        template <IsTuple TTuple, IsTupleReference UTuple>
        constexpr void
        Move(Mutable<TTuple> destination, Forwarding<UTuple> source)
        noexcept;

        /// \brief Member-wise swap two tuples.
        template <IsTuple TTuple, IsTuple UTuple>
        constexpr void
        Swap(Mutable<TTuple> lhs, Mutable<UTuple> rhs) noexcept;

        /// \brief Member-wise swap two tuples and returns the previous value
        ///        of the former.
        template <IsTuple TTuple, IsTupleReference UTuple>
        [[nodiscard]] constexpr TTuple
        Exchange(Mutable<TTuple> lhs, Forwarding<UTuple> rhs) noexcept;

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
        template <IsTuple TTuple, IsTupleReference UTuple>
        constexpr Int
        PartialMove(Mutable<TTuple> destination, Forwarding<UTuple> source)
        noexcept;

        /// \brief Member-wise swap two tuples until either is exhausted.
        ///
        /// \return Returns the number of swapped elements.
        template <IsTuple TTuple, IsTuple UTuple>
        constexpr Int
        PartialSwap(Mutable<TTuple> lhs, Mutable<UTuple> rhs) noexcept;

        /// \brief Check whether two tuples are member-wise equal.
        /// \remarks Equality implies equivalence.
        template <IsTuple TTuple, IsTuple UTuple>
        [[nodiscard]] constexpr Bool
        AreEqual(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept;

        /// \brief Check whether two tuple are member-wise equivalent.
        /// \remarks Equivalence doesn't imply equality.
        template <IsTuple TTuple, IsTuple UTuple>
        [[nodiscard]] constexpr Bool
        AreEquivalent(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept;

        /// \brief Member-wise compare two tuples lexicographically.
        template <IsTuple TTuple, IsTuple UTuple>
        [[nodiscard]] constexpr Ordering
        Compare(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept;

    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Element access.
    // ===============

    /// \brief Access a tuple element by index.
    ///
    /// \remarks Ill-formed if no such element exists.
    template <Int TIndex, typename... UElements>
    [[nodiscard]] constexpr decltype(auto)
    Get(Immutable<Tuple<UElements...>> tuple) noexcept;

    /// \brief Access a tuple element by index.
    ///
    /// \remarks Ill-formed if no such element exists.
    template <Int TIndex, typename... UElements>
    [[nodiscard]] constexpr decltype(auto)
    Get(Mutable<Tuple<UElements...>> tuple) noexcept;

    /// \brief Access a tuple element by index.
    ///
    /// \remarks Ill-formed if no such element exists.
    template <Int TIndex, typename... UElements>
    [[nodiscard]] constexpr decltype(auto)
    Get(Immovable<Tuple<UElements...>> tuple) noexcept;

    /// \brief Access a tuple element by index.
    ///
    /// \remarks Ill-formed if no such element exists.
    template <Int TIndex, typename... UElements>
    [[nodiscard]] constexpr decltype(auto)
    Get(Movable<Tuple<UElements...>> tuple) noexcept;

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
    template <IsTupleReference... TTuples>
    [[nodiscard]] constexpr auto
    Concatenate(Forwarding<TTuples>... tuples) noexcept;

    /// \brief Flatten a tuple recursively.
    template <IsTupleReference TTuple>
    [[nodiscard]] constexpr auto
    Flatten(Forwarding<TTuple> tuple) noexcept;

    /// \brief Flatten a tuple recursively.
    template <typename TElement>
    [[nodiscard]] constexpr auto
    Flatten(Forwarding<TElement> element) noexcept;

    // Swap.
    // =====

    /// \brief Member-wise swap two tuples.
    template <typename... TTypes, typename... UTypes>
    requires (sizeof...(TTypes) == sizeof...(UTypes))
    constexpr void
    Swap(Mutable<Tuple<TTypes...>> lhs,
         Mutable<Tuple<UTypes...>> rhs) noexcept;

    /// \brief Member-wise swap two tuples and returns the previous value of
    ///        the former.
    template <typename... TTypes, typename... UTypes>
    requires (sizeof...(TTypes) == sizeof...(UTypes))
    [[nodiscard]] constexpr Tuple<TTypes...>
    Exchange(Mutable<Tuple<TTypes...>> lhs,
             Forwarding<Tuple<UTypes...>> rhs) noexcept;

    // Comparison.
    // ===========

    /// \brief Check whether two tuples are member-wise equivalent.
    /// \brief Equivalence doesn't imply equality.
    template <typename... TTypes, typename... UTypes>
    requires (sizeof...(TTypes) == sizeof...(UTypes))
    [[nodiscard]] constexpr Bool
    operator==(Immutable<Tuple<TTypes...>> lhs,
               Immutable<Tuple<UTypes...>> rhs) noexcept;

    /// \brief Member-wise compare two tuples.
    template <typename... TTypes, typename... UTypes>
    requires (sizeof...(TTypes) == sizeof...(UTypes))
    [[nodiscard]] constexpr Ordering
    operator<=>(Immutable<Tuple<TTypes...>> lhs,
                Immutable<Tuple<UTypes...>> rhs) noexcept;

    /************************************************************************/
    /* TYPE TRAITS                                                          */
    /************************************************************************/

    /// \brief Partial template specialization for tuples.
    template <typename... TTypes>
    struct TupleRank<Tuple<TTypes...>>
        : Templates::IntType<sizeof...(TTypes)> {};

    /// \brief Partial template specialization for tuples.
    template <Int TIndex, typename... TTypes>
    struct TupleElementType<TIndex, Tuple<TTypes...>>
        : Templates::Alias<
            Templates::ElementTypeOf<TIndex,
                                     Templates::TypeList<TTypes...>>> {};

}

// ===========================================================================

namespace std
{
    /************************************************************************/
    /* STRUCTURED BINDINGS                                                  */
    /************************************************************************/

    /// \brief Rank of a tuple.
    template <Syntropy::IsTuple TTuple>
    struct std::tuple_size<TTuple>;

    /// \brief Type of a tuple element, by index.
    template <std::size_t TIndex,
              Syntropy::IsTuple TTuple>
    struct std::tuple_element<TIndex, TTuple>;

    /// \brief Access a tuple element, by index.
    template <std::size_t TIndex,
              Syntropy::IsTuple TTuple>
    decltype(auto) get(Syntropy::Immutable<TTuple> tuple);

    /// \brief Access a tuple element, by index..
    template <std::size_t TIndex,
              Syntropy::IsTuple TTuple>
    decltype(auto) get(Syntropy::Movable<TTuple> tuple);
}

// ===========================================================================

#include "details/tuple.inl"

// ===========================================================================
