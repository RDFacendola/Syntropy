
/// \file calendar.h
/// \brief This header is part of the syntropy time system. It contains calendar-related classes and functionalities.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "hinnant/date/date.h"
#include <chrono>

namespace syntropy
{
    /// \brief Exposes calendar-related functionalities.
    /// \author Raffaele D. Facendola
    struct Calendar
    {
        /// \brief Get the date associated to a time point.
        /// \param time Time point to get the date from.
        /// \return Returns the date associated to the provided time point as day-month-year.
        static date::year_month_day GetDate(std::chrono::system_clock::time_point time = std::chrono::system_clock::now());

        /// \brief Get the time of the day associated to a time point.
        /// \param time Time point to get the time-of-day from.
        /// \return Returns the time of the day associated to the provided time point as 
        static date::time_of_day<std::chrono::milliseconds> GetTimeOfDay(std::chrono::system_clock::time_point time = std::chrono::system_clock::now());
    };
}