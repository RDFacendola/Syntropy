
/// \file worker.h
/// \brief This header is part of the synergy task system. It contains definitions for worker threads.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <atomic>
#include <thread>
#include <condition_variable>
#include <mutex>

#include "patterns/observable.h"

#include "task/task.h"
#include "task/task_pool.h"
#include "task/task_queue.h"
#include "task/task_execution_context.h"

namespace syntropy::synergy
{
    /// \brief Worker thread used to execute tasks.
    /// A worker thread sleeps until there's at least one task to execute.
    /// \author Raffaele D. Facendola - June 2017
    class Worker
    {
    public:

        /// \brief Create a new worker thread.
        /// \param max_tasks Maximum number of tasks allocated by the worker.
        Worker(size_t max_tasks = 0x400);

        /// \brief No copy constructor.
        Worker(const Worker&) = delete;

        /// \brief No move constructor.
        Worker(Worker&&) = delete;

        /// \brief No assignment operator.
        Worker& operator=(const Worker&) = delete;

        /// \brief Default destructor.
        ~Worker() = default;

        /// \brief Starts the worker loop synchronously.
        void Start();

        /// \brief Request loop termination.
        /// Tasks that were not executed are canceled.
        void Stop();

        /// \brief Check whether the worker thread is running.
        /// \return Returns true if the worker thread is running, returns false otherwise.
        bool IsRunning() const;

        /// \brief Enqueue a new task for execution.
        void EnqueueTask(std::shared_ptr<Task> task);

        /// \brief Dequeue a task scheduled on this worker.
        /// \return Returns a task scheduled on this worker. If no such task exists, returns nullptr.
        std::shared_ptr<Task> DequeueTask();

        /// \brief Get the execution context associated to this worker.
        /// \return Returns the execution context associated to this worker. If the worker is not running returns nullptr.
        TaskExecutionContext* GetExecutionContext();

        /// \brief Observable event called whenever a new task is enqueued in this worker.
        Observable<Worker&>& OnTaskEnqueued();

        /// \brief Observable event called whenever the worker ran out of tasks to execute.
        Observable<Worker&>& OnStarving();

        ///\brief Observable event called whenever the worker becomes ready to accept tasks for execution.
        Observable<Worker&>& OnReady();

    private:

        /// \brief Fetch a new task for execution.
        /// The thread is put to sleep if no task could be fetched.
        /// \return Returns a pointer to the next task to execute if the thread is running, returns nullptr otherwise.
        std::shared_ptr<Task> FetchTask();

        std::atomic<TaskExecutionContext*> execution_context_{ nullptr };       ///< \brief Execution context for this worker.

        TaskQueue tasks_;                                                       ///< \brief Tasks that are scheduled in this worker ready for execution. Other tasks in the system are referenced via task dependencies.

        std::atomic_bool is_running_{ false };                                  ///< \brief Whether the worker is running.

        std::mutex mutex_;                                                      ///< \brief Used for synchronization purposes.

        std::condition_variable wake_up_;                                       ///< \brief Condition variable used to wake up a sleeping thread.

        Event<Worker&> on_task_enqueued_;                                       ///< \brief Event called whenever a new task is enqueued in this worker.

        Event<Worker&> on_starving_;                                            ///< \brief Event called whenever the worker ran out of tasks to execute.

        Event<Worker&> on_ready_;                                               ///< \brief Event called whenever the worker becomes ready to accept tasks for execution.
    };
}
