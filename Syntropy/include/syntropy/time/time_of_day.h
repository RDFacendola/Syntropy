
/// \file time_of_day.h
/// \brief This header is part of the syntropy time system. It contains classes and functionalities used to handle time-of-days.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <cstdint>
#include <chrono>
#include <ostream>

namespace syntropy
{
    /************************************************************************/
    /* TIME OF DAY                                                          */
    /************************************************************************/

    /// \brief Represents a time of day.
    /// \author Raffaele D. Facendola - May 2020.
    class TimeOfDay
    {
    public:

        /// \brief Type of the underlying clock.
        using TClock = std::chrono::system_clock;

        /// \brief Type of a point in time.
        using TTimePoint = TClock::time_point;

        /// \brief Create a new date from a point in time.
        /// Default to current time.
        TimeOfDay(TTimePoint time_point = TClock::now());

        /// \brief Get the hours.
        const std::chrono::hours GetHours() const;

        /// \brief Get the minutes.
        const std::chrono::minutes GetMinutes() const;

        /// \brief Get the seconds.
        const std::chrono::seconds GetSeconds() const;

        /// \brief Get the milliseconds.
        const std::chrono::milliseconds GetMilliseconds() const;

    private:

        /// \brief Hours.
        std::chrono::hours hours_{ 0 };

        /// \brief Minutes.
        std::chrono::minutes minutes_{ 0 };

        /// \brief Seconds.
        std::chrono::seconds seconds_{ 0 };

        /// \brief Milliseconds.
        std::chrono::milliseconds milliseconds_{ 0 };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Stream insertion for time-of-days.
    std::ostream& operator<<(std::ostream& lhs, const TimeOfDay& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // TimeOfDay.

    inline const std::chrono::hours TimeOfDay::GetHours() const
    {
        return hours_;
    }

    inline const std::chrono::minutes TimeOfDay::GetMinutes() const
    {
        return minutes_;
    }

    inline const std::chrono::seconds TimeOfDay::GetSeconds() const
    {
        return seconds_;
    }

    inline const std::chrono::milliseconds TimeOfDay::GetMilliseconds() const
    {
        return milliseconds_;
    }

}