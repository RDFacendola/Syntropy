
/// \file templates_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of basic templates machinery.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

/************************************************************************/
/* FORWARD DECLARATIONS                                                 */
/************************************************************************/

namespace Syntropy::Templates
{
    template <typename... TTyes>
    struct TypeList;
}

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* ALWAYS FALSE                                                        */
    /************************************************************************/

    /// \brief Boolean constant which consume any template argument and evaluates to false.
    template <typename...>
    inline constexpr Bool AlwaysFalse = false;

    /************************************************************************/
    /* IS TYPE LIST                                                         */
    /************************************************************************/

    /// \brief Constant equal to true if TType is a TypeList, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTypeList = false;

    /// \brief Partial template specialization for TypeLists.
    template <typename... TTypes>
    inline constexpr Bool IsTypeList<TypeList<TTypes...>> = true;

    /************************************************************************/
    /* IDENTITY                                                             */
    /************************************************************************/

    /// \brief Exposes a member type equal to TType
    /// \remarks The identity transform is meant to establish non-deduced contexts in template argument deduction.
    template <typename TType>
    struct Identity
    {
        using Type = TType;
    };

}