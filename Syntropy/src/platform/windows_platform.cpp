#include "platform/windows_platform.h"

#ifdef _WIN32

#include <intrin.h>

#include "syntropy.h"

namespace syntropy
{
    namespace platform
    {

        //////////////// PLATFORM ////////////////
        
        bool Platform::IsDebuggerAttached()
        {
            return true;
        }

        void Platform::Crash(const char* message)
        {
            SYNTROPY_UNUSED(message);
        }

        //diagnostics::Callstack Platform::GenerateCallstack()
        //{
        //    return diagnostics::Callstack(SYNTROPY_TRACE);
        //}

    }
}

#endif
