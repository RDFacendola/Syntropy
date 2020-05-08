
/// \file verbosity.h
/// \brief This header is part of the Syntropy diagnostic module. It contains definition for channel verbosity.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <cstdint>
#include <limits>

#include "syntropy/diagnostics/severity.h"

namespace syntropy
{
    /************************************************************************/
    /* VERBOSITY                                                            */
    /************************************************************************/

    /// \brief Represents the verbosity of a channel, which is used to filter events based on their severity.
    /// \author Raffaele D. Facendola - May 2020.
    enum class Verbosity : int8_t
    {
        /// \brief Verbosity for all event severities.
        kAll = std::numeric_limits<int8_t>::min(),

        /// \brief Verbosity for debugging events and above.
        kDebug = -1,

        /// \brief Verbosity for infos and above.
        kInformative = 0,

        /// \brief Verbosity for warnings and above.
        kWarning = 1,

        /// \brief Verbosity for errors and above.
        kError = 2,

        /// \brief Verbosity for critical events and above.
        kCritical = 3,

        /// \brief Verbosity for fatal events only.
        kFatal = 4,

        /// \brief Verbosity for no event severities at all.
        kNone = std::numeric_limits<int8_t>::max(),
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Check whether a severity is equal to a verbosity level.
    constexpr bool operator==(Severity lhs, Verbosity rhs);

    /// \brief Check whether a severity is not-equal to a verbosity level.
    constexpr bool operator!=(Severity lhs, Verbosity rhs);

    /// \brief Check whether a severity is strictly greater than a verbosity level.
    constexpr bool operator>(Severity lhs, Verbosity rhs);

    /// \brief Check whether a severity is strictly less than a verbosity level.
    constexpr bool operator<(Severity lhs, Verbosity rhs);

    /// \brief Check whether a severity is equal or greater than a verbosity level.
    constexpr bool operator>=(Severity lhs, Verbosity rhs);

    /// \brief Check whether a severity is equal or less than a verbosity level.
    constexpr bool operator<=(Severity lhs, Verbosity rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.

    constexpr bool operator==(Severity lhs, Verbosity rhs)
    {
        return static_cast<std::int8_t>(lhs) == static_cast<std::int8_t>(rhs);
    }

    constexpr bool operator!=(Severity lhs, Verbosity rhs)
    {
        return !(lhs == rhs);
    }

    constexpr bool operator>(Severity lhs, Verbosity rhs)
    {
        return static_cast<std::int8_t>(lhs) > static_cast<std::int8_t>(rhs);
    }

    constexpr bool operator<(Severity lhs, Verbosity rhs)
    {
        return static_cast<std::int8_t>(lhs) < static_cast<std::int8_t>(rhs);
    }

    constexpr bool operator>=(Severity lhs, Verbosity rhs)
    {
        return !(lhs < rhs);
    }

    constexpr bool operator<=(Severity lhs, Verbosity rhs)
    {
        return !(lhs > rhs);
    }

}
