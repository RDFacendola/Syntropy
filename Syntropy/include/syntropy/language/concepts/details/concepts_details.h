
/// \file concepts_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of fundamental concepts definitions.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/templates/traits.h"
#include "syntropy/language/support/support.h"

// ===========================================================================

namespace Syntropy::Concepts::Details
{
    /************************************************************************/
    /* CONCEPTS                                                             */
    /************************************************************************/

    // Core language concepts.
    // =======================

    /// \brief Concept for types which are exactly equal to UType and vice-versa.
    template <typename TType, typename UType>
    concept SameAs = Templates::IsSame<TType, UType>
        && Templates::IsSame<UType, TType>;

    /// \brief Concept for types deriving from TBase ignoring constant-qualifiers.
    template <typename TDerived, typename TBase>
    concept DerivedFrom = Templates::IsBaseOf<TBase, TDerived> && Templates::IsConvertible<const TDerived*, const TBase*>;

    /// \brief Concept for types convertible to UType.
    template <typename TType, typename UType>
    concept ConvertibleTo = Templates::IsConvertible<TType, UType>
        && requires(Templates::AddRValueReference<TType>(&function)())
    {
        static_cast<UType>(function());
    };

    /// \brief Concept for types that share a common reference type.
    template <typename TType, typename UType>
    concept CommonReferenceWith = SameAs<Templates::CommonReference<TType, UType>, Templates::CommonReference<UType, TType>>
        && ConvertibleTo<TType, Templates::CommonReference<TType, UType>>
        && ConvertibleTo<UType, Templates::CommonReference<TType, UType>>;

    /// \brief Concept for types that share a common type.
    template <typename TType, typename UType>
    concept CommonWith = SameAs<Templates::CommonType<TType, UType>, Templates::CommonType<UType, TType>>
        && requires
    {
        static_cast<Templates::CommonType<TType, UType>>(Declval<TType>());
        static_cast<Templates::CommonType<TType, UType>>(Declval<UType>());
    }
    && CommonReferenceWith<Templates::AddLValueConstReference<TType>, Templates::AddLValueConstReference<UType>>
    && CommonReferenceWith<Templates::AddLValueReference<Templates::CommonType<TType, UType>>, Templates::CommonReference<Templates::AddLValueConstReference<TType>, Templates::AddLValueConstReference<UType>>>;

    /// \brief Concept for boolean types.
    template <typename TType>
    concept Boolean = Templates::IsBoolean<TType>;

    /// \brief Concept for signed integral number types.
    template <typename TType>
    concept Integral = Templates::IsIntegral<TType>;

    /// \brief Concept for real number types.
    template <typename TType>
    concept Real = Templates::IsReal<TType>;

    /// \brief Concept for an expression type which can be assigned from UType
    template <typename TType, typename UType>
    concept AssignableFrom = Templates::IsLValueReference<TType>
        && CommonReferenceWith<const Templates::RemoveReference<TType>&, const Templates::RemoveReference<UType>&>
        && requires(TType lhs, UType && rhs)
    {
        { lhs = Forward<UType>(rhs) } -> SameAs<TType>;
    };

    /// \brief Concept for a type whose instances are swappable.
    template <typename TType>
    concept Swappable = requires(TType & lhs, TType & rhs)
    {
        Swap(lhs, rhs);
    };

    /// \brief Concept for a type whose instances can be swapped with instances of type UType.
    template <typename TType, typename UType>
    concept SwappableWith = CommonReferenceWith<TType, UType>
        && requires(TType && lhs, UType && rhs)
    {
        Swap(Forward<TType>(lhs), Forward<TType>(rhs));
        Swap(Forward<TType>(lhs), Forward<UType>(rhs));
        Swap(Forward<UType>(rhs), Forward<TType>(rhs));
        Swap(Forward<UType>(rhs), Forward<UType>(rhs));
    };

    /// \brief Concept for types whose instances can safely be destroyed at the end of their lifetime (including reference types).
    template <typename TType>
    concept Destructible = Templates::IsDestructible<TType>;

    /// \brief Concept for types whose instances can be initialized with a set of arguments TArgs....
    template <typename TType, typename... TArguments>
    concept ConstructibleFrom = Destructible<TType>
        && Templates::IsConstructible<TType, TArguments...>;

    /// \brief Concept for types that can be value-initialized (T()), direct-list-initialized from and empty initializer list (T{}) or default-initialized (T t).
    template <typename TType>
    concept DefaultInitializable = ConstructibleFrom<TType>
        && requires { TType{}; }
        && requires { ::new (static_cast<void*>(nullptr)) TType; };

    /// \brief Concept for types that are reference-type or object-type constructible from rvalue of the same type in both direct-initialization and copy-initialization.
    template <typename TType>
    concept MoveConstructible = ConstructibleFrom<TType, TType>
        && ConvertibleTo<TType, TType>;

    /// \brief Concept for types that are either lvalue references or, if move-constructible, copy-constructible by means of direct and copy-initialization that leave the source unchanged afte the copy.
    template <typename TType>
    concept CopyConstructible = MoveConstructible<TType>
        && ConstructibleFrom<TType, TType&>
        && ConvertibleTo<TType&, TType>
        && ConstructibleFrom<TType, const TType&>
        && ConvertibleTo<const TType&, TType>
        && ConstructibleFrom<TType, const TType>
        && ConvertibleTo<const TType, TType>;

    // Comparison concepts.
    // ====================

    template <typename TType, typename UType>
    concept EqualityComparableHelper = requires(const Templates::RemoveReference<TType>& lhs, const Templates::RemoveReference<UType>& rhs)
    {
        { lhs == rhs } -> Boolean;
        { lhs != rhs } -> Boolean;
        { rhs == lhs } -> Boolean;
        { rhs != lhs } -> Boolean;
    };

    /// \brief Concepts for types whose instances that can be equality-comparable with themselves.
    template <typename TType>
    concept EqualityComparable = EqualityComparableHelper<TType, TType>;

    /// \brief Concepts for types whose instances that can be equality-comparable with themselves.
    template <typename TType, typename UType>
    concept EqualityComparableWith = EqualityComparable<TType>
        && EqualityComparable<UType>
        && CommonReferenceWith<const Templates::RemoveReference<TType>&, const Templates::RemoveReference<UType>&>
        && EqualityComparable<Templates::CommonReference<const Templates::RemoveReference<TType>&, const Templates::RemoveReference<UType>&>>
        && EqualityComparableHelper<TType, UType>;

    /// \brief Concepts for types whose instances can be compared to each other and yield results consistent with a strict total order on TType.
    template <typename TType>
    concept TotallyOrdered = EqualityComparable<TType>
        && requires(const Templates::RemoveReference<TType> & lhs, const Templates::RemoveReference<TType> & rhs)
    {
        { lhs < rhs } -> Boolean;
        { lhs > rhs } -> Boolean;
        { lhs <= rhs } -> Boolean;
        { lhs >= rhs } -> Boolean;
    };

    /// \brief Concepts for types whose instances can be compared to instances of type UType and yield results consistent with a strict total order on TType.
    template <typename TType, typename UType>
    concept TotallyOrderedWith = TotallyOrdered<TType>
        && TotallyOrdered<UType>
        && EqualityComparableWith<TType, UType>
        && TotallyOrdered<Templates::CommonReference<const Templates::RemoveReference<TType>&, const Templates::RemoveReference<UType>&>>
        && requires(const Templates::RemoveReference<TType> & lhs, const Templates::RemoveReference<UType> & rhs)
    {
        { lhs < rhs } -> Boolean;
        { lhs > rhs } -> Boolean;
        { lhs <= rhs } -> Boolean;
        { lhs >= rhs } -> Boolean;
        { rhs < lhs } -> Boolean;
        { rhs > lhs } -> Boolean;
        { rhs <= lhs } -> Boolean;
        { rhs >= lhs } -> Boolean;
    };

    // Object concepts.
    // ================

    /// \brief Concept for types that can be moved in and to (move constructed, move assigned and reference-swappable).
    template <typename TType>
    concept Movable = Templates::IsObject<TType>
        && MoveConstructible<TType>
        && AssignableFrom<TType&, TType>
        && Swappable<TType>;


    /// \brief Concept for types that can be copied, moved and swapped.
    template <typename TType>
    concept Copyable = CopyConstructible<TType>
        && Movable<TType>
        && AssignableFrom<TType&, TType&>
        && AssignableFrom<TType&, const TType&>
        && AssignableFrom<TType&, const TType>;

    /// \brief Concept for types that are both copyable and default constructible.
    template <typename TType>
    concept Semiregular = Copyable<TType> && DefaultInitializable<TType>;

    /// \brief Concept for types that are copyable, default constructible and equality comparable.
    template <typename TType>
    concept Regular = Semiregular<TType> && EqualityComparable<TType>;

    // Callable concepts.
    // ==================

    /// \brief Concept for callable types that can be called with a set of arguments TArguments.
    template <typename TCallable, typename... TArguments>
    concept Invocable = requires(TCallable && callable, TArguments&&... arguments)
    {
        Invoke(Forward<TCallable>, Forward<TArguments>(arguments)...);
    };

    /// \brief Concept for callable types that can be called with a set of arguments TArguments and preserve both callable object state and argument state.
    template <typename TCallable, typename... TArguments>
    concept RegularInvocable = Invocable<TCallable, TArguments...>;

    /// \brief Concept for callable types that can be called with a set of arguments TArguments and preserve both callable object state and argument state.
    template <typename TPredicate, typename... TArguments>
    concept Predicate = RegularInvocable<TPredicate>
        && Boolean<Templates::InvokeResult<TPredicate, TArguments...>>;

    /// \brief Concept for callable types that define a binary relation between TType and UType.
    template <typename TPredicate, typename TType, typename UType>
    concept Relation = Predicate<TPredicate, TType, TType>
        && Predicate<TPredicate, TType, UType>
        && Predicate<TPredicate, UType, TType>
        && Predicate<TPredicate, UType, UType>;

    /// \brief Concept for predicates that define an equivalence relation between TType and UType.
    /// This concept requires that the equivalence relation is reflexive, symmetric and transitive.
    /// \remarks The distinction between this and Relation is purely semantic.
    template <typename TPredicate, typename TType, typename UType>
    concept EquivalenceRelation = Relation<TPredicate, TType, UType>;

    /// \brief Concept for predicates which impose strict weak ordering on their arguments.
    /// This concept requires that the predicate is irreflexive, transitive and, given another predicate E(a,b) : !P(a,b) && !P(b, a), E is transitive.
    /// \remarks The distinction between this and Relation is purely semantic.
    template <typename TPredicate, typename TType, typename UType>
    concept StrictWeakOrder = Relation<TPredicate, TType, UType>;
}

// ===========================================================================