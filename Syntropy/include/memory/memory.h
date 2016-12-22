
/// \file memory.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <cstdint>

namespace syntropy
{

    /// \brief A pool of memory addresses.
    /// The pool reserves a range of virtual memory address but allocates only when needed.
    /// This class is not thread-safe!
    /// \author Raffaele D. Facendola - December 2016
    class MemoryAddressPool
    {
    public:

        /// \brief Create a new address pool.
        /// \param count Maximum number of addresses to memorize.
        MemoryAddressPool(size_t count);

        /// \brief Insert an element inside the pool.
        /// The element can be extracted at any time via Extract().
        /// \param address Address to acquire.
        void Push(void* address);

        /// \brief Extract an element from the pool.
        /// Do not call this method if IsEmpty() is true.
        /// \return Returns any extracted element from the pool.
        void* Pop();

        /// \brief Check whether the pool is empty.
        /// \return Returns true if the pool is empty, returns false otherwise.
        bool IsEmpty() const;

    private:

        /// \brief Convert an address count to a memory size.
        size_t ToMemorySize(size_t count) const;

        /// \brief Convert a memory size to an address count.
        size_t ToAddressCount(size_t size) const;

        void* StorageOffset(size_t offset) const;

        void* storage_;                             ///< \brief Pointer to the storage.

        size_t size_;                               ///< \brief Current size of the pool.

        size_t capacity_;                           ///< \brief Current allocated capacity of the pool.

    };

    /// \brief Wraps the low-level calls used to handle virtual memory allocation.
    /// \author Raffaele D. Facendola - December 2016
    class Memory
    {
    public:

        /// \brief Get the singleton instance.
        /// \return Returns the singleton instance;
        static Memory& GetInstance();

        /// \brief Get the virtual memory allocation granularity.
        /// \return Returns the virtual memory allocation granularity, in bytes.
        virtual size_t GetAllocationGranularity() const = 0;

        /// \brief Reserve a block of virtual memory without allocating it.
        /// \param size Size of the block of memory to reserve, in bytes. Must be a multiple of the allocation granularity.
        /// \return Returns a pointer to the reserved memory block. If the reservation couldn't be fulfilled, returns nullptr.
        /// \remarks The returned pointer is guaranteed to be aligned to the allocation granularity.
        virtual void* Reserve(size_t size) = 0;

        /// \brief Free a previously reserved  block of virtual memory.
        /// The content of the reserved block is deallocated.
        /// \param address Address of the memory block to free.
        /// \return Returns true if the block could be freed, returns false otherwise.
        /// \remarks The provided address must match the address returned by Reserve().
        virtual bool Free(void* address) = 0;

        /// \brief Allocate a memory block.
        /// \param address Base address of the memory block to allocate. Must be aligned to the allocation granularity.
        /// \param size Size of the block to allocate, in bytes. Must be a multiple of the allocation granularity.
        /// \return Returns true if the allocation could be performed, returns false otherwise.
        /// \remarks address and size must refer to a memory region that was previously reserved via Reserve().
        virtual bool Allocate(void* address, size_t size) = 0;
        
        /// \brief Deallocate a memory block.
        /// \param address Base address of the memory block to deallocate. Must be aligned to the allocation granularity.
        /// \param size Size of the block to deallocate, in bytes. Must be a multiple of the allocation granularity.
        virtual bool Deallocate(void* address, size_t size) = 0;

    };

    /// \brief Get the current system instance.
    /// \brief Returns the current system instance.
    Memory& GetMemory();

    /// \brief Get the size of a pointer in bytes.
    /// \return Returns the size of a pointer in bytes.
    constexpr size_t GetPointerSize();

}
