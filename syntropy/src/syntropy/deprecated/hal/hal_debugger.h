
/// \file hal_debugger.h
/// \brief This header is part of the Syntropy hardware abstraction layer module. It contains methods to access debugger functionalities.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/diagnostics/stack_trace.h"

namespace Syntropy::HAL::Diagnostics
{
    /************************************************************************/
    /* HAL DEBUGGER                                                         */
    /************************************************************************/

    /// \brief Exposes debugging methods.
    /// \author Raffaele D. Facendola - May 2020.
    namespace HALDebugger
    {
        /// \brief Get the stack trace on the current thread.
        StackTrace GetStackTrace(StackTraceElement here);
    }

}