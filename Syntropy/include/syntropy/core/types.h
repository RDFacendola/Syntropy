
/// \file types.h
/// \brief This header is part of Syntropy core module. It contains definitions for primitive data types.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <cstdint>

namespace syntropy
{
    /************************************************************************/
    /* TYPE ALIASES                                                         */
    /************************************************************************/

    /// \brief Type of the null pointer literal, nullptr.
    using Null = std::nullptr_t;

    /// \brief Boolean value.
    using Bool = bool;

    /// \brief 64-bit integer value.
    using Int = std::int64_t;

    /// \brief 32-bit floating point value.
    using Float = float;

    /// \brief 8-bit integer value.
    using Byte = std::int8_t;

    /************************************************************************/
    /* FIXED-WIDTH INTEGER TYPES                                            */
    /************************************************************************/

    /// \brief 8-bit integer value.
    /// Fixed-width types are meant to be used with low-level IO and network systems.
    using Fix8 = std::int8_t;

    /// \brief 16-bit integer value.
    /// Fixed-width types are meant to be used with low-level IO and network systems.
    using Fix16 = std::int16_t;

    /// \brief 32-bit integer value.
    /// Fixed-width types are meant to be used with low-level IO and network systems.
    using Fix32 = std::int32_t;

    /// \brief 64-bit integer value.
    /// Fixed-width types are meant to be used with low-level IO and network systems.
    using Fix64 = std::int64_t;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Truncate a floating point value to integer.
    constexpr Int ToInt(Float rhs);

    /// \brief Convert an integer value to a floating point value.
    constexpr Float ToFloat(Int rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.

    constexpr Int ToInt(Float rhs)
    {
        return static_cast<Int>(rhs);
    }

    constexpr Float ToFloat(Int rhs)
    {
        return static_cast<Float>(rhs);
    }

}


