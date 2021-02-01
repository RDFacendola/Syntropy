
/// \file concepts.h
/// \brief This header is part of the Syntropy language module.
///        It contains definitions for fundamental concepts.
///
/// \author Raffaele D. Facendola - Nov 2020

#pragma once

#include "syntropy/language/templates/details/concepts_details.h"

// ===========================================================================

namespace Syntropy::Concepts
{
    /************************************************************************/
    /* CONCEPTS                                                             */
    /************************************************************************/

    // Core language concepts.
    // =======================

    /// \brief Concept for types which are exactly equal to UType
    ///        and vice-versa.
    template <typename TType, typename UType>
    concept SameAs
        = Details::SameAs<TType, UType>;

    /// \brief Concept for types convertible to UType.
    template <typename TType, typename UType>
    concept ConvertibleTo
        = Details::ConvertibleTo<TType, UType>;

    /// \brief Concept for types deriving from TBase ignoring
    ///        constant-qualifiers.
    template <typename TDerived, typename TBase>
    concept DerivedFrom
        = Details::DerivedFrom<TDerived, TBase>;

    /// \brief Concept for types that share a common reference type.
    template <typename TType, typename UType>
    concept CommonReferenceWith
        = Details::CommonReferenceWith<TType, UType>;

    /// \brief Concept for types that share a common type.
    template <typename TType, typename UType>
    concept CommonWith
        = Details::CommonWith<TType, UType>;

    /// \brief Concept for boolean types.
    template <typename TType>
    concept Boolean
        = Details::Boolean<TType>;

    /// \brief Concept for signed integral number types.
    template <typename TType>
    concept Integral
        = Details::Integral<TType>;

    /// \brief Concept for real number types.
    template <typename TType>
    concept Real
        = Details::Real<TType>;

    /// \brief Concept for an expression type which can be assigned from UType
    template <typename TType, typename UType>
    concept AssignableFrom
        = Details::AssignableFrom<TType, UType>;

    /// \brief Concept for a type whose instances are swappable.
    template <typename TType>
    concept Swappable
        = Details::Swappable<TType>;

    /// \brief Concept for a type whose instances can be swapped with
    ///        instances of type UType.
    template <typename TType, typename UType>
    concept SwappableWith
        = Details::SwappableWith<TType, UType>;

    /// \brief Concept for types whose instances can safely be destroyed at
    ///        the end of their lifetime (including reference types).
    template <typename TType>
    concept Destructible
        = Details::Destructible<TType>;

    /// \brief Concept for types whose instances can be initialized with a set
    ///        of arguments TArgs....
    template <typename TType, typename... TArguments>
    concept ConstructibleFrom
        = Details::ConstructibleFrom<TType, TArguments...>;

    /// \brief Concept for types that can be value-initialized (T()),
    ///        direct-list-initialized from and empty initializer list (T{})
    ///        or default-initialized (T t).
    template <typename TType>
    concept DefaultInitializable
        = Details::DefaultInitializable<TType>;

    /// \brief Concept for types that are reference-type or object-type
    ///        constructible from rvalue of the same type in both
    ///        direct-initialization and copy-initialization.
    template <typename TType>
    concept MoveConstructible
        = Details::MoveConstructible<TType>;

    /// \brief Concept for types that are either lvalue references or, if
    ///        move-constructible, copy-constructible by means of direct and
    ///        copy-initialization that leave the source unchanged after
    ///        the copy.
    template <typename TType>
    concept CopyConstructible
        = Details::CopyConstructible<TType>;

    // Comparison concepts.
    // ====================

    /// \brief Models a type TType for which the equality and inequality
    ///        operators are defined.
    template <typename TType>
    concept EqualityComparable
    = Details::EqualityComparable<TType>;

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
        = Details::PartiallyOrdered<TType>;

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
        = Details::TotallyOrdered<TType>;

    /// \brief Models a class TType which is both equality-comparable and
    partially-ordered against the (possibly different) type UType.
    template <typename TType, typename UType>
    concept TotallyOrderedWith
        = Details::TotallyOrderedWith<TType, UType>;

    // Object concepts.
    // ================

    /// \brief Concept for types that can be moved and swapped.
    template <typename TType>
    concept Movable
        = Details::Movable<TType>;

    /// \brief Concept for types that can be copied, moved and swapped.
    template <typename TType>
    concept Copyable
        = Details::Copyable<TType>;

    /// \brief Concept for types that are both copyable and
    ///        default constructible.
    template <typename TType>
    concept Semiregular
        = Details::Semiregular<TType>;

    /// \brief Concept for types that are copyable, default constructible
    ///        and equality comparable.
    template <typename TType>
    concept Regular
        = Details::Regular<TType>;

    // Callable concepts.
    // ==================

    /// \brief Concept for callable types that can be called with a set of
    ///        arguments TArguments.
    template <typename TCallable, typename... TArguments>
    concept Invocable
        = Details::Invocable<TCallable, TArguments...>;

    /// \brief Concept for callable types that can be called with a set of
    ///        arguments TArguments and preserve both callable object state
    ///        and argument state.
    ///
    /// \remarks The distinction between this and Invocable is purely semantic,
    ///          as Invocable may not satisfy equality preservation.
    template <typename TCallable, typename... TArguments>
    concept RegularInvocable
        = Details::RegularInvocable<TCallable, TArguments...>;

    /// \brief Concept for callable types that can be called with a set of
    ///        arguments TArguments and preserve both callable object state
    ///        and argument state.
    template <typename TCallable, typename... TArguments>
    concept Predicate
        = Details::Predicate<TCallable, TArguments...>;

    /// \brief Concept for callable types that define a binary relation
    ///        between TType and UType.
    template <typename TPredicate, typename TType, typename UType>
    concept Relation
        = Details::Relation<TPredicate, TType, UType>;

    /// \brief Concept for predicates that define an equivalence relation
    ///        between TType and UType.
    ///
    /// This concept requires that the equivalence relation is reflexive,
    /// symmetric and transitive.
    ///
    /// \remarks The distinction between this and Relation is purely semantic.
    template <typename TPredicate, typename TType, typename UType>
    concept EquivalenceRelation
        = Details::EquivalenceRelation<TPredicate, TType, UType>;

    /// \brief Concept for predicates which impose strict weak ordering on
    ///        their arguments.
    ///
    /// This concept requires that the predicate is irreflexive, transitive
    /// and, given another predicate E(a,b) : !P(a,b) && !P(b, a), E is
    /// transitive.
    ///
    /// \remarks The distinction between this and Relation is purely semantic.
    template <typename TPredicate, typename TType, typename UType>
    concept StrictWeakOrder
        = Details::StrictWeakOrder<TPredicate, TType, UType>;

    // Type concepts.
    // ==============

    /// \brief Concept for void types.
    template<typename TType>
    concept VoidType
        = Details::VoidType<TType>;

    /// \brief Concept for null types.
    template<typename TType>
    concept NullType
        = Details::NullType<TType>;

    /// \brief Concept for enum types.
    template <typename TType>
    concept EnumType
        = Details::EnumType<TType>;

    /// \brief Concept for class types.
    template <typename TType>
    concept ClassType
        = Details::ClassType<TType>;

    /// \brief Concept for pointer types.
    template <typename TType>
    concept PointerType
        = Details::PointerType<TType>;

    /// \brief Concept for lvalue references.
    template <typename TType>
    concept LValueReferenceType
        = Details::LValueReferenceType<TType>;

    /// \brief Concept for rvalue references.
    template <typename TType>
    concept RValueReferenceType
        = Details::RValueReferenceType<TType>;

    /// \brief Concept for object types.
    template <typename TType>
    concept ObjectType
        = Details::ObjectType<TType>;

    // Properties concepts.
    // ====================

    /// \brief Concept for immutable types.
    template <typename TType>
    concept ImmutableType
        = Details::ImmutableType<TType>;

    /// \brief Concept for mutable types.
    template <typename TType>
    concept MutableType
        = Details::MutableType<TType>;

    /// \brief Concept for trivial types.
    template <typename TType>
    concept TrivialType
        = Details::TrivialType<TType>;

    /// \brief Concept for trivially-copyable types.
    template <typename TType>
    concept TriviallyCopyableType
        = Details::TriviallyCopyableType<TType>;

    /// \brief Concepts for standard-layout types.
    template <typename TType>
    concept StandardLayoutType
        = Details::StandardLayoutType<TType>;

    /// \brief Constant for polymorphic types.
    template <typename TType>
    concept PolymorphicType
        = Details::PolymorphicType<TType>;

    /// \brief Constant for final types.
    template <typename TType>
    concept FinalType
        = Details::FinalType<TType>;

    // Type operations.
    // ================

    /// \brief Constant equal to true if TType  default-constructible,
    ///        equal to false otherwise.
    template <typename TType>
    concept DefaultConstructibleType
        = Details::DefaultConstructibleType<TType>;

    /// \brief Constant equal to true if TType  trivially default
    ///        constructible, equal to false otherwise.
    template <typename TType>
    concept TriviallyDefaultConstructibleType
        = Details::TriviallyDefaultConstructibleType<TType>;

    /// \brief Constant equal to true if TType  implicitly default
    ///        constructible, equal to false otherwise.
    template <typename TType>
    concept ImplicitlyDefaultConstructibleType
        = Details::ImplicitlyDefaultConstructibleType<TType>;

    /// \brief Constant equal to true if TType  copy-constructible, equal
    ///        to false otherwise.
    template <typename TType>
    concept CopyConstructibleType
        = Details::CopyConstructibleType<TType>;

    /// \brief Constant equal to true if TType  trivially-copy-constructible,
    ///        equal to false otherwise.
    template <typename TType>
    concept TriviallyCopyConstructibleType
        = Details::TriviallyCopyConstructibleType<TType>;

    /// \brief Constant equal to true if TType  move-constructible, equal to
    ///        false otherwise.
    template <typename TType>
    concept MoveConstructibleType
        = Details::MoveConstructibleType<TType>;

    /// \brief Constant equal to true if TType  trivially-move-constructible,
    ///        equal to false otherwise.
    template <typename TType>
    concept TriviallyMoveConstructibleType
        = Details::TriviallyMoveConstructibleType<TType>;

    /// \brief Constant equal to true if TType  copy-assignable, equal
    ///        to false otherwise.
    template <typename TType>
    concept CopyAssignableType
        = Details::CopyAssignableType<TType>;

    /// \brief Constant equal to true if TType  trivially-copy-constructible,
    ///        equal to false otherwise.
    template <typename TType>
    concept TriviallyCopyAssignableType
        = Details::TriviallyCopyAssignableType<TType>;

    /// \brief Constant equal to true if TType  move-assignable,
    ///        equal to false otherwise.
    template <typename TType>
    concept MoveAssignableType
        = Details::MoveAssignableType<TType>;

    /// \brief Constant equal to true if TType  trivially-move-constructible,
    ///        equal to false otherwise.
    template <typename TType>
    concept TriviallyMoveAssignableType
        = Details::TriviallyMoveAssignableType<TType>;

    /// \brief Constant equal to true if TType  assignable from UType in
    ///        unevaluated context, equal to false otherwise.
    template <typename TType, typename UType>
    concept AssignableType
        = Details::AssignableType<TType, UType>;

    /// \brief Constant equal to true if TType can be constructed by
    ///        TArguments... arguments, equal to false otherwise.
    ///
    /// The trait supports TypeLists in the form
    /// Constructible<TypeList<A,B,C>,
    ///               TypeList<a>,
    ///               TypeList<b, bb>,
    ///               TypeList<>>.
    ///
    /// TArguments must have the same exact rank as the number of elements
    /// in TType, otherwise the program  ill-formed.
    template <typename TType, typename... TArguments>
    concept ConstructibleType
        = Details::ConstructibleType<TType, TArguments...>;

    /// \brief Constant equal to true if TType can be constructed from an
    ///        initializer lt of TArguments, equal to false otherwise.
    ///
    /// The trait supports TypeLists in the form
    /// ImplicitlyConstructible<TypeList<A,B,C>,
    ///                         TypeList<a>,
    ///                         TypeList<b, bb>, TypeList<>>.
    ///
    /// TArguments must have the same exact rank as the number of element
    /// in TType, otherwise the program  ill-formed.
    ///
    /// \remarks Given a function Foo(const TType&), TType is implicitly
    ///          constructible if calling Foo({arguments...}) is well-formed.
    template <typename TType, typename... TArguments>
    concept ImplicitlyConstructibleType
        = Details::ImplicitlyConstructibleType<TType, TArguments...>;

    /// \brief Constant equal to true if TType  destructible,
    ///        equal to false otherwise.
    ///
    /// A type is destructible if it  a reference type or, if equal to an
    /// object-type, if calling its destructor by means of t.~TType() in
    /// unevaluated context  well-formed.
    ///
    /// A type is *not* destructible if it is equal to void, a function type or
    /// an array type of unknown bounds. It is not destructible also if its
    /// destructor is ill-formed in unevaluated context.
    template <typename TType>
    concept DestructibleType
        = Details::DestructibleType<TType>;

    /// \brief Constant equal to true if TType  both destructible and
    ///        trivially-destructible, equal to false otherwise.
    template <typename TType>
    concept TriviallyDestructibleType
        = Details::TriviallyDestructibleType<TType>;

    /// \brief Constant equal to true if an instance of type TType can be
    ///        compared equal to an instance of type UType, equal to false
    ///        otherwise.
    template <typename TType, typename UType>
    concept ComparableForEqualityType
        = Details::ComparableForEqualityType<TType, UType>;

    // Type relationship.
    // ==================

    /// \brief Concepts for types TType that are template specialization
    ///        of TTemplate.
    template<typename TType, template <typename...> typename TTemplate>
    concept TemplateSpecializationOf
        = Details::TemplateSpecializationOf<TType, TTemplate>;

}

// ===========================================================================
