
/// \file verbosity.h
/// \brief This header is part of the Syntropy diagnostic module. It contains definition for channel verbosity.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <cstdint>
#include <limits>

#include "syntropy/core/types.h"
#include "syntropy/diagnostics/severity.h"

namespace syntropy
{
    /************************************************************************/
    /* VERBOSITY                                                            */
    /************************************************************************/

    /// \brief Represents the verbosity of a channel, which is used to filter events based on their severity.
    /// \author Raffaele D. Facendola - May 2020.
    enum class Verbosity : Fix8
    {
        /// \brief Verbosity for all event severities.
        kAll = std::numeric_limits<Fix8>::min(),

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
        kNone = std::numeric_limits<Fix8>::max(),
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Check whether a severity is equal to a verbosity level.
    constexpr Bool operator==(Severity lhs, Verbosity rhs);

    /// \brief Check whether a severity is not-equal to a verbosity level.
    constexpr Bool operator!=(Severity lhs, Verbosity rhs);

    /// \brief Check whether a severity is strictly greater than a verbosity level.
    constexpr Bool operator>(Severity lhs, Verbosity rhs);

    /// \brief Check whether a severity is strictly less than a verbosity level.
    constexpr Bool operator<(Severity lhs, Verbosity rhs);

    /// \brief Check whether a severity is equal or greater than a verbosity level.
    constexpr Bool operator>=(Severity lhs, Verbosity rhs);

    /// \brief Check whether a severity is equal or less than a verbosity level.
    constexpr Bool operator<=(Severity lhs, Verbosity rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.

    constexpr Bool operator==(Severity lhs, Verbosity rhs)
    {
        return static_cast<Byte>(lhs) == static_cast<Byte>(rhs);
    }

    constexpr Bool operator!=(Severity lhs, Verbosity rhs)
    {
        return !(lhs == rhs);
    }

    constexpr Bool operator>(Severity lhs, Verbosity rhs)
    {
        return static_cast<Byte>(lhs) > static_cast<Byte>(rhs);
    }

    constexpr Bool operator<(Severity lhs, Verbosity rhs)
    {
        return static_cast<Byte>(lhs) < static_cast<Byte>(rhs);
    }

    constexpr Bool operator>=(Severity lhs, Verbosity rhs)
    {
        return !(lhs < rhs);
    }

    constexpr Bool operator<=(Severity lhs, Verbosity rhs)
    {
        return !(lhs > rhs);
    }

}
