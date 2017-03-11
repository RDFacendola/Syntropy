
/// \file block_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains allocators for large chunks of memory that can be used as a base for more specialized ones.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "memory.h"
#include "linear_allocator.h"
#include "diagnostics/debug.h"
#include "math/math.h"

namespace syntropy
{

    /// \brief Block allocator used to allocate fixed-size memory blocks on a contiguous address range.
    /// Both allocations and deallocations are performed on demand: when a memory block is needed it gets mapped to the system memory; when no longer needed the memory is returned to the system.
    /// This allocator ensures that only the memory being currently used is allocated but may suffer from kernel calls overhead. If a smaller, more performance-aware, allocator is needed check MonotonicBlockAllocator.
    /// \author Raffaele D. Facendola - January 2017
    class BlockAllocator
    {
    public:

        /// \brief Create a new block allocator.
        /// \param capacity Amount of memory reserved by the allocator.
        /// \param block_size Size of each block, in bytes.
        BlockAllocator(size_t capacity, size_t block_size);

        /// \brief Create a new block allocator.
        /// \param memory_range Memory range used by the allocator.
        /// \param block_size Size of each block, in bytes.
        /// \remarks The allocator doesn't take ownership of the memory range provided as input.
        BlockAllocator(const MemoryRange& memory_range, size_t block_size);

        /// \brief No copy constructor.
        BlockAllocator(const BlockAllocator&) = delete;

        /// \brief Destructor.
        ~BlockAllocator() = default;

        /// \brief No assignment operator.
        BlockAllocator& operator=(const BlockAllocator&) = delete;

        /// \brief Allocate a memory block.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate();

        /// \brief Allocate a memory block of a specific size.
        /// \param size Size of the block to allocate, in bytes. Must be equal or smaller than the block size.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate(size_t size);

        /// \brief Reserve a memory block of a specific size.
        /// The reserved block doesn't get physically allocated.
        /// \param size Size of the block to reserve, in bytes. Must be equal or smaller than the block size.
        /// \return Returns a pointer to the reserved memory block.
        void* Reserve(size_t size);

        /// \brief Free a memory block.
        /// \param block Address of the block to free.
        void Free(void* block);

        /// \brief Get the size of each block.
        /// \return Returns the size of each block in bytes.
        size_t GetBlockSize() const;

        /// \brief Get the maximum amount of memory that can be allocated by this allocator, in bytes.
        /// \return Returns the maximum amount of memory that can be allocated by this allocator, in bytes.
        size_t GetCapacity() const;

        /// \brief Check whether an address belongs to this allocator.
        /// \param address Address to check.
        /// \return Returns true if address belongs to this allocator, returns false otherwise.
        /// \remarks The address doesn't have to be physically allocated or reserved.
        bool ContainsAddress(void* address) const;

    private:

        size_t block_size_;                         ///< \brief Size of each block in bytes.

        MemoryPool memory_pool_;                    ///< \brief Virtual memory range owned by this allocator. Empty if the allocator owns no virtual memory.

        MemoryRange memory_range_;                  ///< \brief Memory range managed by the allocator. May refer to memory_pool_ or to a range owned by someone else.

        void* head_;                                ///< \brief Pointer to the first unmapped block.

        VectorAllocator<uintptr_t> free_list_;      ///< \brief Stack of free blocks.

    };

    /// \brief Block allocator used to allocate fixed-size memory blocks on a contiguous address range.
    /// This allocator uses a no-deallocation policy to avoid kernel calls: free blocks are kept allocated and recycled when possible.
    /// Allocations are performed on demand.
    /// \author Raffaele D. Facendola - January 2017
    class MonotonicBlockAllocator
    {
    public:

        /// \brief Create a new monotonic block allocator.
        /// \param capacity Amount of memory reserved by the allocator.
        /// \param block_size Size of each block, in bytes.
        MonotonicBlockAllocator(size_t capacity, size_t block_size);

        /// \brief Create a new monotonic block allocator.
        /// \param memory_range Memory range used by the allocator.
        /// \param block_size Size of each block, in bytes.
        /// \remarks The allocator doesn't take ownership of the memory range provided as input.
        MonotonicBlockAllocator(const MemoryRange& memory_range, size_t block_size);

        /// \brief No copy constructor.
        MonotonicBlockAllocator(const MonotonicBlockAllocator&) = delete;

        /// \brief Destructor.
        ~MonotonicBlockAllocator() = default;

        /// \brief No assignment operator.
        MonotonicBlockAllocator& operator=(const MonotonicBlockAllocator&) = delete;

        /// \brief Allocate a memory block.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate();
        
        /// \brief Free a memory block.
        /// \param block Address of the block to free.
        void Free(void* block);

        /// \brief Get the size of each block.
        /// \return Returns the size of each block in bytes.
        size_t GetBlockSize() const;

        /// \brief Get the current allocation size, in bytes.
        /// \return Returns the total amount of allocations performed so far by this allocator, in bytes.
        size_t GetAllocationSize() const;

        /// \brief Get the amount of system memory committed by the allocator, in bytes.
        /// Note that the stack allocator allocates all the memory it needs upfront.
        /// \return Returns the amount of system memory committed by the allocator, in bytes.
        size_t GetCommitSize() const;

        /// \brief Get the memory range managed by this allocator.
        /// \return Returns the memory range managed by this allocator.
        const MemoryRange& GetRange() const;

    private:

        /// \brief Utility structure for free blocks.
        struct Block
        {
            Block* next_;               ///< \brief Pointer to the next free block.
        };

        size_t block_size_;             ///< \brief Size of each block in bytes.

        LinearAllocator allocator_;     ///< \brief Underlying linear allocator.

        Block* free_;                   ///< \brief First free block.

    };

}