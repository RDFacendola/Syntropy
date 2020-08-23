
/// \file severity.h
/// \brief This header is part of the Syntropy diagnostic module. It contains definition for event severity.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <cstdint>
#include <ostream>

#include "syntropy/language/fundamentals.h"

namespace Syntropy
{
    /************************************************************************/
    /* SEVERITY                                                             */
    /************************************************************************/

    /// \brief Represents the severity of an event.
    /// \author Raffaele D. Facendola - November 2016
    enum class Severity : Int
    {
        /// \brief Severity used for debugging and tracing messages.
        kDebug = -1,

        /// \brief Severity used for informative events that cause no side-effects on the application.
        kInformative = 0,

        /// \brief Severity used for uncommon events that may cause undesired, yet acceptable, side-effects on the application.
        kWarning = 1,

        /// \brief Severity used for rare events that have substantial impact on the application. Those events should not occur on a production version of the application.
        kError = 2,

        /// \brief Severity used for critical events that will cause data loss. Those events should never occur.
        kCritical = 3,

        /// \brief Severity used for exceptional events that will cause data loss and\or application instability. Those events should never occur.
        kFatal = 4,
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Stream insertion for Severity.
    std::ostream& operator<<(std::ostream& out, Severity severity);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Severity.

    inline std::ostream& operator<<(std::ostream& out, Severity severity)
    {
        if (severity == Severity::kDebug)
        {
            out << "Debug";
        }
        else if (severity == Severity::kInformative)
        {
            out << "Info";
        }
        else if (severity == Severity::kWarning)
        {
            out << "Warning";
        }
        else if (severity == Severity::kError)
        {
            out << "Error";
        }
        else if (severity == Severity::kCritical)
        {
            out << "Critical";
        }
        else if (severity == Severity::kFatal)
        {
            out << "Fatal";
        }
        else
        {
            out << "(Unknown)";
        }

        return out;
    }

}
