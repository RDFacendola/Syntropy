/// \file tuple.h
/// \brief This header is part of Syntropy core module. It contains definitions for tuples.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation.h"
#include "syntropy/language/templates/templates.h"

namespace Syntropy::Experimental
{
    /************************************************************************/
    /* TUPLE <ELEMENTS...>                                                  */
    /************************************************************************/

     /// \brief Represents a fixed-size collection of heterogeneous elements.
     /// \author Raffaele D. Facendola - September 2020.
    template <typename... TTypes>
    struct Tuple;

    /// \brief Recursive tuple definition.
    template <typename TType, typename... TTypes>
    struct Tuple<TType, TTypes...> : private Tuple<TTypes...>
    {
        /// \brief List of template argument types.
        using TTypeList = Templates::TypeList<TType, TTypes...>;
    };

    /// \brief Empty tuple.
    template <>
    struct Tuple<>
    {
        /// \brief Default constructor.
        constexpr Tuple() noexcept = default;
 
        /// \brief Default copy constructor.
        constexpr Tuple(const Tuple&) noexcept = default;
 
        /// \brief Default copy-assignment.
        constexpr Tuple& operator=(const Tuple&) noexcept = default;
    };
}
