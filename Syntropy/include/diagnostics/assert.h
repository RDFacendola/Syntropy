
/// \file assert.h
/// \brief This header is part of the syntropy diagnostic system. It contains utility macros used to check for code invariants and conditions.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "platform/macros.h"

/// \brief If the condition is not verified causes the debugger to break or the application to crash.
#define SYNTROPY_ASSERT(condition) \
    if(!(condition)) { SYNTROPY_TRAP; }

/// \brief Same as SYNTROPY_ASSERT. Used to check function preconditions.
#define SYNTROPY_PRECONDITION(condition) \
    SYNTROPY_ASSERT(condition)

/// \brief Same as SYNTROPY_ASSERT. Used to check function postconditions.
#define SYNTROPY_POSTCONDITION(condition) \
    SYNTROPY_ASSERT(condition)

