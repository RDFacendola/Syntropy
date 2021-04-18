
/// \file priority.h
///
/// \brief This header is part of Syntropy language module.
///        It contains template definitions for priority type tags.
///
/// Customization point objects based on:
/// https://quuxplusone.github.io/blog/2018/03/19/
/// customization-points-for-functions/
///
/// \author Raffaele D. Facendola - September 2020.

// ===========================================================================

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
    struct Priority
        : Priority<TPriority - 1>{};

    /// \brief Topmost priority tag type.
    template <>
    struct Priority<0> {};

    /// \brief Constant of the tag-type Priority used to sort an overload
    ///        function set according to an explicit priority value.
    template <Int TPriority>
    inline constexpr
    Priority kPriority
        = Priority<TPriority>{};

    /************************************************************************/
    /* TYPE ALIASES                                                         */
    /************************************************************************/

    // Core idea based on this amazing post:
    // https://wandbox.org/permlink/AB9uQxO2MymNDDtt

    /// \brief Priority of a custom extension implementation.
    using ExtensionPriority
        = Priority<4>;

    /// \brief Priority of a member-function implementation.
    using MemberFunctionPriority
        = Priority<3>;

    /// \brief Priority of a member-operator implementation.
    using MemberOperatorPriority
        = Priority<2>;

    /// \brief Priority of a non-member-function implementation.
    using NonMemberFunctionPriority
        = Priority<1>;

    /// \brief Priority of a fallback implementation.
    using FallbackPriority
        = Priority<0>;

    /// \brief Highest priority among method implementations.
    inline constexpr
    Priority kMaxPriority
        = ExtensionPriority{};

}

// ===========================================================================
