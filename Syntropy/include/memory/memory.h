
/// \file memory.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <cstdint>

#include "diagnostics/diagnostics.h"

namespace syntropy
{

    /// \brief Log context for the syntropy memory system.
    extern const diagnostics::Context MemoryCtx;

    /// \brief Wraps the low-level calls used to handle virtual memory allocation.
    /// \author Raffaele D. Facendola - December 2016
    class Memory
    {
    public:

        /// \brief Get the singleton instance.
        /// \return Returns the singleton instance;
        static Memory& GetInstance();

        /// \brief Offset an address.
        /// \param address Address to perform the offset from.
        /// \param offset Offset with respect the address, in bytes.
        /// \return Returns the address offset'd by the specified offset amount.
        template <typename T>
        static constexpr T* Offset(T* address, int64_t offset);

        /// \brief Get the virtual memory reservation granularity.
        /// This value is always a multiple of the allocation granularity.
        /// \return Returns the virtual memory reservation granularity, in bytes.
        virtual size_t GetReservationGranularity() const = 0;

        /// \brief Get the virtual memory allocation granularity.
        /// \return Returns the virtual memory allocation granularity, in bytes.
        virtual size_t GetAllocationGranularity() const = 0;

        /// \brief Reserve and allocate a block of virtual memory.
        /// This method has the same effect as a Reserve() followed by a Commit().
        /// \param size Size of the block of memory to allocate, in bytes.
        /// \return Returns a pointer to the allocated memory block. If the allocation couldn't be fulfilled, returns nullptr.
        /// \remarks The returned pointer is guaranteed to be aligned to the reservation granularity and span a region which is at least as big as requested.
        virtual void* Allocate(size_t size) = 0;

        /// \brief Free a virtual memory block.
        /// \param address Address of the memory block to release.
        /// \return Returns true if the block could be released, returns false otherwise.
        /// \remarks The provided address must match the address returned by Reserve(), otherwise the behaviour is undefined.
        virtual bool Free(void* address) = 0;

        /// \brief Reserve a block of virtual memory without allocating it.
        /// Use Commit() in order to access a reserved memory block.
        /// \param size Size of the block of memory to reserve, in bytes.
        /// \return Returns a pointer to the reserved memory block. If the reservation couldn't be fulfilled, returns nullptr.
        /// \remarks The returned pointer is guaranteed to be aligned to the reservation granularity and span a region which is at least as big as requested.
        virtual void* Reserve(size_t size) = 0;

        /// \brief Commit a reserved memory block.
        /// This method allocates all the pages containing at least one byte in the range [address, address + size] and makes them accessible by the application.
        /// \param address Base address of the memory block to commit.
        /// \param size Size of the block to commit, in bytes.
        /// \return Returns true if the memory could be committed, returns false otherwise.
        /// \remarks address and size must refer to a memory region that was previously reserved via Reserve().
        virtual bool Commit(void* address, size_t size) = 0;

        /// \brief Decommit a memory block.
        /// This method decommits all the pages containing at least one byte in the range [address, address + size].
        /// \param address Base address of the memory block to decommit.
        /// \param size Size of the block to decommit, in bytes.
        virtual bool Decommit(void* address, size_t size) = 0;

    };

    /// \brief Get the current system instance.
    /// \brief Returns the current system instance.
    Memory& GetMemory();

}

namespace syntropy
{
    // Implementation

    //////////////// MEMORY ////////////////

    template <typename T>
    static constexpr T* Memory::Offset(T* address, int64_t offset)
    {
        return reinterpret_cast<T*>(reinterpret_cast<int8_t*>(address) + offset);
    }

}