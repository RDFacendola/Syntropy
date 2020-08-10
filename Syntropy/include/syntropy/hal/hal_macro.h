
/// \file hal_macro.h
/// \brief This header is part of Syntropy hardware abstraction layer module. It contains platform-specific macros.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

namespace Syntropy
{
    /************************************************************************/
    /* MSVC COMPILER                                                        */
    /************************************************************************/

    #ifdef _MSC_VER

     /// \brief Expands to the current function name.
     #define SYNTROPY_HAL_FUNCTION \
         __FUNCTION__
 
     /// \brief Causes the debugger to break if attached, or the application to terminate otherwise.
     #define SYNTROPY_HAL_TRAP \
         { __debugbreak(); }

    /************************************************************************/
    /* UNKNOWN                                                              */
    /************************************************************************/

    #else

    #error "SYNTROPY_HAL_FUNCTION not implemented for this platform."
    #error "SYNTROPY_HAL_TRAP not implemented for this platform."

    #endif

}
