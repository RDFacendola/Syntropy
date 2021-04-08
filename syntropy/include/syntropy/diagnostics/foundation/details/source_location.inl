
/// \file source_location.inl
///
/// \author Raffaele D. Facendola - April 2021

#pragma once

#include "syntropy/language/preprocessor/macro.h"
#include "syntropy/core/algorithms/swap.h"

// ===========================================================================

namespace Syntropy::Diagnostics
{
    /************************************************************************/
    /* MACROS                                                               */
    /************************************************************************/

    #undef SYNTROPY_HERE
    #define SYNTROPY_HERE \
        Syntropy::Diagnostics::SourceLocation{ SYNTROPY_FILE, \
                                               SYNTROPY_FUNCTION, \
                                               SYNTROPY_LINE }

    /************************************************************************/
    /* SOURCE LOCATION                                                      */
    /************************************************************************/

    inline SourceLocation
    ::SourceLocation(String file_name, String function_name, Int line) noexcept
        : file_name_(Move(file_name))
        , function_name_(Move(function_name))
        , line_(line)
    {

    }

    inline Immutable<String>
    SourceLocation
    ::GetFileName() const noexcept
    {
        return file_name_;
    }

    inline Immutable<String>
    SourceLocation
    ::GetFunctionName() const noexcept
    {
        return function_name_;
    }

    inline Int
    SourceLocation
    ::GetLine() const noexcept
    {
        return line_;
    }

    inline void
    SourceLocation
    ::Swap(Mutable<SourceLocation> other) noexcept
    {
        Algorithms::Swap(file_name_, other.file_name_);
        Algorithms::Swap(function_name_, other.function_name_);
        Algorithms::Swap(line_, other.line_);
    }

}

// ===========================================================================
