
/// \file utility.h
/// \brief This header is part of Syntropy language module. It contains extensions to standard header <utility>
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/syntropy.h"

#include <utility>

namespace syntropy
{
    /************************************************************************/
    /* DEFAULT CONSTRUCT T                                                  */
    /************************************************************************/

    /// \brief Empty tag type used to disambiguate between functions that forward tuples to construct objects.
    /// \author Raffaele D. Facendola - April 2020.
    struct DefaultConstructT
    { 
        /// \brief Default constructor.
        explicit DefaultConstructT() = default;
    };

    /// \brief Constant of the tag type DefaultConstructT used to create objects via default-constructor.
    inline constexpr DefaultConstructT kDefaultConstruct = DefaultConstructT();

}