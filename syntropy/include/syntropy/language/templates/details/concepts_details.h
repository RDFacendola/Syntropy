
/// \file concepts_details.h
///
/// \brief This header is part of Syntropy language module.
///        It contains implementation details of fundamental concepts
///         definitions.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/templates/invoke.h"

// ===========================================================================

namespace Syntropy
{
    /// \brief Swap lhs with rhs.
    template <typename TType>
    constexpr void
    Swap(Mutable<TType> lhs, Mutable<TType> rhs) noexcept;
}

// ===========================================================================

namespace Syntropy::Concepts::Details
{
    /************************************************************************/
    /* TYPE TRAITS                                                          */
    /************************************************************************/

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

    // Properties concepts.
    // ====================

    /// \brief Concept for immutable types.
    template <typename TType>
    concept ImmutableType = std::is_const_v<TType>;

    /// \brief Concept for mutable types.
    template <typename TType>
    concept MutableType = !std::is_const_v<TType>;

    /// \brief Concept for trivial types.
    template <typename TType>
    concept TrivialType = std::is_trivial_v<TType>;

    /// \brief Concept for trivially-copyable types.
    template <typename TType>
    concept TriviallyCopyableType = std::is_trivially_copyable_v<TType>;

    /// \brief Concepts for standard-layout types.
    template <typename TType>
    concept StandardLayoutType = std::is_standard_layout_v<TType>;

    /// \brief Constant for polymorphic types.
    template <typename TType>
    concept PolymorphicType = std::is_polymorphic_v<TType>;

    /// \brief Constant for final types.
    template <typename TType>
    concept FinalType = std::is_final_v<TType>;

    // Type operation concepts.
    // ========================

    /// \brief Dummy method used to copy construct an instance.
    template <typename TType>
    void CopyConstruct(const TType&);

    /// \brief Detect whether TType is implicitly default constructible from
    ///        an empty list.
    template <typename TType>
    using DetectImplicitDefaultConstructor
        = decltype(CopyConstruct<TType>({}));

    /// \brief Detect whether TType is implicitly direct-constructible from
    ///        a list of arguments.
    template <typename TType, typename... TArguments>
    using DetectImplicitDirectConstructor
        = decltype(CopyConstruct<TType>({ Declval<TArguments>()... }));

    /// \brief Detect whether an instance of TType can be compared equal to
    ///        an instance of type UType.
    template <typename TType, typename UType>
    using DetectEqualityComparison
        = decltype(Declval<TType>() == Declval<UType>());

    /// \brief Constant equal to true if TType  default-constructible,
    ///        equal to false otherwise.
    template <typename TType>
    concept DefaultConstructibleType = std::is_default_constructible_v<TType>;

    /// \brief Constant equal to true if TType  trivially default
    ///        constructible, equal to false otherwise.
    template <typename TType>
    concept TriviallyDefaultConstructibleType
        = std::is_trivially_default_constructible_v<TType>;

    /// \brief Constant equal to true if TType  implicitly default
    ///        constructible, equal to false otherwise.
    template <typename TType>
    concept ImplicitlyDefaultConstructibleType
        = Templates::IsValidExpression<DetectImplicitDefaultConstructor,
                                       TType>;

    /// \brief Constant equal to true if TType  copy-constructible,
    ///        equal to false otherwise.
    template <typename TType>
    concept CopyConstructibleType = std::is_copy_constructible_v<TType>;

    /// \brief Constant equal to true if TType  trivially-copy-constructible,
    ///        equal to false otherwise.
    template <typename TType>
    concept TriviallyCopyConstructibleType
        = std::is_trivially_copy_constructible_v<TType>;

    /// \brief Constant equal to true if TType  move-constructible,
    ///        equal to false otherwise.
    template <typename TType>
    concept MoveConstructibleType = std::is_move_constructible_v<TType>;

    /// \brief Constant equal to true if TType  trivially-move-constructible,
    ///        equal to false otherwise.
    template <typename TType>
    concept TriviallyMoveConstructibleType
        = std::is_trivially_move_constructible_v<TType>;

    /// \brief Constant equal to true if TType  copy-assignable,
    ///        equal to false otherwise.
    template <typename TType>
    concept CopyAssignableType = std::is_copy_assignable_v<TType>;

    /// \brief Constant equal to true if TType  trivially-copy-constructible,
    ///        equal to false otherwise.
    template <typename TType>
    concept TriviallyCopyAssignableType
        = std::is_trivially_copy_assignable_v<TType>;

    /// \brief Constant equal to true if TType  move-assignable,
    ///        equal to false otherwise.
    template <typename TType>
    concept MoveAssignableType = std::is_move_assignable_v<TType>;

    /// \brief Constant equal to true if TType  trivially-move-constructible,
    ///        equal to false otherwise.
    template <typename TType>
    concept TriviallyMoveAssignableType
        = std::is_trivially_move_assignable_v<TType>;

    /// \brief Constant equal to true if TType  assignable from UType
    ///        in unevaluated context, equal to false otherwise.
    template <typename TType, typename UType>
    concept AssignableType = std::is_assignable_v<TType, UType>;

    /// \brief Constant equal to true if TType can be constructed
    ///        by TArguments... arguments, equal to false otherwise.
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
        = std::is_constructible_v<TType, TArguments...>;

    /// \brief Constant equal to true if TType can be constructed
    ///        from an initializer lt of TArguments, equal to false otherwise.
    ///
    /// The trait supports TypeLists in the form
    /// ImplicitlyConstructible<TypeList<A,B,C>,
    ///                         TypeList<a>,
    ///                         TypeList<b, bb>,
    ///                         TypeList<>>.
    ///
    /// TArguments must have the same exact rank as the number of elements in
    /// TType, otherwise the program  ill-formed.
    ///
    /// Given a function Foo(const TType&), TType is implicitly constructible
    /// if calling Foo({arguments...}) is well-formed.
    template <typename TType, typename... TArguments>
    concept ImplicitlyConstructibleType
        = Templates::IsValidExpression<DetectImplicitDirectConstructor,
                                       TType,
                                       TArguments...>;

    /// \brief Constant equal to true if TType  destructible,
    ///        equal to false otherwise.
    ///
    /// A type is destructible if it  a reference type or,
    /// if equal to an object-type, if calling its destructor
    /// by means of t.~TType() in unevaluated context  well-formed.
    ///
    /// A type is *not* destructible if it  equal to void, a function type
    /// or an array type of unknown bounds. It  not destructible also if its
    /// destructor is ill-formed in unevaluated context.
    template <typename TType>
    concept DestructibleType = std::is_destructible_v<TType>;

    /// \brief Constant equal to true if TType  both destructible and
    ///        trivially-destructible, equal to false otherwise.
    template <typename TType>
    concept TriviallyDestructibleType
        = std::is_trivially_destructible_v<TType>;

    /// \brief Constant equal to true if an instance of type TType can be
    ///        compared equal to an instance of type UType, equal to false
    ///        otherwise.
    template <typename TType, typename UType>
    concept ComparableForEqualityType
        = Templates::IsValidExpression<DetectEqualityComparison, TType, UType>;

    /************************************************************************/
    /* CONCEPTS                                                             */
    /************************************************************************/

    // Core language concepts.
    // =======================

    /// \brief Concept for types which are exactly equal to UType
    ///        and vice-versa.
    template <typename TType, typename UType>
    concept SameAs
         = std::is_same_v<TType, UType>
        && std::is_same_v<UType, TType>;

    /// \brief Concept for types convertible to UType.
    template <typename TType, typename UType>
    concept ConvertibleTo
         = std::is_convertible_v<TType, UType>
        && requires(Templates::AddRValueReference<TType>(&function)())
        {
            static_cast<UType>(function());
        };

    /// \brief Concept for types deriving from TBase ignoring
    ///        constant-qualifiers.
    template <typename TDerived, typename TBase>
    concept DerivedFrom
         = std::is_base_of_v<TBase, TDerived>
        && std::is_convertible_v<Ptr<TDerived>, Ptr<TBase>>;

    /// \brief Concept for types that share a common reference type.
    template <typename TType, typename UType>
    concept CommonReferenceWith
         = SameAs<Templates::CommonReference<TType, UType>,
                  Templates::CommonReference<UType, TType>>
        && ConvertibleTo<TType, Templates::CommonReference<TType, UType>>
        && ConvertibleTo<UType, Templates::CommonReference<TType, UType>>;

    /// \brief Concept for types that share a common type.
    template <typename TType, typename UType>
    concept CommonWith
         = SameAs<Templates::CommonType<TType, UType>,
                  Templates::CommonType<UType, TType>>
        && requires
        {
            static_cast<Templates::CommonType<TType, UType>>(Declval<TType>());
            static_cast<Templates::CommonType<TType, UType>>(Declval<UType>());
        }
        && CommonReferenceWith<Templates::AddLValueConstReference<TType>,
                               Templates::AddLValueConstReference<UType>>
        && CommonReferenceWith<Templates::AddLValueReference<
                                   Templates::CommonType<TType, UType>>,
                               Templates::CommonReference<
                                   Templates::AddLValueConstReference<TType>,
                                   Templates::AddLValueConstReference<UType>>>;

    /// \brief Concept for boolean types.
    template <typename TType>
    concept Boolean = SameAs<TType, Bool>;

    /// \brief Concept for signed integral number types.
    template <typename TType>
    concept Integral
         = SameAs<Templates::RemoveConstReference<TType>, Int>
        || SameAs<TType, Fix8>
        || SameAs<TType, Fix16>
        || SameAs<TType, Fix32>
        || SameAs<TType, Fix64>
        || SameAs<TType, Enum8>
        || SameAs<TType, Enum16>
        || SameAs<TType, Enum32>
        || SameAs<TType, Enum64>;

    /// \brief Concept for real number types.
    template <typename TType>
    concept Real = SameAs<TType, Float>;

    /// \brief Concept for an expression type which can be assigned from UType
    template <typename TType, typename UType>
    concept AssignableFrom
         = LValueReferenceType<TType>
        && CommonReferenceWith<Immutable<Templates::RemoveReference<TType>>,
                               Immutable<Templates::RemoveReference<UType>>>
        && requires(TType lhs, Forwarding<UType> rhs)
        {
            { lhs = Forward<UType>(rhs) } -> SameAs<TType>;
        };

    /// \brief Concept for a type whose instances are swappable.
    template <typename TType>
    concept Swappable
        = requires(Mutable<TType> lhs, Mutable<TType> rhs)
        {
            Swap(lhs, rhs);
        };

    /// \brief Concept for a type whose instances can be swapped with
    ///        instances of type UType.
    template <typename TType, typename UType>
    concept SwappableWith
         = CommonReferenceWith<TType, UType>
        && requires(Forwarding<TType> lhs, Forwarding<UType> rhs)
        {
            Swap(Forward<TType>(lhs), Forward<TType>(rhs));
            Swap(Forward<TType>(lhs), Forward<UType>(rhs));
            Swap(Forward<UType>(rhs), Forward<TType>(rhs));
            Swap(Forward<UType>(rhs), Forward<UType>(rhs));
        };

    /// \brief Concept for types whose instances can safely be destroyed at
    ///        the end of their lifetime (including reference types).
    template <typename TType>
    concept Destructible = std::is_destructible_v<TType>;

    /// \brief Concept for types whose instances can be initialized with a set
    ///        of arguments TArgs....
    template <typename TType, typename... TArguments>
    concept ConstructibleFrom
         = Destructible<TType>
        && ConstructibleType<TType, TArguments...>;

    /// \brief Concept for types that can be value-initialized (T()),
    ///        direct-list-initialized from and empty initializer list (T{})
    ///        or default-initialized (T t).
    template <typename TType>
    concept DefaultInitializable
         = ConstructibleFrom<TType>
        && requires { TType{}; }
        && requires { ::new (static_cast<void*>(nullptr)) TType; };

    /// \brief Concept for types that are reference-type or object-type
    ///        constructible from rvalue of the same type in both
    ///        direct-initialization and copy-initialization.
    template <typename TType>
    concept MoveConstructible
         = ConstructibleFrom<TType, TType>
        && ConvertibleTo<TType, TType>;

    /// \brief Concept for types that are either lvalue references or,
    ///        if move-constructible, copy-constructible by means of direct
    ///        and copy-initialization that leave the source unchanged
    ///        after the copy.
    template <typename TType>
    concept CopyConstructible
         = MoveConstructible<TType>
        && ConstructibleFrom<TType, Mutable<TType>>
        && ConvertibleTo<Mutable<TType>, TType>
        && ConstructibleFrom<TType, Immutable<TType>>
        && ConvertibleTo<Immutable<TType>, TType>
        && ConstructibleFrom<TType, Const<TType>>
        && ConvertibleTo<Const<TType>, TType>;

    // Comparison concepts.
    // ====================

    /// \brief Models a type TType for which the equality and inequality
    ///        operators against the (possibly different) type
    ///        UType are defined.
    template <typename TType, typename UType>
    concept EqualityComparableWithHelper
         = requires(Immutable<Templates::RemoveReference<TType>> lhs,
                    Immutable<Templates::RemoveReference<UType>>rhs)
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

    /// \brief Models a type TType for which the equality and
    ///        inequality operators are defined.
    template <typename TType>
    concept EqualityComparable
         = EqualityComparableWithHelper<TType, TType>;

    /// \brief Models a type TType for which the equality and inequality
    ///        operators against the (possibly different) type
    ///        UType are defined.
    template <typename TType, typename UType>
    concept EqualityComparableWith
         = EqualityComparable<TType>
        && EqualityComparable<UType>
        && CommonReferenceWith<Immutable<Templates::RemoveReference<TType>>,
                               Immutable<Templates::RemoveReference<UType>>>
        && EqualityComparable<Templates::CommonReference<
               Immutable<Templates::RemoveReference<TType>>,
               Immutable<Templates::RemoveReference<UType>>>>
        && EqualityComparableWithHelper<TType, UType>;

    /// \brief Models a type TType for which the less-than, greater-than,
    ///        less-than-or-equal-to and greater-than-or-equal-to operators
    ///        against the (possibly different) type UType are defined.
    template <typename TType, typename UType>
    concept PartiallyOrderedWithHelper
        = requires(Immutable<Templates::RemoveReference<TType>> lhs,
                   Immutable<Templates::RemoveReference<UType>> rhs)
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

    /// \brief Models a type TType for which the less-than, greater-than,
    ///        less-than-or-equal-to and greater-than-or-equal-to
    ///        operators are defined.
    template <typename TType>
    concept PartiallyOrdered
         = PartiallyOrderedWithHelper<TType, TType>;

    /// \brief Models a type TType for which the less-than, greater-than,
    ///        less-than-or-equal-to and greater-than-or-equal-to operators
    ///        against the (possibly different) type UType are defined.
    template <typename TType, typename UType>
    concept PartiallyOrderedWith
         = PartiallyOrdered<TType>
        && PartiallyOrdered<UType>
        && CommonReferenceWith<Immutable<Templates::RemoveReference<TType>>,
                               Immutable<Templates::RemoveReference<UType>>>
        && PartiallyOrdered<Templates::CommonReference<
               Immutable<Templates::RemoveReference<TType>>,
               Immutable<Templates::RemoveReference<UType>>>>
        && PartiallyOrderedWithHelper<TType, UType>;

    /// \brief Models a class TType which is both equality-comparable and
    ///        partially-ordered against the (possibly different) type UType.
    template <typename TType, typename UType>
    concept TotallyOrderedWithHelper
         = EqualityComparableWith<TType, UType>
        && PartiallyOrderedWith<TType, UType>;

    /// \brief Models a class TType which is both equality-comparable
    ///        and partially-ordered.
    template <typename TType>
    concept TotallyOrdered
         = TotallyOrderedWithHelper<TType, TType>;

    /// \brief Models a class TType which is both equality-comparable and
    ///        partially-ordered against the (possibly different) type UType.
    template <typename TType, typename UType>
    concept TotallyOrderedWith
         = TotallyOrdered<TType>
        && TotallyOrdered<UType>
        && CommonReferenceWith<Immutable<Templates::RemoveReference<TType>>,
                               Immutable<Templates::RemoveReference<UType>>>
        && TotallyOrdered<Templates::CommonReference<
               Immutable<Templates::RemoveReference<TType>>,
               Immutable<Templates::RemoveReference<UType>>>>
        && TotallyOrderedWithHelper<TType, UType>;

    // Object concepts.
    // ================

    /// \brief Concept for types that can be moved in and to (move constructed,
    ///        move assigned and reference-swappable).
    template <typename TType>
    concept Movable
         = ObjectType<TType>
        && MoveConstructible<TType>
        && AssignableFrom<Mutable<TType>, TType>
        && Swappable<TType>;


    /// \brief Concept for types that can be copied, moved and swapped.
    template <typename TType>
    concept Copyable
         = CopyConstructible<TType>
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
        && EqualityComparable<TType>;

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

    /// \brief Concept for callable types that can be called with a set of
    ///        arguments TArguments and preserve both callable object state
    ///        and argument state.
    template <typename TCallable, typename... TArguments>
    concept RegularInvocable
         = Invocable<TCallable, TArguments...>;

    /// \brief Concept for callable types that can be called with a set of
    ///        arguments TArguments and preserve both callable object state
    ///        and argument state.
    template <typename TPredicate, typename... TArguments>
    concept Predicate
         = RegularInvocable<TPredicate>
        && Boolean<Templates::InvokeResult<TPredicate, TArguments...>>;

    /// \brief Concept for callable types that define a binary
    ///        relation between# TType and UType.
    template <typename TPredicate, typename TType, typename UType>
    concept Relation
         = Predicate<TPredicate, TType, TType>
        && Predicate<TPredicate, TType, UType>
        && Predicate<TPredicate, UType, TType>
        && Predicate<TPredicate, UType, UType>;

    /// \brief Concept for predicates that define an equivalence relation
    ///        between TType and UType.
    ///
    /// This concept requires that the equivalence relation is reflexive,
    /// symmetric and transitive.
    ///
    /// \remarks The distinction between this and Relation is purely semantic.
    template <typename TPredicate, typename TType, typename UType>
    concept EquivalenceRelation
        = Relation<TPredicate, TType, UType>;

    /// \brief Concept for predicates which impose strict weak ordering on
    //         their arguments.
    ///
    /// This concept requires that the predicate is irreflexive,
    /// transitive and, given another predicate
    /// E(a,b) : !P(a,b) && !P(b, a), E is transitive.
    ///
    /// \remarks The distinction between this and Relation is purely semantic.
    template <typename TPredicate, typename TType, typename UType>
    concept StrictWeakOrder
        = Relation<TPredicate, TType, UType>;

    // Type relationship.
    // ==================

    /// \brief Constant equal to true if TType is a specialization of
    ///        TTemplate, equal to false otherwise.
    template<typename TType, template <typename...> typename TTemplate>
    constexpr Bool
    TemplateSpecializationOfHelper = false;

    /// \brief Partial template specialization for
    ///        template specializations (duh...).
    template<template <typename...> typename TTemplate, typename... TTypes>
    constexpr Bool
    TemplateSpecializationOfHelper<TTemplate<TTypes...>, TTemplate> = true;

    /// \brief Concepts for types TType that are template specialization of
    ///        TTemplate.
    template<typename TType, template <typename...> typename TTemplate>
    concept TemplateSpecializationOf
         = TemplateSpecializationOfHelper<TType, TTemplate>;

}

// ===========================================================================
