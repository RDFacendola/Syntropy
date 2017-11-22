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

    bool Task::ScheduleConditional()
    {
        auto previous = dependency_count_.fetch_sub(1, std::memory_order_relaxed);

        SYNTROPY_ASSERT(previous >= 1);         // Be sure this task was not "over-scheduled".

        return previous == 1;
    }

    void Task::ContinueWith(std::shared_ptr<Task> task)
    {
        if (task.get() != this)
        {
            MoveSuccessors(task->successors_);
        }
    }

    void Task::MoveSuccessors(std::vector<std::shared_ptr<Task>>& successors)
    {
        if (successors.size() == 0)
        {
            std::swap(successors_, successors);
        }
        else
        {
            successors.reserve(successors.size() + successors_.size());

            std::copy(std::begin(successors_), std::end(successors_), std::back_inserter(successors));

            successors_.clear();
        }
    }


}
