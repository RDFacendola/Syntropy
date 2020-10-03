
/// \file operations_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of operations-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation.h"

#include "syntropy/language/templates/templates.h"

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
    using IsImplicitlyDefaultConstructible = decltype(CopyConstruct<TType>({}));

    /************************************************************************/
    /* ARE DEFAULT CONSTRUCTIBLE                                            */
    /************************************************************************/

    /// \brief If each type in the type list TTypeList is default constructible exposes a member constant Value equal to true, otherwise equal to false.
    template <typename TTypeList>
    struct AreDefaultConstructible
    {
        static_assert(AlwaysFalse<TTypeList>, "Not a TypeList.");
    };

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes>
    struct AreDefaultConstructible<TypeList<TTypes...>>
    {
        static inline constexpr Bool kValue = std::conjunction_v<std::is_default_constructible<TTypes>...>;
    };

}