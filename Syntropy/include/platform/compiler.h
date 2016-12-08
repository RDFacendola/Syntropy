
/// \file compiler.h
/// \brief This header is part of the syntropy HAL (hardware abstraction layer) system. It contains the definition of compiler-specific functionalities.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

// List of supported compilers

#include "compiler/msvc.h"

// Fallback macros

#ifndef SYNTROPY_TRAP

/// \brief Causes the debugger to break.
#define SYNTROPY_TRAP

#endif

#ifndef SYNTROPY_FILE

/// \brief Expands to the current file name.
#define SYNTROPY_FILE \
    __FILE__

#endif

#ifndef SYNTROPY_FUNCTION

/// \brief Expands to the current function name.
#define SYNTROPY_FUNCTION \
    __func__

#endif

#ifndef SYNTROPY_LINE

/// \brief Expands to the current line number.
#define SYNTROPY_LINE \
    __LINE__

#endif
