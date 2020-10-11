
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
    /* IS IMPLICITLY DEFAULT CONSTRUCTIBLE                                  */
    /************************************************************************/

    /// \brief Dummy method used to copy construct an argument.
    template <typename TType>
    void CopyConstruct(const TType&);

    /// \brief Check if TType is implicitly default constructible from an empty list.
    template <typename TType>
    using TestIsImplicitlyDefaultConstructible = decltype(CopyConstruct<TType>({}));

    template <typename TType>
    inline constexpr Bool IsImplicitlyDefaultConstructible = IsValidExpression<TestIsImplicitlyDefaultConstructible, TType>;

    /************************************************************************/
    /* ARE DEFAULT CONSTRUCTIBLE                                            */
    /************************************************************************/

    /// \brief If each type in TTypes is default constructible exposes a member constant Value equal to true, otherwise equal to false.
    template <typename... TTypes>
    struct AreDefaultConstructible
    {
        static inline constexpr Bool kValue = (AreDefaultConstructible<TTypes>::kValue && ...);
    };

    /// \brief Specialization for individual types.
    template <typename TType>
    struct AreDefaultConstructible<TType>
    {
        static inline constexpr Bool kValue = std::is_default_constructible_v<TType>;
    };

    /// \brief Specialization for empty type lists.
    template <>
    struct AreDefaultConstructible<TypeList<>>
    {
        static inline constexpr Bool kValue = true;
    };

    /// \brief Partial template specialization for type lists.
    template <typename TType, typename... TTypes>
    struct AreDefaultConstructible<TypeList<TType, TTypes...>> : AreDefaultConstructible<TType, TTypes...>
    {

    };

    //

    /// \brief If each type in TTypes is implicitly default constructible exposes a member constant Value equal to true, otherwise equal to false.
    template <typename... TTypes>
    struct AreImplicitlyDefaultConstructible
    {
        static inline constexpr Bool kValue = (AreImplicitlyDefaultConstructible<TTypes>::kValue && ...);
    };

    /// \brief Specialization for individual types.
    template <typename TType>
    struct AreImplicitlyDefaultConstructible<TType>
    {
        static inline constexpr Bool kValue = IsImplicitlyDefaultConstructible<TType>;
    };

    /// \brief Specialization for empty type lists.
    template <>
    struct AreImplicitlyDefaultConstructible<TypeList<>>
    {
        static inline constexpr Bool kValue = true;
    };

    /// \brief Partial template specialization for type lists.
    template <typename TType, typename... TTypes>
    struct AreImplicitlyDefaultConstructible<TypeList<TType, TTypes...>> : AreImplicitlyDefaultConstructible<TType, TTypes...>
    {

    };

    /************************************************************************/
    /* ARE COPY CONSTRUCTIBLE                                               */
    /************************************************************************/

    /// \brief If each type in TTypes is copy-constructible exposes a member constant Value equal to true, otherwise equal to false.
    template <typename... TTypes>
    struct AreCopyConstructible
    {
        static inline constexpr Bool kValue = (AreCopyConstructible<TTypes>::kValue && ...);
    };

    /// \brief Specialization for individual types.
    template <typename TType>
    struct AreCopyConstructible<TType>
    {
        static inline constexpr Bool kValue = std::is_copy_constructible_v<TType>;
    };

    /// \brief Specialization for empty type lists.
    template <>
    struct AreCopyConstructible<TypeList<>>
    {
        static inline constexpr Bool kValue = true;
    };

    /// \brief Partial template specialization for type lists.
    template <typename TType, typename... TTypes>
    struct AreCopyConstructible<TypeList<TType, TTypes...>> : AreCopyConstructible<TType, TTypes...>
    {

    };

    /************************************************************************/
    /* ARE CONSTRUCTIBLE                                                    */
    /************************************************************************/

    /// \brief If each type in the type list TTypeList is copy-constructible exposes a member constant Value equal to true, otherwise equal to false.
    template <typename TTypeList, typename TArgumentList>
    struct AreConstructible
    {
        static_assert(AlwaysFalse<TTypeList, TArgumentList>, "Not a TypeList.");
    };

    /// \brief Specialization for empty lists.
    template <>
    struct AreConstructible<TypeList<>, TypeList<>>
    {
        static inline constexpr Bool kValue = true;
    };

    /// \brief Specialization for when type list is exhausted before the argument list.
    template <typename TArgument, typename... TArguments>
    struct AreConstructible<TypeList<>, TypeList<TArgument, TArguments...>>
    {
        static inline constexpr Bool kValue = false;
    };

    /// \brief Specialization for when argument list is exhausted before the type list.
    template <typename TType, typename... TTypes>
    struct AreConstructible<TypeList<TType, TTypes...>, TypeList<>>
    {
        static inline constexpr Bool kValue = false;
    };

    /// \brief Partial template specialization for type lists. Type is constructed by a list of types.
    template <typename TType, typename... TTypes, typename... TArguments, typename... UArguments>
    struct AreConstructible<TypeList<TType, TTypes...>, TypeList<TypeList<UArguments...>, TArguments...>>
    {
        static inline constexpr Bool kValue = std::is_constructible_v<TType, UArguments...> && AreConstructible<TypeList<TTypes...>, TypeList<TArguments...>>::kValue;
    };

}