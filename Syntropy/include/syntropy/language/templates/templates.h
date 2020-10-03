
/// \file templates.h
/// \brief This header is part of Syntropy language module. It contains basic definitions for template machinery.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/details/templates_details.h"

namespace Syntropy::Templates
{
    /************************************************************************/
    /* TYPE LIST                                                            */
    /************************************************************************/

    /// \brief Represents a ordered sequence of types.
    /// \author Raffaele D. Facendola - August 2020.
    template <typename... TTypes>
    struct TypeList {};

    /// \brief Constant equal to true if TType is a TypeList<>, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTypeList = Details::IsTypeList<TType>;

    /// \brief Type equal to TypeList if TType is a TypeList, otherwise the program is ill-formed.
    template <typename TType>
    using ExpectTypeList = Details::ExpectTypeList<TType>;

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
    /* ALWAYS FALSE                                                        */
    /************************************************************************/

    /// \brief Boolean constant which consume any template argument and evaluates to false.
    template <typename...>
    inline constexpr Bool AlwaysFalse = false;

    /************************************************************************/
    /* VOID                                                                 */
    /************************************************************************/

    /// \brief Metafunction that maps a sequence of types to void.
    template <typename...>
    using Void = void;
}