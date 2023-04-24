#ifdef _WIN64

#include "syntropy/hal/hal_debugger.h"

/************************************************************************/
/* HEADERS & LIBRARIES                                                  */
/************************************************************************/

#pragma comment(lib, "DbgHelp.lib")

#pragma warning(push)
#pragma warning(disable:4091)

 #include <Windows.h>
 #include <DbgHelp.h>

#undef max

#pragma warning(pop)

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* HAL DIAGNOSTICS                                                      */
    /************************************************************************/

    [[nodiscard]] inline Bool
    HALDiagnostics
    ::IsDebuggerAttached() noexcept
    {
        return IsDebuggerPresent() != 0;
    }
}

// ===========================================================================

#endif
