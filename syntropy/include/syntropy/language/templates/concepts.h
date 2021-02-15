
/// \file concepts.h
///
/// \brief This header is part of the Syntropy language module.
///        It contains definitions for fundamental concepts.
///
/// \author Raffaele D. Facendola - Nov 2020

#pragma once

// ===========================================================================

#include "concepts.details.h"

// ===========================================================================

namespace Syntropy::Concepts
{
    /************************************************************************/
    /* CONCEPTS                                                             */
    /************************************************************************/

    // Core concepts.
    // ==============

    /// \brief Concept for types which are exactly equal to UType.
    template <typename TType, typename UType>
    concept SameAs = Details::SameAs<TType, UType>;

    /// \brief Concept for types convertible to UType.
    template <typename TType, typename UType>
    concept ConvertibleTo = Details::ConvertibleTo<TType, UType>;

    /// \brief Concept for immutable types.
    template <typename TType>
    concept ImmutableType = Details::ImmutableType<TType>;

    /// \brief Concept for mutable types.
    template <typename TType>
    concept MutableType = Details::MutableType<TType>;

    // Type concepts.
    // ==============

    /// \brief Concept for void types.
    template<typename TType>
    concept VoidType = Details::VoidType<TType>;

    /// \brief Concept for null types.
    template<typename TType>
    concept NullType = Details::NullType<TType>;

    /// \brief Concept for enum types.
    template <typename TType>
    concept EnumType = Details::EnumType<TType>;

    /// \brief Concept for class types.
    template <typename TType>
    concept ClassType = Details::ClassType<TType>;

    /// \brief Concept for pointer types.
    template <typename TType>
    concept PointerType = Details::PointerType<TType>;

    /// \brief Concept for lvalue references.
    template <typename TType>
    concept LValueReferenceType = Details::LValueReferenceType<TType>;

    /// \brief Concept for rvalue references.
    template <typename TType>
    concept RValueReferenceType = Details::RValueReferenceType<TType>;

    /// \brief Concept for object types.
    template <typename TType>
    concept ObjectType = Details::ObjectType<TType>;

    // Fundamental types concepts.
    // ===========================

    /// \brief Concept for boolean types.
    template <typename TType>
    concept Boolean = Details::Boolean<TType>;

    /// \brief Concept for signed integral number types.
    template <typename TType>
    concept Integral = Details::Integral<TType>;

    /// \brief Concept for real number types.
    template <typename TType>
    concept Real = Details::Real<TType>;

    // Polymorphism.
    // =============

    /// \brief Concept for types deriving from TBase.
    template <typename TDerived, typename TBase>
    concept DerivedFrom = Details::DerivedFrom<TDerived, TBase>;

    /// \brief Constant for polymorphic types.
    template <typename TType>
    concept PolymorphicType = Details::PolymorphicType<TType>;

    /// \brief Constant for final types.
    template <typename TType>
    concept FinalType = Details::FinalType<TType>;

    // Properties concepts.
    // ====================

    /// \brief Concepts for standard-layout types.
    template <typename TType>
    concept StandardLayoutType = Details::StandardLayoutType<TType>;

    /// \brief Concept for default-constructible types.
    template <typename TType>
    concept DefaultConstructibleType
        = Details::DefaultConstructibleType<TType>;

    /// \brief Concept for copy-constructible types.
    template <typename TType>
    concept CopyConstructibleType = Details::CopyConstructibleType<TType>;

    /// \brief Concept for move-constructible types.
    template <typename TType>
    concept MoveConstructibleType = Details::MoveConstructibleType<TType>;

    /// \brief Concept for types that can be constructed by TArguments... .
    template <typename TType, typename... TArguments>
    concept ConstructibleFrom
        = Details::ConstructibleFrom<TType, TArguments...>;

    /// \brief Concept for copy-assignable types.
    template <typename TType>
    concept CopyAssignableType = Details::CopyAssignableType<TType>;

    /// \brief Concept for move-assignable types.
    template <typename TType>
    concept MoveAssignableType = Details::MoveAssignableType<TType>;

    /// \brief Concept for types that can be assigned from UType.
    template <typename TType, typename UType>
    concept AssignableFrom = Details::AssignableFrom<TType, UType>;

    /// \brief Concept for types whose instances can safely be destroyed at
    ///        the end of their lifetime.
    template <typename TType>
    concept DestructibleType = Details::DestructibleType<TType>;

    // Trivial.
    // ========

    /// \brief Concept for trivially default-constructible types.
    template <typename TType>
    concept TriviallyDefaultConstructibleType
        = Details::TriviallyDefaultConstructibleType<TType>;

    /// \brief Concept for trivially copy-constructible types.
    template <typename TType>
    concept TriviallyCopyConstructibleType
        = Details::TriviallyCopyConstructibleType<TType>;

    /// \brief Concept for trivially move-constructible types.
    template <typename TType>
    concept TriviallyMoveConstructibleType
        = Details::TriviallyMoveConstructibleType<TType>;

    /// \brief Concept for trivially copy-assignable types.
    template <typename TType>
    concept TriviallyCopyAssignableType
        = Details::TriviallyCopyAssignableType<TType>;

    /// \brief Concept for trivially move-assignable types.
    template <typename TType>
    concept TriviallyMoveAssignableType
        = Details::TriviallyMoveAssignableType<TType>;

    /// \brief Concept for types whose instances can safely be destroyed at
    ///        the end of their lifetime via trivial destructor.
    template <typename TType>
    concept TriviallyDestructibleType
        = Details::TriviallyDestructibleType<TType>;

    /// \brief Concept for trivially-copyable types.
    template <typename TType>
    concept TriviallyCopyableType = Details::TriviallyCopyableType<TType>;

    /// \brief Concept for trivial types.
    template <typename TType>
    concept TrivialType = Details::TrivialType<TType>;

    // Implicit.
    // =========

    /// \brief Concept for types that can be implicitly default constructed
    //         from an empty initializer list.
    template <typename TType>
    concept ImplicitlyDefaultConstructibleType
        = Details::ImplicitlyDefaultConstructibleType<TType>;

    /// \brief Concept for types that can be implicitly direct-constructed
    ///        from a initializer list.
    template <typename TType, typename... TArguments>
    concept ImplicitlyConstructibleType
        = Details::ImplicitlyConstructibleType<TType, TArguments...>;

    // Comparison concepts.
    // ====================

    /// \brief Models a type TType for which the equality and inequality
    ///        operators are defined.
    template <typename TType>
    concept EqualityComparable
        = Details::EqualityComparableWith<TType, TType>;

    /// \brief Models a type TType for which the equality and inequality
    ///        operators against the (possibly different) type UType
    ///        are defined.
    template <typename TType, typename UType>
    concept EqualityComparableWith
        = Details::EqualityComparableWith<TType, UType>;

    /// \brief Models a type TType for which the less-than, greater-than,
    ///        less-than-or-equal-to and greater-than-or-equal-to operators
    ///        are defined.
    template <typename TType>
    concept PartiallyOrdered
        = Details::PartiallyOrderedWith<TType, TType>;

    /// \brief Models a type TType for which the less-than, greater-than,
    ///        less-than-or-equal-to and greater-than-or-equal-to operators
    ///        against the (possibly different) type UType are defined.
    template <typename TType, typename UType>
    concept PartiallyOrderedWith
        = Details::PartiallyOrderedWith<TType, UType>;

    /// \brief Models a class TType which is both equality-comparable and
    ///        partially-ordered.
    template <typename TType>
    concept TotallyOrdered
        = EqualityComparable<TType>
       && PartiallyOrdered<TType>;

    /// \brief Models a class TType which is both equality-comparable and
    ///        partially-ordered against the (possibly different) type UType.
    template <typename TType, typename UType>
    concept TotallyOrderedWith
        = EqualityComparableWith<TType, UType>
       && PartiallyOrderedWith<TType, UType>;

    // Templates concepts.
    // ===================

    /// \brief Concepts for types TType that are template specialization
    ///        of TTemplate.
    template<typename TType, template <typename...> typename TTemplate>
    concept TemplateSpecializationOf
        = Details::TemplateSpecializationOf<TType, TTemplate>;

    // Object concepts.
    // ================

    /// \brief Concept for types that can be value-initialized (T()),
    ///        direct-list-initialized from and empty initializer list (T{})
    ///        or default-initialized (T t).
    template <typename TType>
    concept DefaultInitializable
        = Details::DefaultInitializable<TType>;

    /// \brief Concept for a type whose instances are swappable.
    template <typename TType>
    concept Swappable
        = Details::Swappable<TType>;

    /// \brief Concept for types that can be moved in and to (move constructed,
    ///        move assigned and reference-swappable).
    template <typename TType>
    concept Movable
         = ObjectType<TType>
        && MoveConstructibleType<TType>
        && AssignableFrom<Mutable<TType>, TType>
        && Swappable<TType>;

    /// \brief Concept for types that can be copied, moved and swapped.
    template <typename TType>
    concept Copyable
         = CopyConstructibleType<TType>
        && Movable<TType>
        && AssignableFrom<Mutable<TType>, Mutable<TType>>
        && AssignableFrom<Mutable<TType>, Immutable<TType>>
        && AssignableFrom<Mutable<TType>, Const<TType>>;

    /// \brief Concept for types that are both copyable and default
    ///        constructible.
    template <typename TType>
    concept Semiregular
         = Copyable<TType>
         && DefaultInitializable<TType>;

    /// \brief Concept for types that are copyable, default constructible and
    ///        equality comparable.
    template <typename TType>
    concept Regular
         = Semiregular<TType>
        && EqualityComparableWith<TType, TType>;

    // Callable concepts.
    // ==================

    /// \brief Concept for callable types that can be called with a set of
    ///        arguments TArguments.
    template <typename TCallable, typename... TArguments>
    concept Invocable = Details::Invocable<TCallable, TArguments...>;

    /// \brief Concept for callable types that can be called with a set of
    ///        arguments TArguments and preserve both callable object state
    ///        and argument state.
    ///
    /// \remarks The distinction between this and Invocable is purely semantic,
    ///          as Invocable may not satisfy equality preservation.
    template <typename TCallable, typename... TArguments>
    concept RegularInvocable
         = Invocable<TCallable, TArguments...>;

    /// \brief Concept for callable types that can be called with a set of
    ///        arguments TArguments and preserve both callable object state
    ///        and argument state.
    template <typename TCallable, typename... TArguments>
    concept Predicate
         = Invocable<TCallable, TArguments...>
        && Boolean<Templates::InvokeResult<TCallable, TArguments...>>;

    /// \brief Concept for callable types that define a binary relation
    ///        between TType and UType.
    template <typename TCallable, typename TType, typename UType>
    concept Relation
         = Predicate<TCallable, TType, TType>
        && Predicate<TCallable, TType, UType>
        && Predicate<TCallable, UType, TType>
        && Predicate<TCallable, UType, UType>;

}

// ===========================================================================
