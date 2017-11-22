
/// \file scheduler.h
/// \brief This header is part of the synergy task system. It contains definitions for task schedulers.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <thread>
#include <vector>
#include <memory>

#include "task/task.h"
#include "task/worker.h"
#include "task/task_execution_context.h"

namespace syntropy::synergy
{
    /// \brief Scheduler used to schedule and allocate tasks.
    /// \author Raffaele D. Facendola - June 2017
    class Scheduler
    {
        template <typename TCallable>
        friend void DetachTask(TCallable&& callable);

    public:

        /// \brief Get the scheduler singleton instance.
        static Scheduler& GetInstance();

        /// \brief Destructor.
        ~Scheduler();

    private:

        static thread_local Worker* thread_worker_;         ///< \brief Worker associated to this thread.

        /// \brief Singleton. Prevents direct instantiation.
        Scheduler();

        /// \brief No copy constructor.
        Scheduler(const Scheduler&) = delete;

        /// \brief No assignment operator.
        Scheduler& operator=(const Scheduler&) = delete;

        /// \brief Get a reference to any task execution context in the scheduler.
        /// \return Returns the context associated to this thread, if no such context exists a random context is returned instead.
        TaskExecutionContext& GetExecutionContext();

        std::vector<std::unique_ptr<Worker>> workers_;      ///< \brief Workers used to execute tasks concurrently.

        std::vector<std::thread> worker_threads_;           ///< \brief Threads associated with each worker.
    };

    /// \brief Get a reference to the Scheduler singleton.
    Scheduler& GetScheduler();

    /// \brief Create and schedule a new task from a callable object.
    /// \tparam TCallable Type of the callable object to wrap inside the task.
    /// \param callable Callable object to wrap inside the task.
    template <typename TCallable>
    void DetachTask(TCallable&& callable)
    {
        return GetScheduler().GetExecutionContext().DetachTask(std::forward<TCallable>(callable));
    }
}
