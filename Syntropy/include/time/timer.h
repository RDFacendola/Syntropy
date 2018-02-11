
/// \file timer.h
/// \brief This header is part of the syntropy time system. It contains classes and functionalities used to keep track of time intervals.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <chrono>

namespace syntropy
{
    /// \brief Timer class used to get time intervals.
    /// \tparam TDuration Time interval type.
    /// \tparam TClock Underlying clock type.
    /// \author Raffaele D. Facendola - November 2017.
    template <typename TDuration, typename TClock>
    class Timer
    {
    public:

        /// \brief Create a new timer.
        /// \param start Whether the timer automatically starts after being created.
        Timer(bool ticking = true)
            : ticking_(ticking)
            , base()
        {
            if (ticking_)
            {
                Start();
            }
            else
            {
                duration_ = TDuration::zero();
            }
        }

        /// \brief Start counting.
        void Start()
        {
            ticking_ = true;
            base_ = TClock::now();                      // Overrides previous values.
        }

        /// \brief Stop counting.
        /// \return Returns the time passed since the last call to Start().
        TDuration Stop()
        {
            if (ticking_)
            {
                duration_ = (*this)();
                ticking_ = false;
            }

            return duration_;
        }

        /// \brief Restart the timer.
        /// \return Returns the time passed since the last call to Start().
        TDuration Restart()
        {
            auto ticks = Stop();

            Start();

            return ticks;
        }

        /// \brief Get the time duration accumulated so far.
        /// \return If the timer is still ticking returns the time duration since the last call to Start(), otherwise returns the time duration between the last Start() and the next Stop().
        TDuration operator()() const
        {
            if (ticking_)
            {
                return std::chrono::duration_cast<TDuration>(TClock::now() - base_);
            }
            else
            {
                return duration_;
            }
        }

    private:

        bool ticking_ = false;                          ///< \brief Whether the timer is currently ticking.

        union
        {
            typename TClock::time_point base_;          ///< \brief Base time point of the timer. Valid only if the timer is ticking.

            TDuration duration_;                        ///< \brief Time duration. Valid only if the timer is not ticking.
        };

    };

    /// \brief High resolution timer definition.
    template <typename TDuration = std::chrono::high_resolution_clock::duration>
    using HighResolutionTimer = Timer<TDuration, std::chrono::high_resolution_clock>;

}