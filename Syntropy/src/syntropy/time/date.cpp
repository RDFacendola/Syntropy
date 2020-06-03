#include "syntropy/time/date.h"

#include "hinnant/date/date.h"

namespace syntropy
{
    /************************************************************************/
    /* DATE                                                                 */
    /************************************************************************/

    Date::Date(TTimePoint time_point)
    {
        auto year_month_day = date::year_month_day{ date::floor<date::days>(time_point) };

        day_ = ToInt(static_cast<unsigned>(year_month_day.day()));
        month_ = ToInt(static_cast<unsigned>(year_month_day.month()));
        year_ = ToInt(static_cast<int>(year_month_day.year()));
    }

}