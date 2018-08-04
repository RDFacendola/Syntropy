
/// \file block_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains allocators for large chunks of memory that can be used as a base for more specialized allocators.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <limits>

#include "memory/memory.h"
#include "memory/bytes.h"

#include "memory/allocators/linear_allocator.h"

namespace syntropy
{

    /// \brief Block allocator used to allocate fixed-size memory blocks on a contiguous address range.
    /// Both allocations and deallocations are performed on demand: when a memory block is needed it gets mapped to the system memory; when no longer needed the memory is returned to the system.
    /// This allocator ensures that only the memory being currently used is committed but it may suffer from kernel calls overhead.
    /// \author Raffaele D. Facendola - January 2017
    class BlockAllocator
    {
    public:

        /// \brief Default constructor.
        BlockAllocator();

        /// \brief Create a new block allocator.
        /// \param capacity Amount of memory reserved by the allocator.
        /// \param block_size Size of each block, in bytes.
        BlockAllocator(Bytes capacity, Bytes block_size);

        /// \brief Create a new block allocator.
        /// \param memory_range Memory range used by the allocator.
        /// \param block_size Size of each block, in bytes.
        /// \remarks The allocator doesn't take ownership of the memory range provided as input.
        BlockAllocator(const MemoryRange& memory_range, Bytes block_size);

        /// \brief No copy constructor.
        BlockAllocator(const BlockAllocator&) = delete;

        /// \brief Move constructor.
        BlockAllocator(BlockAllocator&& other);

        /// \brief Destructor.
        ~BlockAllocator() = default;

        /// \brief No assignment operator.
        BlockAllocator& operator=(const BlockAllocator&) = delete;

        /// \brief Allocate a memory block.
        /// Allocated memory blocks are guaranteed to be aligned to the allocator's block size.
        /// \param commit_size Amount of memory to commit inside the block. This value is rounded up to the next page size and capped to the block size.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate(Bytes commit_size);

        /// \brief Reserve a memory block.
        /// Reserved memory blocks are guaranteed to be aligned to the allocator's block size.
        /// The block must be committed to the system memory via Memory::Commit(...).
        /// \return Returns a pointer to the reserved memory block.
        void* Reserve();

        /// \brief Free a memory block.
        /// \param block Address of the block to free.
        void Free(void* block);

        /// \brief Get the size of each block.
        /// \return Returns the size of each block in bytes.
        Bytes GetBlockSize() const;

        /// \brief Get the memory range managed by this allocator.
        /// \return Returns the memory range managed by this allocator.
        const MemoryRange& GetRange() const;

    private:

        /// \brief Header for a block whose purpose is to track other free blocks.
        struct FreeBlock
        {
            /// \brief Create a new free block.
            /// \param next Pointer to the next free block.
            /// \param capacity Maximum amount of free blocks that can be referenced by this block.
            FreeBlock(FreeBlock* next, size_t capacity);

            FreeBlock* next_;           ///< \brief Pointer to next chunk.

            size_t count_;               ///< \brief Current amount of free blocks referenced by this block.

            size_t capacity_;           ///< \brief Maximum amount of free blocks that can be referenced by this block.

            uintptr_t base_;            ///< \brief First element in the chunk, other elements are contiguous to this one.

            /// \brief Pop a free block referenced by this block.
            /// Do not call this method if the block is empty.
            /// \return Returns the address of a free block.
            void* PopBlock();

            /// \brief Push a free block inside this block.
            /// Do not call this method if the block is full.
            /// \param block Address of the free block.
            void PushBlock(void* block);
            
            /// \brief Check whether this block is empty.
            bool IsEmpty() const;

            /// \brief Check whether this block is full and cannot reference any other free block.
            bool IsFull() const;

        };

        Bytes block_size_;              ///< \brief Size of each block in bytes.

        LinearAllocator allocator_;     ///< \brief Underlying linear allocator.

        FreeBlock* free_list_;          ///< \brief List of free block addresses. Linked-list of chunks. Each chunk contains many free addresses.

    };

    /// \brief Block allocator used to allocate fixed-size memory blocks on a contiguous address range.
    /// This allocator uses a no-deallocation policy to avoid kernel calls: free blocks are kept allocated and recycled when possible.
    /// Allocations are performed on demand.
    /// \author Raffaele D. Facendola - January 2017
    class StaticBlockAllocator
    {
    public:

        /// \brief Default constructor.
        StaticBlockAllocator();

        /// \brief Create a new monotonic block allocator.
        /// \param capacity Amount of memory reserved by the allocator.
        /// \param block_size Size of each block, in bytes.
        StaticBlockAllocator(Bytes capacity, Bytes block_size);

        /// \brief Create a new monotonic block allocator.
        /// \param memory_range Memory range used by the allocator.
        /// \param block_size Size of each block, in bytes.
        /// \remarks The allocator doesn't take ownership of the memory range provided as input.
        StaticBlockAllocator(const MemoryRange& memory_range, Bytes block_size);

        /// \brief No copy constructor.
        StaticBlockAllocator(const StaticBlockAllocator&) = delete;

        /// \brief Move constructor.
        StaticBlockAllocator(StaticBlockAllocator&& other);

        /// \brief Destructor.
        ~StaticBlockAllocator() = default;

        /// \brief No assignment operator.
        StaticBlockAllocator& operator=(const StaticBlockAllocator&) = delete;

        /// \brief Allocate a memory block.
        /// Allocated memory blocks are guaranteed to be aligned to the allocator's block size.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate();
        
        /// \brief Free a memory block.
        /// \param block Address of the block to free.
        void Free(void* block);

        /// \brief Get the size of each block.
        /// \return Returns the size of each block in bytes.
        Bytes GetBlockSize() const;

        /// \brief Get the memory range managed by this allocator.
        /// \return Returns the memory range managed by this allocator.
        const MemoryRange& GetRange() const;

    private:

        /// \brief Utility structure for free blocks.
        struct Block
        {
            Block* next_;               ///< \brief Pointer to the next free block.
        };

        Bytes block_size_;              ///< \brief Size of each block in bytes.

        LinearAllocator allocator_;     ///< \brief Underlying linear allocator.

        Block* free_list_;              ///< \brief List of free blocks. Linked list.

    };

}