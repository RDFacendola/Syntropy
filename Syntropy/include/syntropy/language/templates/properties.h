
/// \file properties.h
/// \brief This header is part of Syntropy language module. It contains properties-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/details/properties_details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* IS CONST                                                             */
    /************************************************************************/

    /// \brief Constant equal to true if TType is constant, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsConst = Details::IsConst<TType>;

    /// \brief Constant equal to true if TType is constant (read-only), equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsReadOnly = Details::IsReadOnly<TType>;

    /// \brief Constant equal to true if TType is non-constant (read-write), equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsReadWrite = Details::IsReadWrite<TType>;

    /************************************************************************/
    /* IS TRIVIAL                                                           */
    /************************************************************************/

    /// \brief Constant equal to true if TType is trivial, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTrivial = Details::IsTrivial<TType>;

    /************************************************************************/
    /* IS TRIVIALLY COPYABLE                                                */
    /************************************************************************/

    /// \brief Constant equal to true if TType is trivially copyable, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyCopyable = Details::IsTriviallyCopyable<TType>;

    /************************************************************************/
    /* IS STANDARD LAYOUT                                                   */
    /************************************************************************/

    /// \brief Constant equal to true if TType is a standard-layout class, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsStandardLayout = Details::IsStandardLayout<TType>;

    /************************************************************************/
    /* IS POLYMORPHIC                                                       */
    /************************************************************************/

    /// \brief Constant equal to true if TType is polymorphic class type, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsPolymorphic = Details::IsPolymorphic<TType>;

    /************************************************************************/
    /* IS FINAL                                                             */
    /************************************************************************/

    /// \brief Constant equal to true if TType is final class type, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsFinal = Details::IsFinal<TType>;
}

// ===========================================================================
