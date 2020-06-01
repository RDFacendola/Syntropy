#include "syntropy/diagnostics/debugger.h"

#include "syntropy/hal/hal_debugger.h"

namespace syntropy
{
    /************************************************************************/
    /* DEBUGGER                                                             */
    /************************************************************************/

    Bool Debugger::IsAttached()
    {
        return HALDebugger::IsAttached();
    }

}