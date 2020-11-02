
/// \file type_list.h
/// \brief This header is part of Syntropy language module. It contains definitions for type list templates.
///
/// \author Raffaele D. Facendola - November 2020.

#pragma once

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/rank.h"

#include "syntropy/language/templates/details/type_list_details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* TYPE LIST                                                            */
    /************************************************************************/

    /// \brief Represents a ordered sequence of types.
    /// \author Raffaele D. Facendola - August 2020.
    template <typename... TTypes>
    using TypeList = Details::TypeList<TTypes...>;

    /************************************************************************/
    /* TYPE TRAITS                                                          */
    /************************************************************************/

    /// \brief Integer constant equal to the index a type TType in TTypeList.
    /// \remarks If TType doesn't appear exactly once in TTypeList the program is ill-formed.
    template <typename TType, typename TTypeList>
    inline constexpr Int TypeListIndex = Details::TypeListIndex<TType, TTypeList>;

    /// \brief Provides indexed access to type list elements' types.
    template <Int VIndex, typename TTypeList>
    using TypeListElement = Details::TypeListElement<VIndex, TTypeList>;

    /// \brief Discards the first VCount elements in a type list and provides a type list with the remaining ones.
    template <Int VCount, typename TTypeList>
    using TypeListPopFront = Details::TypeListPopFront<VCount, TTypeList>;

    /// \brief Partial template specialization for TypeLists.
    template <typename... TTypes>
    inline constexpr Int RankImplementation<TypeList<TTypes...>> = sizeof...(TTypes);
}

// ===========================================================================