
/// \file macro.h
/// \brief This header is part of Syntropy language module. It contains generic macros.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "syntropy/hal/hal_macro.h"
#include "syntropy/language/type_traits.h"

namespace Syntropy
{
    /************************************************************************/
    /* MACROS                                                               */
    /************************************************************************/

    /// \brief Special macro used to "forward-declare" a macro and move its definition somewhere else.
    /// This macro is intended to separate macro declaration site from definition and to avoid clutter near the macro declaration site.
    /// Declaration and definition site must exist in the same file!
    /// \usage 
    /// Declaration site. Forward declaration only. Any usage before definition will trigger a static assert.
    ///  #define MY_MACRO(arg0, arg1) SYNTROPY_MACRO_DECLARATION(arg0, arg1)
    /// 
    /// Definition site. Undef previous forward declaration and provide actual definition.
    ///  #undef MY_MACRO
    ///  #define MY_MACRO(arg0, arg1) (arg0 + arg1)
    #define SYNTROPY_MACRO_DECLARATION(...) \
    []() { static_assert(AlwaysFalseV<#__VA_ARGS__>, "Undefined macro!"); }

    /// \brief Use x as part of an expression to suppress unused warning.
    #define SYNTROPY_UNUSED(x) \
        (void)(x)

    /// \brief Expand the provided arguments
    /// \example SYNTROPY_EXPAND(a, b, c) -> a, b, c
    /// \example SYNTROPY_EXPAND((foo, bar)) -> foo, bar
    #define SYNTROPY_EXPAND(...) \
        __VA_ARGS__

    /// \brief Map any arguments to nothing.
    #define SYNTROPY_DROP(...)

    /// \brief Drop the leading arguments of x enclosed among parentheses, leaving the rest untouched.
    /// \example SYNTROPY_STRIP((foo) bar) -> bar
    #define SYNTROPY_STRIP(x) \
        SYNTROPY_DROP x

    /// \brief Remove the enclosing parentheses from the leading argument of x
    /// \example SYNTROPY_PAIR((foo) bar) -> foo bar
    #define SYNTROPY_PAIR(x) \
        SYNTROPY_EXPAND x

    /// \brief Select the first argument from the argument list.
    #define SYNTROPY_SELECT_1ST(_0, ...) \
        _0

    /// \brief Select the second argument from the argument list.
    #define SYNTROPY_SELECT_2ND(_0, _1, ...) \
        _1

    /// \brief Select the third argument from the argument list.
    #define SYNTROPY_SELECT_3RD(_0, _1, _2, ...) \
        _2

    /// \brief Select the fourth argument from the argument list.
    #define SYNTROPY_SELECT_4TH(_0, _1, _2, _3, ...) \
        _3

    /// \brief Select the fifth argument from the argument list.
    #define SYNTROPY_SELECT_5TH(_0, _1, _2, _3, _4, ...) \
        _4

    /// \brief Drops the first argument from the argument list.
    #define SYNTROPY_DROP_1(_0, ...) \
        __VA_ARGS__

    /// \brief Drops the first two arguments from the argument list.
    #define SYNTROPY_DROP_2(_0, _1, ...) \
        __VA_ARGS__

    /// \brief Drops the first three arguments from the argument list.
    #define SYNTROPY_DROP_3(_0, _1, _2, ...) \
        __VA_ARGS__

    /// \brief Drops the first four arguments from the argument list.
    #define SYNTROPY_DROP_4(_0, _1, _2, _3, ...) \
        __VA_ARGS__

    /// \brief Drops the first five arguments from the argument list.
    #define SYNTROPY_DROP_5(_0, _1, _2, _3, _4, ...) \
        __VA_ARGS__

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
