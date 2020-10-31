
/// \file templates.h
/// \brief This header is part of Syntropy language module. It contains basic definitions for template machinery.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/details/templates_details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* TYPE LIST                                                            */
    /************************************************************************/

    /// \brief Represents a ordered sequence of types.
    /// \author Raffaele D. Facendola - August 2020.
    template <typename... TTypes>
    struct TypeList {};

    /************************************************************************/
    /* IS TYPE LIST                                                         */
    /************************************************************************/

    /// \brief Constant equal to true if TType is a TypeList<>, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTypeList = Details::IsTypeList<TType>;

    /************************************************************************/
    /* TYPE LIST INDEX                                                      */
    /************************************************************************/

    /// \brief Integer constant equal to the index of the first occurrence TType in TTypeList.
    /// \remarks If TType doesn't appear in TTypeList the program is ill-formed.
    template <typename TType, typename TTypeList>
    inline constexpr Int TypeListIndex = Details::TypeListIndex<TType, TTypeList>;

    /************************************************************************/
    /* TYPE LIST ELEMENT                                                    */
    /************************************************************************/

    /// \brief Provides indexed access to type list elements' types.
    template <Int VIndex, typename TTypeList>
    using TypeListElement = Details::TypeListElement<VIndex, TTypeList>;

    /************************************************************************/
    /* TYPE LIST POP FRONT                                                  */
    /************************************************************************/

    /// \brief Discards the first VCount elements in a type list and provides a type list with the remaining elements.
    template <Int VCount, typename TTypeList>
    using TypeListPopFront = Details::TypeListPopFront<VCount, TTypeList>;

    /************************************************************************/
    /* TYPE LIST SIZE                                                       */
    /************************************************************************/

    /// \brief Constant equal to the number of elements in a type list.
    template <typename TTypeList>
    inline constexpr Int TypeListSize = Details::TypeListSize<TTypeList>;

    /************************************************************************/
    /* CONSTANT                                                             */
    /************************************************************************/

    /// \brief Wraps a static constant of type TType.
    template <typename TType, TType VValue>
    struct Constant
    {
        static constexpr TType kValue = VValue;
    };

    /************************************************************************/
    /* ALIAS                                                                */
    /************************************************************************/

    /// \brief Wraps an alias type Type.
    template <typename TType>
    struct Alias
    {
        using Type = typename TType;
    };

    /************************************************************************/
    /* TRUE                                                                 */
    /************************************************************************/

    /// \brief Boolean constant equal to true.
    using True = Constant<Bool, true>;

    /************************************************************************/
    /* FALSE                                                                */
    /************************************************************************/

    /// \brief Boolean constant equal to false.
    using False = Constant<Bool, false>;

    /************************************************************************/
    /* ALWAYS TRUE                                                          */
    /************************************************************************/

    /// \brief Boolean constant which consume any template argument and evaluates to true.
    template <typename...>
    inline constexpr Bool AlwaysTrue = false;

    /************************************************************************/
    /* ALWAYS FALSE                                                         */
    /************************************************************************/

    /// \brief Boolean constant which consume any template argument and evaluates to false.
    template <typename...>
    inline constexpr Bool AlwaysFalse = false;

    /************************************************************************/
    /* ILL-FORMED                                                           */
    /************************************************************************/

    /// \brief An alias type that results in an ill-formed program.
    template <typename... TTypes>
    struct IllFormed
    {
        static_assert(AlwaysFalse<TTypes...>, "The program is ill-formed.");
    };

    /************************************************************************/
    /* VOID                                                                 */
    /************************************************************************/

    /// \brief Metafunction that maps a sequence of types to void.
    template <typename...>
    using Void = void;

    /************************************************************************/
    /* IDENTITY                                                             */
    /************************************************************************/

    /// \brief Type equal to TType.
    /// \remarks The identity transform is meant to establish non-deduced contexts in template argument deduction.
    template <typename TType>
    using Identity = typename Details::Identity<TType>::Type;

}

// ===========================================================================