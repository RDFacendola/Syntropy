#include "syntropy/diagnostics/foundation/debugger.h"

#include "syntropy/hal/hal_debugger.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* DEBUGGER                                                             */
    /************************************************************************/

    [[nodiscard]] inline Bool
    Diagnostics
    ::IsDebuggerAttached() noexcept
    {
        return HALDiagnostics::IsDebuggerAttached();
    }
}

// ===========================================================================
