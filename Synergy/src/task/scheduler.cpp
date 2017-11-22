#include "task/scheduler.h"

#include "diagnostics/assert.h"
#include "patterns/scope_guard.h"

namespace syntropy::synergy
{

    /************************************************************************/
    /* SCHEDULER                                                            */
    /************************************************************************/
    
    thread_local Worker* Scheduler::thread_worker_ = nullptr;

    Scheduler& Scheduler::GetInstance()
    {
        static Scheduler instance;
        return instance;
    }

    Scheduler::Scheduler()
        : workers_(std::thread::hardware_concurrency())             // #TODO This number should be configurable since we may want to save some cores for other threads.
    {
        // Create workers and worker threads.

        worker_threads_.reserve(workers_.size());

        for (auto&& worker : workers_)
        {
            worker = std::make_unique<Worker>(0x400);               // #TODO Maximum number of tasks per worker should be configurable.

            // #TODO Setup worker thread affinity such that each worker is always scheduled on the same core.

            worker_threads_.emplace_back([&worker]()
            {
                auto cleanup = MakeScopeGuard([]() { Scheduler::thread_worker_ = nullptr; });

                Scheduler::thread_worker_ = worker.get();

                worker->Start();                                    // Start worker loop.
            });
        }
    }

    Scheduler::~Scheduler()
    {
        // Request worker termination and wait for the worker threads to return.

        for (auto&& worker : workers_)
        {
            worker->Stop();
        }

        for (auto&& worker_thread : worker_threads_)
        {
            worker_thread.join();
        }
    }

    TaskExecutionContext& Scheduler::GetExecutionContext()
    {
        auto& worker = Scheduler::thread_worker_ ?
            *Scheduler::thread_worker_ :                            // Local thread worker to improve cache performances.
            *workers_.front();                                      // Random worker to improve load balancing.

        auto execution_context = worker.GetExecutionContext();

        SYNTROPY_ASSERT(execution_context);

        return *execution_context;
    }

    Scheduler& GetScheduler()
    {
        return Scheduler::GetInstance();
    }

}
