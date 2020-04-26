
/// \file assert.h
/// \brief This header is part of the Syntropy diagnostic module. It contains utility macros used to check for code invariants and assertions.
///
/// \author Raffaele D. Facendola - 2016

#pragma once


/// \brief Causes the debugger to break or the application to crash.
#define SYNTROPY_TRAP \
    SYNTROPY_TRAP_IMPL

/// \brief If the condition is not verified causes the debugger to break or the application to crash.
#define SYNTROPY_ASSERT(condition) \
    if(!(condition)) { SYNTROPY_TRAP; }

/************************************************************************/
/* DETAILS                                                              */
/************************************************************************/

namespace syntropy::details
{
#ifdef _MSC_VER

#include <intrin.h>

#define SYNTROPY_TRAP_IMPL __debugbreak()

#else

#error "SYNTROPY_TRAP_IMPL is undefined!"

#endif
}