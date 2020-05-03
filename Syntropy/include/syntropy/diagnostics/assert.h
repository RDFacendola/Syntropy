
/// \file assert.h
/// \brief This header is part of the Syntropy diagnostic module. It contains utility macros used to check for code invariants and assertions.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "syntropy/diagnostics/debugger.h"

/// \brief If the condition is not verified causes the debugger to break if attached or the application to terminate.
#define SYNTROPY_ASSERT(condition) \
    if(!(condition)) { SYNTROPY_TRAP; }
