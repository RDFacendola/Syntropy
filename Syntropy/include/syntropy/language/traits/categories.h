
/// \file categories.h
/// \brief This header is part of Syntropy language module. It contains categories-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/traits/relationships.h"

namespace Syntropy::Templates
{
    /************************************************************************/
    /* IS VOID                                                              */
    /************************************************************************/

    /// \brief Constant equal to true if TType is void-type, equal to false otherwise.
    template<typename TType>
    inline constexpr Bool IsVoid = std::is_void<TType>;

    /************************************************************************/
    /* IS NULL                                                              */
    /************************************************************************/

    /// \brief Constant equal to true if TType is a null-type, equal to false otherwise.
    template<typename TType>
    inline constexpr Bool IsNull = std::is_null_pointer_v<TType>;

    /************************************************************************/
    /* IS BOOLEAN                                                           */
    /************************************************************************/

    /// \brief Constant equal to true if TType is boolean, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsBoolean = IsSame<TType, Bool>;

    /************************************************************************/
    /* IS INTEGRAL                                                          */
    /************************************************************************/

    /// \brief Constant equal to true if TType is integral, equal to false otherwise.
    /// \remarks Note that this trait is fundamentally different than std::is_integral as it only yields true for int and fixed-size ints. Booleans, characters and bytes are not considered to be integrals.
    template <typename TType>
    inline constexpr Bool IsIntegral = IsSame<TType, Int> || IsSame<TType, Fix8> || IsSame<TType, Fix16> || IsSame<TType, Fix32> || IsSame<TType, Fix64>;

    /************************************************************************/
    /* IS REAL                                                              */
    /************************************************************************/

    /// \brief Constant equal to true if TType is a real number, equal to false otherwise.
    /// \remarks Note that this traits is different than std::is_floating_point as it only yields true for floats. Doubles are not supported.
    template <typename TType>
    inline constexpr Bool IsReal = IsSame<TType, Float>;

    /************************************************************************/
    /* IS ENUM                                                              */
    /************************************************************************/

    /// \brief Constant equal to true if TType is an enumeration type, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsEnum = std::is_enum_v<TType>;

    /************************************************************************/
    /* IS CLASS                                                             */
    /************************************************************************/

    /// \brief Constant equal to true if TType is a non-union class type, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsClass = std::is_class_v<TType>;

    /************************************************************************/
    /* IS LVALUE REFERENCE                                                  */
    /************************************************************************/

    /// \brief Constant equal to true if TType is a lvalue reference, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsLValueReference = std::is_lvalue_reference_v<TType>;

    /************************************************************************/
    /* IS RVALUE REFERENCE                                                  */
    /************************************************************************/

    /// \brief Constant equal to true if TType is a rvalue reference, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsRValueReference = std::is_rvalue_reference_v<TType>;

}