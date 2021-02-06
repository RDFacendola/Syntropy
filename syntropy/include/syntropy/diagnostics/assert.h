
/// \file assert.h
///
/// \brief This header is part of the Syntropy diagnostic module.
///        It contains utility macros used to check for code invariants
///        and assertions.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "syntropy/language/preprocessor/macro.h"

#include "syntropy/diagnostics/debugger.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* RUN-TIME DIAGNOSTICS                                                 */
    /************************************************************************/

    /// \brief If the condition is not verified causes the debugger to break
    ///        if attached or the application to terminate.
#define SYNTROPY_ASSERT(condition) \
        SYNTROPY_MACRO_DECLARATION(condition)

    /// \brief Macro used to check on conditions that will cause undefined
    ///        behaviors.
    ///
    /// If the provided condition is not verified causes the debugger to break
    /// if attached or the application to terminate.
#define SYNTROPY_UNDEFINED_BEHAVIOR(condition, message) \
        SYNTROPY_MACRO_DECLARATION(condition)

}

// ===========================================================================

#include "assert.inl"

// ===========================================================================
