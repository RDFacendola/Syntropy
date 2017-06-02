
/// \file macros.h
/// \brief This header is part of the syntropy HAL (hardware abstraction layer) system. It contains platform-specific macros.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#ifdef _MSC_VER

#include <intrin.h>

/// \brief Expands to the current file name.
#define SYNTROPY_FILE \
    __FILE__

/// \brief Expands to the current line number.
#define SYNTROPY_LINE \
    __LINE__

/// \brief Expands to the current function name.
#define SYNTROPY_FUNCTION \
    __FUNCTION__

/// \brief Causes the debugger to break.
#define SYNTROPY_TRAP \
    __debugbreak()

#endif
