
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

    /// \brief True if UType is the same as TType.
    template <typename TType, typename UType>
    concept IsSame
        = Details::IsSame<TType, UType>;

    /// \brief True if TType is convertible to UType.
    template <typename TType, typename UType>
    concept IsConvertible
        = Details::IsConvertible<TType, UType>;

    /// \brief True if TType is a read-only value-type.
    template <typename TType>
    concept IsReadOnlyValue
        = Details::IsReadOnlyValue<TType>;

    /// \brief True if TType is a read-write value-type.
    template <typename TType>
    concept IsReadWriteValue
        = Details::IsReadWriteValue<TType>;

    /// \brief True if TType is an immutable reference-type.
    template <typename TType>
    concept IsImmutableReference
        = Details::IsImmutableReference<TType>;

    /// \brief True if TType is a mutable reference-type.
    template <typename TType>
    concept IsMutableReference
        = Details::IsMutableReference<TType>;

    /// \brief True if TType is a mutable reference-type.
    template <typename TType>
    concept IsMovableReference
        = Details::IsMovableReference<TType>;

    /// \brief True if TType is a immovable reference-type.
    template <typename TType>
    concept IsImmovableReference
        = Details::IsImmovableReference<TType>;

    // Fundamental types concepts.
    // ===========================

    /// \brief True if all types are boolean.
    template <typename TType>
    concept IsBoolean
        = Details::IsBoolean<TType>;

    /// \brief True if all types are integral.
    template <typename TType>
    concept IsIntegral
        = Details::IsIntegral<TType>;

    /// \brief True if all types are real.
    template <typename TType>
    concept IsReal
        = Details::IsReal<TType>;

    // Polymorphism.
    // =============

    /// \brief True if TBase is a base class for TDerived.
    template <typename TBase, typename TDerived>
    concept IsBaseOf
        = Details::IsBaseOf<TBase, TDerived>;

    /// \brief True if TType is polymorphic
    template <typename TType>
    concept IsPolymorphic
        = Details::IsPolymorphic<TType>;

    /// \brief True if all types are final.
    template <typename TType>
    concept IsFinal
        = Details::IsFinal<TType>;

    // Properties concepts.
    // ====================

    /// \brief Concept for types constructible from TArguments.
    template <typename TType, typename... TArguments>
    concept IsConstructible
        = Details::IsConstructible<TType, TArguments...>;

    /// \brief Concept for types that are both move-constructible and
    ///        move-assignable.
    template <typename TType>
    concept IsMovable
        = Details::IsMovable<TType>;

    /// \brief Concept for types that are movable, copy-constructible and
    ///        copy-assignable.
    template <typename TType>
    concept IsCopyable
        = Details::IsCopyable<TType>;

    /// \brief True if TType can be assignable from UType.
    template <typename TType, typename UType>
    concept IsAssignable
        = Details::IsAssignable<TType, UType>;

    // Trivial.
    // ========

    /// \brief Concept for types that are both trivially-copy-constructible and
    ///        trivially-copy-assignable.
    template <typename TType>
    concept IsTriviallyCopyable
        = Details::IsTriviallyCopyable<TType>;

    /// \brief Concepts for types that are both trivially copyable,
    ///        trivially-default-constructibles and trivially-destructible.
    template <typename TType>
    concept IsTrivial
        = Details::IsTrivial<TType>;

    // Comparison concepts.
    // ====================

    /// \brief Concepts for types that can be compared equal to themselves.
    template <typename TType>
    concept IsEqualityComparable
        = Details::IsEqualityComparable<TType>;

    /// \brief Concepts for types that can be three-way compared to themselves.
    template <typename TType>
    concept IsComparable
        = Details::IsComparable<TType>;

    // Templates.
    // ==========

    /// \brief Concept for types that are template specialization of TTemplate.
    template<typename TType, template <typename...> typename TTemplate>
    concept IsTemplateSpecializationOf
        = Details::IsTemplateSpecializationOf<TType, TTemplate>;

    // Type lists.
    // ===========

    /// \brief Concept for template specialization of Sequence.
    template <typename TType>
    concept IsTypeList
        = Details::IsTypeList<TType> ;

    // Sequence.
    // =========

    /// \brief True if all types are Sequences.
    template <typename TType>
    concept IsSequence
        = Details::IsSequence<TType>;

    /// \brief True if all sequences are monotonically ncreasing contiguous
    ///        sequences.
    template <typename TType>
    concept IsContiguousSequence
        = Details::IsContiguousSequence<TType>;

}

// ===========================================================================
