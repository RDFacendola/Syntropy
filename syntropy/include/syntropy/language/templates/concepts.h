
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
    concept IsSame = Details::IsSame<TType, UType>;

    /// \brief Concept for types convertible to UType.
    template <typename TType, typename UType>
    concept IsConvertible = Details::IsConvertible<TType, UType>;

    /// \brief Concept for immutable reerence-types.
    template <typename TType>
    concept IsImmutable = Details::IsImmutable<TType>;

    /// \brief Concept for mutable reference-types.
    template <typename TType>
    concept IsMutable = Details::IsMutable<TType>;

    /// \brief Concept for movable reerence-types.
    template <typename TType>
    concept IsMovable = Details::IsMovable<TType>;

    /// \brief Concept for immovable reference-types.
    template <typename TType>
    concept IsImmovable = Details::IsImmovable<TType>;

    /// \brief Concept for
    template <typename TType>
    concept IsReference = Details::IsReference<TType>;

    /// \brief Concept for rvalue references.
    template <typename TType>
    concept IsForwarding = Details::IsForwarding<TType>;

    // Fundamental types concepts.
    // ===========================

    /// \brief Concept for boolean types.
    template <typename TType>
    concept IsBoolean = Details::IsBoolean<TType>;

    /// \brief Concept for signed integral number types.
    template <typename TType>
    concept IsIntegral = Details::IsIntegral<TType>;

    /// \brief Concept for real number types.
    template <typename TType>
    concept IsReal = Details::IsReal<TType>;

    // Polymorphism.
    // =============

    /// \brief Concept for types deriving from TBase.
    template <typename TDerived, typename TBase>
    concept IsDerivedFrom = Details::IsDerivedFrom<TDerived, TBase>;

    /// \brief Concept for types which are base classes for TDerived.
    template <typename TBase, typename TDerived>
    concept IsBaseOf = Details::IsDerivedFrom<TDerived, TBase>;

    /// \brief Constant for polymorphic types.
    template <typename TType>
    concept IsPolymorphic = Details::IsPolymorphic<TType>;

    /// \brief Constant for final types.
    template <typename TType>
    concept IsFinal = Details::IsFinal<TType>;

    // Properties concepts.
    // ====================

    /// \brief Concepts for standard-layout types.
    template <typename TType>
    concept IsStandardLayoutType = Details::IsStandardLayoutType<TType>;

    /// \brief Concept for types that can be constructed by TArguments... .
    template <typename TType, typename... TArguments>
    concept IsConstructibleFrom
        = Details::IsConstructibleFrom<TType, TArguments...>;

    /// \brief Concept for default-constructible types.
    template <typename TType>
    concept IsDefaultConstructible = Details::IsDefaultConstructible<TType>;

    /// \brief Concept for types that can be value-initialized (T()),
    ///        direct-list-initialized from and empty initializer list (T{})
    ///        or default-initialized (T t).
    template <typename TType>
    concept IsDefaultInitializable = Details::IsDefaultInitializable<TType>;

    /// \brief Concept for copy-constructible types.
    template <typename TType>
    concept IsCopyConstructible = Details::IsCopyConstructible<TType>;

    /// \brief Concept for move-constructible types.
    template <typename TType>
    concept IsMoveConstructible = Details::IsMoveConstructible<TType>;

    /// \brief Concept for copy-assignable types.
    template <typename TType>
    concept IsCopyAssignable = Details::IsCopyAssignable<TType>;

    /// \brief Concept for move-assignable types.
    template <typename TType>
    concept IsMoveAssignable = Details::IsMoveAssignable<TType>;

    /// \brief Concept for types that can be assigned from UType.
    template <typename TType, typename UType>
    concept IsAssignableFrom = Details::IsAssignableFrom<TType, UType>;

    /// \brief Concept for types whose instances can safely be destroyed at
    ///        the end of their lifetime.
    template <typename TType>
    concept IsDestructible = Details::IsDestructible<TType>;

    // Trivial.
    // ========

    /// \brief Concept for trivially default-constructible types.
    template <typename TType>
    concept IsTriviallyDefaultConstructible
        = Details::IsTriviallyDefaultConstructible<TType>;

    /// \brief Concept for trivially copy-constructible types.
    template <typename TType>
    concept IsTriviallyCopyConstructible
        = Details::IsTriviallyCopyConstructible<TType>;

    /// \brief Concept for trivially move-constructible types.
    template <typename TType>
    concept IsTriviallyMoveConstructible
        = Details::IsTriviallyMoveConstructible<TType>;

    /// \brief Concept for trivially copy-assignable types.
    template <typename TType>
    concept IsTriviallyCopyAssignable
        = Details::IsTriviallyCopyAssignable<TType>;

    /// \brief Concept for trivially move-assignable types.
    template <typename TType>
    concept IsTriviallyMoveAssignable
        = Details::IsTriviallyMoveAssignable<TType>;

    /// \brief Concept for types whose instances can safely be destroyed at
    ///        the end of their lifetime via trivial destructor.
    template <typename TType>
    concept IsTriviallyDestructible
        = Details::IsTriviallyDestructible<TType>;

    /// \brief Concept for trivially-copyable types.
    template <typename TType>
    concept IsTriviallyCopyable = Details::IsTriviallyCopyable<TType>;

    /// \brief Concept for trivial types.
    template <typename TType>
    concept IsTrivial = Details::IsTrivial<TType>;

    // Implicit.
    // =========

    /// \brief Concept for types that can be implicitly default constructed
    //         from an empty initializer list.
    template <typename TType>
    concept IsImplicitlyDefaultConstructible
        = Details::IsImplicitlyDefaultConstructible<TType>;

    /// \brief Concept for types that can be implicitly direct-constructed
    ///        from a initializer list.
    template <typename TType, typename... TArguments>
    concept IsImplicitlyConstructibleFrom
        = Details::IsImplicitlyConstructibleFrom<TType, TArguments...>;

    // Comparison concepts.
    // ====================

    /// \brief Concept for types which define both the equality and inequality
    ///        operators.
    template <typename TType>
    concept IsEqualityComparable
        = Details::IsEqualityComparableWith<TType, TType>;

    /// \brief Concept for types which define both the equality and inequality
    ///        operators against a possibily different type.
    template <typename TType, typename UType>
    concept IsEqualityComparableWith
        = Details::IsEqualityComparableWith<TType, UType>;

    /// \brief Concept for types which define boh the less-than, greater-than,
    ///        less-than-or-equal-to and greater-than-or-equal-to operators.
    template <typename TType>
    concept IsPartiallyOrdered
        = Details::IsPartiallyOrderedWith<TType, TType>;

    /// \brief Concept for types which define boh the less-than, greater-than,
    ///        less-than-or-equal-to and greater-than-or-equal-to operators
    ///        against a possibly different type.
    template <typename TType, typename UType>
    concept IsPartiallyOrderedWith
        = Details::IsPartiallyOrderedWith<TType, UType>;

    /// \brief Concept for types which are both equality-comparable and
    ///        partially-ordered.
    template <typename TType>
    concept IsOrdered
        = IsEqualityComparable<TType>
       && IsPartiallyOrdered<TType>;

    /// \brief Concept for types which are both equality-comparable and
    ///        partially-ordered against the a possibly different type.
    template <typename TType, typename UType>
    concept IsOrderedWith
        = IsEqualityComparableWith<TType, UType>
       && IsPartiallyOrderedWith<TType, UType>;

    // Templates concepts.
    // ===================

    /// \brief Concepts for types that are template specialization
    ///        of a type template.
    template<typename TType, template <typename...> typename TTemplate>
    concept IsTemplateSpecializationOf
        = Details::IsTemplateSpecializationOf<TType, TTemplate>;

}

// ===========================================================================
