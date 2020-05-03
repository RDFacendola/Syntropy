
/// \file hal_debugger.h
/// \brief This header is part of the Syntropy hardware abstraction layer module. It contains methods to access debugger functionalities.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <cstdint>

#include "syntropy/diagnostics/stack_trace.h"

namespace syntropy
{
    /************************************************************************/
    /* HAL DEBUGGER                                                         */
    /************************************************************************/

    /// \brief Exposes debugging methods.
    /// \author Raffaele D. Facendola - May 2020.
    namespace HALDebugger
    {
        /// \brief Check whether the debugger is attached.
        bool IsAttached();

        /// \brief Get the stack trace on the current thread.
        StackTrace GetStackTrace(StackTraceElement here);
    }

}