
/// \file log_macros.h
/// \brief This header is part of the Syntropy diagnostic module. It contains definition for log macros.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/diagnostics/stack_trace.h"
#include "syntropy/diagnostics/log_manager.h"
#include "syntropy/diagnostics/log_event.h"

namespace Syntropy
{
    /************************************************************************/
    /* MACROS                                                               */
    /************************************************************************/

    /// \brief Log a debug message.
    /// \usage SYNTROPY_DEBUG(context, "This is the number: ", 2, "!");
    #define SYNTROPY_DEBUG(context, ...) \
        SYNTROPY_MACRO_DECLARATION(context, __VA_ARGS__)

    /// \brief Log an informative message.
    /// \usage SYNTROPY_INFO(context, "This is the number: ", 2, "!");
    #define SYNTROPY_INFO(context, ...) \
        SYNTROPY_MACRO_DECLARATION(context, __VA_ARGS__)

    /// \brief Log a warning message.
    /// \usage SYNTROPY_WARNING(context, "This is the number: ", 2, "!");
    #define SYNTROPY_WARNING(context, ...) \
        SYNTROPY_MACRO_DECLARATION(context, __VA_ARGS__)

    /// \brief Log an error message, causing the debugger to break.
    /// \usage SYNTROPY_ERROR(context, "This is the number: ", 2, "!");
    #define SYNTROPY_ERROR(context, ...) \
        SYNTROPY_MACRO_DECLARATION(context, __VA_ARGS__)

    /// \brief Log a critical error message with full stack trace and causing the debugger to break.
    /// \usage SYNTROPY_CRITICAL(context, "This is the number: ", 2, "!");
    #define SYNTROPY_CRITICAL(context, ...) \
        SYNTROPY_MACRO_DECLARATION(context, __VA_ARGS__)

    /// \brief Log a fatal error message with full stack trace causing the debugger to break and the application to crash.
    /// \usage SYNTROPY_FATAL(context, "This is the number: ", 2, "!");
    #define SYNTROPY_FATAL(context, ...) \
        SYNTROPY_MACRO_DECLARATION(context, __VA_ARGS__)

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Macros.

    #undef  SYNTROPY_DEBUG
    #define SYNTROPY_DEBUG(context, ...) \
        Syntropy::GetLogManager().Send(Syntropy::MakeLogEvent(Syntropy::Severity::kDebug, context, SYNTROPY_HERE, __VA_ARGS__))

    #undef  SYNTROPY_INFO
    #define SYNTROPY_INFO(context, ...) \
        Syntropy::GetLogManager().Send(Syntropy::MakeLogEvent(Syntropy::Severity::kInformative, context, SYNTROPY_HERE, __VA_ARGS__))

    #undef  SYNTROPY_WARNING
    #define SYNTROPY_WARNING(context, ...) \
        Syntropy::GetLogManager().Send(Syntropy::MakeLogEvent(Syntropy::Severity::kWarning, context, SYNTROPY_HERE, __VA_ARGS__))

    #undef  SYNTROPY_ERROR
    #define SYNTROPY_ERROR(context, ...) \
        { \
            Syntropy::GetLogManager().Send(Syntropy::MakeLogEvent(Syntropy::Severity::kError, context, SYNTROPY_HERE, __VA_ARGS__)); \
            SYNTROPY_BREAK; \
        }

    #undef  SYNTROPY_CRITICAL
    #define SYNTROPY_CRITICAL(context, ...) \
        { \
            Syntropy::GetLogManager().Send(Syntropy::MakeLogEvent(Syntropy::Severity::kCritical, context, Syntropy::Debugger::GetStackTrace( SYNTROPY_HERE ), __VA_ARGS__)).Flush(); \
            SYNTROPY_BREAK; \
        }

    #undef  SYNTROPY_FATAL
    #define SYNTROPY_FATAL(context, ...) \
        { \
            Syntropy::GetLogManager().Send(Syntropy::MakeLogEvent(Syntropy::Severity::kFatal, context, Syntropy::Debugger::GetStackTrace( SYNTROPY_HERE ), __VA_ARGS__)).Flush(); \
            SYNTROPY_TRAP; \
        }

}
