
/// \file operations_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of operations-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation.h"

#include "syntropy/language/templates/templates.h"
#include "syntropy/language/templates/sfinae.h"

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* HELPERS                                                              */
    /************************************************************************/

    /// \brief Dummy method used to copy construct an instance.
    template <typename TType>
    void CopyConstruct(const TType&);

    /// \brief Check if TType is implicitly default constructible from an empty list.
    template <typename TType>
    using TestIsImplicitlyDefaultConstructible = decltype(CopyConstruct<TType>({}));

    /// \brief Check if TType is implicitly direct-constructible from a list of arguments.
    template <typename TType, typename... TArguments>
    using TestIsImplicitlyDirectConstructible = decltype(CopyConstruct<TType>({std::declval<TArguments>()...}));

    /************************************************************************/
    /* IS DEFAULT CONSTRUCTIBLE                                             */
    /************************************************************************/

    /// \brief Constant equal to true if TType is default-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsDefaultConstructible = std::is_default_constructible_v<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsDefaultConstructible<TypeList<TTypes...>> = (IsDefaultConstructible<TTypes> && ...);

    /// \brief Constant equal to true if TType is trivially default constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyDefaultConstructible = std::is_trivially_default_constructible_v<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsTriviallyDefaultConstructible<TypeList<TTypes...>> = (IsTriviallyDefaultConstructible<TTypes> && ...);

    /// \brief Constant equal to true if TType is implicitly default constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsImplicitlyDefaultConstructible = IsValidExpression<TestIsImplicitlyDefaultConstructible, TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsImplicitlyDefaultConstructible<TypeList<TTypes...>> = (IsImplicitlyDefaultConstructible<TTypes> && ...);

    /************************************************************************/
    /* IS COPY CONSTRUCTIBLE                                                */
    /************************************************************************/

    /// \brief Constant equal to true if TType is copy-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsCopyConstructible = std::is_copy_constructible_v<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsCopyConstructible<TypeList<TTypes...>> = (IsCopyConstructible<TTypes> && ...);

    /// \brief Constant equal to true if TType is trivially-copy-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyCopyConstructible = std::is_trivially_copy_constructible_v<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsTriviallyCopyConstructible<TypeList<TTypes...>> = (IsTriviallyCopyConstructible<TTypes> && ...);

    /************************************************************************/
    /* IS MOVE CONSTRUCTIBLE                                                */
    /************************************************************************/

    /// \brief Constant equal to true if TType is move-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsMoveConstructible = std::is_move_constructible_v<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsMoveConstructible<TypeList<TTypes...>> = (IsMoveConstructible<TTypes> && ...);

    /// \brief Constant equal to true if TType is trivially-move-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyMoveConstructible = std::is_trivially_move_constructible_v<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsTriviallyMoveConstructible<TypeList<TTypes...>> = (IsTriviallyMoveConstructible<TTypes> && ...);

    /************************************************************************/
    /* IS COPY ASSIGNABLE                                                   */
    /************************************************************************/

    /// \brief Constant equal to true if TType is copy-assignable, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsCopyAssignable = std::is_copy_assignable_v<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsCopyAssignable<TypeList<TTypes...>> = (IsCopyAssignable<TTypes> && ...);

    /// \brief Constant equal to true if TType is trivially-copy-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyCopyAssignable = std::is_trivially_copy_assignable_v<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsTriviallyCopyAssignable<TypeList<TTypes...>> = (IsTriviallyCopyAssignable<TTypes> && ...);

    /************************************************************************/
    /* IS MOVE ASSIGNABLE                                                   */
    /************************************************************************/

    /// \brief Constant equal to true if TType is move-assignable, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsMoveAssignable = std::is_move_assignable_v<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsMoveAssignable<TypeList<TTypes...>> = (IsMoveAssignable<TTypes> && ...);

    /// \brief Constant equal to true if TType is trivially-move-constructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyMoveAssignable = std::is_trivially_move_assignable_v<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsTriviallyMoveAssignable<TypeList<TTypes...>> = (IsTriviallyMoveAssignable<TTypes> && ...);

    /************************************************************************/
    /* IS CONSTRUCTIBLE                                                     */
    /************************************************************************/

    // IsConstructible.
    // ================

    /// \brief Helper constant used to unwrap a type list to a variable number of template arguments.
    template <Bool VEnabled, typename TType, typename... TArguments>
    inline constexpr Bool IsConstructibleHelper = IllFormed<TType, TArguments...>::kValue;

    /// \brief Specialization for arguments wrapped in type lists.
    /// Tests the first type with the first argument set and the remaining types with remaining argument sets recursively.
    template <typename TType, typename... TTypes, typename... TArguments, typename... TArgumentLists>
    inline constexpr Bool IsConstructibleHelper<true, TypeList<TType, TTypes...>, TypeList<TArguments...>, TArgumentLists...> = std::is_constructible_v<TType, TArguments...> && IsConstructibleHelper<true, TypeList<TTypes...>, TArgumentLists...>;

    // \brief Specialization for empty type list.
    template <>
    inline constexpr Bool IsConstructibleHelper<true, TypeList<>> = true;

    /// \brief Constant equal to true if TType can be constructed by TArguments... arguments, equal to false otherwise.
    template <typename TType, typename... TArguments>
    inline constexpr Bool IsConstructible = std::is_constructible_v<TType, TArguments...>;

    /// \brief Specialization for type lists.
    template <typename... TTypes, typename... TArgumentLists>
    inline constexpr Bool IsConstructible<TypeList<TTypes...>, TArgumentLists...> = IsConstructibleHelper<sizeof...(TTypes) == sizeof...(TArgumentLists), TypeList<TTypes...>, TArgumentLists...>;

    // IsImplicitlyConstructible.
    // ==========================

    /// \brief Helper constant used to unwrap a type list to a variable number of template arguments.
    template <Bool VEnabled, typename TType, typename... TArguments>
    inline constexpr Bool IsImplicitlyConstructibleHelper = IllFormed<TType, TArguments...>::kValue;

    /// \brief Specialization for arguments wrapped in type lists.
    /// Tests the first type with the first argument set and the remaining types with remaining argument sets recursively.
    template <typename TType, typename... TTypes, typename... TArguments, typename... TArgumentLists>
    inline constexpr Bool IsImplicitlyConstructibleHelper<true, TypeList<TType, TTypes...>, TypeList<TArguments...>, TArgumentLists...> = IsValidExpression<TestIsImplicitlyDirectConstructible, TType, TArguments...> && IsImplicitlyConstructibleHelper<true, TypeList<TTypes...>, TArgumentLists...>;

    // \brief Specialization for empty type list.
    template <>
    inline constexpr Bool IsImplicitlyConstructibleHelper<true, TypeList<>> = true;

    /// \brief Constant equal to true if TType is implicitly default constructible, equal to false otherwise.
    template <typename TType, typename... TArguments>
    inline constexpr Bool IsImplicitlyConstructible = IsValidExpression<TestIsImplicitlyDirectConstructible, TType, TArguments...>;

    /// \brief Specialization for type lists.
    template <typename... TTypes, typename... TArgumentLists>
    inline constexpr Bool IsImplicitlyConstructible<TypeList<TTypes...>, TArgumentLists...> = IsImplicitlyConstructibleHelper<sizeof...(TTypes) == sizeof...(TArgumentLists), TypeList<TTypes...>, TArgumentLists...>;

    /************************************************************************/
    /* IS DESTRUCTIBLE                                                      */
    /************************************************************************/

    /// \brief Constant equal to true if TType is trivially-destructible, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyDestructible = std::is_trivially_destructible_v<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsTriviallyDestructible<TypeList<TTypes...>> = (IsTriviallyDestructible<TTypes> && ...);

}