
/// \file utility.h
/// \brief This header is part of Syntropy language module. It contains extensions to standard header <utility>
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/syntropy.h"

#include <type_traits>
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
    inline constexpr DefaultConstructT kDefaultConstruct = DefaultConstructT{};

    /************************************************************************/
    /* TYPE TAG T                                                           */
    /************************************************************************/

    /// \brief Empty tag type used to propagate a type when explicit type is not supported (e.g. empty template constructors).
    /// \author Raffaele D. Facendola - May 2020.
    template <typename TType>
    struct TypeT
    {
        /// \brief Default constructor.
        explicit TypeT() = default;
    };

    /// \brief Constant of the tag type TypeT used to propagate types.
    template <typename TType>
    inline constexpr TypeT<TType> kType = TypeT<TType>{};

    /************************************************************************/
    /* DON'T CARE T                                                         */
    /************************************************************************/

    /// \brief Empty tag type which can be constructed or called with any number of arguments doing nothing.
    /// Can be used with lambdas to discard any number of arguments.
    /// \author Raffaele D. Facendola - September 2016.
    struct DontCareT
    {
        /// \brief Construct from any argument number and types.
        template <typename... TArguments>
        constexpr DontCareT(TArguments&&...) {};

        /// \brief Assign from any argument number and types.
        template <typename... TArguments>
        constexpr void operator=(TArguments&&...) const {};
    };

    /// \brief Constant of the tag type DontCareT used to discard any number of arguments.
    inline constexpr DontCareT kDontCare = DontCareT{};

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Forms lvalue reference to const type of rhs.
    /// \remarks Identical to C++17 std::as_const, wrapped for better consistency with AsNonConst.
    template <typename TType>
    constexpr std::add_const_t<TType>& AsConst(TType& rhs) noexcept;

    /// \brief Const rvalue reference deleted to disallow rvalue arguments.
    template <typename TType>
    constexpr void AsConst(const TType&&) = delete;

    /// \brief Forms lvalue reference to non-const type of rhs.
    /// \remarks Useful to write non-const getters from const ones without duplicating the implementation. Other usages are discouraged.
    template <typename TType>
    constexpr TType& AsNonConst(const TType& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    template <typename TType>
    constexpr std::add_const_t<TType>& AsConst(TType& rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr TType& AsNonConst(const TType& rhs) noexcept
    {
        return const_cast<TType&>(rhs);
    }

}