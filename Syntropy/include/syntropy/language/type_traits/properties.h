
/// \file properties.h
/// \brief This header is part of Syntropy language module. It contains properties-related type traits.
///
/// This header contains mostly aliases to the standard traits, eventually modified to suite Syntropy needs and to enforce code style consistency.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"

namespace Syntropy::Traits
{
    /************************************************************************/
    /* IS CONST                                                             */
    /************************************************************************/

    /// \brief Constant equal to true if TType is constant, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsConst = std::is_const_v<TType>;

    /************************************************************************/
    /* IS TRIVIAL                                                           */
    /************************************************************************/

    /// \brief Constant equal to true if TType is trivial, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTrivial = std::is_trivial_v<TType>;

    /************************************************************************/
    /* IS TRIVIALLY COPYABLE                                                */
    /************************************************************************/

    /// \brief Constant equal to true if TType is trivially copyable, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyCopyable = std::is_trivially_copyable_v<TType>;

    /************************************************************************/
    /* IS STANDARD LAYOUT                                                   */
    /************************************************************************/

    /// \brief Constant equal to true if TType is a standard-layout class, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsStandardLayout = std::is_standard_layout_v<TType>;

    /************************************************************************/
    /* IS POLYMORPHIC                                                       */
    /************************************************************************/

    /// \brief Constant equal to true if TType is polymorphic class type, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsPolymorphic = std::is_polymorphic_v<TType>;

    /************************************************************************/
    /* IS FINAL                                                             */
    /************************************************************************/

    /// \brief Constant equal to true if TType is final class type, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsFinal = std::is_final_v<TType>;
}