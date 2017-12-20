#include "task/task_queue.h"

#include <algorithm>

#include "diagnostics/assert.h"

namespace syntropy::synergy
{

    /************************************************************************/
    /* TASK QUEUE                                                           */
    /************************************************************************/

    TaskQueue::TaskQueue(size_t capacity)
        : tasks_(capacity)
    {

    }

    std::shared_ptr<Task> TaskQueue::PopBack()
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (begin_index_ != end_index_)
        {
            end_index_ = (end_index_ > 0) ? end_index_ - 1 : tasks_.size() - 1;         // Decrement and wrap around.

            auto task = tasks_[end_index_];

            tasks_[end_index_] = nullptr;

            return task;
        }

        return nullptr;
    }

    void TaskQueue::PushBack(std::shared_ptr<Task> task)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        tasks_[end_index_] = task;

        end_index_ = (end_index_ + 1) % tasks_.size();                                  // Increment and wrap around.

        SYNTROPY_ASSERT(begin_index_ != end_index_);                                    // Ensure the queue is not full.
    }

    std::shared_ptr<Task> TaskQueue::PopFront()
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (begin_index_ != end_index_)
        {
            auto task = tasks_[begin_index_];

            tasks_[begin_index_] = nullptr;

            begin_index_ = (begin_index_ + 1) % tasks_.size();                          // Increment and wrap around.

            return task;
        }

        return nullptr;
    }

    void TaskQueue::Clear()
    {
        std::lock_guard<std::mutex> lock(mutex_);

        while (begin_index_ != end_index_)
        {
            tasks_[begin_index_] = nullptr;

            begin_index_ = (begin_index_ + 1) % tasks_.size();
        }
    }
}
