
/// \file tuple_details.h
///
/// \brief This header is part of Syntropy core module.
//         It contains implementation details for tuples.
///
/// \author Raffaele D. Facendola - 2020.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/type_traits.h"

// ===========================================================================

namespace Syntropy::Details
{
    /************************************************************************/
    /* TUPLE                                                                */
    /************************************************************************/

    /// \brief Alias for a type-list.
    template <typename... TElements>
    using TupleList = Templates::TypeList<TElements...>;

    // Default constructor.
    // ====================

    /// \brief True if all tuple elements are default-constructible.
    template <typename... TElements>
    inline constexpr
    Bool TupleDefaultConstructor
        = (Templates::IsConstructible<TElements> && ...);

    /// \brief True if at least one tuple element is default-constructible.
    template <typename... TTypes>
    inline constexpr
    Bool TupleExplicitDefaultConstructor
        = !(Templates::IsConstructible<TTypes> && ...);

    // Direct constructor.
    // ===================

    /// \brief True if each element in TElements can be constructed from
    ///        its respective element in UElements.
    template <typename TElements, typename... UElements>
    inline constexpr
    Bool TupleDirectConstructor
        = false;

    /// \brief Specialization for type list - parameter pack pairs.
    template <typename... TElements, typename... UElements>
    requires (sizeof...(TElements) == sizeof...(UElements))
    inline constexpr
    Bool TupleDirectConstructor<TupleList<TElements...>, UElements...>
        = (Templates::IsConstructible<
               TElements, Templates::ForwardingOf<UElements>> && ...);

    /// \brief True if at least one among UElements is not implicitly
    ///        convertible to its relative element in TElements.
    template <typename TElements, typename... UElements>
    inline constexpr
    Bool TupleExplicitDirectConstructor
        = false;

    /// \brief Specialization for type list - parameter pack pairs.
    template <typename... TElements, typename... UElements>
    requires (sizeof...(TElements) == sizeof...(UElements))
    inline constexpr
    Bool TupleExplicitDirectConstructor<TupleList<TElements...>, UElements...>
        = !(Templates::IsConvertible<
                Templates::ForwardingOf<UElements>, TElements> && ...);

    // Copy constructor.
    // =================

    /// \brief True if each element in TElements can be
    ///        converting-copy-constructed from its respective element in
    ///        UElements.
    template <typename TElements, typename... UElements>
    inline constexpr
    Bool TupleCopyConstructor
        = false;

    /// \brief Specialization for type list - parameter pack pairs.
    template <typename... TElements, typename... UElements>
    requires (sizeof...(TElements) == sizeof...(UElements))
    inline constexpr
    Bool TupleCopyConstructor<TupleList<TElements...>, UElements...>
        = (Templates::IsConstructible<
               TElements, Templates::ImmutableOf<UElements>> && ...);

    /// \brief True if at least one among UElements is not implicitly
    ///        copy-convertible to its relative element in TElements.
    template <typename TElements, typename... UElements>
    inline constexpr
    Bool TupleExplicitCopyConstructor
        = false;

    /// \brief Specialization for type list - parameter pack pairs.
    template <typename... TElements, typename... UElements>
    requires (sizeof...(TElements) == sizeof...(UElements))
    inline constexpr
    Bool TupleExplicitCopyConstructor<TupleList<TElements...>, UElements...>
        = !(Templates::IsConvertible<
                Templates::ImmutableOf<UElements>, TElements> && ...);

    // Move constructor.
    // =================

    /// \brief True if each element in TElements can be
    ///        converting-move-constructed from its respective element in
    ///        UElements.
    template <typename TElements, typename... UElements>
    inline constexpr
    Bool TupleMoveConstructor
        = false;

    /// \brief Specialization for type list - parameter pack pairs.
    template <typename... TElements, typename... UElements>
    requires (sizeof...(TElements) == sizeof...(UElements))
    inline constexpr
    Bool TupleMoveConstructor<TupleList<TElements...>, UElements...>
        = (Templates::IsConstructible<
               TElements, Templates::MovableOf<UElements>> && ...);

    /// \brief True if at least one among UElements is not implicitly
    ///        copy-convertible to its relative element in TElements.
    template <typename TElements, typename... UElements>
    inline constexpr
    Bool TupleExplicitMoveConstructor
        = false;

    /// \brief Specialization for type list - parameter pack pairs.
    template <typename... TElements, typename... UElements>
    requires (sizeof...(TElements) == sizeof...(UElements))
    inline constexpr
    Bool TupleExplicitMoveConstructor<TupleList<TElements...>, UElements...>
        = !(Templates::IsConvertible<
                Templates::MovableOf<UElements>, TElements> && ...);

    // Copy assignment.
    // ================

    /// \brief True if each element in TElements can be
    ///        converting-copy-assigned from its respective element in
    ///        UElements.
    template <typename TElements, typename... UElements>
    inline constexpr
    Bool TupleCopyAssignment
        = false;

    /// \brief Specialization for type list - parameter pack pairs.
    template <typename... TElements, typename... UElements>
    requires (sizeof...(TElements) == sizeof...(UElements))
    inline constexpr
    Bool TupleCopyAssignment<TupleList<TElements...>, UElements...>
        = (Templates::IsAssignable<
               TElements, Templates::ImmutableOf<UElements>> && ...);

   // Move assignment.
   // ================

   /// \brief True if each element in TElements can be
   ///        converting-move-assigned from its respective element in
   ///        UElements.
   template <typename TElements, typename... UElements>
   inline constexpr
   Bool TupleMoveAssignment
       = false;

   /// \brief Specialization for type list - parameter pack pairs.
   template <typename... TElements, typename... UElements>
   requires (sizeof...(TElements) == sizeof...(UElements))
   inline constexpr
   Bool TupleMoveAssignment<TupleList<TElements...>, UElements...>
       = (Templates::IsAssignable<
              TElements, Templates::MovableOf<UElements>> && ...);

    /************************************************************************/
    /* TUPLE BASE                                                           */
    /************************************************************************/

    /// \brief Walk tuple class hierarchy upwards until TCount drops to 0 or
    ///        the tuple is exhausted.
    template <Int TCount, typename TTuple>
    struct TupleBaseHelper {};

    /// \brief Specialization for tuples.
    template <template <typename...> typename TTuple,
              Int TCount,
              typename TElement,
              typename... TElements>
    struct TupleBaseHelper<TCount, TTuple<TElement, TElements...>>
        : TupleBaseHelper<TCount - 1, TTuple<TElements...>> {};

    /// \brief End of recursion.
    template <template <typename...> typename TTuple, typename... TElements>
    struct TupleBaseHelper<0, TTuple<TElements...>>
        : Templates::Alias<TTuple<TElements...>> {};

    /// \brief Access a tuple base type by index.
    template <Int TCount, typename TTuple>
    using TupleBase
        = typename TupleBaseHelper<TCount, TTuple>::Type;

}

// ===========================================================================

namespace Syntropy::Records::Details
{
    /************************************************************************/
    /* TYPE TRAITS                                                          */
    /************************************************************************/

    /// \brief Helper function for ElementTypeListOf.
    template <typename TRecord,
              template <Int, typename> typename TElementTypeTrait,
              Int... TIndex>
    auto
    ElementTypeListOfHelper(Templates::Sequence<TIndex...>) noexcept
        -> Templates::TypeList<typename TElementTypeTrait<TIndex,
                                                          TRecord>::Type...>;

    /// \brief List of types of a record's elements.
    template <typename TRecord,
              typename TSequence,
              template <Int, typename> typename TElementTypeTrait>
    using
    ElementTypeListOf = decltype(
        ElementTypeListOfHelper<TRecord, TElementTypeTrait>(TSequence{}));

}

// ===========================================================================
