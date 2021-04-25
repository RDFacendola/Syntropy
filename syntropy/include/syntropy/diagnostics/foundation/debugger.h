
/// \file debugger.h
///
/// \brief This header is part of the Syntropy diagnostic module.
///        It contains debugger functionalities.
///
/// \author Raffaele D. Facendola - 2019.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/preprocessor/macro.h"

#include "syntropy/hal/hal_macro.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* MACROS                                                               */
    /************************************************************************/

    /// \brief Causes the debugger to break if attached, or the application
    ///        to terminate otherwise.
#define SYNTROPY_TRAP \
        SYNTROPY_MACRO_DECLARATION(empty)

    /// \brief Causes the debugger to break if attached.
#define SYNTROPY_BREAK \
        SYNTROPY_MACRO_DECLARATION(empty)

    /************************************************************************/
    /* DIAGNOSTICS                                                          */
    /************************************************************************/

    namespace Diagnostics
    {
        /// \brief Check whether the debugger is attached.
        [[nodiscard]] Bool
        IsDebuggerAttached() noexcept;
    }

}

// ===========================================================================

#include "details/debugger.inl"

// ===========================================================================
