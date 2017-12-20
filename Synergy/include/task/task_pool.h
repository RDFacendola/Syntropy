
/// \file task_pool.h
/// \brief This header is part of the synergy task system. It contains definitions for tasks pools and allocators.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <vector>
#include <memory>

#include "diagnostics/assert.h"

#include "task/task.h"

namespace syntropy::synergy
{

    /// \brief Handles allocation, pooling and construction of tasks.
    /// This pool is not thread-safe.
    /// \author Raffaele D. Facendola - November 2017
    class TaskPool
    {
    public:

        /// \brief Create a new task pool.
        TaskPool() = default;

        /// \brief No copy constructor.
        TaskPool(const TaskPool&) = delete;

        /// \brief No assignment operator.
        TaskPool& operator=(const TaskPool&) = delete;

        /// \brief Construct a task from a callable object.
        /// \param callable Callable object to wrap inside the task.
        /// \param dependencies List of tasks the new task depends upon.
        template <typename TCallable>
        std::shared_ptr<Task> CreateTask(const TaskList& dependencies, TCallable&& callable)
        {
            auto task = std::make_shared<Task>();

            task->Construct(dependencies, std::forward<TCallable>(callable));

            return task;
        }

        /// \brief Construct a task by creating a callable object in-place.
        /// \param arguments Arguments to pass to the task constructor.
        /// \param dependencies List of tasks the new task depends upon.
        template <typename TTask, typename... TArguments>
        std::shared_ptr<Task> EmplaceTask(const TaskList& dependencies, TArguments&&... arguments)
        {
            auto task = std::make_shared<Task>();

            task->Emplace<TTask>(dependencies, std::forward<TArguments>(arguments)...);

            return task;
        }
    };
}
