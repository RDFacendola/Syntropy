
/// \file debugger.h
/// \brief This header is part of the Syntropy diagnostics module. It contains debugger functionalities.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include "syntropy/hal/hal_macro.h"

namespace syntropy
{
    /************************************************************************/
    /* MACROS                                                               */
    /************************************************************************/

    /// \brief Causes the debugger to break if attached, or the application to terminate otherwise.
    #define SYNTROPY_TRAP \
        SYNTROPY_HAL_TRAP

    /// \brief Causes the debugger to break if attached.
    #define SYNTROPY_BREAK \
        if(syntropy::Debugger::IsAttached()){ SYNTROPY_TRAP }

    /************************************************************************/
    /* DEBUGGER                                                             */
    /************************************************************************/

    /// \brief Exposes debugging functionalities.
    /// \author Raffaele D. Facendola - May 2020.
    namespace Debugger
    {
        /// \brief Check whether the debugger is attached.
        bool IsAttached();
    }

}
