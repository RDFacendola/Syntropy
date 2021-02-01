
/// \file macro.h
/// \brief This header is part of Syntropy language module.
///         It contains macro definitions.
///
/// \author Raffaele D. Facendola - Nov 2016

#pragma once

#include "syntropy/hal/hal_macro.h"
#include "syntropy/language/templates/type_traits.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* MACROS                                                               */
    /************************************************************************/

    /// \brief Special macro used to "forward-declare" a macro and move
    ///         its definition somewhere else.
    /// This macro is intended to separate macro declaration site
    ///  from definition and to avoid clutter near the macro declaration site.
    /// Declaration and definition site must exist in the same file!
    /// \usage
    ///
    /// Declaration site. Forward declaration only.
    ///  Any usage before definition will trigger a static assert.
    ///
    ///  #define MY_MACRO(arg0, arg1) SYNTROPY_MACRO_DECLARATION(arg0, arg1)
    ///
    /// Definition site. Undef previous forward declaration and provide
    ///  actual definition.
    ///
    ///  #undef MY_MACRO
    ///  #define MY_MACRO(arg0, arg1) (arg0 + arg1)
    #define SYNTROPY_MACRO_DECLARATION(...) \
    []() { static_assert(Syntropy::Templates::AlwaysFalse<#__VA_ARGS__>, "Undefined macro!"); }

    /// \brief Expands to the current file name.
    #define SYNTROPY_FILE \
        __FILE__

    /// \brief Expands to the current line number.
    #define SYNTROPY_LINE \
        __LINE__

    /// \brief Expands to the current function name.
    #define SYNTROPY_FUNCTION \
        SYNTROPY_HAL_FUNCTION
}

// ===========================================================================
