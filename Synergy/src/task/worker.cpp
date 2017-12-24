#include "task/worker.h"

#include "diagnostics/assert.h"
#include "patterns/scope_guard.h"

namespace syntropy::synergy
{
    /************************************************************************/
    /* WORKER                                                               */
    /************************************************************************/

    Worker::Worker(size_t max_tasks)
        : tasks_(max_tasks)
    {

    }

    void Worker::Start()
    {
        auto cleanup = MakeScopeGuard([this]() { execution_context_ = nullptr; });          // Make sure to reset the execution context after leaving.

        // Setup.

        TaskExecutionContext context;

        execution_context_ = &context;

        auto handle = context.OnTaskReady().Subscribe([this](auto& /*sender*/, auto& args) 
        {
            EnqueueTask(args.task_);                                                        // #TODO Avoid accessing the queue if the task is going to be stolen.

            on_task_enqueued_.Notify(*this);
        });

        is_running_.store(true, std::memory_order_release);

        on_ready_.Notify(*this);                                                            // the worker is now ready to accept new tasks.

        // Main loop.

        while (auto task = FetchTask())                                                     // Outer loop: fetch tasks concurrently.
        {
            while(task && IsRunning())
            {
                task = context.ExecuteTask(std::move(task));                                // Inner loop: non-concurrent depth-first execution to improve scalability and cache locality.
            }
        }

        // Flush remaining tasks.

        tasks_.Clear();
    }

    void Worker::Stop()
    {
        is_running_.store(false, std::memory_order_release);

        wake_up_.notify_all();
    }

    bool Worker::IsRunning() const
    {
        return is_running_.load(std::memory_order_relaxed);
    }

    void Worker::EnqueueTask(std::shared_ptr<Task> task)
    {
        tasks_.PushBack(task);

        wake_up_.notify_all();
    }

    std::shared_ptr<Task> Worker::DequeueTask()
    {
        return tasks_.PopFront();
    }

    std::shared_ptr<Task> Worker::FetchTask()
    {
        std::shared_ptr<Task> task;

        // Wait until there's a new task to execute or a termination was requested.

        std::unique_lock<std::mutex> lock(mutex_);

        wake_up_.wait(lock, [this, &task]()
        {
            if (!IsRunning())
            {
                return true;                        // Return immediately if termination was requested.
            }

            task = tasks_.PopBack();                // Pop a new task.

            if (task == nullptr)
            {
                // Notify the worker is about to starve and check if a new task shows up.

                on_starving_.Notify(*this);

                task = tasks_.PopBack();
            }

            return task != nullptr;
        });

        return task;
    }

    TaskExecutionContext* Worker::GetExecutionContext()
    {
        return execution_context_.load(std::memory_order_relaxed);
    }

    Observable<Worker&>& Worker::OnTaskEnqueued()
    {
        return on_task_enqueued_;
    }

    Observable<Worker&>& Worker::OnStarving()
    {
        return on_starving_;
    }

    Observable<Worker&>& Worker::OnReady()
    {
        return on_ready_;
    }

}
