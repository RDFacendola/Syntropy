
/// \file sequence.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for N-tuples concepts and related
///        functionalities.
///
/// \author Raffaele D. Facendola - Nov 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/core/support/compare.h"
#include "syntropy/language/templates/concepts.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/templates/sequence.h"

#include "syntropy/core/support/swap.h"

#include "syntropy/math/math.h"

// ===========================================================================

#include "details/sequence.details.h"

// ===========================================================================

namespace Syntropy::Sequences::Templates
{
    /************************************************************************/
    /* SEQUENCE                                                             */
    /************************************************************************/

    /// \brief Exposes a member integer kValue equal to the rank of
    ///        the n-tuple.
    template <typename TType>
    struct RankTypeTraits;

    /// \brief Provides indexed access to n-tuple elements' types.
    template <Int VIndex, typename TType>
    struct ElementTypeTraits;

    /// \brief Rank of a n-tuple.
    template <typename TType, typename UType
        = Syntropy::Templates::UnqualifiedOf<TType>>
    inline constexpr Int
    Rank = RankTypeTraits<UType>::kValue;

    /// \brief Type of the VIndex-th element of a n-tuple.
    template <Int VIndex, typename TType, typename UType
        = Syntropy::Templates::UnqualifiedOf<TType>>
    using ElementType = typename ElementTypeTraits<VIndex, UType>::Type;

    /// \brief Constant equal to true if TType provides compile-time access
    ///        to its element types, false otherwise.
    template <typename TType, typename UType
        = Syntropy::Templates::UnqualifiedOf<TType>>
    inline constexpr Bool
    HasElementTypes = Details::HasElementTypes<UType>;

    /// \brief Constant equal to true if TType provides access to all its
    ///        Rank elements, false otherwise.
    template <typename TType, typename UType
        = Syntropy::Templates::UnqualifiedOf<TType>>
    inline constexpr Bool
    HasGetters = Details::HasGetters<UType>;
}

// ===========================================================================

namespace Syntropy::Sequences::Concepts
{
    /************************************************************************/
    /* SEQUENCE                                                             */
    /************************************************************************/

    /// \brief Concept for types that behave as tuples, providing indexed
    ///        compile-time access to its elements.
    template <typename TType, typename UType
        = Syntropy::Templates::UnqualifiedOf<TType>>
    concept Sequence = requires
    {
        /// \brief Rank of the tuple.
        { Syntropy::Sequences::Templates::RankTypeTraits<UType>::kValue }
            -> Syntropy::Concepts::IsIntegral;
    }
    && Templates::HasElementTypes<TType>
    && Templates::HasGetters<TType>;

    /// \brief Concept for reference types that behave as tuple.
    /// \remarks This concept is especially useful when working with
    ///          forwarding references to tuple types, where adding a
    ///          reference would cause no type traits to be found.
    template <typename TType>
    concept SequenceReference
        = Sequence<Syntropy::Templates::UnqualifiedOf<TType>>;
}

// ===========================================================================

namespace Syntropy::Sequences::Templates
{
    /************************************************************************/
    /* SEQUENCE                                                             */
    /************************************************************************/

    /// \brief Constant equal to true if all TNSequences have the same rank,
    ///        false otherwise.
    template <Syntropy::Sequences::Concepts::Sequence... TNSequences>
    inline constexpr Bool
    SameRank = Details::SameRank<
        Syntropy::Templates::UnqualifiedOf<TNSequences>...>;

    /// \brief Generates a sequence that can be used to enumerate all elements
    ///        in a given tuple.
    template <Syntropy::Sequences::Concepts::SequenceReference TTuple>
    using TupleSequenceFor = Syntropy::Templates::MakeSequence<Rank<TTuple>>;
}

// ===========================================================================

namespace Syntropy::Sequences
{
    /************************************************************************/
    /* TUPLES                                                               */
    /************************************************************************/

    // Comparison.
    // ===========

    /// \brief Check whether lhs and rhs are equal.
    template <Concepts::Sequence TTuple, Concepts::Sequence UTuple>
    [[nodiscard]] constexpr Bool
    AreEqual(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept;

    /// \brief Check whether lhs and rhs are equivalent.
    template <Concepts::Sequence TTuple, Concepts::Sequence UTuple>
    [[nodiscard]] constexpr Bool
    AreEquivalent(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept;

    /// \brief Compare two n-tuples lexicographically.
    template <Concepts::Sequence TTuple, Concepts::Sequence UTuple>
    [[nodiscard]] constexpr Ordering
    Compare(Immutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept;

    // Swap.
    // =====

    /// \brief Member-wise swap two tuples.
    template <Concepts::Sequence TTuple, Concepts::Sequence UTuple>
    requires (Templates::Rank<TTuple> == Templates::Rank<UTuple>)
    constexpr void
    Swap(Mutable<TTuple> lhs, Mutable<UTuple> rhs) noexcept;

    /// \brief Swap lhs with rhs and return the old value of lhs.
    template <Concepts::Sequence TTuple, Concepts::Sequence UTuple>
    requires (Templates::Rank<TTuple> == Templates::Rank<UTuple>)
    constexpr TTuple
    Exchange(Mutable<TTuple> lhs, Immutable<UTuple> rhs) noexcept;

    /// \brief Swap lhs with rhs and return the old value of lhs.
    template <Concepts::Sequence TTuple, Concepts::Sequence UTuple>
    requires (Templates::Rank<TTuple> == Templates::Rank<UTuple>)
    constexpr TTuple
    Exchange(Mutable<TTuple> lhs, Movable<UTuple> rhs) noexcept;

    // Functional.
    // ===========

    /// \brief Invoke a function with arguments provided in form of n-tuple.
    template <typename TFunction, Concepts::SequenceReference TTuple>
    constexpr decltype(auto)
    Apply(Forwarding<TFunction> function, Forwarding<TTuple> ntuple) noexcept;

    /// \brief Invoke a function to each element in a n-tuple individually.
    template <typename TFunction, Concepts::SequenceReference TTuple>
    constexpr void
    ForEachApply(Forwarding<TFunction> function,
                 Forwarding<TTuple> ntuple) noexcept;

    /// \brief Invoke a function to the VIndex-th element of each provided
    ///        tuple at once.
    template <Int VIndex,
              typename TFunction,
              Concepts::SequenceReference... TSequences>
    constexpr decltype(auto)
    ProjectApply(Forwarding<TFunction> function,
                 Forwarding<TSequences>... tuples) noexcept;

    /// \brief Invoke a function to each argument list generated by projecting
    ///        the i-th element of all the provided tuples at once,
    ///        for each index up to the minimum rank among those tuples.
    template <typename TFunction, Concepts::SequenceReference... TSequences>
    constexpr void
    LockstepApply(Forwarding<TFunction> function,
                  Forwarding<TSequences>... tuples) noexcept;

    /// \brief Create a new instance of type TType using TTuple as constructor
    ///        arguments.
    template <typename TType, Concepts::SequenceReference TTuple>
    [[nodiscard]] constexpr TType
    MakeFromTuple(Forwarding<TTuple> tuple) noexcept;
}

// ===========================================================================

namespace std
{
    /************************************************************************/
    /* STRUCTURED BINDINGS                                                  */
    /************************************************************************/

    /// \brief Size of a Sequence.
    template <Syntropy::Sequences::Concepts::Sequence TTuple>
    struct std::tuple_size<TTuple>;

    /// \brief Type of the VIndex-th element of a Syntropy::Tuple.
    template <std::size_t VIndex, Syntropy::Sequences::Concepts::Sequence TTuple>
    struct std::tuple_element<VIndex, TTuple>;

    /// \brief Get the VIndex-th element of a tuple.
    template <std::size_t VIndex, Syntropy::Sequences::Concepts::Sequence TTuple>
    decltype(auto) get(Syntropy::Immutable<TTuple> tuple);

    /// \brief Get the VIndex-th element of a tuple.
    template <std::size_t VIndex, Syntropy::Sequences::Concepts::Sequence TTuple>
    decltype(auto) get(Syntropy::Movable<TTuple> tuple);
}

// ===========================================================================

#include "details/sequence.inl"

// ===========================================================================
