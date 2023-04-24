#include "synergy/task/scheduler.h"

#include "synergy/patterns/sync_counter.h"

#include "syntropy/platform/threading.h"
#include "syntropy/diagnostics/foundation/assert.h"

#include "syntropy/language/support/scope_guard.h"

#include <thread>
#include <atomic>

namespace Syntropy::synergy
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

    void Scheduler::Initialize(std::optional<platform::AffinityMask> cores)
    {
        auto affinity_mask = cores ? *cores : platform::AffinityMask(0).flip();                 // Either use the specified affinity mask or attempt to use each available core.

        affinity_mask &= platform::Threading::GetProcessAffinity();                             // Discard any core that has no affinity with the current process.

        SYNTROPY_ASSERT(affinity_mask.any());                                                   // Be sure to spawn at least one worker!

        // Start the worker threads, one for each specified core.

        size_t worker_count = affinity_mask.count();

        worker_thread_sync_.Reset(worker_count);

        workers_.reserve(worker_count);

        for (size_t core_index = 0; workers_.size() < worker_count;)
        {
            while (!affinity_mask.test(core_index++));                                          // Advance to the next designated core.

            workers_.emplace_back().StartAsync(platform::AffinityMask().set(core_index - 1));   // #TODO Maximum tasks amount should be unbounded.
        }

        // Wait until each worker thread is ready to run. Without this, external callers would attempt to spawn tasks on workers that may not have had the opportunity to be initialized.

        worker_thread_sync_.Wait();
    }

    void Scheduler::OnTaskEnqueued(Worker& sender)
    {
        // #TODO Make this lock-free.

        // Attempt to yield a task to a random starving worker.

        std::scoped_lock<std::mutex> lock(mutex_);

        if (starving_workers_.empty())
        {
            return;
        }

        auto starving_worker = starving_workers_.back();

        if (auto task = sender.DequeueTask())
        {
            starving_worker->EnqueueTask(task);
            starving_workers_.pop_back();
        }
    }

    void Scheduler::OnWorkerStarving(Worker& sender)
    {
        // #TODO Make this lock-free.

        // Attempt to steal a task from a random non-starving worker.

        std::scoped_lock<std::mutex> lock(mutex_);

        for (auto&& worker : workers_)
        {
            if (auto task = worker.GetWorker().DequeueTask())
            {
                sender.EnqueueTask(task);
                return;
            }
        }

        starving_workers_.emplace_back(&sender);
    }

    void Scheduler::OnWorkerReady(Worker& /*sender*/)
    {
        worker_thread_sync_.Signal();                               // Decrement the counter and block until each other worker is ready to run.
    }

    TaskExecutionContext& Scheduler::GetExecutionContext()
    {
        if (Scheduler::thread_worker_)
        {
            // Use the local thread worker to improve cache performance.

            return *Scheduler::thread_worker_->GetExecutionContext();
        }
        else
        {
            // Pick a random worker to improve load balancing.

            auto worker_it = random_.Pick(std::begin(workers_), std::end(workers_));

            return *worker_it->GetWorker().GetExecutionContext();
        }
    }

    Scheduler& GetScheduler()
    {
        return Scheduler::GetInstance();
    }

    /************************************************************************/
    /* SCHEDULER :: WORKER THREAD                                           */
    /************************************************************************/

    Scheduler::WorkerThread::WorkerThread(size_t max_tasks)
        : worker_(std::make_unique<Worker>(max_tasks))
    {

    }

    Scheduler::WorkerThread::~WorkerThread()
    {
        if (worker_->IsRunning())
        {
            worker_->Stop();
            thread_.join();
        }
    }

    void Scheduler::WorkerThread::StartAsync(std::optional<platform::AffinityMask> affinity)
    {
        SYNTROPY_ASSERT(!worker_->IsRunning());

        thread_ = std::thread([this, affinity]()
        {
            // Set thread affinity. Note that this call may silently fail if the specified affinity is not compatible with the process affinity.

            if (affinity)
            {
                platform::Threading::SetThreadAffinity(*affinity);
            }

            // Setup worker events.

            auto on_enqueued_handle = worker_->OnTaskEnqueued().Subscribe([](auto&& sender)
            {
                GetScheduler().OnTaskEnqueued(sender);
            });

            auto on_starving_handle = worker_->OnStarving().Subscribe([](auto&& sender)
            {
                GetScheduler().OnWorkerStarving(sender);
            });

            auto on_ready_handle = worker_->OnReady().Subscribe([this](auto&& sender)
            {
                GetScheduler().OnWorkerReady(sender);
            });

            // Start worker loop synchronously.

            auto cleanup = MakeScopeGuard([]() { Scheduler::thread_worker_ = nullptr; });

            Scheduler::thread_worker_ = worker_.get();

            worker_->Start();
        });
    }

    Worker& Scheduler::WorkerThread::GetWorker()
    {
        return *worker_;
    }

}
