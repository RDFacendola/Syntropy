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
    /* TYPE TRAITS                                                          */
    /************************************************************************/

    /// \brief Number of elements in a record.
    template <typename TRecord>
    struct TupleRank
    {
        // : Syntropy::Templates::IntType<rank>
    };

    /// \brief Type of an element in a record, by index.
    template <Int TIndex, typename TRecord>
    struct TupleElementType
    {
        // : Syntropy::Templates::Alias<element type>;
    };

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
        /// \brief Access a record element by index.
        template <Int TIndex, typename TRecord, typename = void>
        struct Get;
    }

    /************************************************************************/
    /* RECORD                                                               */
    /************************************************************************/

    /// \brief Concept for a record whose elements can be accessed at
    ///        compile-time by index.
    template <typename TRecord>
    concept Record = requires
    {
        /// \brief Rank of the record.
        { TupleRank<TRecord>::kValue } -> Templates::IsIntegral;
    };

    /// \brief Concept for a reference to a record.
    template <typename TRecord>
    concept RecordReference
        = Record<Templates::UnqualifiedOf<TRecord>>;

    /// \brief Tag type associated to records.
    struct RecordTag{};

    /************************************************************************/
    /* RECORDS                                                              */
    /************************************************************************/

    namespace Records
    {
        /// \brief Number of elements in a record.
        template <RecordReference TRecord>
        inline constexpr
        Int RankOf
            = TupleRank<Templates::UnqualifiedOf<TRecord>>::kValue;

        /// \brief True if two records have the same rank, false otherwise.
        template <RecordReference TRecord, RecordReference URecord>
        inline constexpr
        Bool IsSameRank
            = (RankOf<TRecord> == RankOf<URecord>);

        /// \brief Type of a record element.
        ///
        /// \remarks Ill-formed if exceeding record bounds.
        template <Int TIndex, RecordReference TRecord>
        using ElementTypeOf
            = typename TupleElementType<
                TIndex, Templates::UnqualifiedOf<TRecord>>::Type;

        /// \brief Generates a sequence that can be used to enumerate all
        ///        elements in a record.
        template <RecordReference TRecord>
        using SequenceOf
            = Templates::MakeSequence<RankOf<TRecord>>;

        /// \brief List of types of a record's elements.
        template <RecordReference TRecord>
        using ElementTypeListOf
            = Details::ElementTypeListOf<TRecord,
                                         SequenceOf<TRecord>,
                                         TupleElementType>;

        /// \brief Index of the first element with type TElement in a record.
        ///
        /// \remarks Ill-formed if no such element exist.
        template <typename TElement, RecordReference TRecord>
        inline constexpr
        Int ElementIndexOf
            = Templates::ElementIndexOf<TElement, ElementTypeListOf<TRecord>>;

        /// \brief Access a record element by index.
        ///
        /// \remarks Ill-formed if the index exceeds record's bounds.
        template <Int TIndex, RecordReference TRecord>
        [[nodiscard]] constexpr decltype(auto)
        Get(Forwarding<TRecord> record) noexcept;

        /// \brief Access the first element of type TElement in a record.
        ///
        /// \remarks Ill-formed if no such element exist.
        template <typename TElement, RecordReference TRecord>
        [[nodiscard]] constexpr decltype(auto)
        Get(Forwarding<TRecord> record) noexcept;

        /// \brief Invoke a function with arguments provided in form of record.
        template <typename TFunction, RecordReference TRecord>
        constexpr decltype(auto)
        Apply(Forwarding<TFunction> function, Forwarding<TRecord> record)
        noexcept;

        /// \brief Apply a function to each record elements individually.
        template <typename TFunction, RecordReference TRecord>
        constexpr void
        ForEachApply(Forwarding<TFunction> function,
                     Forwarding<TRecord> record) noexcept;

        /// \brief Apply a function to the index-th element of each record at
        ///        once.
        template <Int TIndex, typename TFunction, RecordReference... TRecords>
        constexpr decltype(auto)
        ProjectApply(Forwarding<TFunction> function,
                     Forwarding<TRecords>... records) noexcept;

        /// \brief Apply a function to each argument list generated by
        ///        projecting the i-th records elements, for each index up to
        ///        the minimum rank among those records.
        template <typename TFunction, RecordReference... TRecords>
        constexpr void
        LockstepApply(Forwarding<TFunction> function,
                      Forwarding<TRecords>... records) noexcept;

        /// \brief Create a new instance of TType using TRecord as constructor
        ///        arguments.
        template <typename TType, RecordReference TRecord>
        [[nodiscard]] constexpr TType
        MakeFromRecord(Forwarding<TRecord> record) noexcept;

        /// \brief Member-wise copy a record to another one with the same rank.
        template <Record TRecord, Record URecord>
        constexpr void
        Copy(Mutable<TRecord> destination, Immutable<URecord> source)
        noexcept;

        /// \brief Member-wise move a record to another one with the same rank.
        template <Record TRecord, RecordReference URecord>
        constexpr void
        Move(Mutable<TRecord> destination, Forwarding<URecord> source)
        noexcept;

        /// \brief Member-wise swap two records.
        template <Record TRecord, Record URecord>
        constexpr void
        Swap(Mutable<TRecord> lhs, Mutable<URecord> rhs) noexcept;

        /// \brief Member-wise swap two records and returns the previous value
        ///        of the former.
        template <Record TRecord, RecordReference URecord>
        [[nodiscard]] constexpr TRecord
        Exchange(Mutable<TRecord> lhs, Forwarding<URecord> rhs) noexcept;

        /// \brief Member-wise copy a record to another one, until either is
        ///        exhausted.
        /// \return Returns the number of copied elements.
        template <Record TRecord, Record URecord>
        constexpr Int
        PartialCopy(Mutable<TRecord> destination, Immutable<URecord> source)
        noexcept;

        /// \brief Member-wise move a record to another one, until either is
        ///        exhausted.
        /// \return Returns the number of copied elements.
        template <Record TRecord, RecordReference URecord>
        constexpr Int
        PartialMove(Mutable<TRecord> destination, Forwarding<URecord> source)
        noexcept;

        /// \brief Member-wise swap two records until either is exhausted.
        ///
        /// \return Returns the number of swapped elements.
        template <Record TRecord, Record URecord>
        constexpr Int
        PartialSwap(Mutable<TRecord> lhs, Mutable<URecord> rhs) noexcept;

        /// \brief Check whether two records are member-wise equal.
        /// \remarks Equality implies equivalence.
        template <Record TRecord, Record URecord>
        [[nodiscard]] constexpr Bool
        AreEqual(Immutable<TRecord> lhs, Immutable<URecord> rhs) noexcept;

        /// \brief Check whether two record are member-wise equivalent.
        /// \remarks Equivalence doesn't imply equality.
        template <Record TRecord, Record URecord>
        [[nodiscard]] constexpr Bool
        AreEquivalent(Immutable<TRecord> lhs, Immutable<URecord> rhs) noexcept;

        /// \brief Member-wise compare two records lexicographically.
        template <Record TRecord, Record URecord>
        [[nodiscard]] constexpr Ordering
        Compare(Immutable<TRecord> lhs, Immutable<URecord> rhs) noexcept;

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
    template <RecordReference... TRecords>
    [[nodiscard]] constexpr auto
    Concatenate(Forwarding<TRecords>... records) noexcept;

    /// \brief Flatten a tuple recursively.
    template <RecordReference TRecord>
    [[nodiscard]] constexpr auto
    Flatten(Forwarding<TRecord> record) noexcept;

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

    /// \brief Rank of a record.
    template <Syntropy::Record TRecord>
    struct std::tuple_size<TRecord>;

    /// \brief Type of a record element, by index.
    template <std::size_t TIndex,
              Syntropy::Record TRecord>
    struct std::tuple_element<TIndex, TRecord>;

    /// \brief Access a record element, by index.
    template <std::size_t TIndex,
              Syntropy::Record TRecord>
    decltype(auto) get(Syntropy::Immutable<TRecord> record);

    /// \brief Access a record element, by index..
    template <std::size_t TIndex,
              Syntropy::Record TRecord>
    decltype(auto) get(Syntropy::Movable<TRecord> record);
}

// ===========================================================================

#include "details/tuple.inl"

// ===========================================================================
