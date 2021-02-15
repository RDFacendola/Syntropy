
/// \file concepts.details.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/templates/invoke.h"

// ===========================================================================

namespace Syntropy::Concepts::Details
{
    /************************************************************************/
    /* CONCEPTS                                                             */
    /************************************************************************/

    // Core concepts.
    // ==============

    /// \brief Concept for types which are exactly equal to UType.
    template <typename TType, typename UType>
    concept SameAs = std::is_same_v<TType, UType>;

    /// \brief Concept for types convertible to UType.
    template <typename TType, typename UType>
    concept ConvertibleTo = std::is_convertible_v<TType, UType>;

    /// \brief Concept for immutable types.
    template <typename TType>
    concept ImmutableType = std::is_const_v<TType>;

    /// \brief Concept for mutable types.
    template <typename TType>
    concept MutableType = !std::is_const_v<TType>;

    // Type concepts.
    // ==============

    /// \brief Concept for void types.
    template<typename TType>
    concept VoidType = std::is_void_v<TType>;

    /// \brief Concept for null types.
    template<typename TType>
    concept NullType = std::is_null_pointer_v<TType>;

    /// \brief Concept for enum types.
    template <typename TType>
    concept EnumType = std::is_enum_v<TType>;

    /// \brief Concept for class types.
    template <typename TType>
    concept ClassType = std::is_class_v<TType>;

    /// \brief Concept for pointer types.
    template <typename TType>
    concept PointerType = std::is_pointer_v<TType>;

    /// \brief Concept for lvalue references.
    template <typename TType>
    concept LValueReferenceType = std::is_lvalue_reference_v<TType>;

    /// \brief Concept for rvalue references.
    template <typename TType>
    concept RValueReferenceType = std::is_rvalue_reference_v<TType>;

    /// \brief Concept for object types.
    template <typename TType>
    concept ObjectType = std::is_object_v<TType>;

    // Fundamental types concepts.
    // ===========================

    /// \brief Concept for boolean types.
    template <typename TType>
    concept Boolean
         = SameAs<Templates::ImmutableOf<TType>, Immutable<Bool>>;

    /// \brief Concept for signed integral number types.
    template <typename TType>
    concept Integral
         = SameAs<Templates::ImmutableOf<TType>, Immutable<Int>>
        || SameAs<Templates::ImmutableOf<TType>, Immutable<Fix8>>
        || SameAs<Templates::ImmutableOf<TType>, Immutable<Fix16>>
        || SameAs<Templates::ImmutableOf<TType>, Immutable<Fix32>>
        || SameAs<Templates::ImmutableOf<TType>, Immutable<Fix64>>
        || SameAs<Templates::ImmutableOf<TType>, Immutable<Enum8>>
        || SameAs<Templates::ImmutableOf<TType>, Immutable<Enum16>>
        || SameAs<Templates::ImmutableOf<TType>, Immutable<Enum32>>
        || SameAs<Templates::ImmutableOf<TType>, Immutable<Enum64>>;

    /// \brief Concept for real number types.
    template <typename TType>
    concept Real
         = SameAs<Templates::ImmutableOf<TType>, Immutable<Float>>;

     // Polymorphism.
     // =============

     /// \brief Concept for types deriving from TBase ignoring
     ///        constant-qualifiers.
     template <typename TDerived, typename TBase>
     concept DerivedFrom = std::is_base_of_v<TBase, TDerived>;

     /// \brief Concept for polymorphic types.
     template <typename TType>
     concept PolymorphicType = std::is_polymorphic_v<TType>;

     /// \brief Concept for final types.
     template <typename TType>
     concept FinalType = std::is_final_v<TType>;

    // Properties concepts.
    // ====================

    /// \brief Concepts for standard-layout types.
    template <typename TType>
    concept StandardLayoutType = std::is_standard_layout_v<TType>;

    /// \brief Concept for default-constructible types.
    template <typename TType>
    concept DefaultConstructibleType = std::is_default_constructible_v<TType>;

    /// \brief Concept for copy-constructible types.
    template <typename TType>
    concept CopyConstructibleType = std::is_copy_constructible_v<TType>;

    /// \brief Concept for move-constructible types.
    template <typename TType>
    concept MoveConstructibleType = std::is_move_constructible_v<TType>;

    /// \brief Concept for types that can be constructed by TArguments... .
    template <typename TType, typename... TArguments>
    concept ConstructibleFrom
        = std::is_constructible_v<TType, TArguments...>;

    /// \brief Concept for copy-assignable types.
    template <typename TType>
    concept CopyAssignableType = std::is_copy_assignable_v<TType>;

    /// \brief Concept for move-assignable types.
    template <typename TType>
    concept MoveAssignableType = std::is_move_assignable_v<TType>;

/// \brief Concept for types that can be assigned from UType.
    template <typename TType, typename UType>
    concept AssignableFrom = std::is_assignable_v<TType, UType>;

    /// \brief Concept for types whose instances can safely be destroyed at
    ///        the end of their lifetime (including reference types).
    template <typename TType>
    concept DestructibleType = std::is_destructible_v<TType>;

    // Trivial.
    // ========

    /// \brief Concept for trivially default-constructible types.
    template <typename TType>
    concept TriviallyDefaultConstructibleType
        = std::is_trivially_default_constructible_v<TType>;

    /// \brief Concept for trivially copy-constructible types.
    template <typename TType>
    concept TriviallyCopyConstructibleType
        = std::is_trivially_copy_constructible_v<TType>;

    /// \brief Concept for trivially move-constructible types.
    template <typename TType>
    concept TriviallyMoveConstructibleType
        = std::is_trivially_move_constructible_v<TType>;

    /// \brief Concept for trivially copy-assignable types.
    template <typename TType>
    concept TriviallyCopyAssignableType
        = std::is_trivially_copy_assignable_v<TType>;

    /// \brief Concept for trivially move-assignable types.
    template <typename TType>
    concept TriviallyMoveAssignableType
        = std::is_trivially_move_assignable_v<TType>;

    /// \brief Concept for types whose instances can safely be destroyed at
    ///        the end of their lifetime via trivial destructor.
    template <typename TType>
    concept TriviallyDestructibleType
        = std::is_trivially_destructible_v<TType>;

    /// \brief Concept for trivially-copyable types.
    template <typename TType>
    concept TriviallyCopyableType = std::is_trivially_copyable_v<TType>;

    /// \brief Concept for trivial types.
    template <typename TType>
    concept TrivialType = std::is_trivial_v<TType>;

    // Implicit.
    // =========

    /// \brief Dummy method used to copy construct an instance.
    template <typename TType>
    void CopyConstruct(const TType&);

    /// \brief Concept for types that can be implicitly default constructed
    //         from an empty initializer list.
    template <typename TType>
    concept ImplicitlyDefaultConstructibleType
        = requires()
        {
            { CopyConstruct<TType>({}) };
        };

    /// \brief Concept for types that can be implicitly direct-constructed
    ///        from a initializer list.
    template <typename TType, typename... TArguments>
    concept ImplicitlyConstructibleType
        = requires()
        {
            { CopyConstruct<TType>( { Declval<TArguments>()... }) };
        };

    // Comparison concepts.
    // ====================

    /// \brief Models a type TType for which the equality and inequality
    ///        operators against the (possibly different) type
    ///        UType are defined.
    template <typename TType, typename UType>
    concept EqualityComparableWith
        = requires(Templates::ImmutableOf<TType> lhs,
                   Templates::ImmutableOf<UType> rhs)
        {
            /// \brief Compare lhs and rhs for equality.
            { lhs == rhs } -> Boolean;

            /// \brief Compare lhs and rhs for inequality.
            { lhs != rhs } -> Boolean;

            /// \brief Compare rhs and lhs for equality.
            { rhs == lhs } -> Boolean;

            /// \brief Compare rhs and lhs for inequality.
            { rhs != lhs } -> Boolean;
        };

    /// \brief Models a type TType for which the less-than, greater-than,
    ///        less-than-or-equal-to and greater-than-or-equal-to operators
    ///        against the (possibly different) type UType are defined.
    template <typename TType, typename UType>
    concept PartiallyOrderedWith
        = requires(Templates::ImmutableOf<TType> lhs,
                   Templates::ImmutableOf<UType> rhs)
        {
            /// \brief Check whether lhs is less-than rhs.
            { lhs < rhs } -> Boolean;

            /// \brief Check whether lhs is greater-than rhs.
            { lhs > rhs } -> Boolean;

            /// \brief Check whether lhs is less-than or equal-to rhs.
            { lhs <= lhs } -> Boolean;

            /// \brief Check whether lhs is greater-than or equal-to rhs.
            { lhs >= lhs } -> Boolean;

            /// \brief Check whether rhs is less-than lhs.
            { rhs < lhs } -> Boolean;

            /// \brief Check whether rhs is greater-than lhs.
            { rhs > lhs } -> Boolean;

            /// \brief Check whether rhs is less-than or equal-to lhs.
            { rhs <= lhs } -> Boolean;

            /// \brief Check whether rhs is greater-than or equal-to lhs.
            { rhs >= lhs } -> Boolean;
        };

    // Templates concepts.
    // ===================

    /// \brief Constant equal to true if TType is a specialization of
    ///        TTemplate, equal to false otherwise.
    template<typename TType, template <typename...> typename TTemplate>
    constexpr Bool
    TemplateSpecializationOf = false;

    /// \brief Partial template specialization for
    ///        template specializations (duh...).
    template<template <typename...> typename TTemplate, typename... TTypes>
    constexpr Bool
    TemplateSpecializationOf<TTemplate<TTypes...>, TTemplate> = true;

    // Object concepts.
    // ================

    /// \brief Concept for types that can be value-initialized (T()),
    ///        direct-list-initialized from and empty initializer list (T{})
    ///        or default-initialized (T t).
    template <typename TType>
    concept DefaultInitializable
         = ConstructibleFrom<TType>
        && requires { TType{}; }
        && requires { ::new (static_cast<RWTypelessPtr>(nullptr)) TType; };

    /// \brief Concept for a type whose instances are swappable.
    template <typename TType>
    concept Swappable
        = AssignableFrom<Mutable<TType>, Movable<TType>>
       && MoveConstructibleType<TType>;

    // Callable concepts.
    // ==================

    /// \brief Concept for callable types that can be called with a set of
    ///        arguments TArguments.
    template <typename TCallable, typename... TArguments>
    concept Invocable
         = requires(Forwarding<TCallable> callable, TArguments&&... arguments)
         {
             Invoke(Forward<TCallable>, Forward<TArguments>(arguments)...);
         };

}

// ===========================================================================
