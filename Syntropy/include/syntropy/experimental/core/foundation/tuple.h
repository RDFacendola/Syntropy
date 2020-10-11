/// \file tuple.h
/// \brief This header is part of Syntropy core module. It contains definitions for tuples.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation.h"
#include "syntropy/language/templates/templates.h"

#include "syntropy/experimental/core/foundation/details/tuple_details.h"

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
        /// \brief Type of the base class.
        using TBaseClass = Tuple<TTypes...>;

        /// \brief Tuple default constructor. Disabled if exists at least one element which is not default-constructible.
        template<typename UType = TType, Details::EnableTupleExplicitDefaultConstructor<UType, TTypes...> = nullptr>
        constexpr explicit Tuple() noexcept;

        /// \brief Tuple default constructor.Disabled if exists one element which is not default - constructible.
        template<typename UType = TType, Details::EnableTupleImplicitDefaultConstructor<UType, TTypes...> = nullptr>
        constexpr Tuple() noexcept;

        /// \brief Head element.
        TType element_;
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

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Tuple<Elements...>.
    // ===================

    template <typename TType, typename... TTypes>
    template <typename UType, Details::EnableTupleExplicitDefaultConstructor<UType, TTypes...>>
    constexpr Tuple<TType, TTypes...>::Tuple() noexcept
        : TBaseClass()
        , element_{}
    {

    }

    template <typename TType, typename... TTypes>
    template <typename UType, Details::EnableTupleImplicitDefaultConstructor<UType, TTypes...>>
    constexpr Tuple<TType, TTypes...>::Tuple() noexcept
        : TBaseClass()
        , element_{}
    {

    }
}
