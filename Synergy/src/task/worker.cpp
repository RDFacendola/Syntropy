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

        is_running_.store(true, std::memory_order_relaxed);

        auto handle = context.OnTaskReady().Subscribe([this](auto& /*sender*/, auto& args) 
        {
            EnqueueTask(args.task_);
        });

        // Main loop.

        while (IsRunning())
        {
            auto task = FetchTask();                                                        // Wait until a new task is ready to be executed (concurrent access).

            while(task && IsRunning())
            {
                task = context.ExecuteTask(std::move(task));                                // Depth-first execution to improve cache locality and avoid accessing the task queue concurrently.
            }
        }

        // Flush remaining tasks.

        tasks_.Clear();
    }

    void Worker::Stop()
    {
        is_running_.store(false, std::memory_order_relaxed);

        wake_up_.notify_all();
    }

    bool Worker::IsRunning() const
    {
        return is_running_.load(std::memory_order_relaxed);
    }

    TaskExecutionContext* Worker::GetExecutionContext()
    {
        return execution_context_.load(std::memory_order_relaxed);
    }

    void Worker::EnqueueTask(std::shared_ptr<Task> task)
    {
        tasks_.PushBack(task);

        wake_up_.notify_all();
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
                return true;
            }

            task = tasks_.PopBack();

            return task != nullptr;
        });

        return task;
    }

}
