
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

    /// \brief If each type in the type list TTypeList is default constructible exposes a member constant Value equal to true, otherwise equal to false.
    template <typename TTypeList>
    struct AreDefaultConstructible
    {
        static_assert(AlwaysFalse<TTypeList>, "Not a TypeList.");
    };

    /// \brief Specialization for empty lists.
    template <>
    struct AreDefaultConstructible<TypeList<>>
    {
        static inline constexpr Bool kValue = true;
    };

    /// \brief Partial template specialization for type lists.
    template <typename TType, typename... TTypes>
    struct AreDefaultConstructible<TypeList<TType, TTypes...>>
    {
        static inline constexpr Bool kValue = std::conjunction_v<std::is_default_constructible<TType>, std::is_default_constructible<TTypes>...>;
    };

    //

    /// \brief If each type in the type list TTypeList is implicitly default constructible exposes a member constant Value equal to true, otherwise equal to false.
    template <typename TTypeList>
    struct AreImplicitlyDefaultConstructible
    {
        static_assert(AlwaysFalse<TTypeList>, "Not a TypeList.");
    };

    /// \brief Specialization for empty lists.
    template <>
    struct AreImplicitlyDefaultConstructible<TypeList<>>
    {
        static inline constexpr Bool kValue = true;
    };

    /// \brief Partial template specialization for type lists.
    template <typename TType, typename... TTypes>
    struct AreImplicitlyDefaultConstructible<TypeList<TType, TTypes...>>
    {
        static inline constexpr Bool kValue = IsImplicitlyDefaultConstructible<TType> && (IsImplicitlyDefaultConstructible<TTypes> && ...);
    };

    /************************************************************************/
    /* ARE COPY CONSTRUCTIBLE                                               */
    /************************************************************************/

    /// \brief If each type in the type list TTypeList is copy-constructible exposes a member constant Value equal to true, otherwise equal to false.
    template <typename TTypeList>
    struct AreCopyConstructible
    {
        static_assert(AlwaysFalse<TTypeList>, "Not a TypeList.");
    };

    /// \brief Specialization for empty lists.
    template <>
    struct AreCopyConstructible<TypeList<>>
    {
        static inline constexpr Bool kValue = true;
    };

    /// \brief Partial template specialization for type lists.
    template <typename TType, typename... TTypes>
    struct AreCopyConstructible<TypeList<TType, TTypes...>>
    {
        static inline constexpr Bool kValue = std::conjunction_v<std::is_copy_constructible<TType>, std::is_copy_constructible<TTypes>...>;
    };
}