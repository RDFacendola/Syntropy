#include "time/date.h"

namespace syntropy
{
    date::year_month_day GetDate(std::chrono::system_clock::time_point time)
    {
        return date::year_month_day{ date::floor<date::days>(time) };
    }

    date::time_of_day<std::chrono::milliseconds> GetTimeOfDay(std::chrono::system_clock::time_point time)
    {
        auto today = date::floor<date::days>(time);
        return date::make_time(std::chrono::duration_cast<std::chrono::milliseconds>(time - today));
    }
}