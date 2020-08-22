
/// \file interpolations.h
/// \brief This header is part of the syntropy math system. It contains interpolating functions.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/language.h"

namespace Syntropy
{
    /************************************************************************/
    /* MATH                                                                 */
    /************************************************************************/

    /// \brief Exposes interpolating functions.
    /// \author Raffaele D. Facendola - May 2020.
    namespace Math
    {
        /// \brief Linearly interpolate lhs and rhs using alpha as blending factor.
        template <typename TType>
        constexpr TType Lerp(const TType& lhs, const TType& rhs, Float alpha);
    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    template <typename TType>
    constexpr TType Math::Lerp(const TType& lhs, const TType& rhs, Float alpha)
    {
        return lhs + (rhs - lhs) * alpha;
    }

}