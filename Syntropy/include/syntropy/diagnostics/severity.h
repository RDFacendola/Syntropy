
/// \file severity.h
/// \brief This header is part of the Syntropy diagnostic module. It contains definition for event severity.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <cstdint>
#include <ostream>

namespace syntropy
{
    /************************************************************************/
    /* SEVERITY                                                             */
    /************************************************************************/

    /// \brief Represents the severity of an event.
    /// \author Raffaele D. Facendola - November 2016
    enum class Severity : int8_t
    {
        /// \brief Severity used for informative and tracing events that cause no side-effects on the application.
        kInformative = 0,

        /// \brief Severity used for uncommon events that may cause undesired, yet acceptable, side-effects on the application.
        kWarning = 1,                                                  

        /// \brief Severity used for rare events that have substantial impact on the application. Those events should not occur on a production version of the application.
        kError = 2,

        /// \brief Severity used for exceptional events that will cause the application to crash. Those events should never occur.
        kCritical = 3
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
        if (severity == Severity::kInformative)
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
        else
        {
            out << "(Unknown)";
        }

        return out;
    }

}
