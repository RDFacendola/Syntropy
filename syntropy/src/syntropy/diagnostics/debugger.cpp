#include "syntropy/diagnostics/debugger.h"

#include "syntropy/hal/hal_debugger.h"

// ===========================================================================

namespace Syntropy::Diagnostics
{
    /************************************************************************/
    /* DEBUGGER                                                             */
    /************************************************************************/

    [[nodiscard]] Bool IsDebuggerAttached() noexcept
    {
        return HAL::Diagnostics::IsDebuggerAttached();
    }
}

// ===========================================================================
