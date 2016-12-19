
/// \file memory.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <cstdint>

namespace syntropy
{

    /// \brief Represents a range of memory addresses.
    /// \author Raffaele D. Facendola - December 2016
    template <typename TTag>
    class MemoryRange
    {
    public:

        /// \brief Create a new memory range.
        /// \param base_address Base address of the range.
        /// \param count Number of addresses inside the range.
        MemoryRange(void* base_address, size_t count);

        /// \brief Get the amount of memory addresses stored inside this range.
        size_t GetCount() const;

        /// \brief Get the base address of the range.
        /// \return Returns the base address of the range.
        void* GetBaseAddress() const;

        /// \brief Get a typed pointer inside the memory block.
        /// \param offset Offset, in sizeof(TPtr) bytes.
        template <typename TPtr>
        TPtr* GetPointer(size_t offset = 0) const;

        /// \brief Check whether this range is valid.
        /// \return Returns true if the range is valid, returns false otherwise.
        operator bool() const;

    private:

        void* base_address_;            ///< \brief Pointer to the first address of the range. If the range is valid, this value is non-zero.

        size_t count_;                  ///< \brief Number of addresses in the range.
    };

    /// \brief Tag identifying a memory block.
    struct MemoryBlockTag {};
    
    /// \brief Tag identifying a virtual memory range.
    struct VirtualRangeTag {};

    /// \brief Utility type for virtual memory ranges.
    /// A virtual memory range represents a group of contiguous memory addresses that can be reserved or released.
    using VirtualMemoryRange = MemoryRange<VirtualRangeTag>;

    /// \brief Utility type for memory blocks.
    /// A memory block represents a group of contiguous memory pages that can be allocated or freed.
    using MemoryBlock = MemoryRange<MemoryBlockTag>;

    /// \brief A stack of memory addresses.
    /// The stack reserves a range of virtual memory address but allocates only when needed.
    /// The stack is guaranteed to never reallocate.
    /// \author Raffaele D. Facendola - December 2016
    class MemoryAddressStack
    {
    public:

        /// \brief Create a new address stack.
        /// \param count Maximum number of addresses to memorize.
        /// \param factor Size of the pages
        MemoryAddressStack(size_t count);

        void Push(void* address);

        void* Pop();

        bool IsEmpty() const;

    private:

        VirtualMemoryRange memory_range_;           ///< \brief Memory range reserved for this stack.

        size_t size_;                               ///< \brief Current size of the stack.

    };

    /// \brief Wraps the low-level calls used to handle virtual memory allocation.
    /// \author Raffaele D. Facendola - December 2016
    class Memory
    {
    public:

        /// \brief Get the singleton instance.
        /// \return Returns the singleton instance;
        static Memory& GetInstance();

        /// \brief Reserve a range of virtual memory addresses.
        /// The reserved address range is guaranteed to be contiguous. The base address is rounded down to the nearest multiple of the allocation granularity.
        /// This method reserves the addresses but it doesn't allocate any memory.
        /// \param count Number of addresses to reserve. This number is rounded up to the nearest multiple of the allocation granularity.
        /// \return If the method succeeds return a non-empty memory range whose size is at least equal to count, otherwise returns an empty range.
        virtual VirtualMemoryRange ReserveVirtualRange(size_t count) = 0;

        /// \brief Release a range of virtual memory addresses.
        /// \param range Range to release.
        /// \return Returns true if the memory could be released, returns false otherwise.
        /// \remarks Accessing an address within the range after this method results in an access violation exception.
        virtual bool ReleaseVirtualRange(const VirtualMemoryRange& range) = 0;

        /// \brief Allocate a memory block within a virtual address range.
        /// This method causes physical memory to be allocated. Any address within the returned memory block can be accessed for read or write.
        /// \param range Range within which the allocation is performed.
        /// \param offset Offset of the block from the base address of the range, in bytes. This number is rounded down to the nearest multiple of the memory page size.
        /// \param size Size of the memory block. This number is rounded up to the nearest multiple of the memory page size.
        /// \return If the method succeeds returns a non-empty memory block whose size is at least equal to size, otherwise returns an empty range.
        virtual MemoryBlock AllocMemoryBlock(const VirtualMemoryRange& range, size_t offset, size_t size) = 0;

        /// \brief Free a memory block.
        /// \param block Memory block to free.
        /// \return Returns true if the memory could be freed, returns false otherwise.
        /// \remarks Accessing an address within the block after this method results in an access violation exception.
        virtual bool FreeMemoryBlock(const MemoryBlock& block) = 0;

    };

    /// \brief Get the current system instance.
    /// \brief Returns the current system instance.
    Memory& GetMemory();

}

namespace syntropy
{
    // Implementation

    //////////////// MEMORY RANGE ////////////////

    template <typename TTag>
    MemoryRange<TTag>::MemoryRange(void* base_address, size_t count)
        : base_address_(base_address)
        , count_(count)
    {

    }

    template <typename TTag>
    size_t MemoryRange<TTag>::GetCount() const
    {
        return count_;
    }

    template <typename TTag>
    void* MemoryRange<TTag>::GetBaseAddress() const
    {
        return base_address_;
    }

    template <typename TTag>
    template <typename TPtr>
    TPtr* MemoryRange<TTag>::GetPointer(size_t offset) const
    {
        return reinterpret_cast<TPtr*>(base_address_) + offset;
    }

    template <typename TTag>
    MemoryRange<TTag>::operator bool() const
    {
        return base_address_;
    }

}
