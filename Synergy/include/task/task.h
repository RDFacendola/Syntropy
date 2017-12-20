
/// \file task.h
/// \brief This header is part of the synergy task system. It contains definitions for tasks and schedulers.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <vector>
#include <memory>
#include <atomic>
#include <cstddef>

#include "diagnostics/assert.h"

namespace syntropy::synergy
{
    class Task;

    /// \brief A list of tasks.
    using TaskList = std::vector<std::shared_ptr<Task>>;

    /// \brief Represents the atomic unit of a parallel computation.
    /// Tasks are expected to perform a small, non-blocking computation.
    /// #TODO Tasks should not share a cache line with other tasks, as it would create false sharing among different worker threads.
    /// \author Raffaele D. Facendola - November 2017
    class Task : public std::enable_shared_from_this<Task>
    {
    public:

        /// \brief Construct the task from a callable object.
        /// \tparam TCallable Type of the callable object to wrap inside the task.
        /// \param dependencies List of tasks the new task depends upon.
        /// \param callable Callable object to wrap inside the task.
        template <typename TCallable>
        void Construct(const TaskList& dependencies, TCallable&& callable)
        {
            executable_ = std::make_unique<Executable<TCallable>>(std::forward<TCallable>(callable));               // #TODO Use proper allocator.

            SetDependencies(dependencies);
        }

        /// \brief Construct the task by creating a callable object in-place.
        /// \tparam TTask Type of the callable object to construct.
        /// \tparam TArguments Types of the arguments to pass to the callable object.
        /// \param dependencies List of tasks the new task depends upon.
        /// \param arguments Arguments to pass to the callable object constructor.
        template <typename TTask, typename... TArguments>
        void Emplace(const TaskList& dependencies, TArguments&&... arguments)
        {
            executable_ = std::make_unique<Executable<TTask>>(std::forward<TArguments>(arguments)...);              // #TODO Use proper allocator.

            SetDependencies(dependencies);
        }

        /// \brief Execute this task.
        void Execute();

        /// \brief Set task dependencies, replacing any existing one.
        /// This method can only be called if this task has no outstanding dependency.
        /// \tparam TDependencies Type of the collection containing the dependencies. Must be an iterable container of Tasks.
        /// \param dependencies Container of dependencies for this task.
        void SetDependencies(const TaskList& dependencies);

        /// \brief Attempt to schedule this task by decreasing its dependency count by one.
        /// \return Returns true if the task can be scheduled, returns false otherwise. This method is guaranteed to return true only to one of any concurrent threads.
        bool ScheduleConditional();

        /// \brief Move successors from this task to another task.
        /// \param task Task to move the successors to.
        void ContinueWith(std::shared_ptr<Task> task);

        /// \brief Move successors from this task to the provided collection.
        /// \param collection Collection to move the successors to.
        void MoveSuccessors(TaskList& successors);

    private:

        /// \brief Interface for executable tasks.
        struct IExecutable
        {
            /// \brief Execute this task.
            virtual void operator()() = 0;
        };

        /// \brief Concrete class for executable tasks.
        template <typename TExecutable>
        struct Executable;

        std::atomic_size_t dependency_count_{ 0 };              ///< \brief Number of tasks this task depends upon, plus one if the task wasn't scheduled yet.

        TaskList successors_;                                   ///< \brief List of tasks depending on this task.

        std::unique_ptr<IExecutable> executable_;               ///< \brief Executable. #TODO Small object optimization.
    };

    /// \brief Concrete class for executable tasks.
    /// Wraps a callable object that can be executed on any thread.
    template <typename TExecutable>
    struct Task::Executable : Task::IExecutable
    {
        /// \brief Create a new concrete task.
        template <typename... TArgs>
        Executable(TArgs&&... arguments)
            : executable_(std::forward<TArgs>(arguments)...)
        {

        }

        /// \brief Execute the task.
        virtual void operator()() override
        {
            executable_();
        }

    private:

        TExecutable executable_;                            ///< \brief Executable object.
    };

}

