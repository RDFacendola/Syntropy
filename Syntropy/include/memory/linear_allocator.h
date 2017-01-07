
/// \file frame_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains linear, frame and stack-based allocators.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <cstdint>
#include <cstddef>

namespace syntropy
{

    /// \brief Used to allocate memory on a pre-allocated contiguous memory block.
    /// Use this allocator to group together allocations having the same lifespan and when high performances are needed and no particular deallocation handling is required.
    /// Memory is allocated upfront to avoid kernel calls while allocating. Pointer-level deallocation is not supported.
    /// \author Raffaele D. Facendola - January 2017
    class LinearAllocator
    {
    public:

        /// \brief Create a new allocator.
        /// \param capacity Amount of memory reserved by the allocator.
        LinearAllocator(size_t capacity);

        /// \brief Default destructor.
        ~LinearAllocator();

        /// \brief Allocate a new memory block.
        /// \param size Size of the memory block to allocate, in bytes.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate(size_t size);

        /// \brief Allocate a new aligned memory block.
        /// \param size Size of the memory block to allocate, in bytes.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate(size_t size, size_t alignment);

        /// \brief Free all the memory blocks allocated so far.
        void Free();

        /// \brief Save the status of the allocator.
        /// To restore it use RestoreStatus().
        void SaveStatus();

        /// \brief Restore the last saved status.
        /// Calling this method multiple times causes older status to be restored.
        /// \return Returns true if a saved status was restored, returns false otherwise.
        bool RestoreStatus();

        /// \brief Get the total amount of memory that was allocated on this allocator.
        /// \return Returns the total amount of memory that was allocated on this allocator, in bytes.
        size_t GetSize() const;

        /// \brief Get the effective memory footprint of this allocator.
        /// \return Returns the amount of memory effectively allocated by this allocator, in bytes.
        size_t GetCapacity() const;

    private:

        int8_t* base_;                              ///< \brief Base pointer to the memory chunk reserved for this allocator.

        int8_t* head_;                              ///< \brief Pointer to the first unallocated memory block.

        size_t* status_;                            ///< \brief Points to the last saved status. Grows backwards from the top of the allocator range.

        size_t capacity_;                           ///< \brief Maximum capacity of the allocator.

    };

    /// \brief Packs a pair of LinearAllocator together such that allocations performed during a frame are available to the next frame as well.
    /// All the actions are performed only of the currently active allocator. After a flip the inactive allocators become active and vice-versa.
    /// \author Raffaele D. Facendola - January 2017
    class DoubleBufferedAllocator
    {
    public:

        /// \brief Create a new allocator.
        /// \param capacity Amount of memory reserved by each allocator.
        DoubleBufferedAllocator(size_t capacity);

        /// \brief Allocate a new memory block on the current allocator.
        /// \param size Size of the memory block to allocate, in bytes.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate(size_t size);

        /// \brief Allocate a new aligned memory block on the current allocator.
        /// \param size Size of the memory block to allocate, in bytes.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate(size_t size, size_t alignment);

        /// \brief Free all the memory blocks allocated so far in the current allocator.
        void Free();

        /// \brief Flips the linear allocators, activating the next one.
        /// This method causes the inactive allocator to become active and vice-versa. The new active allocator is freed.
        void Flip();

        /// \brief Save the status of the current allocator.
        /// To restore the status use RestoreStatus().
        void SaveStatus();

        /// \brief Restore the last saved status of the current allocator.
        /// Calling this method multiple times causes older status to be restored as well.
        /// \return Returns true if a saved status was restored, returns false otherwise.
        bool RestoreStatus();

        /// \brief Get the total amount of memory that was allocated on this allocator.
        /// \return Returns the total amount of memory that was allocated on this allocator, in bytes.
        size_t GetSize() const;

        /// \brief Get the effective memory footprint of this allocator.
        /// \return Returns the amount of memory effectively allocated by this allocator, in bytes.
        size_t GetCapacity() const;

    private:

        LinearAllocator allocators_[2];             ///< \brief Linear allocators pair.

        LinearAllocator* current_;                  ///< \brief Current allocator.

    };

}