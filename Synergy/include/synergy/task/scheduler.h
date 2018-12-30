
/// \file scheduler.h
/// \brief This header is part of the synergy task system. It contains definitions for task schedulers.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <thread>
#include <vector>
#include <memory>
#include <optional>

#include "syntropy/math/random.h"

#include "syntropy/platform/threading.h"

#include "synergy/patterns/sync_counter.h"

#include "synergy/task/task.h"
#include "synergy/task/worker.h"
#include "synergy/task/task_execution_context.h"

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

        /// \brief Default destructor. 
        /// Aborts any pending work and blocks the calling thread until each worker thread finishes its execution.
        ~Scheduler() = default;

        /// \brief Initialize the scheduler.
        /// Calling other methods on this class before calling "Initialize()" is undefined behaviour.
        /// \param cores Cores reserved for scheduler execution. If left unset all the available cores are taken into account.
        /// \remarks Cores having no affinity for the current process are ignored and do not spawn any worker thread.
        void Initialize(std::optional<platform::AffinityMask> cores = std::nullopt);

    private:

        /// \brief Associate each worker object with its own running thread.
        struct WorkerThread
        {
            /// \brief Create a new worker thread.
            /// \param max_tasks Maximum number of tasks that can be allocated by the worker.

            WorkerThread(size_t max_tasks = 0x400);

            WorkerThread(WorkerThread&& other) = default;

            /// \brief Destroy the worker object. 
            /// Requests tasks termination and blocks the calling thread until the worker thread finishes its execution.
            ~WorkerThread();

            /// \brief Start the worker thread asynchronously.
            /// \param affinity Core affinity for the thread being started. If left unset the thread is started with default affinity.
            void StartAsync(std::optional<platform::AffinityMask> affinity = std::nullopt);

            /// \brief Get the worker object.
            Worker& GetWorker();

        private:

            std::unique_ptr<Worker> worker_;                    ///< \brief Worker object used to execute tasks.

            std::thread thread_;                                ///< \brief Thread the worker object is spinning onto.
        };

        static thread_local Worker* thread_worker_;             ///< \brief Worker associated to this thread.

        /// \brief Singleton. Prevents direct instantiation.
        Scheduler() = default;

        /// \brief No copy constructor.
        Scheduler(const Scheduler&) = delete;

        /// \brief No assignment operator.
        Scheduler& operator=(const Scheduler&) = delete;

        /// \brief Called whenever a task is enqueued in one worker.
        /// \param sender Worker the task was enqueued on.
        void OnTaskEnqueued(Worker& sender);

        /// \brief Called whenever a worker ran out of tasks.
        /// \param sender Worker who ran out of tasks.
        void OnWorkerStarving(Worker& sender);

        /// \brief Called whenever a worker is ready for execution.
        /// \param sender Worker who's ready to execute tasks.
        void OnWorkerReady(Worker& sender);

        /// \brief Get a reference to any task execution context in the scheduler.
        /// This function always prefer a thread-local context, if possible.
        /// \return Returns any task execution context.
        TaskExecutionContext& GetExecutionContext();

        std::vector<WorkerThread> workers_;                     ///< \brief Workers used to execute tasks concurrently.

        mutable std::mutex mutex_;

        std::vector<Worker*> starving_workers_;

        Random random_;                                         ///< \brief Internal random number generator.

        SyncCounter worker_thread_sync_;                        ///< \brief Object used to synchronize worker threads.
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
