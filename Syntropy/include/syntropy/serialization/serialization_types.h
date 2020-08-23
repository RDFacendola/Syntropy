
/// \file serialization_types.h
/// \brief This header is part of Syntropy serialization module. It contains definitions for serialization-related fundamental data types.
///
/// \author Raffaele D. Facendola - August 2020

#pragma once

#include <ostream>
#include <cstdint>
#include <cstddef>

#include "syntropy/language/fundamentals.h"

namespace Syntropy
{

    /************************************************************************/
    /* TYPE CAST                                                            */
    /************************************************************************/

    /// \brief Cast rhs to a 8-bit integer value.
    template <typename TNumber>
    constexpr Fix8 ToFix8(TNumber rhs) noexcept;

    /// \brief Cast rhs to a 16-bit integer value.
    template <typename TNumber>
    constexpr Fix16 ToFix16(TNumber rhs) noexcept;

    /// \brief Cast rhs to a 32-bit integer value.
    template <typename TNumber>
    constexpr Fix32 ToFix32(TNumber rhs) noexcept;

    /// \brief Cast rhs to a 64-bit integer value.
    template <typename TNumber>
    constexpr Fix64 ToFix64(TNumber rhs) noexcept;

    /************************************************************************/
    /* STREAM INSERTION                                                     */
    /************************************************************************/

    /// \brief Stream insertion for 8-bit integer types.
    std::ostream& operator<<(std::ostream& out, Fix8 rhs) noexcept;

    /// \brief Stream insertion for 16-bit integer types.
    std::ostream& operator<<(std::ostream& out, Fix16 rhs) noexcept;

    /// \brief Stream insertion for 32-bit integer types.
    std::ostream& operator<<(std::ostream& out, Fix32 rhs) noexcept;

    /// \brief Stream insertion for 64-bit types.
    std::ostream& operator<<(std::ostream& out, Fix64 rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Type cast.
    // ==========

    template <typename TNumber>
    constexpr Fix8 ToFix8(TNumber rhs) noexcept
    {
        return static_cast<Fix8>(rhs);
    }

    template <typename TNumber>
    constexpr Fix16 ToFix16(TNumber rhs) noexcept
    {
        return static_cast<Fix16>(rhs);
    }

    template <typename TNumber>
    constexpr Fix32 ToFix32(TNumber rhs) noexcept
    {
        return static_cast<Fix32>(rhs);
    }

    template <typename TNumber>
    constexpr Fix64 ToFix64(TNumber rhs) noexcept
    {
        return static_cast<Fix64>(rhs);
    }

    // Stream insertion.
    // =================

    inline std::ostream& operator<<(std::ostream& out, Fix8 rhs) noexcept
    {
        out << ToInt(rhs);

        return out;
    }

    inline std::ostream& operator<<(std::ostream& out, Fix16 rhs) noexcept
    {
        out << ToInt(rhs);

        return out;
    }

    inline std::ostream& operator<<(std::ostream& out, Fix32 rhs) noexcept
    {
        out << ToInt(rhs);

        return out;
    }

    inline std::ostream& operator<<(std::ostream& out, Fix64 rhs) noexcept
    {
        out << ToInt(rhs);

        return out;
    }

}


