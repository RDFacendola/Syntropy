#include "task/task.h"

#include "diagnostics/assert.h"

namespace syntropy::synergy
{

    /************************************************************************/
    /* SCHEDULER                                                            */
    /************************************************************************/

    thread_local Worker* Scheduler::thread_worker_ = nullptr;

    Scheduler::Scheduler()
        : workers_(std::thread::hardware_concurrency())
    {
        CreateWorkerThreads();
    }

    Scheduler& Scheduler::GetInstance()
    {
        static Scheduler instance;
        return instance;
    }

    Scheduler::~Scheduler()
    {
        // TODO: Send the "terminate" signal to the workers and wait
    }

    void Scheduler::CreateWorkerThreads()
    {
        // TODO: Register to worker events

        for (auto&& worker : workers_)
        {
            std::thread worker_thread([&worker]()
            {
                Scheduler::thread_worker_ = &worker;        // Update the thread-local worker pointer.

                worker.Run();                               // Worker loop.
            });

            worker_thread.detach();                         // The worker thread is detached: threading capabilities are managed directly by the worker object.
        }
    }

    Worker& Scheduler::GetWorker()
    {
        // Attempts to return this thread's worker to improve cache locality.

        if (thread_worker_)
        {
            return *thread_worker_;
        }

        // TODO: If no worker is found, get the worker with the least amount of tasks to improve load balancing.
        // TODO: Maybe the worker with the highest amount of predecessors of the task being created? (cache locality)

        return workers_.front();
    }

    void Scheduler::Join()
    {
        std::unique_lock<std::mutex> lock(mutex_);

        has_no_tasks_.wait(lock, [this]()
        {
            return tasks_count_.load(std::memory_order_acquire) == 0;
        });
    }

    Scheduler& GetScheduler()
    {
        return Scheduler::GetInstance();
    }

    /************************************************************************/
    /* WORKER                                                               */
    /************************************************************************/

    Worker::Worker()
        : is_running_(false)
    {

    }

    void Worker::PushTaskForExecution(TaskPtr task)
    {
        SYNTROPY_PRECONDITION(task->IsReadyForExecution());

        tasks_.push_back(task);

        wake_up_.notify_all();
    }

    TaskPtr Worker::PopTaskForExecution()
    {
        if (tasks_.size() > 0)
        {
            auto task = std::move(tasks_.back());

            tasks_.pop_back();

            return task;
        }

        return nullptr;
    }

    void Worker::Run()
    {
        is_running_.store(true, std::memory_order_release);

        while (IsRunning())
        {
            auto task = PopTaskForExecution();

            if (task)
            {
                ExecuteTask(std::move(task));
            }
            else
            {
                WaitForTasks();
            }
        }

        on_terminated_.Notify(*this);
    }

    void Worker::Stop()
    {
        is_running_.store(false, std::memory_order_release);

        wake_up_.notify_all();
    }

    bool Worker::IsRunning() const
    {
        return is_running_.load(std::memory_order_consume);
    }

    Observable<Worker&>& Worker::OnOutOfTasks()
    {
        return on_out_of_tasks_;
    }

    Observable<Worker&, Task&>& Worker::OnTaskAdded()
    {
        return on_task_added_;
    }

    Observable<Worker&, Task&>& Worker::OnTaskExecuted()
    {
        return on_task_executed_;
    }

    Observable<Worker&>& Worker::OnTerminated()
    {
        return on_terminated_;
    }

    void Worker::ExecuteTask(TaskPtr task)
    {
        task->Execute();

        on_task_executed_.Notify(*this, *task);
    }

    void Worker::WaitForTasks()
    {
        // Notify that the worker ran out of tasks
        on_out_of_tasks_.Notify(*this);

        // Wait until more tasks are added to the worker (or termination is requested)
        std::unique_lock<std::mutex> lock(mutex_);

        wake_up_.wait(lock, [this]()
        {
            return tasks_.size() > 0 || !IsRunning();
        });
    }

    /************************************************************************/
    /* TASK                                                                 */
    /************************************************************************/

    Task::Task()
        : dependencies_count_(0u)
    {

    }

    void Task::Execute()
    {
        SYNTROPY_PRECONDITION(IsReadyForExecution());

        auto continuation = DoExecute();

        if (!continuation)
        {
            // Update successors dependencies count if the task has no continuation (aka has been executed)

            for (auto&& successor : successors_)
            {
                successor->DecrementDependencyCount();
            }
        }
        else
        {
            // Schedule the continuation

            if (continuation != this)
            {
                // Move the successors to the continuation
                continuation->AddSuccessors(std::move(successors_));
            }
        }

        // Clear out successors list (shared ptr may keep the tasks alive)
        successors_.clear();
    }

    bool Task::IsReadyForExecution() const
    {
        return dependencies_count_.load(std::memory_order_acquire) == 0;
    }

    void Task::DecrementDependencyCount()
    {
        if (dependencies_count_.fetch_sub(1, std::memory_order_relaxed) == 1)
        {
            // Schedule this task for execution

        }
    }

    void Task::AddPredecessors(std::initializer_list<TaskPtr> predecessors)
    {
        // Increment the dependency count

        dependencies_count_.fetch_add(predecessors.size(), std::memory_order_release);

        // Update the predecessors

        for (auto&& predecessor : predecessors)
        {
            predecessor->successors_.emplace_back(shared_from_this());
        }
    }

    void Task::AddSuccessors(std::vector<TaskPtr> successors)
    {
        successors_.reserve(successors_.size() + successors.size());

        std::copy
        (
            successors.begin(),
            successors.end(),
            std::back_inserter(successors_)
        );
    }

    /************************************************************************/
    /* LAMBDA TASK                                                          */
    /************************************************************************/

    LambdaTask::LambdaTask()
        : lambda_([]() { return nullptr; })
    {

    }

    LambdaTask::LambdaTask(std::function<Task*(void)> lambda)
        : lambda_(std::move(lambda))
    {

    }

    Task* LambdaTask::DoExecute()
    {
        return lambda_();
    }
}
