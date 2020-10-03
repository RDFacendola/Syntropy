
/// \file categories.h
/// \brief This header is part of Syntropy language module. It contains class-operations-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/logic.h"
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
    inline constexpr Bool IsDefaultConstructible = std::is_default_constructible_v<TType>;

    /// \brief Constant equal to true if TType is trivially default constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyDefaultConstructible = std::is_trivially_default_constructible_v<TType>;

    /// \brief Constant equal to true if TType is implicitly default constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsImplicitlyDefaultConstructible = Details::IsImplicitlyDefaultConstructible<TType>;

    /************************************************************************/
    /* ARE DEFAULT CONSTRUCTIBLE                                            */
    /************************************************************************/

    /// \brief Constant equal to true if all types in TTypeList are default constructible, false otherwise.
    template <typename TTypeList>
    inline constexpr Bool AreDefaultConstructible = Details::AreDefaultConstructible<TTypeList>::kValue;

    /// \brief Constant equal to true if all types in TTypeList are implicitly default constructible, false otherwise.
    template <typename TTypeList>
    inline constexpr Bool AreImplicitlyDefaultConstructible = Details::AreImplicitlyDefaultConstructible<TTypeList>::kValue;

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
    /* ARE COPY CONSTRUCTIBLE                                               */
    /************************************************************************/

    /// \brief Constant equal to true if all types in TTypeList are copy-constructible, false otherwise.
    template <typename TTypeList>
    inline constexpr Bool AreCopyConstructible = Details::AreCopyConstructible<TTypeList>::kValue;

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
    /* IS CONSTRUCTIBLE                                                     */
    /************************************************************************/

    /// \brief Constant equal to true if TType can be constructed by TArguments... arguments, equal to false otherwise.
    template <typename TType, typename... TArguments>
    inline constexpr Bool IsConstructible = std::is_constructible_v<TType, TArguments...>;

    /************************************************************************/
    /* ARE CONSTRUCTIBLE                                                    */
    /************************************************************************/

    /// \brief Constant equal to true if each type in TTypeList can be constructed via the respective argument in the list TArgumentList, equal to false otherwise.
    /// \remarks If TTyleList and TArgumentList rank are different, this constant is false.
    template <typename TTypeList, typename TArgumentList>
    inline constexpr Bool AreConstructible = Details::AreConstructible<TTypeList, TArgumentList>::kValue;

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
    inline constexpr Bool IsTriviallySwappable = Templates::Conjunction<IsTriviallyDestructible<TType>, IsTriviallyMoveConstructible<TType>, IsTriviallyMoveAssignable<TType>, Negation<IsValidExpression<HasSwap, TType>>>;

}