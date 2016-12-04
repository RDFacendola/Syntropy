#include "platform/windows_platform.h"

#ifdef _WIN32

#include <intrin.h>

#include "syntropy.h"

namespace syntropy
{
    namespace platform
    {

        //////////////// DEBUG ////////////////
        
        bool Debug::IsDebuggerAttached()
        {
            return true;
        }

        diagnostics::StackTrace Debug::GetStackTrace()
        {
            return SYNTROPY_TRACE;
        }
    }
}

#endif
