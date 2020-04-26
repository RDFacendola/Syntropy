#include "synergy/task/task_execution_context.h"

#include "syntropy/patterns/scope_guard.h"

namespace syntropy::synergy
{
    /************************************************************************/
    /* TASK EXEUCTION CONTEXT                                               */
    /************************************************************************/

    thread_local TaskExecutionContext* TaskExecutionContext::innermost_context_ = nullptr;

    std::shared_ptr<Task> TaskExecutionContext::ExecuteTask(std::shared_ptr<Task> task)
    {
        // Push a new context on the stack.

        auto cleanup = MakeScopeGuard([outer_context = innermost_context_, this]()
        {
            TaskExecutionContext::innermost_context_ = outer_context;

            reschedulable_task_ = nullptr;
            pending_tasks_.clear();
            continuation_tasks_.clear();
        });

        TaskExecutionContext::innermost_context_ = this;

        reschedulable_task_ = task;

        // Task execution.

        task->Execute();

        if (auto continuation = GetContinuation())
        {
            task->ContinueWith(continuation);                                               // The task is not yet finished: continue with another task.
        }
        else
        {
            task->MoveSuccessors(pending_tasks_);                                           // The task is finished: schedule each successor.
        }

        // Schedule pending tasks and return.

        return SchedulePendingTasks();
    }

    void TaskExecutionContext::RescheduleTask(const TaskList& dependencies)
    {
        SYNTROPY_ASSERT(reschedulable_task_);                                               // A task can be rescheduled at most once per execution.

        reschedulable_task_->SetDependencies(dependencies);                                 // The current task is not expected to have any dependencies at this point.

        pending_tasks_.emplace_back(reschedulable_task_);

        reschedulable_task_ = nullptr;
    }

    void TaskExecutionContext::YieldTask(const TaskList& dependencies)
    {
        SYNTROPY_ASSERT(reschedulable_task_);                                               // A task can be rescheduled at most once per execution.

        reschedulable_task_->SetDependencies(dependencies);                                 // The current task is not expected to have any dependencies at this point.

        continuation_tasks_.emplace_back(reschedulable_task_);
        pending_tasks_.emplace_back(reschedulable_task_);

        reschedulable_task_ = nullptr;
    }

    std::shared_ptr<Task> TaskExecutionContext::GetContinuation()
    {
        auto continuation_count = continuation_tasks_.size();

        if (continuation_count == 0)
        {
            return nullptr;                                                                 // No continuation.
        }
        else if (continuation_count == 1)
        {
            return continuation_tasks_.back();                                              // Only one continuation, use that task directly.
        }
        else
        {
            return CreateTask(std::move(continuation_tasks_), [] {});                       // Create an empty continuation depending on each of the many continuations so they can be executed concurrently.
        }
    }

    std::shared_ptr<Task> TaskExecutionContext::SchedulePendingTasks()
    {
        std::shared_ptr<Task> next_task;                                                    // Next task to execute after this call. This task is not notified and is returned directly.

        for (auto&& pending_task : pending_tasks_)
        {
            if (pending_task->ScheduleConditional())
            {
                if (next_task)
                {
                    on_task_ready_.Notify(*this, OnTaskReadyEventArgs{ std::move(pending_task) });
                }
                else
                {
                    next_task = std::move(pending_task);
                }
            }
        }

        return next_task;
    }

    Observable<TaskExecutionContext&, const TaskExecutionContext::OnTaskReadyEventArgs&>& TaskExecutionContext::OnTaskReady()
    {
        return on_task_ready_;
    }

    void RescheduleTask(const TaskList& dependencies)
    {
        SYNTROPY_ASSERT(TaskExecutionContext::innermost_context_);

        return TaskExecutionContext::innermost_context_->RescheduleTask(dependencies);
    }

    void YieldTask(const TaskList& dependencies)
    {
        SYNTROPY_ASSERT(TaskExecutionContext::innermost_context_);

        return TaskExecutionContext::innermost_context_->YieldTask(dependencies);
    }

}
