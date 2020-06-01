/// \file timer.h
/// \brief This header is part of the Syntropy time module. It contains classes and functionalities used to keep track of time intervals.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <chrono>

#include "syntropy/core/types.h"

namespace syntropy
{
    /************************************************************************/
    /* TIMER <TDURATION>                                                    */
    /************************************************************************/

    /// \brief Timer class used to get time intervals.
    /// \author Raffaele D. Facendola - November 2017.
    template <typename TDuration = std::chrono::milliseconds>
    class Timer
    {
    public:

        /// \brief Type of the underlying clock.
        using TClock = std::chrono::steady_clock;

        /// \brief Create a new timer.
        /// \param ticking Whether the timer automatically starts after being created.
        Timer(Bool ticking = true);

        /// \brief Start counting.
        void Start();

        /// \brief Stop counting.
        /// \return Returns the time passed since the last call to Start().
        TDuration Stop();

        /// \brief Restart the timer.
        /// \return Returns the time passed since the last call to Start().
        TDuration Restart();

        /// \brief Get the time duration accumulated so far.
        /// \return If the timer is still ticking returns the time duration since the last call to Start(), otherwise returns the time duration between the last Start() and the next Stop().
        TDuration operator()() const;

        /// \brief Get whether the timer is currently ticking.
        Bool IsTicking() const;

    private:

        /// \brief Whether the timer is currently ticking.
        Bool is_ticking_{ false };

        /// \brief Time point to start counting from.
        TClock::time_point base_time_point_ = TClock::now();

        /// \brief Accumulated duration.
        mutable TDuration duration_ = TDuration::zero();

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Timer<TDuration>.

    template <typename TDuration>
    inline Timer<TDuration>::Timer(Bool is_ticking)
        : is_ticking_(is_ticking)
    {
        if (is_ticking_)
        {
            Start();
        }
    }

    template <typename TDuration>
    inline void Timer<TDuration>::Start()
    {
        is_ticking_ = true;
        base_time_point_ = TClock::now();
        duration_ = TDuration::zero();
    }

    template <typename TDuration>
    inline TDuration Timer<TDuration>::Stop()
    {
        is_ticking_ = false;

        return (*this)();
    }

    template <typename TDuration>
    inline TDuration Timer<TDuration>::Restart()
    {
        auto ticks = Stop();

        Start();

        return ticks;
    }

    template <typename TDuration>
    inline TDuration Timer<TDuration>::operator()() const
    {
        if (is_ticking_)
        {
            duration_ = std::chrono::duration_cast<TDuration>(TClock::now() - base_time_point_);
        }

        return duration_;
    }

    template <typename TDuration>
    inline Bool Timer<TDuration>::IsTicking() const
    {
        return is_ticking_;
    }

}