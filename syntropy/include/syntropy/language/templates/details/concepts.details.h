
/// \file concepts.details.h
///
/// \author Raffaele D. Facendola - 2020.

// ===========================================================================

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/type_traits.h"

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* CONCEPTS                                                             */
    /************************************************************************/

    // Core concepts.
    // ==============

    /// \brief Concept for types which are exactly equal to UType.
    template <typename TType, typename UType>
    concept IsSame
        = std::is_same_v<TType, UType>;

    /// \brief Concept for types convertible to UType.
    template <typename TType, typename UType>
    concept IsConvertible
        = std::is_convertible_v<TType, UType>;

    /// \brief True if TType is a read-only value-type.
    template <typename TType>
    concept IsReadOnlyValue
        = IsSame<ReadOnlyOf<TType>, TType>;

    /// \brief True if TType is a read-write value-type.
    template <typename TType>
    concept IsReadWriteValue
        = IsSame<ReadWriteOf<TType>, TType>;

    /// \brief Concept for immutable reference-types.
    template <typename TType>
    concept IsImmutableReference
        = IsSame<ImmutableOf<TType>, TType>;

    /// \brief Concept for mutable reference-types.
    template <typename TType>
    concept IsMutableReference
        = IsSame<MutableOf<TType>, TType>;

    /// \brief Concept for movable reference-types.
    template <typename TType>
    concept IsMovableReference
        = IsSame<MovableOf<TType>, TType>;

    /// \brief Concept for immovable reference-types.
    template <typename TType>
    concept IsImmovableReference
        = IsSame<ImmovableOf<TType>, TType>;

    // Fundamental types concepts.
    // ===========================

    /// \brief Concept for boolean types.
    template <typename TType>
    concept IsBoolean
        = IsSame<UnqualifiedOf<TType>, Bool>;

    /// \brief Concept for signed integral number types.
    template <typename TType>
    concept IsIntegral
         = IsSame<UnqualifiedOf<TType>, Int>
        || IsSame<UnqualifiedOf<TType>, Fix8>
        || IsSame<UnqualifiedOf<TType>, Fix16>
        || IsSame<UnqualifiedOf<TType>, Fix32>
        || IsSame<UnqualifiedOf<TType>, Fix64>
        || IsSame<UnqualifiedOf<TType>, Enum8>
        || IsSame<UnqualifiedOf<TType>, Enum16>
        || IsSame<UnqualifiedOf<TType>, Enum32>
        || IsSame<UnqualifiedOf<TType>, Enum64>;

    /// \brief Concept for real number types.
    template <typename TType>
    concept IsReal
        = IsSame<UnqualifiedOf<TType>, Float>;

    // Polymorphism.
    // =============

    /// \brief Concept for base classes of TDerived.
    template <typename TBase, typename TDerived>
    concept IsBaseOf
        = std::is_base_of_v<UnqualifiedOf<TBase>, UnqualifiedOf<TDerived>>;

    /// \brief Concept for polymorphic types.
    template <typename TType>
    concept IsPolymorphic
        = std::is_polymorphic_v<UnqualifiedOf<TType>>;

    /// \brief Concept for final types.
    template <typename TType>
    concept IsFinal
        = std::is_final_v<UnqualifiedOf<TType>>;

    // Properties concepts.
    // ====================

    /// \brief Concept for types that can be constructed by TArguments... .
    template <typename TType, typename... TArguments>
    concept IsConstructible
        = std::is_constructible_v<UnqualifiedOf<TType>, TArguments...>;

    /// \brief Concept for types that are both move-constructible
    ///        and move-assignable.
    template <typename TType>
    concept IsMovable
         = std::is_move_constructible_v<UnqualifiedOf<TType>>
        && std::is_move_assignable_v<UnqualifiedOf<TType>>;

    /// \brief Concept for types that are movable, copy-constructible and
    ///        copy-assignable.
    template <typename TType>
    concept IsCopyable
         = std::is_move_constructible_v<UnqualifiedOf<TType>>
        && std::is_move_assignable_v<UnqualifiedOf<TType>>
        && std::is_copy_constructible_v<UnqualifiedOf<TType>>
        && std::is_copy_assignable_v<UnqualifiedOf<TType>>;

    /// \brief Concept for types that can be assigned from UType.
    template <typename TType, typename UType>
    concept IsAssignable
        = std::is_assignable_v<TType, UType>;

    // Trivial.
    // ========

    /// \brief Concept for types that are both trivially-copy-constructible,
    ///        trivially-copy-assignable, trivially-move-constructible and
    ///        trivially-move-assignable.
    template <typename TType>
    concept IsTriviallyCopyable
        = std::is_trivially_copyable_v<UnqualifiedOf<TType>>;

    /// \brief Concepts for types that are both trivially copyable,
    ///        trivially-default-constructibles and trivially-destructible.
    template <typename TType>
    concept IsTrivial
         = std::is_trivial_v<UnqualifiedOf<TType>>
        && std::is_trivially_destructible_v<UnqualifiedOf<TType>>;

    // Comparison concepts.
    // ====================

    /// \brief Concept for types that can be compared equal.
    template <typename TType>
    concept IsEqualityComparable
        = requires(ImmutableOf<TType> lhs, ImmutableOf<TType> rhs)
    {
        /// \brief Compare lhs and rhs for equality.
        { lhs == rhs } -> IsBoolean;

        /// \brief Compare lhs and rhs for inequality.
        { lhs != rhs } -> IsBoolean;
    };

    /// \brief Concept for types that can be compared to themselves.
    template <typename TType>
    concept IsComparable
         = IsEqualityComparable<TType>
        && requires(ImmutableOf<TType> lhs, ImmutableOf<TType> rhs)
    {
        /// \brief Check whether lhs is less-than rhs.
        { lhs < rhs } -> IsBoolean;

        /// \brief Check whether lhs is greater-than rhs.
        { lhs > rhs } -> IsBoolean;

        /// \brief Check whether lhs is less-than or equal-to rhs.
        { lhs <= rhs } -> IsBoolean;

        /// \brief Check whether lhs is greater-than or equal-to rhs.
        { lhs >= rhs } -> IsBoolean;

        /// \brief Check whether rhs is less-than lhs.
        { rhs < lhs } -> IsBoolean;

        /// \brief Check whether rhs is greater-than lhs.
        { rhs > lhs } -> IsBoolean;

        /// \brief Check whether rhs is less-than or equal-to lhs.
        { rhs <= lhs } -> IsBoolean;

        /// \brief Check whether rhs is greater-than or equal-to lhs.
        { rhs >= lhs } -> IsBoolean;
    };

    // Templates concepts.
    // ===================

    /// \brief Constant equal to true if TType is a specialization of
    ///        TTemplate, equal to false otherwise.
    template<typename TType, template <typename...> typename TTemplate>
    constexpr
    Bool IsTemplateSpecializationOfHelper
        = false;

    /// \brief Partial template specialization for template specializations.
    template<template <typename...> typename TTemplate, typename... TTypes>
    constexpr
    Bool IsTemplateSpecializationOfHelper<TTemplate<TTypes...>, TTemplate>
        = true;

    /// \brief Concept for types that are template specialization of TTemplate.
    template<typename TType, template <typename...> typename TTemplate>
    constexpr
    Bool IsTemplateSpecializationOf
        = IsTemplateSpecializationOfHelper<UnqualifiedOf<TType>, TTemplate>;

    // Type lists.
    // ===========

    /// \brief True if TType is a type-list.
    template <typename TType>
    inline constexpr
    Bool IsTypeListHelper
        = false;

    /// \brief Partial template specialzation for type lists.
    template <typename... TTypes>
    inline constexpr
    Bool IsTypeListHelper<TypeList<TTypes...>>
        = true;

    /// \brief True if TType is a type-list.
    template <typename TType>
    inline constexpr
    Bool IsTypeList
        = IsTypeListHelper<UnqualifiedOf<TType>>;

    // Sequence.
    // =========

    /// \brief True if TSequence is a sequence.
    template <typename TSequence>
    inline constexpr
    Bool IsSequenceHelper
        = false;

    /// \brief Partial template specialzation for sequences.
    template <Int... TIndices>
    inline constexpr
    Bool IsSequenceHelper<Sequence<TIndices...>>
        = true;

    /// \brief True if TType is a sequence.
    template <typename TSequence>
    inline constexpr
    Bool IsSequence
        = IsSequenceHelper<UnqualifiedOf<TSequence>>;

    /// \brief Constant equal to true if TSequence is a monotonically
    ///        increasing contiguous sequence, equal to false otherwise.
    template <typename TSequence>
    inline constexpr
    Bool IsContiguousSequenceHelper
        = false;

    /// \brief Partial specialization for sequences.
    template <Int THead, Int... TRest>
    inline constexpr Bool
    IsContiguousSequenceHelper<Sequence<THead, TRest...>>
         = IsSame<Sequence<THead, TRest...>,
                  SequenceAdd<THead, MakeSequence<1 + sizeof...(TRest)>>>;

    /// \brief Constant equal to true if TSequence is a monotonically
    ///        increasing contiguous sequence, equal to false otherwise.
    template <typename TSequence>
    inline constexpr
    Bool IsContiguousSequence
        = IsContiguousSequenceHelper<UnqualifiedOf<TSequence>>;

}

// ===========================================================================
