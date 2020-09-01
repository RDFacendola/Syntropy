
/// \file categories.h
/// \brief This header is part of Syntropy language module. It contains class-operations-related type traits.
///
/// This header contains mostly aliases to the standard traits, eventually modified to suite Syntropy needs and to enforce code style consistency.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/type_traits/capabilities.h"
#include "syntropy/language/type_traits/meta.h"

namespace Syntropy::Traits
{
    /************************************************************************/
    /* IS DEFAULT CONSTRUCTIBLE                                             */
    /************************************************************************/

    /// \brief Constant equal to true if TType is default-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsDefaultConstructible = std::is_default_constructible_v<TType>;

    /// \brief Constant equal to true if TType is trivially default constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyDefaultConstructible = std::is_trivially_default_constructible_v<TType>;

    /************************************************************************/
    /* IS COPY CONSTRUCTIBLE                                                */
    /************************************************************************/

    /// \brief Constant equal to true if TType is copy-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsCopyConstructible = std::is_copy_constructible_v<TType>;

    /// \brief Constant equal to true if TType is trivially-copy-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyCopyConstructible = std::is_trivially_copy_constructible_v<TType>;

    /************************************************************************/
    /* IS MOVE CONSTRUCTIBLE                                                */
    /************************************************************************/

    /// \brief Constant equal to true if TType is move-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsMoveConstructible = std::is_move_constructible_v<TType>;

    /// \brief Constant equal to true if TType is trivially-move-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyMoveConstructible = std::is_trivially_move_constructible_v<TType>;

    /************************************************************************/
    /* IS COPY ASSIGNABLE                                                   */
    /************************************************************************/

    /// \brief Constant equal to true if TType is copy-assignable, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsCopyAssignable = std::is_copy_assignable_v<TType>;

    /// \brief Constant equal to true if TType is trivially-copy-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyCopyAssignable = std::is_trivially_copy_assignable_v<TType>;

    /************************************************************************/
    /* IS MOVE ASSIGNABLE                                                   */
    /************************************************************************/

    /// \brief Constant equal to true if TType is move-assignable, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsMoveAssignable = std::is_move_assignable_v<TType>;

    /// \brief Constant equal to true if TType is trivially-move-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyMoveAssignable = std::is_trivially_move_assignable_v<TType>;

    /************************************************************************/
    /* IS DESTRUCTIBLE                                                      */
    /************************************************************************/

    /// \brief Constant equal to true if TType is trivially-destructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyDestructible = std::is_trivially_destructible_v<TType>;

    /************************************************************************/
    /* IS TRIVIALLY SWAPPABLE                                               */
    /************************************************************************/

    /// \brief Constant equal to true if swapping two instances of TType results in trivial operations only, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallySwappable = Conjunction<IsTriviallyDestructible<TType>, IsTriviallyMoveConstructible<TType>, IsTriviallyMoveAssignable<TType>, Negation<IsValidExpression<HasSwap, TType>>>;

}