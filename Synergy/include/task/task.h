
/// \file task.h
/// \brief This header is part of the synergy task system. It contains definitions for tasks and schedulers.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <vector>
#include <memory>

#include <atomic>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <functional>

#include "patterns/observable.h"

namespace syntropy::synergy
{

    class Scheduler;
    class Worker;
    class Task;

    /// \brief Smart pointer used to handle tasks.
    /// Tasks ownership is shared among the Scheduler and the application.
    using TaskPtr = std::shared_ptr<Task>;

    /// \brief Scheduler used to schedule and allocate tasks.
    /// \author Raffaele D. Facendola - June 2017
    class Scheduler
    {
    public:

        /// \brief Get the scheduler singleton instance.
        static Scheduler& GetInstance();

        /// \brief Destructor.
        ~Scheduler();

        /// \brief Wait for the workers to finish their execution.
        void Join();

        /// \brief Create and schedule a new task.
        /// \tparam TTask Type of task to create. Must derive from Task.
        /// \param predecessors List of tasks that must be executed before executing the new task.
        /// \param arguments Arguments used to construct a new instance of TTask.
        /// \return Returns a pointer to the new task. If the task couldn't be created, returns nullptr.
        template <typename TTask, typename... TArguments>
        TaskPtr CreateTask(std::initializer_list<TaskPtr> predecessors, TArguments&&... arguments);

    private:

        /// \brief Singleton. Prevents direct instantiation.
        Scheduler();

        /// \brief Create the worker threads.
        void CreateWorkerThreads();

        /// \brief Select the best worker from the scheduler.
        /// \return Returns the worker associated to this thread if any, otherwise returns the worker with the least amount of pending tasks.
        Worker& GetWorker();

        static thread_local Worker* thread_worker_;         ///< \brief Worker of this thread. nullptr if this thread has no associated worker.

        std::vector<Worker> workers_;                       ///< \brief Actual workers. One for each worker thread.

        std::mutex mutex_;                                  ///< \brief Used for synchronization purposes.

        std::condition_variable has_no_tasks_;              ///< \brief Condition variable that is signaled when there are no more tasks in the system waiting to be executed.

        std::atomic_size_t tasks_count_;                    ///< \brief Total number of tasks in the system (either pending or being executed).
    };

    /// \brief Worker thread used to execute tasks.
    /// \author Raffaele D. Facendola - June 2017
    class Worker
    {
    public:

        /// \brief Create a new worker thread.
        Worker();

        /// \brief No copy constructor.
        Worker(const Worker&) = delete;
        
        /// \brief Default move constructor.
        Worker(Worker&& other) = default;

        /// \brief No assignment operator.
        Worker& operator=(const Worker&) = delete;

        /// \brief Default destructor.
        ~Worker() = default;

        /// \brief Add a new task ready for execution on this worker.
        void PushTaskForExecution(TaskPtr task);

        /// \brief Get a new task ready for execution on this worker.
        /// \return Returns a task ready for execution. If no task exists returns nullptr.
        TaskPtr PopTaskForExecution();

        /// \brief Create and schedule a new task on this worker.
        /// \tparam TTask Type of task to create. Must derive from Task.
        /// \param predecessors List of tasks that must be executed before executing the new task.
        /// \param arguments Arguments used to construct a new instance of TTask.
        /// \return Returns a pointer to the new task. If the task couldn't be created, returns nullptr.
        template <typename TTask, typename... TArguments>
        TaskPtr CreateTask(std::initializer_list<TaskPtr> predecessors, TArguments&&... arguments);

        /// \brief Starts the main loop.
        /// Synchronous call.
        void Run();

        /// \brief Request thread termination.
        void Stop();

        /// \brief Check whether the worker thread is running.
        /// \return Returns true if the worker thread is running, returns false otherwise.
        bool IsRunning() const;

        /// \brief Event signaled whenever a this worker ran out of work to do.
        Observable<Worker&>& OnOutOfTasks();

        /// \brief Event signaled whenever a new task is scheduled on this worker.
        Observable<Worker&, Task&>& OnTaskAdded();

        /// \brief Event signaled whenever a task has been executed by this worker.
        Observable<Worker&, Task&>& OnTaskExecuted();

        /// \brief Event signaled whenever the worker thread has ended.
        Observable<Worker&>& OnTerminated();

    private:

        /// \brief Execute a task.
        /// \param task Task to execute.
        void ExecuteTask(TaskPtr task);

        /// \brief Wait until more tasks are ready to be executed on this worker or termination is requested.
        void WaitForTasks();

        // Any thread can push task in this queue - Scheduling can be performed by any thread

        std::vector<TaskPtr> tasks_;                    ///< \brief Tasks that are scheduled in this worker ready for execution.

        std::atomic_bool is_running_;                   ///< \brief Whether the worker is running.

        std::mutex mutex_;                              ///< \brief Used for synchronization purposes.

        std::condition_variable wake_up_;               ///< \brief Condition variable that is signaled whenever the worker has tasks to execute or a termination is requested. Used to wakeup a sleeping thread.

        Event<Worker&> on_out_of_tasks_;                ///< \brief Event signaled whenever a this worker ran out of work to do.

        Event<Worker&, Task&> on_task_added_;           ///< \brief Event signaled whenever a new task is scheduled on this worker.

        Event<Worker&, Task&> on_task_executed_;        ///< \brief Event signaled whenever a task has been executed by this worker.

        Event<Worker&> on_terminated_;                  ///< \brief Event signaled whenever the worker thread has ended.
    };

    /// \brief Represents the atomic unit of a parallel computation.
    /// \author Raffaele D. Facendola - June 2017
    class Task : public std::enable_shared_from_this<Task>
    {
        friend class Worker;

    public:

        /// \brief Default constructor.
        Task();

        /// \brief Virtual destructor.
        virtual ~Task() = default;

        /// \brief Execute the tasks.
        /// This method may unlock successor tasks for execution.
        /// \return Returns true if the task was exhausted, returns false otherwise.
        void Execute();

        /// \brief Check whether this task is ready for execution.
        /// \param Returns true if this task has no outstanding dependencies and can be executed, returns false otherwise.
        bool IsReadyForExecution() const;

    protected:

        virtual Task* DoExecute() = 0;

    private:

        /// \brief Reduce the dependency count by one.
        /// If the dependency count drops to 0 the task is scheduled for execution.
        void DecrementDependencyCount();

        /// \brief Add predecessors to this task.
        /// \param predecessors List of predecessors to add.
        void AddPredecessors(std::initializer_list<TaskPtr> predecessors);

        /// \brief Add new successors to this task.
        /// This method doesn't update the dependency count of the successors.
        void AddSuccessors(std::vector<TaskPtr> successors);

        std::atomic_size_t dependencies_count_;     ///< \brief Number of dependencies yet to execute. This task can be executed when this number drops to 0.

        std::vector<TaskPtr> successors_;           ///< \brief List of tasks that depends on this one.

    };

    /// \brief Represents a simple tasks that executes a lambda function.
    /// \author Raffaele D. Facendola - June 2017
    class LambdaTask : public Task
    {
    public:

        /// \brief Create a new lambda task that does nothing and return no continuation.
        LambdaTask();

        /// \brief Create a new lambda task.
        LambdaTask(std::function<Task*(void)> lambda);

    private:

        virtual Task* DoExecute() override;

        std::function<Task*(void)> lambda_;         ///< \brief Task to execute.
    };

    /// \brief Get a reference to the Scheduler singleton.
    Scheduler& GetScheduler();

}

// Implementation

namespace syntropy::synergy
{

    /************************************************************************/
    /* SCHEDULER                                                            */
    /************************************************************************/

    template <typename TTask, typename... TArguments>
    TaskPtr Scheduler::CreateTask(std::initializer_list<TaskPtr> predecessors, TArguments&&... arguments)
    {
        tasks_count_.fetch_add(1, std::memory_order_acq_rel);

        return GetWorker().CreateTask<TTask>(std::move(predecessors), std::forward<TArguments>(arguments)...);
    }

    /************************************************************************/
    /* WORKER                                                               */
    /************************************************************************/

    template <typename TTask, typename... TArguments>
    TaskPtr Worker::CreateTask(std::initializer_list<TaskPtr> predecessors, TArguments&&... arguments)
    {
        // TODO: Use a proper allocator (lock free)

        auto task = std::make_shared<TTask>(std::forward<TArguments>(arguments)...);

        // BEGIN OF CRITICAL SECTION - Other threads should see these changes happen atomically.

        task->AddPredecessors(std::move(predecessors));

        // Schedule the new task if possible

        if (task->IsReadyForExecution())
        {
            PushTaskForExecution(std::move(task));
        }

        // END OF CRITICAL SECTION

        // Notify that a new task has been scheduled on this worker.
        on_task_added_.Notify(*this, *task);

        return task;
    }

}
