
/// \file categories.h
/// \brief This header is part of Syntropy language module. It contains class-operations-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/capabilities.h"
#include "syntropy/language/templates/sfinae.h"
#include "syntropy/language/templates/templates.h"

#include "syntropy/language/templates/details/operations_details.h"

namespace Syntropy::Templates
{
    /************************************************************************/
    /* IS DEFAULT CONSTRUCTIBLE                                             */
    /************************************************************************/

    /// \brief Constant equal to true if TType is default-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsDefaultConstructible = Details::IsDefaultConstructible<TType>;

    /// \brief Constant equal to true if TType is trivially default constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyDefaultConstructible = Details::IsTriviallyDefaultConstructible<TType>;

    /// \brief Constant equal to true if TType is implicitly default constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsImplicitlyDefaultConstructible = Details::IsImplicitlyDefaultConstructible<TType>;

    /************************************************************************/
    /* IS COPY CONSTRUCTIBLE                                                */
    /************************************************************************/

    /// \brief Constant equal to true if TType is copy-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsCopyConstructible = Details::IsCopyConstructible<TType>;

    /// \brief Constant equal to true if TType is trivially-copy-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyCopyConstructible = Details::IsTriviallyCopyConstructible<TType>;

    /************************************************************************/
    /* IS MOVE CONSTRUCTIBLE                                                */
    /************************************************************************/

    /// \brief Constant equal to true if TType is move-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsMoveConstructible = Details::IsMoveConstructible<TType>;

    /// \brief Constant equal to true if TType is trivially-move-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyMoveConstructible = Details::IsTriviallyMoveConstructible<TType>;

    /************************************************************************/
    /* IS COPY ASSIGNABLE                                                   */
    /************************************************************************/

    /// \brief Constant equal to true if TType is copy-assignable, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsCopyAssignable = Details::IsCopyAssignable<TType>;

    /// \brief Constant equal to true if TType is trivially-copy-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyCopyAssignable = Details::IsTriviallyCopyAssignable<TType>;

    /************************************************************************/
    /* IS MOVE ASSIGNABLE                                                   */
    /************************************************************************/

    /// \brief Constant equal to true if TType is move-assignable, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsMoveAssignable = Details::IsMoveAssignable<TType>;

    /// \brief Constant equal to true if TType is trivially-move-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyMoveAssignable = Details::IsTriviallyMoveAssignable<TType>;

    /************************************************************************/
    /* IS CONSTRUCTIBLE                                                     */
    /************************************************************************/

    /// \brief Constant equal to true if TType can be constructed by TArguments... arguments, equal to false otherwise.
    /// This trait supports TypeLists in the form IsConstructible<TypeList<A,B,C>, TypeList<a>, TypeList<b>, TypeList<>>.
    /// Type-lists TArguments must have the same exact rank as the number of elements in TType, otherwise the program is ill-formed.
    template <typename TType, typename... TArguments>
    inline constexpr Bool IsConstructible = Details::IsConstructible<TType, TArguments...>;

    /************************************************************************/
    /* IS DESTRUCTIBLE                                                      */
    /************************************************************************/

    /// \brief Constant equal to true if TType is trivially-destructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyDestructible = Details::IsTriviallyDestructible<TType>;

    /************************************************************************/
    /* IS TRIVIALLY SWAPPABLE                                               */
    /************************************************************************/

    /// \brief Constant equal to true if swapping two instances of TType results in trivial operations only, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallySwappable = (IsTriviallyDestructible<TType> && IsTriviallyMoveConstructible<TType> && IsTriviallyMoveAssignable<TType> && (!IsValidExpression<HasSwap, TType>));

}