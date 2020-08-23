
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


