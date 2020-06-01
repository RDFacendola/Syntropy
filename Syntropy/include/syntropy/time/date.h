
/// \file date.h
/// \brief This header is part of the syntropy time system. It contains classes and functionalities used to handle dates.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <chrono>
#include <ostream>

#include "syntropy/core/types.h"

namespace syntropy
{
    /************************************************************************/
    /* DATE                                                                 */
    /************************************************************************/

    /// \brief Represents a date.
    /// \author Raffaele D. Facendola - May 2020.
    class Date
    {
    public:

        /// \brief Type of the underlying clock.
        using TClock = std::chrono::system_clock;

        /// \brief Type of a point in time.
        using TTimePoint = TClock::time_point;

        /// \brief Create a new date from a point in time.
        /// Default to current date.
        Date(TTimePoint time_point = TClock::now());

        /// \brief Get the day of the month.
        Int GetDay() const;

        /// \brief Get the month of the year.
        Int GetMonth() const;

        /// \brief Get the year.
        Int GetYear() const;

    private:

        /// \brief Day of the month.
        Byte day_{ 1 };

        /// \brief Month of the year.
        Byte month_{ 1 };

        /// \brief Year.
        std::int16_t year_{ 0 };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

        /// \brief Stream insertion for dates.
    std::ostream& operator<<(std::ostream& lhs, const Date& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Date.

    inline Int Date::GetDay() const
    {
        return day_;
    }

    inline Int Date::GetMonth() const
    {
        return month_;
    }

    inline Int Date::GetYear() const
    {
        return year_;
    }

    // Non-member functions.

    inline std::ostream& operator<<(std::ostream& lhs, const Date& rhs)
    {
        lhs << rhs.GetYear() << "-" << rhs.GetMonth() << "-" << rhs.GetDay();

        return lhs;
    }

}