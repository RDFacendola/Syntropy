#include "syntropy/time/calendar.h"

namespace syntropy
{
    /************************************************************************/
    /* CALENDAR                                                             */
    /************************************************************************/

    date::year_month_day Calendar::GetDate(std::chrono::system_clock::time_point time)
    {
        return date::year_month_day{ date::floor<date::days>(time) };
    }

    date::time_of_day<std::chrono::milliseconds> Calendar::GetTimeOfDay(std::chrono::system_clock::time_point time)
    {
        auto today = date::floor<date::days>(time);

        return date::make_time(std::chrono::duration_cast<std::chrono::milliseconds>(time - today));
    }
}