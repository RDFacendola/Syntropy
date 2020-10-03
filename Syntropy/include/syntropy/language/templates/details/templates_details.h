
/// \file templates_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of basic templates machinery.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

namespace Syntropy::Templates
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    template <typename... TTyes>
    struct TypeList;

}
namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* IS TYPE LIST                                                         */
    /************************************************************************/

    /// \brief Constant equal to true if TType is a TypeList, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTypeList = false;

    /// \brief Partial template specialization for TypeLists.
    template <typename... TTypes>
    inline constexpr Bool IsTypeList<TypeList<TTypes...>> = true;

}