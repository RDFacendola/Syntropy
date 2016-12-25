
/// \file memory.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <cstdint>

namespace syntropy
{

    /// \brief Wraps the low-level calls used to handle virtual memory allocation.
    /// \author Raffaele D. Facendola - December 2016
    class Memory
    {
    public:

        /// \brief Get the singleton instance.
        /// \return Returns the singleton instance;
        static Memory& GetInstance();

        /// \brief Get the size of a pointer in bytes.
        /// \return Returns the size of a pointer in bytes.
        static constexpr size_t GetPointerSize();

        /// \brief Check whether an address is aligned to a given boundary.
        /// \param address Address to check the alignment of.
        /// \param alignment Size of the alignment.
        /// \return Returns true if the address is aligned to alignment, returns false otherwise.
        static constexpr bool IsAlignedTo(void* address, size_t alignment);

        /// \brief Get the memory footprint of a number of elements.
        /// \tparam Type of the elements.
        /// \param count Number of elements.
        /// \return Returns the memory footprint of count elements of type T, in bytes.
        template <typename T>
        static constexpr size_t GetElementSize(size_t count);

        /// \brief Get the number of elements that can be stored in a given amount of memory.
        /// \tparam Type of the elements.
        /// \param size Memory size.
        /// \return Returns the number of elements of type T that can be stored in a memory block of size size.
        template <typename T>
        static constexpr size_t GetElementCount(size_t size);

        /// \brief Get the virtual memory reservation granularity.
        /// \return Returns the virtual memory reservation granularity, in bytes.
        virtual size_t GetReservationGranularity() const = 0;

        /// \brief Get the virtual memory allocation granularity.
        /// \return Returns the virtual memory allocation granularity, in bytes.
        virtual size_t GetAllocationGranularity() const = 0;

        /// \brief Round up a memory size to the next page boundary.
        /// \return Returns the memory size rounded to the next page boundary.
        size_t Ceil(size_t size) const;

        /// \brief Reserve a block of virtual memory without allocating it.
        /// \param size Size of the block of memory to reserve, in bytes.
        /// \return Returns a pointer to the reserved memory block. If the reservation couldn't be fulfilled, returns nullptr.
        /// \remarks The returned pointer is guaranteed to be aligned to the reservation granularity and span a region which is at least as big as requested.
        virtual void* Reserve(size_t size) = 0;

        /// \brief Release a previously reserved  block of virtual memory.
        /// The content of the reserved block is deallocated first.
        /// \param address Address of the memory block to free.
        /// \return Returns true if the block could be freed, returns false otherwise.
        /// \remarks The provided address must match the address returned by Reserve(), otherwise the behaviour is undefined.
        virtual bool Release(void* address) = 0;

        /// \brief Allocate a memory block.
        /// This method allocates all the pages containing at least one byte in the range [address, address + size].
        /// \param address Base address of the memory block to allocate.
        /// \param size Size of the block to allocate, in bytes.
        /// \return Returns true if the allocation could be performed, returns false otherwise.
        /// \remarks address and size must refer to a memory region that was previously reserved via Reserve().
        virtual bool Allocate(void* address, size_t size) = 0;

        /// \brief Deallocate a memory block.
        /// This method deallocates all the pages containing at least one byte in the range [address, address + size].
        /// \param address Base address of the memory block to deallocate. Rounded down to the nearest multiple of the allocation granularity.
        /// \param size Size of the block to deallocate, in bytes. Rounded up to the nearest multiple the allocation granularity.
        virtual bool Deallocate(void* address, size_t size) = 0;

    };

    /// \brief Get the current system instance.
    /// \brief Returns the current system instance.
    Memory& GetMemory();

}

namespace syntropy
{
    // Implementation

    //////////////// MEMORY ////////////////

    inline constexpr size_t Memory::GetPointerSize()
    {
        return sizeof(void*);
    }

    inline constexpr bool Memory::IsAlignedTo(void* address, size_t alignment)
    {
        return reinterpret_cast<size_t>(address) % alignment == 0;
    }

    template <typename T>
    inline constexpr size_t Memory::GetElementSize(size_t count)
    {
        return sizeof(T) * count;
    }

    template <typename T>
    inline constexpr size_t Memory::GetElementCount(size_t size)
    {
        return size / sizeof(T);
    }

}