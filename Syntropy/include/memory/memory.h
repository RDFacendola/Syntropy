
/// \file memory.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <cstdint>

#include "diagnostics/diagnostics.h"
#include "math/math.h"

namespace syntropy
{

    /// \brief Log context for the syntropy memory system.
    extern const diagnostics::Context MemoryCtx;

    /// \brief Represents a reserved range of virtual memory addresses.
    /// Address ranges are meant to represent the memory managed by global allocators, therefore, once reserved, they cannot be freed.
    /// Virtual memory ranges can be copied and moved around but they will always refer to the same reserved memory.
    /// \author Raffaele D. Facendola - December 2016
    class VirtualMemoryRange
    {
    public:

        /// \brief Create a new virtual memory range.
        /// \param base First address in the range.
        /// \param end First address *not* in the range.
        VirtualMemoryRange(void* base, void* end);

        /// \brief Dereferencing operator. Access the base address of the range.
        /// \return Returns the base address of the range.
        int8_t* operator*() const;

        /// \brief Get the total capacity of the memory range, in bytes.
        /// \return Returns the total capacity of the memory range, in bytes.
        size_t GetCapacity() const;

        /// \brief Allocate a memory block within this memory range.
        /// This method allocates all the pages containing at least one byte in the range [address, address + size]. See Memory::GetAllocationGranularity()
        /// \param address Address of the memory block to allocate.
        /// \param size Size of the block to allocate, in bytes.
        /// \return Returns true if the memory could be committed, returns false otherwise.
        bool Allocate(void* address, size_t size);

        /// \brief Free a memory block.
        /// This method free all the pages containing at least one byte in the range [address, address + size].
        /// \param address Base address of the memory block to free.
        /// \param size Size of the block to free, in bytes.
        bool Free(void* address, size_t size);

        /// \brief Free the entire memory range, undoing any allocation performed so far.
        /// The virtual memory range is still reserved after this call.
        void Free();

    private:

        int8_t* base_;          ///< \brief First address in the memory range

        int8_t* end_;           ///< \brief One past the last address in the memory range.
    };

    /// \brief Wraps the low-level calls used to handle virtual memory allocation.
    /// \author Raffaele D. Facendola - December 2016
    class Memory
    {
    public:

        // Memory manipulation

        /// \brief Offset an address.
        /// \param address Address to perform the offset from.
        /// \param offset Offset with respect the address, in bytes.
        /// \return Returns the address offsetted by the specified amount.
        template <typename T>
        static constexpr T* Offset(T* address, int64_t offset);

        /// \brief Check whether an address is contained in a memory range.
        /// \param begin First address of the range.
        /// \param end One past the last address of the range.
        /// \param address Address to check.
        /// \return Returns true if the address is contained in the range [begin; end) (begin included, end excluded)
        static constexpr bool IsContained(void* begin, void* end, void* address);

        /// \brief Get the size of a memory range in bytes.
        /// \param begin First address of the range.
        /// \param end One past the alst address of the range.
        /// \return Returns the amount of bytes between the two addresses in bytes.
        static constexpr size_t GetRangeSize(void* begin, void* end);

        /// \brief Round an allocation size to the next allocation granularity boundary.
        /// \param size Size to round up.
        /// \return Returns the size extended such that is a multiple of the allocation granularity.
        static size_t CeilToAllocationGranularity(size_t size);

        // Memory alignment

        /// \brief Align an address.
        /// \param address Address to align
        /// \param alignment Alignment.
        /// \return Returns the address aligned to the given alignment boundary.
        /// \remarks This method aligns by adding a padding value to the base address.
        template <typename T>
        static constexpr T* Align(T* address, size_t alignment);

        /// \brief Align an address.
        /// \param address Address to align
        /// \param alignment Alignment.
        /// \return Returns the address aligned down to the given alignment boundary.
        /// \remarks This method aligns by subtracting a padding value to the base address.
        template <typename T>
        static constexpr T* AlignDown(T* address, size_t alignment);

        /// \brief Check whether an address is aligned to a particular size.
        /// \param address Address to check.
        /// \param alignment Alignment to test against.
        /// \return Returns true if address is aligned to the provided alignment, returns false otherwise.
        static constexpr bool IsAlignedTo(void* address, size_t alignment);

        /// \brief Get the singleton instance.
        /// \return Returns the singleton instance;
        static Memory& GetInstance();

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
        /// \param align Alignment of the reserved memory range.
        /// \return Returns a the reserved virtual memory range.
        virtual VirtualMemoryRange Reserve(size_t size, size_t alignment) = 0;

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
    constexpr T* Memory::Offset(T* address, int64_t offset)
    {
        return reinterpret_cast<T*>(reinterpret_cast<int8_t*>(address) + offset);
    }

    inline constexpr bool Memory::IsContained(void* begin, void* end, void* address)
    {
        return reinterpret_cast<uintptr_t>(begin) <= reinterpret_cast<uintptr_t>(address) &&
               reinterpret_cast<uintptr_t>(address) < reinterpret_cast<uintptr_t>(end);
    }
    
    inline constexpr size_t Memory::GetRangeSize(void* begin, void* end)
    {
        return reinterpret_cast<uintptr_t>(end) - reinterpret_cast<uintptr_t>(begin);
    }

    template <typename T>
    constexpr T* Memory::Align(T* address, size_t alignment)
    {
        return reinterpret_cast<T*>(Math::Ceil(reinterpret_cast<uintptr_t>(address), alignment));
    }

    template <typename T>
    static constexpr T* Memory::AlignDown(T* address, size_t alignment)
    {
        return reinterpret_cast<T*>(Math::Floor(reinterpret_cast<uintptr_t>(address), alignment));
    }

    constexpr bool Memory::IsAlignedTo(void* address, size_t alignment)
    {
        return reinterpret_cast<uintptr_t>(address) % alignment == 0;
    }

}