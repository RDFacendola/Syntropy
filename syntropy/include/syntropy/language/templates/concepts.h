
/// \file concepts.h
///
/// \brief This header is part of the Syntropy language module.
///        It contains definitions for fundamental concepts.
///
/// \author Raffaele D. Facendola - November 2020

// ===========================================================================

#pragma once

// ===========================================================================

#include "details/concepts.details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* CONCEPTS                                                             */
    /************************************************************************/



    // Core concepts.
    // ==============

    /// \brief True if all types in UTypes are equal to TType.
    template <typename TType, typename... UTypes>
    concept IsSame
        = (Details::IsSame<TType, UTypes> && ...);

    /// \brief True if all types in UTypes are convertible to TType.
    template <typename TType, typename... UTypes>
    concept IsConvertible
        = (Details::IsConvertible<TType, UTypes> && ...);

    /// \brief True if all types are immutable.
    template <typename... TTypes>
    concept IsImmutable
        = (Details::IsImmutable<TTypes> && ...);

    /// \brief True if all types are mutable.
    template <typename... TTypes>
    concept IsMutable
        = (Details::IsMutable<TTypes> && ...);

    /// \brief True if all types are movable.
    template <typename... TTypes>
    concept IsMovable
        = (Details::IsMovable<TTypes> && ...);

    /// \brief True if all types are immovable.
    template <typename... TTypes>
    concept IsImmovable
        = (Details::IsImmovable<TTypes> && ...);

    // Fundamental types concepts.
    // ===========================

    /// \brief True if all types are boolean.
    template <typename... TTypes>
    concept IsBoolean
        = (Details::IsBoolean<TTypes> && ...);

    /// \brief True if all types are integral.
    template <typename... TTypes>
    concept IsIntegral
        = (Details::IsIntegral<TTypes> && ...);

    /// \brief True if all types are real.
    template <typename... TTypes>
    concept IsReal
        = (Details::IsReal<TTypes> && ...);

    // Polymorphism.
    // =============

    /// \brief True if TDerived derives from all types in TBases.
    template <typename TDerived, typename... TBases>
    concept IsDerivedFrom
        = (Details::IsDerivedFrom<TDerived, TBases> && ...);

    /// \brief True if TBase is a base class for all types in TDerived.
    template <typename TBase, typename... TDerived>
    concept IsBaseOf
        = (Details::IsDerivedFrom<TDerived, TBase> && ...);

    /// \brief True if all types are polymorphic.
    template <typename... TTypes>
    concept IsPolymorphic
        = (Details::IsPolymorphic<TTypes> && ...);

    /// \brief True if all types are final.
    template <typename... TTypes>
    concept IsFinal
        = (Details::IsFinal<TTypes> && ...);

    // Properties concepts.
    // ====================

    /// \brief True if all types are standard layout types.
    template <typename... TTypes>
    concept IsStandardLayoutType
        = (Details::IsStandardLayoutType<TTypes> && ...);

    /// \brief True is TType can be constructed by TArguments.
    template <typename TType, typename... TArguments>
    concept IsConstructibleFrom
        = Details::IsConstructibleFrom<TType, TArguments...>;

    /// \brief True if all types are default-constructible.
    template <typename... TTypes>
    concept IsDefaultConstructible
        = (Details::IsDefaultConstructible<TTypes> && ...);

    /// \brief True if all types are default-initializable.
    ///
    /// \remarks A type T is default-initializable if it can be:
    ///          T() - value-initialized
    ///          T{} - direct-list-initialized from an empty initializer list
    ///          T t - default-initialized.
    template <typename... TTypes>
    concept IsDefaultInitializable
        = (Details::IsDefaultInitializable<TTypes> && ...);

    /// \brief True if all types are copy-constructible.
    template <typename... TTypes>
    concept IsCopyConstructible
        = (Details::IsCopyConstructible<TTypes> && ...);

    /// \brief True if all types are move-constructible.
    template <typename... TTypes>
    concept IsMoveConstructible
        = (Details::IsMoveConstructible<TTypes> && ...);

    /// \brief True if all types are copy-assignable.
    template <typename... TTypes>
    concept IsCopyAssignable
        = (Details::IsCopyAssignable<TTypes> && ...);

    /// \brief True if all types are move-assignable.
    template <typename... TTypes>
    concept IsMoveAssignable
        = (Details::IsMoveAssignable<TTypes> && ...);

    /// \brief True if TType can be assignable from all types in UTypes.
    template <typename TType, typename... UTypes>
    concept IsAssignableFrom
        = (Details::IsAssignableFrom<TType, UTypes> && ...);

    /// \brief True if all types in UTypes can be assignable from TType.
    template <typename TType, typename... UTypes>
    concept IsAssignableTo
        = (Details::IsAssignableFrom<UTypes, TType> && ...);

    /// \brief True if all types can be safely destroyed at the end of their
    ///        lifetime.
    template <typename... TTypes>
    concept IsDestructible
        = (Details::IsDestructible<TTypes> && ...);

    // Trivial.
    // ========

    /// \brief True if all types are trivially default-constructible.
    template <typename... TTypes>
    concept IsTriviallyDefaultConstructible
        = (Details::IsTriviallyDefaultConstructible<TTypes> && ...);

    /// \brief True if all types are trivially copy-constructible.
    template <typename... TTypes>
    concept IsTriviallyCopyConstructible
        = (Details::IsTriviallyCopyConstructible<TTypes> && ...);

    /// \brief True if all types are trivially move-constructible.
    template <typename... TTypes>
    concept IsTriviallyMoveConstructible
        = (Details::IsTriviallyMoveConstructible<TTypes> && ...);

    /// \brief True if all types are trivially copy-assignable.
    template <typename... TTypes>
    concept IsTriviallyCopyAssignable
        = (Details::IsTriviallyCopyAssignable<TTypes> && ...);

    /// \brief True if all types are trivially move-assignable.
    template <typename... TTypes>
    concept IsTriviallyMoveAssignable
        = (Details::IsTriviallyMoveAssignable<TTypes> && ...);

    /// \brief True if all types can be safely destroyed at the end of their
    ///        lifetime via a trivial destructor.
    template <typename... TTypes>
    concept IsTriviallyDestructible
        = (Details::IsTriviallyDestructible<TTypes> && ...);

    /// \brief True if all types are trivially-copyable.
    template <typename... TTypes>
    concept IsTriviallyCopyable
        = (Details::IsTriviallyCopyable<TTypes> && ...);

    /// \brief True if all types are trivial.
    template <typename... TTypes>
    concept IsTrivial
        = (Details::IsTrivial<TTypes> && ...);

    // Implicit.
    // =========

    /// \brief True if all types can be implicitly default constructed from an
    ///        empty initializer list.
    template <typename... TTypes>
    concept IsImplicitlyDefaultConstructible
        = (Details::IsImplicitlyDefaultConstructible<TTypes> && ...);

    /// \brief True if TType can be implicitly direct-constructed from a
    ///        initializer list.
    template <typename TType, typename... TArguments>
    concept IsImplicitlyConstructibleFrom
        = Details::IsImplicitlyConstructibleFrom<TType, TArguments...>;

    // Comparison concepts.
    // ====================

    /// \brief True if all types define both the equality and inequality
    ///        operators.
    template <typename... TTypes>
    concept IsEqualityComparable
        = (Details::IsEqualityComparableWith<TTypes, TTypes> && ...);

    /// \brief True if TType define both the equality and inequality
    ///        operators against all types in UTypes.
    template <typename TType, typename... UTypes>
    concept IsEqualityComparableWith
        = (Details::IsEqualityComparableWith<TType, UTypes> && ...);

    /// \brief True if all types define boh the less-than, greater-than,
    ///        less-than-or-equal-to and greater-than-or-equal-to operators.
    template <typename... TTypes>
    concept IsPartiallyOrdered
        = (Details::IsPartiallyOrderedWith<TTypes, TTypes> && ...);

    /// \brief True if TType define boh the less-than, greater-than,
    ///        less-than-or-equal-to and greater-than-or-equal-to operators
    ///        against all types in UTypes.
    template <typename TType, typename... UTypes>
    concept IsPartiallyOrderedWith
        = (Details::IsPartiallyOrderedWith<TType, UTypes> && ...);

    /// \brief True if all types are both equality-comparable and
    ///        partially-ordered.
    template <typename... TTypes>
    concept IsOrdered
        = IsEqualityComparable<TTypes...>
       && IsPartiallyOrdered<TTypes...>;

    /// \brief True if TType is both equality-comparable and partially-ordered
    ///        against all types in UTypes..
    template <typename TType, typename... UTypes>
    concept IsOrderedWith
        = IsEqualityComparableWith<TType, UTypes...>
       && IsPartiallyOrderedWith<TType, UTypes...>;

    // Templates.
    // ==========

    /// \brief True if TType is a template specialization of all TTemplates.
    template<typename TType, template <typename...> typename... TTemplates>
    concept IsTemplateSpecializationOf
        = (Details::IsTemplateSpecializationOf<TType, TTemplates> && ...);

    /// \brief True if TType is a template for all template specialization in
    ///        TTypes.
    template<template <typename...> typename TTemplate, typename... TTypes>
    concept IsTemplateOf
        = (Details::IsTemplateSpecializationOf<TTypes, TTemplate> && ...);

    // Type lists.
    // ===========

    /// \brief True if all types are TypeLists.
    template <typename... TTypes>
    concept IsTypeList
        = (Details::IsTypeList<TTypes> && ...);

    // Sequence.
    // =========

    /// \brief True if all types are Sequences.
    template <typename... TTypes>
    concept IsSequence
        = (Details::IsSequence<TTypes> && ...);

    /// \brief True if all sequences are monotonically ncreasing contiguous
    ///        sequences.
    template <typename... TSequences>
    concept IsContiguousSequence
        = (Details::IsContiguousSequence<TSequences> && ...);

}

// ===========================================================================
