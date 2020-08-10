#include "syntropy/time/time_of_day.h"

#include "hinnant/date/date.h"

#include <iostream>

namespace Syntropy
{
    /************************************************************************/
    /* TIME OF DAY                                                          */
    /************************************************************************/

    TimeOfDay::TimeOfDay(TTimePoint time_point)
    {
        using TDuration = std::chrono::milliseconds;

        auto today = date::floor<date::days>(time_point);

        auto time_of_day = date::make_time(std::chrono::duration_cast<TDuration>(time_point - today));

        hours_ = time_of_day.hours();
        minutes_ = time_of_day.minutes();
        seconds_ = time_of_day.seconds();
        milliseconds_ = time_of_day.subseconds();
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    std::ostream& operator<<(std::ostream& lhs, const TimeOfDay& rhs)
    {
        lhs.fill('0');
        lhs.flags(std::ios::dec | std::ios::right);
        lhs.width(2);

        lhs << rhs.GetHours().count() << ":";
        lhs << rhs.GetMinutes().count() << ":";
        lhs << rhs.GetSeconds().count() << ".";

        lhs.width(0);

        lhs << rhs.GetMilliseconds().count();

        return lhs;
    }

}