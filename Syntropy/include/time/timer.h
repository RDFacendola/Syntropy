
/// \file timer.h
/// \brief This header is part of the syntropy time system. It contains classes and functionalities used to keep track of time intervals.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <chrono>

namespace syntropy
{
    /// \brief Timer class used to get time intervals.
    /// \tparam TDuration Time interval class used to count ticks.
    /// \tparam TClock Underlying clock type.
    /// \author Raffaele D. Facendola - November 2017.
    template <typename TDuration, typename TClock>
    class Timer
    {
    public:

        /// \brief Arithmetic type representing the number of ticks.
        using TTicks = typename TDuration::rep;

        /// \brief Create a new timer.
        /// \param start Whether the timer automatically starts after being created.
        Timer(bool ticking = true)
            : ticking_(ticking)
        {
            if (ticking_)
            {
                Start();
            }
            else
            {
                ticks_ = TDuration::zero().count();
            }
        }

        /// \brief Start counting.
        void Start()
        {
            ticking_ = true;
            base_ = TClock::now();                      // Overrides previous values.
        }

        /// \brief Stop counting.
        /// \return Returns the number of ticks since the last call to Start().
        TTicks Stop()
        {
            if (ticking_)
            {
                ticks_ = (*this)();
                ticking_ = false;
            }

            return ticks_;
        }

        /// \brief Restart the timer.
        /// \return Returns the number of ticks since the last call to Start().
        TTicks Restart()
        {
            auto ticks = Stop();

            Start();

            return ticks;
        }

        /// \brief Get the amount of ticks accumulated so far.
        /// \return If the timer is still ticking returns the number of ticks since the last call to Start(), otherwise returns the number of ticks between the last Start() and the next Stop().
        TTicks operator()() const
        {
            if (ticking_)
            {
                return std::chrono::duration_cast<TDuration>(TClock::now() - base_).count();
            }
            else
            {
                return ticks_;
            }
        }

    private:

        bool ticking_ = false;                          ///< \brief Whether the timer is currently ticking.

        union
        {
            typename TClock::time_point base_;          ///< \brief Base time point of the timer. Valid only if the timer is ticking.

            TTicks ticks_;                              ///< \brief Ticks counter by the timer. Valid only if the timer is not ticking.
        };

    };

    /// \brief High resolution timer definition.
    template <typename TDuration = std::chrono::high_resolution_clock::duration>
    using HighResolutionTimer = Timer<TDuration, std::chrono::high_resolution_clock>;

}