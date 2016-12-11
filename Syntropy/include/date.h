
/// \file date.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "hinnant/date/date.h"
#include <chrono>

namespace syntropy
{

    inline auto GetDate(std::chrono::system_clock::time_point time)
    {
        return date::year_month_day{ date::floor<date::days>(time) };
    }

    inline auto GetTime(std::chrono::system_clock::time_point time)
    {
        auto today = date::floor<date::days>(time);
        return date::make_time(std::chrono::duration_cast<std::chrono::milliseconds>(time - today));
    }

}