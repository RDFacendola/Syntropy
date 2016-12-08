
/// \file msvc.h
/// \brief This header is part of the syntropy HAL (hardware abstraction layer) system. It contains the definition of MSVC-specific functionalities.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#ifdef _MSC_VER

#include <intrin.h>

/// \brief Causes the debugger to break.
#define SYNTROPY_TRAP \
    __debugbreak()

/// \brief Expands to the current function name.
#define SYNTROPY_FUNCTION \
    __FUNCTION__

#endif
