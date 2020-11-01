
/// \file templates_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of basic templates machinery.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    template <typename... TTyes>
    struct TypeList;
}

// ===========================================================================

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

    /************************************************************************/
    /* TYPE LIST INDEX                                                      */
    /************************************************************************/

    /// \brief Discards the first element of a type list until the first element is equal to TType or the list is exhausted.
    template <typename TType, typename TFirst, typename... UTypes>
    struct TypeListIndexHelper : TypeListIndexHelper<TType, UTypes...>{};

    /// \brief Specialization for type lists starting with TType.
    template <typename TType, typename... UTypes>
    struct TypeListIndexHelper<TType, TType, UTypes...>
    {
        static_assert((!std::is_same_v<TType, UTypes> && ...), "TType must appear exactly once in the type list.");

        static constexpr Int kValue = sizeof...(UTypes);
    };

    /// \brief Integer constant equal to the index of the first occurrence TType in TTypeList.
    /// \remarks If TType doesn't appear in TTypeList the program is ill-formed.
    template <typename TType, typename TTypeList>
    inline constexpr Int TypeListIndex = -1;

    /// \brief Specialization for type lists.
    template <typename TType, typename... UTypes>
    inline constexpr Int TypeListIndex<TType, TypeList<UTypes...>> = sizeof...(UTypes) - TypeListIndexHelper<TType, UTypes...>::kValue - 1;

    /************************************************************************/
    /* TYPE LIST ELEMENT                                                    */
    /************************************************************************/

    /// \brief Provides indexed access to type list elements' types.
    template <Int VIndex, typename TTypeList>
    struct TypeListElementHelper;

    /// \brief Specialization for type lists.
    template <Int VIndex, typename TElement, typename... TElements>
    struct TypeListElementHelper<VIndex, TypeList<TElement, TElements...>> : TypeListElementHelper<VIndex - 1, TypeList<TElements...>> {};

    /// \brief End of recursion.
    template <typename TElement, typename... TElements>
    struct TypeListElementHelper<0, TypeList<TElement, TElements...>>
    {
        using Type = TElement;
    };

    /// \brief Provides indexed access to type list elements' types.
    template <Int VIndex, typename TTypeList>
    using TypeListElement = typename TypeListElementHelper<VIndex, TTypeList>::Type;

    /************************************************************************/
    /* TYPE LIST POP FRONT                                                  */
    /************************************************************************/

    /// \brief Discards the first VCount elements in a type list and provides a type alias equal to a type list with the remaining elements.
    template <Int VCount, typename TTypeList>
    struct TypeListPopFrontHelper;

    /// \brief Specialization for type lists.
    template <Int VCount, typename TElement, typename... TElements>
    struct TypeListPopFrontHelper<VCount, TypeList<TElement, TElements...>> : TypeListPopFrontHelper<VCount - 1, TypeList<TElements...>> {};

    /// \brief End of recursion.
    template <typename TTypeList>
    struct TypeListPopFrontHelper<0, TTypeList>
    {
        using Type = TTypeList;
    };

    /// \brief Discards the first VCount elements in a type list and provides a type alias equal to a type list with the remaining elements.
    template <Int VCount, typename TTypeList>
    using TypeListPopFront = typename Details::TypeListPopFrontHelper<VCount, TTypeList>::Type;

    /************************************************************************/
    /* TYPE LIST SIZE                                                       */
    /************************************************************************/

    /// \brief Constant equal to the rank (size) of a type list.
    template <typename TTuple>
    inline constexpr Int TypeListSize = -1;

    /// \brief Specialization for type lists.
    template <typename... TElements>
    inline constexpr Int TypeListSize<TypeList<TElements...>> = sizeof...(TElements);

    /************************************************************************/
    /* ALWAYS FALSE                                                        */
    /************************************************************************/

    /// \brief Boolean constant which consume any template argument and evaluates to false.
    template <typename...>
    inline constexpr Bool AlwaysFalse = false;

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

// ===========================================================================