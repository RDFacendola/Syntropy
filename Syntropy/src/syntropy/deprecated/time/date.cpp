#include "syntropy/time/date.h"

#include "hinnant/date/date.h"

namespace Syntropy
{
    /************************************************************************/
    /* DATE                                                                 */
    /************************************************************************/

    Date::Date(TTimePoint time_point)
    {
        auto year_month_day = date::year_month_day{ date::floor<date::days>(time_point) };

        day_ = ToFix8(static_cast<unsigned>(year_month_day.day()));
        month_ = ToFix8(static_cast<unsigned>(year_month_day.month()));
        year_ = ToFix16(static_cast<int>(year_month_day.year()));
    }

}