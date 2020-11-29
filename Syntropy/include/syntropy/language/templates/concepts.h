
/// \file concepts.h
/// \brief This header is part of the Syntropy language module. It contains definitions for fundamental concepts.
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

    /// \brief Concept for types which are exactly equal to UType and vice-versa.
    template <typename TType, typename UType>
    concept SameAs = Details::SameAs<TType, UType>;

    /// \brief Concept for types deriving from TBase ignoring constant-qualifiers.
    template <typename TDerived, typename TBase>
    concept DerivedFrom = Details::DerivedFrom<TDerived, TBase>;

    /// \brief Concept for types convertible to UType.
    template <typename TType, typename UType>
    concept ConvertibleTo = Details::ConvertibleTo<TType, UType>;

    /// \brief Concept for types that share a common reference type.
    template <typename TType, typename UType>
    concept CommonReferenceWith = Details::CommonReferenceWith<TType, UType>;

    /// \brief Concept for types that share a common type.
    template <typename TType, typename UType>
    concept CommonWith = Details::CommonWith<TType, UType>;

    /// \brief Concept for boolean types.
    template <typename TType>
    concept Boolean = Details::Boolean<TType>;

    /// \brief Concept for signed integral number types.
    template <typename TType>
    concept Integral = Details::Integral<TType>;

    /// \brief Concept for real number types.
    template <typename TType>
    concept Real = Details::Real<TType>;

    /// \brief Concept for an expression type which can be assigned from UType
    template <typename TType, typename UType>
    concept AssignableFrom = Details::AssignableFrom<TType, UType>;

    /// \brief Concept for a type whose instances are swappable.
    template <typename TType>
    concept Swappable = Details::Swappable<TType>;

    /// \brief Concept for a type whose instances can be swapped with instances of type UType.
    template <typename TType, typename UType>
    concept SwappableWith = Details::SwappableWith<TType, UType>;

    /// \brief Concept for types whose instances can safely be destroyed at the end of their lifetime (including reference types).
    template <typename TType>
    concept Destructible = Details::Destructible<TType>;

    /// \brief Concept for types whose instances can be initialized with a set of arguments TArgs....
    template <typename TType, typename... TArguments>
    concept ConstructibleFrom = Details::ConstructibleFrom<TType, TArguments...>;

    /// \brief Concept for types that can be value-initialized (T()), direct-list-initialized from and empty initializer list (T{}) or default-initialized (T t).
    template <typename TType>
    concept DefaultInitializable = Details::DefaultInitializable<TType>;

    /// \brief Concept for types that are reference-type or object-type constructible from rvalue of the same type in both direct-initialization and copy-initialization.
    template <typename TType>
    concept MoveConstructible = Details::MoveConstructible<TType>;

    /// \brief Concept for types that are either lvalue references or, if move-constructible, copy-constructible by means of direct and copy-initialization that leave the source unchanged afte the copy.
    template <typename TType>
    concept CopyConstructible = Details::CopyConstructible<TType>;

    // Object concepts.
    // ================

    /// \brief Concept for types that can be moved and swapped.
    template <typename TType>
    concept Movable = Details::Movable<TType>;

    /// \brief Concept for types that can be copied, moved and swapped.
    template <typename TType>
    concept Copyable = Details::Copyable<TType>;

    /// \brief Concept for types that are both copyable and default constructible.
    template <typename TType>
    concept Semiregular = Details::Semiregular<TType>;

    /// \brief Concept for types that are copyable, default constructible and equality comparable.
    template <typename TType>
    concept Regular = Details::Regular<TType>;

    // Callable concepts.
    // ==================

    /// \brief Concept for callable types that can be called with a set of arguments TArguments.
    template <typename TCallable, typename... TArguments>
    concept Invocable = Details::Invocable<TCallable, TArguments...>;

    /// \brief Concept for callable types that can be called with a set of arguments TArguments and preserve both callable object state and argument state.
    /// \remarks The distinction between this and Invocable is purely semantic, as Invocable may not satisfy equality preservation.
    template <typename TCallable, typename... TArguments>
    concept RegularInvocable = Details::RegularInvocable<TCallable, TArguments...>;

    /// \brief Concept for callable types that can be called with a set of arguments TArguments and preserve both callable object state and argument state.
    template <typename TCallable, typename... TArguments>
    concept Predicate = Details::Predicate<TCallable, TArguments...>;

    /// \brief Concept for callable types that define a binary relation between TType and UType.
    template <typename TPredicate, typename TType, typename UType>
    concept Relation = Details::Relation<TPredicate, TType, UType>;

    /// \brief Concept for predicates that define an equivalence relation between TType and UType.
    /// This concept requires that the equivalence relation is reflexive, symmetric and transitive.
    /// \remarks The distinction between this and Relation is purely semantic.
    template <typename TPredicate, typename TType, typename UType>
    concept EquivalenceRelation = Details::EquivalenceRelation<TPredicate, TType, UType>;

    /// \brief Concept for predicates which impose strict weak ordering on their arguments.
    /// This concept requires that the predicate is irreflexive, transitive and, given another predicate E(a,b) : !P(a,b) && !P(b, a), E is transitive.
    /// \remarks The distinction between this and Relation is purely semantic.
    template <typename TPredicate, typename TType, typename UType>
    concept StrictWeakOrder = Details::StrictWeakOrder<TPredicate, TType, UType>;
}

// ===========================================================================