
/// \file time.h
/// \brief This header is part of the syntropy time system. It contains time-related classes and functionalities.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "hinnant/date/date.h"
#include <chrono>

namespace syntropy
{
    /// \brief Floating-point seconds amount.
    using Seconds = std::chrono::duration<float>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Get the time of the day associated to a time point.
    /// \param time Time point to get the time-of-day from.
    /// \return Returns the time of the day associated to the provided time point as ticks relative to the begin of the day.
    template <typename TDuration = std::chrono::milliseconds>
    date::time_of_day<TDuration> GetTimeOfDay(std::chrono::system_clock::time_point time = std::chrono::system_clock::now());

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    template <typename TDuration>
    date::time_of_day<TDuration> GetTimeOfDay(std::chrono::system_clock::time_point time)
    {
        auto today = date::floor<date::days>(time);

        return date::make_time(std::chrono::duration_cast<TDuration>(time - today));
    }
}