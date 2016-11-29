
/// \file platform.h
/// \brief This header is part of the syntropy HAL (hardware abstraction layer) system. It contains MS Windows-specific functionalities.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <intrin.h>

/// \brief Expands to an object representing the location of the current line of code.
#define SYNTROPY_TRACE \
    syntropy::diagnostics::Trace(__FILE__, __FUNCTION__, __LINE__)

/// \brief Break the execution and calls the debugger. Does nothing if no debugger was attached.
#define SYNTROPY_BREAK \
    __debugbreak();

#define SYNTROPY_HALT \
    

namespace syntropy
{
    namespace platform
    {
        
    }
}