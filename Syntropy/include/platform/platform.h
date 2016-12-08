
/// \file platform.h
/// \brief This header is part of the syntropy HAL (hardware abstraction layer) system. It contains classes used to handle platform-specific functionalities.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "windows_platform.h"

// Fallback macros

#ifndef SYNTROPY_BREAK

#pragma message("Warning: SYNTROPY_BREAK macro was not defined")

/// \brief Causes the debugger to break. Fallback macro: does nothing!
#define SYNTROPY_BREAK

#endif

#ifndef SYNTROPY_HERE

/// \brief Expands to an object representing the location of the current line of code. Fallback macro: plain function name, without namespaces or classes attached.
#define SYNTROPY_HERE \
        syntropy::diagnostics::StackTraceElement(__FILE__, __func__, __LINE__)

#endif

#ifndef SYNTROPY_TRACE

#pragma message("Warning: SYNTROPY_TRACE macro was not defined")

/// \brief Expands to an object representing the current stack trace. Fallback macro: stack walking is not supported, get the most recent function instead.
#define SYNTROPY_TRACE \
        SYNTROPY_HERE

#endif
