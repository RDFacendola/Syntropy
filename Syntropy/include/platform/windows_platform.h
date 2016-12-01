/// \file windows_platform.h
/// \brief This header is part of the syntropy HAL (hardware abstraction layer) system. It contains Windows-specific functionalities.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#ifdef _WIN32

#include <intrin.h>

/// \brief Expands to an object representing the location of the current line of code.
#define HAL_TRACE \
    syntropy::diagnostics::Trace(__FILE__, __FUNCTION__, __LINE__)

#define HAL_DEBUG_BREAK \
    if(syntropy::platform::Platform::IsDebuggerAttached()) { __debugbreak(); }

namespace syntropy
{
    namespace diagnostics 
    {
        struct Callstack;
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
