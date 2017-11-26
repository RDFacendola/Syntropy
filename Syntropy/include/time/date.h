
/// \file date.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "hinnant/date/date.h"
#include <chrono>

namespace syntropy
{

    /// \brief Get the date associated to a time point.
    date::year_month_day GetDate(std::chrono::system_clock::time_point time);

    /// \brief Get the time of the day associated to a time point.
    date::time_of_day<std::chrono::milliseconds> GetTimeOfDay(std::chrono::system_clock::time_point time);

}