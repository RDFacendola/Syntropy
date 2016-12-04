/// \file windows_platform.h
/// \brief This header is part of the syntropy HAL (hardware abstraction layer) system. It contains Windows-specific functionalities.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#ifdef _WIN32

#include <intrin.h>

#include "diagnostics/diagnostics.h"

#define SYNTROPY_BREAK \
    if(syntropy::platform::Debug::IsDebuggerAttached()) { __debugbreak(); }

namespace syntropy
{
    namespace diagnostics 
    {
        struct StackTrace;
    }

    namespace platform
    {

        struct Debug
        {
            static bool IsDebuggerAttached();

            static diagnostics::StackTrace GetStackTrace();
        };

    }
}

#endif
