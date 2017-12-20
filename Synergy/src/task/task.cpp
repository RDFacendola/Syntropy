#include "task/task.h"

#include <algorithm>

#include "task/scheduler.h"
#include "task/worker.h"

namespace syntropy::synergy
{
    /************************************************************************/
    /* TASK                                                                 */
    /************************************************************************/

    void Task::Execute()
    {
        if (executable_)
        {
            (*executable_)();
        }
    }

    void Task::SetDependencies(const TaskList& dependencies)
    {
        SYNTROPY_ASSERT(dependency_count_.load(std::memory_order_acquire) == 0);

        auto shared_this = shared_from_this();

        dependency_count_.store(dependencies.size() + 1, std::memory_order_release);        // Additional dependency needed to schedule the task manually after this call.

        for (auto&& dependency : dependencies)
        {
            dependency->successors_.emplace_back(shared_this);
        }
    }

    bool Task::ScheduleConditional()
    {
        auto previous = dependency_count_.fetch_sub(1, std::memory_order_relaxed);

        SYNTROPY_ASSERT(previous >= 1);                                                     // Be sure this task was not "over-scheduled".

        return previous == 1;
    }

    void Task::ContinueWith(std::shared_ptr<Task> task)
    {
        if (task.get() != this)
        {
            MoveSuccessors(task->successors_);
        }
    }

    void Task::MoveSuccessors(TaskList& successors)
    {
        if (successors.size() == 0)
        {
            std::swap(successors_, successors);
        }
        else
        {
            successors.reserve(successors.size() + successors_.size());

            std::copy(std::begin(successors_), std::end(successors_), std::back_inserter(successors));
        }

        successors_.clear();
    }


}
