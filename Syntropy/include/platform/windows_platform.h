/// \file windows_platform.h
/// \brief This header is part of the syntropy HAL (hardware abstraction layer) system. It contains Windows-specific functionalities.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#ifdef _WIN32

#include <intrin.h>

#define SYNTROPY_FILE \
   __FILE__

#define SYNTROPY_FUNCTION \
   __FUNCTION__

#define SYNTROPY_LINE \
   __LINE__

#define SYNTROPY_BREAK \
    if(syntropy::platform::Platform::IsDebuggerAttached()) { __debugbreak(); }

namespace syntropy
{
    namespace diagnostics 
    {
        struct StackTrace;
    }

    namespace platform
    {

        struct Platform
        {
            static bool IsDebuggerAttached();

            static void Crash(const char* message);
            
            //static diagnostics::Callstack GenerateCallstack();
        };

    }
}

#endif
