
/// \file templates_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of basic templates machinery.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* CONSTANT                                                             */
    /************************************************************************/

    /// \brief Exposes a member variable kValue of type TType and equal to VValue.
    template <typename TType, TType VValue>
    struct Constant
    {
        static constexpr TType kValue = VValue;
    };

    /************************************************************************/
    /* ALIAS                                                                */
    /************************************************************************/

    /// \brief Exposes a member type Type equal to TType.
    template <typename TType>
    struct Alias
    {
        using Type = typename TType;
    };

    /************************************************************************/
    /* TRUE \ FALSE                                                         */
    /************************************************************************/

    /// \brief Boolean constant equal to true.
    using True = Constant<Bool, true>;

    /// \brief Boolean constant equal to false.
    using False = Constant<Bool, false>;

    /************************************************************************/
    /* ALWAYS TRUE \ FALSE                                                  */
    /************************************************************************/

    /// \brief Boolean constant which consume any template argument and evaluates to true.
    template <typename...>
    inline constexpr Bool AlwaysTrue = false;

    /// \brief Boolean constant which consume any template argument and evaluates to false.
    template <typename...>
    inline constexpr Bool AlwaysFalse = false;

    /************************************************************************/
    /* VOID                                                                 */
    /************************************************************************/

    /// \brief Metafunction that maps a sequence of types to void.
    template <typename...>
    using Void = void;

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
    /* IDENTITY                                                             */
    /************************************************************************/

    /// \brief Exposes a member type equal to TType.
    /// \remarks The identity transform is meant to establish non-deduced contexts in template argument deduction.
    template <typename TType>
    struct Identity
    {
        using Type = TType;
    };

}

// ===========================================================================