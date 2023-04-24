
/// \file hal_debugger.h
/// \brief This header is part of the Syntropy hardware abstraction layer module. It contains methods to access debugger functionalities.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include "syntropy/language/foundation/foundation.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* HAL DIAGNOSTICS                                                      */
    /************************************************************************/

    namespace HALDiagnostics
    {
        /// \brief Check whether the debugger is attached.
        [[nodiscard]] Bool IsDebuggerAttached() noexcept;
    }
}

// ===========================================================================
