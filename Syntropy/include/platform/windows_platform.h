/// \file windows_platform.h
/// \brief This header is part of the syntropy HAL (hardware abstraction layer) system. It contains Windows-specific functionalities.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#ifdef _WIN64

#include <intrin.h>

#include "diagnostics/diagnostics.h"

#define SYNTROPY_BREAK \
    if(syntropy::platform::GetDebug().IsDebuggerAttached()) { __debugbreak(); }

#define SYNTROPY_TRACE \
    syntropy::platform::GetDebug().GetStackTrace(SYNTROPY_HERE)

namespace syntropy
{
    namespace diagnostics 
    {
        struct StackTrace;
    }

    namespace platform
    {

        class Debug
        {
        public:

            static Debug& GetInstance();

            ~Debug() = default;

            bool IsDebuggerAttached();

            diagnostics::StackTrace GetStackTrace(diagnostics::StackTraceElement caller);

        private:

            struct Implementation;

            Debug();

            std::unique_ptr<Implementation> implementation_;        ///< \brief Actual implementation of the class. Prevents namespace pollution.

        };

        Debug& GetDebug();

    }
}

#endif
