#include "syntropy/diagnostics/debugger.h"

#include "syntropy/hal/hal_debugger.h"

namespace Syntropy
{
    /************************************************************************/
    /* DEBUGGER                                                             */
    /************************************************************************/

    Bool Debugger::IsAttached()
    {
        return HALDebugger::IsAttached();
    }

}