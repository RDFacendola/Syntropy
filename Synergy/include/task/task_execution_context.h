
/// \file task_execution_context.h
/// \brief This header is part of the synergy task system. It contains definitions for contexts used to execute tasks.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <memory>
#include <algorithm>

#include "patterns/observable.h"

#include "task/task.h"
#include "task/task_pool.h"

namespace syntropy::synergy
{
    /// \brief Context object used to execute, spawn and continue tasks.
    /// \author Raffaele D. Facendola - November 2017
    class TaskExecutionContext
    {
        template <typename TDependencies, typename TCallable>
        friend std::shared_ptr<Task> CreateTask(TDependencies&& dependencies, TCallable&& callable);

        template <typename TTask, typename TDependencies, typename... TArguments>
        friend std::shared_ptr<Task> EmplaceTask(TDependencies&& dependencies, TArguments&&... arguments);

        template <typename TDependencies, typename TCallable>
        friend std::shared_ptr<Task> CreateTaskContinuation(TDependencies&& dependencies, TCallable&& callable);

        template <typename TTask, typename TDependencies, typename... TArguments>
        friend std::shared_ptr<Task> EmplaceTaskContinuation(TDependencies&& dependencies, TArguments&&... arguments);

        template <typename TDependencies>
        friend void RescheduleTask(TDependencies&& dependencies);

        template <typename TDependencies>
        friend void YieldTask(TDependencies&& dependencies);

    public:

        /// \brief Arguments of the event called whenever a task became ready for execution.
        struct OnTaskReadyEventArgs
        {
            std::shared_ptr<Task> task_;                ///< \brief Task ready for execution.
        };

        /// \brief Execute a task that runs without dependencies nor successors on this execution context.
        /// The task is scheduled immediately after creation.
        /// \tparam TCallable Type of the callable object to wrap inside the task.
        /// \param callable Callable object to wrap inside the task.
        template <typename TCallable>
        void DetachTask(TCallable&& callable)
        {
            auto task = task_pool_.CreateTask(std::initializer_list<std::shared_ptr<Task>>{}, std::forward<TCallable>(callable));

            task->ScheduleConditional();            // The task has no dependencies: this call must yield true.

            on_task_ready_.Notify(*this, OnTaskReadyEventArgs{ std::move(task) });
        }

        /// \brief Execute the provided task.
        /// \return Returns the next task to execute.
        std::shared_ptr<Task> ExecuteTask(std::shared_ptr<Task> task);

        /// \brief Observable event called whenever a new task is ready for execution.
        Observable<TaskExecutionContext&, const OnTaskReadyEventArgs&>& OnTaskReady();

    private:

        template <typename TDependencies, typename TCallable>
        std::shared_ptr<Task> CreateTask(TDependencies&& dependencies, TCallable&& callable)
        {
            auto task = task_pool_.CreateTask(std::forward<TDependencies>(dependencies), std::forward<TCallable>(callable));

            pending_tasks_.emplace_back(task);

            return task;
        }

        template <typename TTask, typename TDependencies, typename... TArguments>
        std::shared_ptr<Task> EmplaceTask(TDependencies&& dependencies, TArguments&&... arguments)
        {
            auto task = task_pool_.EmplaceTask<TTask>(std::forward<TDependencies>(dependencies), std::forward<TArguments>(arguments)...);

            pending_tasks_.emplace_back(task);

            return task;
        }

        template <typename TDependencies, typename TCallable>
        std::shared_ptr<Task> CreateTaskContinuation(TDependencies&& dependencies, TCallable&& callable)
        {
            auto task = task_pool_.CreateTask(std::forward<TDependencies>(dependencies), std::forward<TCallable>(callable));

            continuation_tasks_.emplace_back(task);
            pending_tasks_.emplace_back(task);

            return task;
        }

        template <typename TTask, typename TDependencies, typename... TArguments>
        std::shared_ptr<Task> EmplaceTaskContinuation(TDependencies&& dependencies, TArguments&&... arguments)
        {
            auto task = task_pool_.EmplaceTask<TTask>(std::forward<TDependencies>(dependencies), std::forward<TArguments>(arguments)...);

            continuation_tasks_.emplace_back(task);
            pending_tasks_.emplace_back(task);

            return task;
        }

        /// \brief Set the current task to be rescheduled as a new task after its current execution.
        /// A task can either be yielded via YieldTask or rescheduled via RescheduleTask, but not both at the same time.
        /// A task can be rescheduled at most once per execution.
        /// \tparam TDependencies Type of the container for task dependencies. The container must contain TaskHandlers and should be iterable.
        /// \param dependencies List of tasks the current task continuation depends upon.
        template <typename TDependencies>
        void RescheduleTask(TDependencies&& dependencies)
        {
            SYNTROPY_ASSERT(reschedulable_task_);                                               // A task can be rescheduled at most once per execution.

            reschedulable_task_->SetDependencies(std::forward<TDependencies>(dependencies));    // The current task is not expected to have any dependencies at this point.

            pending_tasks_.emplace_back(reschedulable_task_);

            reschedulable_task_ = nullptr;
        }

        /// \brief Set the current task to be rescheduled as a continuation of itself after its current execution.
        /// A task can either be yielded via YieldTask or rescheduled via RescheduleTask, but not both at the same time.
        /// A task can be yielded at most once per execution.
        /// \tparam TDependencies Type of the container for task dependencies. The container must contain TaskHandlers and should be iterable.
        /// \param dependencies List of tasks the current task continuation depends upon.
        template <typename TDependencies>
        void YieldTask(TDependencies&& dependencies)
        {
            SYNTROPY_ASSERT(reschedulable_task_);                                               // A task can be rescheduled at most once per execution.

            reschedulable_task_->SetDependencies(std::forward<TDependencies>(dependencies));    // The current task is not expected to have any dependencies at this point.

            continuation_tasks_.emplace_back(reschedulable_task_);
            pending_tasks_.emplace_back(task);

            reschedulable_task_ = nullptr;
        }

        /// \brief Get the continuation task for the task being executed.
        /// After calling this method, continuation_tasks_ content becomes valid but undefined.
        /// \return Returns the continuation task for the task being executed.
        std::shared_ptr<Task> GetContinuation();

        /// \brief Schedule current pending tasks.
        /// After calling this method, pending_tasks_ content becomes valid but undefined.
        /// \return Returns the next task to be executed on this context. This task, if present, is not notified.
        std::shared_ptr<Task> SchedulePendingTasks();

        static thread_local TaskExecutionContext* innermost_context_;                           ///< \brief Current task execution context.

        TaskPool task_pool_;                                                                    ///< \brief Pool used to allocate new tasks.

        std::shared_ptr<Task> reschedulable_task_;                                              ///< \brief Task that can be rescheduled in this context. It can either contain the current task or nullptr if the task was already rescheduled.

        std::vector<std::shared_ptr<Task>> pending_tasks_;                                      ///< \brief Pending tasks waiting to be scheduled.

        std::vector<std::shared_ptr<Task>> continuation_tasks_;                                 ///< \brief Continuations for the task being executed. Elements in this collection are guaranteed to be contained inside pending_tasks_.

        Event<TaskExecutionContext&, const OnTaskReadyEventArgs&> on_task_ready_;               ///< \brief Event called whenever a new task became ready for execution.
    };

    /// \brief Create a new task from a callable object.
    /// \param arguments Arguments to pass to the task creation. See Task::Construct.
    /// \return Returns the new task.
    template <typename TDependencies, typename TCallable>
    std::shared_ptr<Task> CreateTask(TDependencies&& dependencies, TCallable&& callable)
    {
        SYNTROPY_ASSERT(TaskExecutionContext::innermost_context_);

        return TaskExecutionContext::innermost_context_->CreateTask(std::forward<TDependencies>(dependencies), std::forward<TCallable>(callable));
    }

    /// \brief Create a new task constructing the callable object in-place.
    /// \tparam TTask Type of the callable object to construct.
    /// \param arguments Arguments to pass to the task in-place creation. See Task::Emplace.
    /// \return Returns the new task.
    template <typename TTask, typename TDependencies, typename... TArguments>
    std::shared_ptr<Task> EmplaceTask(TDependencies&& dependencies, TArguments&&... arguments)
    {
        SYNTROPY_ASSERT(TaskExecutionContext::innermost_context_);

        return TaskExecutionContext::innermost_context_->CreateTask(std::forward<TDependencies>(dependencies), std::forward<TArguments>(arguments)...);
    }

    /// \brief Create a continuation for the current task from a callable object.
    /// \param arguments Arguments to pass to the task creation. See TaskPool::CreateTask.
    /// \return Returns the new task.
    template <typename TDependencies, typename TCallable>
    std::shared_ptr<Task> CreateTaskContinuation(TDependencies&& dependencies, TCallable&& callable)
    {
        SYNTROPY_ASSERT(TaskExecutionContext::innermost_context_);

        return TaskExecutionContext::innermost_context_->CreateTaskContinuation(std::forward<TDependencies>(dependencies), std::forward<TCallable>(callable));
    }

    /// \brief Create a continuation for the current task constructing the callable object in-place.
    /// \tparam TTask Type of the callable object to construct.
    /// \param arguments Arguments to pass to the task in-place creation. See TaskPool::EmplaceTask.
    /// \return Returns the new task.
    template <typename TTask, typename TDependencies, typename... TArguments>
    std::shared_ptr<Task> EmplaceTaskContinuation(TDependencies&& dependencies, TArguments&&... arguments)
    {
        SYNTROPY_ASSERT(TaskExecutionContext::innermost_context_);

        return TaskExecutionContext::innermost_context_->EmplaceTaskContinuation<TTask>(std::forward<TDependencies>(dependencies), std::forward<TArguments>(arguments)...);
    }

    /// \brief Set the current task to be rescheduled as a new task after its current execution.
    /// A task can either be yielded via YieldTask or rescheduled via RescheduleTask, but not both at the same time.
    /// A task can be rescheduled at most once per execution.
    template <typename TDependencies>
    void RescheduleTask(TDependencies&& dependencies)
    {
        SYNTROPY_ASSERT(TaskExecutionContext::innermost_context_);

        return TaskExecutionContext::innermost_context_->RescheduleTask(std::forward<TDependencies>(dependencies));
    }

    /// \brief Set the current task to be rescheduled as a continuation of itself after its current execution.
    /// A task can either be yielded via YieldTask or rescheduled via RescheduleTask, but not both at the same time.
    /// A task can be yielded at most once per execution.
    /// \tparam TDependencies Type of the container for task dependencies. The container must contain TaskHandlers and should be iterable.
    /// \param dependencies List of tasks the current task continuation depends upon.
    template <typename TDependencies>
    void YieldTask(TDependencies&& dependencies)
    {
        SYNTROPY_ASSERT(TaskExecutionContext::innermost_context_);

        return TaskExecutionContext::innermost_context_->YieldTask(std::forward<TDependencies>(dependencies));
    }

}