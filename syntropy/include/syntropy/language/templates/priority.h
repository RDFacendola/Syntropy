
/// \file priority.h
/// \brief This header is part of Syntropy language module.
///        It contains template definitions for priority type tags.
///
/// Customization point objects based on:
/// https://quuxplusone.github.io/blog/2018/03/19/
/// customization-points-for-functions/
///
/// \author Raffaele D. Facendola - Sep 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* PRIORITY                                                             */
    /************************************************************************/

    /// \brief A compile-time tag-type used to sort an overload function
    ///        set according to an explicit priority value.
    template <Int TPriority>
    struct Priority : Priority<TPriority - 1>
    {

    };

    /// \brief Topmost priority tag type.
    template <>
    struct Priority<0>
    {

    };

    /// \brief Constant of the tag-type Priority used to sort an overload
    ///        function set according to an explicit priority value.
    template <Int TPriority>
    inline constexpr Priority kPriority
        = Priority<TPriority>{};

}

// ===========================================================================
