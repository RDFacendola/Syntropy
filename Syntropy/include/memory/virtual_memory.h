
/// \file virtual_memory.h
/// \brief This header is part of the syntropy memory management system. It contains functionalities used to manage virtual memory.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "memory/bytes.h"
#include "memory/memory.h"

namespace syntropy
{

    /// \brief Wraps the low-level calls used to handle virtual memory allocation.
    /// \author Raffaele D. Facendola - December 2016
    class VirtualMemory
    {
    public:

        /// \brief Get the virtual memory page size.
        /// \return Returns the virtual memory page size, in bytes.
        static Bytes GetPageSize();

        /// \brief Reserve a range of virtual memory addresses.
        /// Reserved memory pages must be committed via Commit() before accessing them.
        /// \param size Size of the range to reserve, in bytes.
        /// \return Returns the first address in the reserved range. If the method fails, returns nullptr.
        /// \remark The reserved memory is guaranteed to be aligned to virtual memory page boundary.
        static void* Reserve(Bytes size);

        /// \brief Allocate a range of virtual memory addresses.
        /// This method has the same effect as a Reserve() followed by a Commit().
        /// \param size Size of the range to reserve, in bytes.
        /// \return Returns the first address in the allocated range. If the method fails, returns nullptr.
        /// \remark The allocated memory is guaranteed to be aligned to virtual memory page boundary.
        static void* Allocate(Bytes size);

        /// \brief Release a range of virtual memory addresses.
        /// \param address First address of the range to release. Must match any return value of a previous Reserve() / Allocate(), otherwise the behaviour is unspecified.
        /// \return Returns true if the range could be released, returns false otherwise.
        static bool Release(void* address);

        /// \brief Commit a reserved virtual memory block.
        /// This method allocates all the pages containing at least one byte in the range [address, address + size] and makes them accessible by the application.
        /// \param address Base address of the memory block to commit.
        /// \param size Size of the block to commit, in bytes.
        /// \return Returns true if the memory could be committed, returns false otherwise.
        /// \remarks address and size must refer to a memory region that was previously reserved via Reserve().
        static bool Commit(void* address, Bytes size);

        /// \brief Decommit a virtual memory block.
        /// This method decommits all the pages containing at least one byte in the range [address, address + size].
        /// \param address Base address of the memory block to decommit.
        /// \param size Size of the block to decommit, in bytes.
        static bool Decommit(void* address, Bytes size);

        /// \brief Round a size up to the next virtual page size.
        /// \param size Size to round up.
        /// \return Returns the size extended such that is a multiple of the page size.
        static Bytes CeilToPageSize(Bytes size);

    };

    /// \brief Represents a pool of contiguous virtual addresses.
    /// The pool reserves a range of virtual memory. Actual allocation\deallocation must be performed manually.
    /// \author Raffaele D. Facendola - March 2017
    class MemoryPool
    {
    public:

        /// \brief Create a new empty pool.
        MemoryPool();

        /// \brief Create a new pool.
        /// \param size Size of the pool, in bytes.
        MemoryPool(Bytes size);

        /// \brief Create a new aligned pool
        /// \param size Size of the pool, in bytes.
        /// \param alignment Alignment of the pool, in bytes.
        MemoryPool(Bytes size, Bytes alignment);

        /// \brief No copy ctor.
        MemoryPool(const MemoryPool&) = delete;

        /// \brief Move ctor.
        MemoryPool(MemoryPool&& other);

        /// \brief Default destructor.
        ~MemoryPool();

        /// \brief No assignment operator.
        MemoryPool& operator=(const MemoryPool&) = delete;

        /// \brief Dereferencing operator. Access the base address of the pool.
        /// \return Returns the base address of the pool.
        void* operator*() const;

        /// \brief Access an element in the pool.
        /// \param offset Offset with respect to the first element of the pool.
        /// \return Returns a pointer to the element (base+offset).
        void* operator[](Bytes offset) const;

        /// \brief Get the size of the pool, in bytes.
        /// \return Returns the size of the pool, in bytes.
        Bytes GetSize() const;

        /// \brief Get the pool's memory range.
        operator MemoryRange() const;

    private:

        void* pool_;                ///< \brief Pointer to the virtual memory buffer.

        MemoryRange range_;         ///< \brief Memory range. Accounts for any required alignment.

    };

}

