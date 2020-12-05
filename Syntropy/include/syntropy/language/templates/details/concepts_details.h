
/// \file concepts_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of fundamental concepts definitions.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"

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
    concept DerivedFrom = Templates::IsBaseOf<TBase, TDerived> && Templates::IsConvertible<PtrRef<TDerived>, PtrRef<TBase>>;

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
        && CommonReferenceWith<Ref<Templates::RemoveReference<TType>>, Ref<Templates::RemoveReference<UType>>>
        && requires(TType lhs, ForwardRef<UType> rhs)
    {
        { lhs = Forward<UType>(rhs) } -> SameAs<TType>;
    };

    /// \brief Concept for a type whose instances are swappable.
    template <typename TType>
    concept Swappable = requires(Mutable<TType> lhs, Mutable<TType> rhs)
    {
        Swap(lhs, rhs);
    };

    /// \brief Concept for a type whose instances can be swapped with instances of type UType.
    template <typename TType, typename UType>
    concept SwappableWith = CommonReferenceWith<TType, UType>
        && requires(ForwardRef<TType> lhs, ForwardRef<UType> rhs)
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
        && ConstructibleFrom<TType, Mutable<TType>>
        && ConvertibleTo<Mutable<TType>, TType>
        && ConstructibleFrom<TType, Ref<TType>>
        && ConvertibleTo<Ref<TType>, TType>
        && ConstructibleFrom<TType, Val<TType>>
        && ConvertibleTo<Val<TType>, TType>;

    // Comparison concepts.
    // ====================

    /// \brief Models a type TType for which the equality and inequality operators against the (possibly different) type UType are defined.
    template <typename TType, typename UType>
    concept EqualityComparableWithHelper = requires(Ref<Templates::RemoveReference<TType>> lhs, Ref<Templates::RemoveReference<UType>>rhs)
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

    /// \brief Models a type TType for which the equality and inequality operators are defined.
    template <typename TType>
    concept EqualityComparable = EqualityComparableWithHelper<TType, TType>;

    /// \brief Models a type TType for which the equality and inequality operators against the (possibly different) type UType are defined.
    template <typename TType, typename UType>
    concept EqualityComparableWith = EqualityComparable<TType>
        && EqualityComparable<UType>
        && CommonReferenceWith<Ref<Templates::RemoveReference<TType>>,Ref<Templates::RemoveReference<UType>>>
        && EqualityComparable<Templates::CommonReference<Ref<Templates::RemoveReference<TType>>, Ref<Templates::RemoveReference<UType>>>>
        && EqualityComparableWithHelper<TType, UType>;

    /// \brief Models a type TType for which the less-than, greater-than, less-than-or-equal-to and greater-than-or-equal-to operators against the (possibly different) type UType are defined.
    template <typename TType, typename UType>
    concept PartiallyOrderedWithHelper = requires(Ref<Templates::RemoveReference<TType>> lhs, Ref<Templates::RemoveReference<UType>> rhs)
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

    /// \brief Models a type TType for which the less-than, greater-than, less-than-or-equal-to and greater-than-or-equal-to operators are defined.
    template <typename TType>
    concept PartiallyOrdered = PartiallyOrderedWithHelper<TType, TType>;

    /// \brief Models a type TType for which the less-than, greater-than, less-than-or-equal-to and greater-than-or-equal-to operators against the (possibly different) type UType are defined.
    template <typename TType, typename UType>
    concept PartiallyOrderedWith = PartiallyOrdered<TType>
        && PartiallyOrdered<UType>
        && CommonReferenceWith<Ref<Templates::RemoveReference<TType>>, Ref<Templates::RemoveReference<UType>>>
        && PartiallyOrdered<Templates::CommonReference<Ref<Templates::RemoveReference<TType>>, Ref<Templates::RemoveReference<UType>>>>
        && PartiallyOrderedWithHelper<TType, UType>;

    /// \brief Models a class TType which is both equality-comparable and partially-ordered against the (possibly different) type UType.
    template <typename TType, typename UType>
    concept TotallyOrderedWithHelper = EqualityComparableWith<TType, UType> && PartiallyOrderedWith<TType, UType>;

    /// \brief Models a class TType which is both equality-comparable and partially-ordered.
    template <typename TType>
    concept TotallyOrdered = TotallyOrderedWithHelper<TType, TType>;

    /// \brief Models a class TType which is both equality-comparable and partially-ordered against the (possibly different) type UType.
    template <typename TType, typename UType>
    concept TotallyOrderedWith = TotallyOrdered<TType>
        && TotallyOrdered<UType>
        && CommonReferenceWith<Ref<Templates::RemoveReference<TType>>, Ref<Templates::RemoveReference<UType>>>
        && TotallyOrdered<Templates::CommonReference<Ref<Templates::RemoveReference<TType>>, Ref<Templates::RemoveReference<UType>>>>
        && TotallyOrderedWithHelper<TType, UType>;

    // Object concepts.
    // ================

    /// \brief Concept for types that can be moved in and to (move constructed, move assigned and reference-swappable).
    template <typename TType>
    concept Movable = Templates::IsObject<TType>
        && MoveConstructible<TType>
        && AssignableFrom<Mutable<TType>, TType>
        && Swappable<TType>;


    /// \brief Concept for types that can be copied, moved and swapped.
    template <typename TType>
    concept Copyable = CopyConstructible<TType>
        && Movable<TType>
        && AssignableFrom<Mutable<TType>, Mutable<TType>>
        && AssignableFrom<Mutable<TType>, Ref<TType>>
        && AssignableFrom<Mutable<TType>, Val<TType>>;

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
    concept Invocable = requires(ForwardRef<TCallable> callable, TArguments&&... arguments)
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
