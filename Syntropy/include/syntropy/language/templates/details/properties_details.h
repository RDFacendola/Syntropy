
/// \file properties_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details about properties-related template machinery.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/details/type_list_details.h"

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* IS CONST                                                             */
    /************************************************************************/

    /// \brief Constant equal to true if TType is constant, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsConst = std::is_const_v<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsConst<TypeList<TTypes...>> = (IsConst<TTypes> && ...);

    /// \brief Constant equal to true if TType is constant (read-only), equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsReadOnly = std::is_const_v<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsReadOnly<TypeList<TTypes...>> = (IsReadOnly<TTypes> && ...);

    /// \brief Constant equal to true if TType is non-constant (read-write), equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsReadWrite = !std::is_const_v<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsReadWrite<TypeList<TTypes...>> = (IsReadWrite<TTypes> && ...);

    /************************************************************************/
    /* IS TRIVIAL                                                           */
    /************************************************************************/

    /// \brief Constant equal to true if TType is trivial, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTrivial = std::is_trivial_v<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsTrivial<TypeList<TTypes...>> = (IsTrivial<TTypes> && ...);

    /************************************************************************/
    /* IS TRIVIALLY COPYABLE                                                */
    /************************************************************************/

    /// \brief Constant equal to true if TType is trivially copyable, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsTriviallyCopyable = std::is_trivially_copyable_v<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsTriviallyCopyable<TypeList<TTypes...>> = (IsTriviallyCopyable<TTypes> && ...);

    /************************************************************************/
    /* IS STANDARD LAYOUT                                                   */
    /************************************************************************/

    /// \brief Constant equal to true if TType is a standard-layout class, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsStandardLayout = std::is_standard_layout_v<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsStandardLayout<TypeList<TTypes...>> = (IsStandardLayout<TTypes> && ...);

    /************************************************************************/
    /* IS POLYMORPHIC                                                       */
    /************************************************************************/

    /// \brief Constant equal to true if TType is polymorphic class type, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsPolymorphic = std::is_polymorphic_v<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsPolymorphic<TypeList<TTypes...>> = (IsPolymorphic<TTypes> && ...);

    /************************************************************************/
    /* IS FINAL                                                             */
    /************************************************************************/

    /// \brief Constant equal to true if TType is final class type, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsFinal = std::is_final_v<TType>;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Bool IsFinal<TypeList<TTypes...>> = (IsFinal<TTypes> && ...);

}

// ===========================================================================
