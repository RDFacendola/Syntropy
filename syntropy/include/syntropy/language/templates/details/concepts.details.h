
/// \file concepts.details.h
///
/// \author Raffaele D. Facendola - 2020.

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
    concept IsSame = std::is_same_v<TType, UType>;

    /// \brief Concept for types convertible to UType.
    template <typename TType, typename UType>
    concept IsConvertible = std::is_convertible_v<TType, UType>;

    /// \brief Concept for immutable reference-types.
    template <typename TType>
    concept IsImmutable = IsSame<ImmutableOf<TType>, TType>;

    /// \brief Concept for mutable reference-types.
    template <typename TType>
    concept IsMutable = IsSame<MutableOf<TType>, TType>;

    /// \brief Concept for movable reerence-types.
    template <typename TType>
    concept IsMovable = IsSame<MovableOf<TType>, TType>;

    /// \brief Concept for immovable reference-types.
    template <typename TType>
    concept IsImmovable = IsSame<ImmovableOf<TType>, TType>;

    /// \brief Concept for lvalue references.
    template <typename TType>
    concept IsReference = IsSame<ReferenceOf<TType>, TType>;

    /// \brief Concept for rvalue references.
    template <typename TType>
    concept IsForwarding = IsSame<ForwardingOf<TType>, TType>;

    // Fundamental types concepts.
    // ===========================

    /// \brief Concept for boolean types.
    template <typename TType>
    concept IsBoolean = IsSame<UnqualifiedOf<TType>, Bool>;

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
    concept IsReal = IsSame<UnqualifiedOf<TType>, Float>;

    // Polymorphism.
    // =============

    /// \brief Concept for types deriving from TBase ignoring
    ///        constant-qualifiers.
    template <typename TDerived, typename TBase>
    concept IsDerivedFrom = std::is_base_of_v<TBase, TDerived>;

    /// \brief Concept for polymorphic types.
    template <typename TType>
    concept IsPolymorphic = std::is_polymorphic_v<TType>;

    /// \brief Concept for final types.
    template <typename TType>
    concept IsFinal = std::is_final_v<TType>;

    // Properties concepts.
    // ====================

    /// \brief Templates for standard-layout types.
    template <typename TType>
    concept IsStandardLayoutType = std::is_standard_layout_v<TType>;

    /// \brief Concept for types that can be constructed by TArguments... .
    template <typename TType, typename... TArguments>
    concept IsConstructibleFrom
        = std::is_constructible_v<TType, TArguments...>;

    /// \brief Concept for default-constructible types.
    template <typename TType>
    concept IsDefaultConstructible = std::is_default_constructible_v<TType>;

    /// \brief Concept for types that can be value-initialized (T()),
    ///        direct-list-initialized from and empty initializer list (T{})
    ///        or default-initialized (T t).
    template <typename TType>
    concept IsDefaultInitializable
         = IsConstructibleFrom<TType>
        && requires { TType{}; }
        && requires { ::new (static_cast<RWTypelessPtr>(nullptr)) TType; };

    /// \brief Concept for copy-constructible types.
    template <typename TType>
    concept IsCopyConstructible = std::is_copy_constructible_v<TType>;

    /// \brief Concept for move-constructible types.
    template <typename TType>
    concept IsMoveConstructible = std::is_move_constructible_v<TType>;

    /// \brief Concept for copy-assignable types.
    template <typename TType>
    concept IsCopyAssignable = std::is_copy_assignable_v<TType>;

    /// \brief Concept for move-assignable types.
    template <typename TType>
    concept IsMoveAssignable = std::is_move_assignable_v<TType>;

/// \brief Concept for types that can be assigned from UType.
    template <typename TType, typename UType>
    concept IsAssignableFrom = std::is_assignable_v<TType, UType>;

    /// \brief Concept for types whose instances can safely be destroyed at
    ///        the end of their lifetime (including reference types).
    template <typename TType>
    concept IsDestructible = std::is_destructible_v<TType>;

    // Trivial.
    // ========

    /// \brief Concept for trivially default-constructible types.
    template <typename TType>
    concept IsTriviallyDefaultConstructible
        = std::is_trivially_default_constructible_v<TType>;

    /// \brief Concept for trivially copy-constructible types.
    template <typename TType>
    concept IsTriviallyCopyConstructible
        = std::is_trivially_copy_constructible_v<TType>;

    /// \brief Concept for trivially move-constructible types.
    template <typename TType>
    concept IsTriviallyMoveConstructible
        = std::is_trivially_move_constructible_v<TType>;

    /// \brief Concept for trivially copy-assignable types.
    template <typename TType>
    concept IsTriviallyCopyAssignable
        = std::is_trivially_copy_assignable_v<TType>;

    /// \brief Concept for trivially move-assignable types.
    template <typename TType>
    concept IsTriviallyMoveAssignable
        = std::is_trivially_move_assignable_v<TType>;

    /// \brief Concept for types whose instances can safely be destroyed at
    ///        the end of their lifetime via trivial destructor.
    template <typename TType>
    concept IsTriviallyDestructible
        = std::is_trivially_destructible_v<TType>;

    /// \brief Concept for trivially-copyable types.
    template <typename TType>
    concept IsTriviallyCopyable = std::is_trivially_copyable_v<TType>;

    /// \brief Concept for trivial types.
    template <typename TType>
    concept IsTrivial = std::is_trivial_v<TType>;

    // Implicit.
    // =========

    /// \brief Dummy method used to copy construct an instance.
    template <typename TType>
    void CopyConstruct(const TType&);

    /// \brief Concept for types that can be implicitly default constructed
    //         from an empty initializer list.
    template <typename TType>
    concept IsImplicitlyDefaultConstructible = requires()
    {
        { CopyConstruct<TType>({}) };
    };

    /// \brief Concept for types that can be implicitly direct-constructed
    ///        from a initializer list.
    template <typename TType, typename... TArguments>
    concept IsImplicitlyConstructibleFrom = requires()
    {
        { CopyConstruct<TType>( { Declval<TArguments>()... }) };
    };

    // Comparison concepts.
    // ====================

    /// \brief Concept for types which define both the equality and inequality
    ///        operators against a possibily different type.
    template <typename TType, typename UType>
    concept IsEqualityComparableWith = requires(ImmutableOf<TType> lhs,
                                                ImmutableOf<UType> rhs)
    {
        /// \brief Compare lhs and rhs for equality.
        { lhs == rhs } -> IsBoolean;

        /// \brief Compare lhs and rhs for inequality.
        { lhs != rhs } -> IsBoolean;

        /// \brief Compare rhs and lhs for equality.
        { rhs == lhs } -> IsBoolean;

        /// \brief Compare rhs and lhs for inequality.
        { rhs != lhs } -> IsBoolean;
    };

    /// \brief Concept for types which define boh the less-than, greater-than,
    ///        less-than-or-equal-to and greater-than-or-equal-to operators
    ///        against a possibly different type.
    template <typename TType, typename UType>
    concept IsPartiallyOrderedWith = requires(ImmutableOf<TType> lhs,
                                              ImmutableOf<UType> rhs)
    {
        /// \brief Check whether lhs is less-than rhs.
        { lhs < rhs } -> IsBoolean;

        /// \brief Check whether lhs is greater-than rhs.
        { lhs > rhs } -> IsBoolean;

        /// \brief Check whether lhs is less-than or equal-to rhs.
        { lhs <= lhs } -> IsBoolean;

        /// \brief Check whether lhs is greater-than or equal-to rhs.
        { lhs >= lhs } -> IsBoolean;

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
    constexpr Bool
    IsTemplateSpecializationOf = false;

    /// \brief Partial template specialization for
    ///        template specializations (duh...).
    template<template <typename...> typename TTemplate, typename... TTypes>
    constexpr Bool
    IsTemplateSpecializationOf<TTemplate<TTypes...>, TTemplate> = true;

}

// ===========================================================================
