
/// \file hal_macro.h
/// \brief This header is part of Syntropy hardware abstraction layer module.
///        It contains platform-specific macros.
///
/// \author Raffaele D. Facendola - May 2020.

#pragma once

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* MSVC COMPILER                                                        */
    /************************************************************************/

#ifdef _MSC_VER

     /// \brief Expands to the current function name.
     #define SYNTROPY_HAL_FUNCTION \
         __FUNCTION__

     /// \brief Causes the debugger to break if attached, or the application
     ///        to terminate otherwise.
     #define SYNTROPY_HAL_TRAP \
         { __debugbreak(); }

#endif

// ===========================================================================

    /************************************************************************/
    /* CLANG COMPILER                                                       */
    /************************************************************************/

#ifdef __clang__

#include <signal.h>

    /// \brief Expands to the current function name.
    #define SYNTROPY_HAL_FUNCTION \
        __func__

    /// \brief Causes the debugger to break if attached, or the application
    ///         to terminate otherwise.
    #define SYNTROPY_HAL_TRAP \
        { raise(SIGTRAP); }

#endif

// ===========================================================================

    /************************************************************************/
    /* UNKNOWN                                                              */
    /************************************************************************/

#ifndef SYNTROPY_HAL_FUNCTION
    #error "SYNTROPY_HAL_FUNCTION not implemented for this platform."
#endif

#ifndef SYNTROPY_HAL_TRAP
    #error "SYNTROPY_HAL_TRAP not implemented for this platform."
#endif

}

// ===========================================================================
