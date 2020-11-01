
/// \file properties.h
/// \brief This header is part of Syntropy language module. It contains properties-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/details/properties_details.h"

namespace Syntropy::Templates
{
    /************************************************************************/
    /* IS CONST                                                             */
    /************************************************************************/

    /// \brief Constant equal to true if TType is constant, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsConst = std::is_const_v<TType>;

    /// \brief Constant equal to true if TType is constant (read-only), equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsReadOnly = IsConst<TType>;

    /// \brief Constant equal to true if TType is non-constant (read-write), equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsReadWrite = !IsConst<TType>;

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