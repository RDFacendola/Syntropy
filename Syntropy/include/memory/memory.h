
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

    class MemoryRange;

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
        /// \param base Base address of the range.
        /// \param top First address not in the range.
        /// \param address Address to check.
        /// \return Returns true if the address is contained in the range [base; top), returns false otherwise.
        static constexpr bool IsContained(void* base, void* top, void* address);

        /// \brief Get the size of a memory range.
        /// \param base Base address of the range.
        /// \param top First address not in the range.
        /// \return Returns the size of the range [base; top), in bytes.
        static constexpr size_t GetSize(void* base, void* top);

        /// \brief Round an allocation size up to the next page size.
        /// \param size Size to round up.
        /// \return Returns the size extended such that is a multiple of the page size.
        static size_t CeilToPageSize(size_t size);

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

        /// \brief Get the virtual memory page size.
        /// \return Returns the virtual memory page size, in bytes.
        static size_t GetPageSize();

        /// \brief Reserve and allocate a block of virtual memory.
        /// This method has the same effect as a Reserve() followed by a Commit().
        /// \param size Size of the block of memory to allocate, in bytes.
        /// \return Returns a pointer to the allocated memory block. If the allocation couldn't be fulfilled, returns nullptr.
        static void* Allocate(size_t size);

        /// \brief Free a virtual memory block.
        /// \param address Address of the memory block to release.
        /// \return Returns true if the block could be released, returns false otherwise.
        /// \remarks The provided address must match the address returned by Reserve(), otherwise the behaviour is undefined.
        static bool Free(void* address);

        /// \brief Reserve an aligned block of virtual memory without allocating it.
        /// Use Commit() in order to access a reserved memory block.
        /// \param size Size of the block of memory to reserve, in bytes.
        /// \param alignment Alignment of the block of memory to reserve, in bytes.
        /// \return Returns the base address of the reserved virtual memory range.
        static void* Reserve(size_t size, size_t alignment);

        /// \brief Commit a reserved virtual memory block.
        /// This method allocates all the pages containing at least one byte in the range [address, address + size] and makes them accessible by the application.
        /// \param address Base address of the memory block to commit.
        /// \param size Size of the block to commit, in bytes.
        /// \return Returns true if the memory could be committed, returns false otherwise.
        /// \remarks address and size must refer to a memory region that was previously reserved via Reserve().
        static bool Commit(void* address, size_t size);

        /// \brief Decommit a virtual memory block.
        /// This method decommits all the pages containing at least one byte in the range [address, address + size].
        /// \param address Base address of the memory block to decommit.
        /// \param size Size of the block to decommit, in bytes.
        static bool Decommit(void* address, size_t size);

    private:

        /// \brief Header for a single reserved block.
        struct ReservedBlockHeader
        {
            void* base_address_;        ///< \brief Base address of the reserved block.
            void* block_address_;       ///< \brief Actual aligned address of the block.
            size_t size_;               ///< \brief Size of the memory block, in bytes.
            size_t extended_size_;      ///< \brief Size of the entire reserved memory block, in bytes. Includes both the block, the header and eventual padding.
            size_t alignment_;          ///< \brief Alignment of the memory block.
        };

        /// \brief Get a reserved block header starting from a block address.
        /// \param address Block address.
        /// \param page_size System page size.
        static ReservedBlockHeader* GetReservedBlockHeader(void* address, size_t page_size);

    };

    /// \brief Represents a range of contiguous virtual memory addresses.
    /// \author Raffaele D. Facendola - December 2016
    class MemoryRange
    {
    public:

        /// \brief Create a new virtual memory range.
        /// \param base Base address of the range.
        /// \param capacity Capacity of the range, in bytes.
        MemoryRange(void* base, size_t capacity);

        /// \brief No assignment operator.
        MemoryRange& operator=(const MemoryRange&) = delete;

        /// \brief Dereferencing operator. Access the base address of the range.
        /// \return Returns the base address of the range.
        int8_t* operator*() const;

        /// \brief Get the total capacity of the memory range, in bytes.
        /// \return Returns the total capacity of the memory range, in bytes.
        size_t GetCapacity() const;

        /// \brief Check whether a memory sub-range belongs entirely to this memory range.
        /// \param address Base address of the sub-range to check.
        /// \param size Size of the sub-range, in bytes.
        /// \return Returns true if the sub-range [address, address+size) is contained inside this memory range, return false otherwise.
        bool Contains(void* address, size_t size) const;

        /// \brief Allocate a memory block within this memory range.
        /// This method allocates all the memory pages containing at least one byte in the range [address, address + size].
        /// \param address Address of the memory block to allocate.
        /// \param size Size of the block to allocate, in bytes.
        /// \return Returns true if the memory could be committed, returns false otherwise.
        bool Allocate(void* address, size_t size);

        /// \brief Free a memory block.
        /// This method free all the pages containing at least one byte in the range [address, address + size].
        /// \param address Base address of the memory block to free.
        /// \param size Size of the block to free, in bytes.
        bool Free(void* address, size_t size);

    private:

        int8_t* base_;          ///< \brief First address in the memory range. Owning pointer.

        size_t capacity_;       ///< \brief Capacity of the range, in bytes.

    };

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

    inline constexpr bool Memory::IsContained(void* base, void* top, void* address)
    {
        return reinterpret_cast<uintptr_t>(base) <= reinterpret_cast<uintptr_t>(address) &&
               reinterpret_cast<uintptr_t>(address) < reinterpret_cast<uintptr_t>(top);
    }
    
    inline constexpr size_t Memory::GetSize(void* base, void* top)
    {
        return reinterpret_cast<uintptr_t>(top) - reinterpret_cast<uintptr_t>(base);
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