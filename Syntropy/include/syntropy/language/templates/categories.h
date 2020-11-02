
/// \file categories.h
/// \brief This header is part of Syntropy language module. It contains categories-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/details/categories_details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* PRIMARY TYPE CATEGORIES                                              */
    /************************************************************************/

    /// \brief Constant equal to true if TType is void-type, equal to false otherwise.
    template<typename TType>
    inline constexpr Bool IsVoid = Details::IsVoid<TType>;

    /// \brief Constant equal to true if TType is a null-type, equal to false otherwise.
    template<typename TType>
    inline constexpr Bool IsNull = Details::IsNull<TType>;

    /// \brief Constant equal to true if TType is boolean, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsBoolean = Details::IsBoolean<TType>;

    /// \brief Constant equal to true if TType is integral, equal to false otherwise.
    /// \remarks Note that this trait is fundamentally different than std::is_integral as it only yields true for int and fixed-size ints. Booleans, characters and bytes are not considered to be integrals.
    template <typename TType>
    inline constexpr Bool IsIntegral = Details::IsIntegral<TType>;

    /// \brief Constant equal to true if TType is a real number, equal to false otherwise.
    /// \remarks Note that this traits is different than std::is_floating_point as it only yields true for floats. Doubles are not supported.
    template <typename TType>
    inline constexpr Bool IsReal = Details::IsReal<TType>;

    /// \brief Constant equal to true if TType is an enumeration type, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsEnum = Details::IsEnum<TType>;

    /// \brief Constant equal to true if TType is a non-union class type, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsClass = Details::IsClass<TType>;

    /// \brief Constant equal to true if TType is a lvalue reference, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsLValueReference = Details::IsLValueReference<TType>;

    /// \brief Constant equal to true if TType is a rvalue reference, equal to false otherwise.
    template <typename TType>
    inline constexpr Bool IsRValueReference = Details::IsRValueReference<TType>;

}

// ===========================================================================