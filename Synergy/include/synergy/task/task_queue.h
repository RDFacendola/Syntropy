
/// \file task_queue.h
/// \brief This header is part of the synergy task system. It contains definitions queues of tasks that can be enqueued and consumed by different threads.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <mutex>
#include <memory>

#include "synergy/task/task.h"

namespace syntropy::synergy
{

    /// \brief Queue of tasks.
    /// Tasks are expected to be pushed to \ popped from the back by a single worker thread. Tasks can be pushed\popped from the front concurrently.
    /// #TODO: Make this queue lock-free.
    /// \author Raffaele D. Facendola - June 2017
    class TaskQueue
    {
    public:

        /// \brief No copy constructor.
        TaskQueue(const TaskQueue&) = delete;

        /// \brief No assignment operator.
        TaskQueue& operator=(const TaskQueue&) = delete;

        /// \brief Create a new task queue.
        /// \param capacity Maximum capacity for the queue.
        TaskQueue(size_t capacity);

        /// \brief Pop an element from the back.
        /// \return Returns the last element from the back. If the queue is empty returns nullptr.
        std::shared_ptr<Task> PopBack();

        /// \brief Push a new element on the head.
        /// If the queue is full, the behavior is undefined.
        /// \param task Task to push.
        void PushBack(std::shared_ptr<Task> task);

        /// \brief Pop an element from the tail.
        /// \return Returns the last element on the tail. If the queue is empty returns nullptr.
        std::shared_ptr<Task> PopFront();

        /// \brief Remove any existing task from the queue.
        void Clear();

    private:

        mutable std::mutex mutex_;                          ///< \brief Used for synchronization purposes.

        std::vector<std::shared_ptr<Task>> tasks_;          ///< \brief List of tasks in the queue. Circular buffer.

        size_t begin_index_ = 0;                            ///< \brief Index of the first element in the range.

        size_t end_index_ = 0;                              ///< \brief One past the index of the last element index.
    };

}