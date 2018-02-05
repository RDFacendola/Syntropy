
/// \file memory.h
/// \brief This header is part of the syntropy memory management system. It contains common classes and functionalities for memory management.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <cstdint>

#include "math/math.h"

namespace syntropy
{
    /// \brief Wraps the low-level calls used to handle virtual memory allocation.
    /// \author Raffaele D. Facendola - December 2016
    class Memory
    {
    public:

        // Memory manipulation

        /// \brief Add an offset to an address.
        /// \param address Address to offset.
        /// \param offset Offset to add, in bytes.
        /// \return Returns the address moved forward by the specified amount of bytes.
        template <typename T>
        static constexpr T* AddOffset(T* address, size_t offset);

        /// \brief Subtract and offset from an address.
        /// \param address Address to offset.
        /// \param offset Offset to subtract, in bytes.
        /// \return Returns the address moved backward by the specified amount of bytes.
        template <typename T>
        static constexpr T* SubOffset(T* address, size_t offset);

        /// \brief Get the distance of two addresses, in bytes.
        /// \param first First address.
        /// \param second Second address.
        /// \return Returns the signed distance among first and second, in bytes. The result is negative if second is less than first, positive otherwise.
        static constexpr ptrdiff_t GetDistance(const void* first, const void* second);

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

    };

    /// \brief Exposes memory-related debug functionalities.
    /// \author Raffaele D. Facendola - March 2017
    class MemoryDebug
    {
    public:

        /// \brief Mark a memory region as uninitialized.
        /// This method does nothing on release.
        /// \param begin First address in the range.
        /// \param end One past the last address in the range.
        static void MarkUninitialized(void* begin, void* end);

        /// \brief Mark a memory region as free.
        /// This method does nothing on release.
        /// \param begin First address in the range.
        /// \param end One past the last address in the range.
        static void MarkFree(void* begin, void* end);

    private:

        /// \brief Pattern used to identify an uninitialized memory block (ie: allocated but not yet written).
        static constexpr int8_t kUninitializedMemoryPattern = 0x5C;

        /// \brief Pattern used to identify a free memory block (ie: a block that has not been yet allocated).
        static constexpr int8_t kFreeMemoryPattern = 0x5F;

    };

    /// \brief Represents a range of contiguous memory addresses.
    /// The range is of the form [base; top)
    /// \author Raffaele D. Facendola - December 2016
    class MemoryRange
    {
    public:

        /// \brief Create an empty memory range.
        MemoryRange();

        /// \brief Create a memory range.
        /// \param base First address in the range.
        /// \param top One past the last address in the range.
        MemoryRange(void* base, void* top);

        /// \brief Create a memory range.
        /// \param base First address in the range.
        /// \param size Size of the range.
        MemoryRange(void* base, size_t size);

        /// \brief Create a memory range which is the aligned version of another range.
        /// This range is guaranteed to be contained inside the original range.
        /// \param other Range to copy.
        /// \param alignment Alignment of this range.
        MemoryRange(const MemoryRange& other, size_t alignment);

        /// \brief Dereferencing operator. Access the base address of the range.
        /// \return Returns the base address of the range.
        void* operator*() const;

        /// \brief Access an element in the range.
        /// \param offset Offset with respect to the first element of the range.
        /// \return Returns a pointer to offset-bytes after the base of the range.
        void* operator[](size_t offset) const;

        /// \brief Get the address past the end of this range,
        /// \return Returns the address past the end of this range.
        void* GetTop() const;

        /// \brief Get the size of the range, in bytes.
        /// \return Returns the total capacity of the memory range, in bytes.
        size_t GetSize() const;

        /// \brief Check whether a memory range is contained entirely inside this range.
        /// \param memory_range Memory range to check.
        /// \return Returns true if memory_range is contained inside this memory range, returns false otherwise.
        bool Contains(const MemoryRange& memory_range) const;

        /// \brief Check whether an address falls within this memory range.
        /// \param address Address to check.
        /// \return Returns true if address is contained inside this memory range, returns false otherwise.
        bool Contains(void* address) const;

    private:

        void* base_;            ///< \brief First address in the memory range.

        void* top_;             ///< \brief One past the last address in the memory range.

    };

}

// Implementation

namespace syntropy
{

    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    template <typename T>
    constexpr T* Memory::AddOffset(T* address, size_t offset)
    {
        return reinterpret_cast<T*>(reinterpret_cast<int8_t*>(address) + offset);
    }

    template <typename T>
    constexpr T* Memory::SubOffset(T* address, size_t offset)
    {
        return reinterpret_cast<T*>(reinterpret_cast<int8_t*>(address) - offset);
    }

    constexpr ptrdiff_t Memory::GetDistance(const void* first, const void* second)
    {
        return reinterpret_cast<intptr_t>(second) - reinterpret_cast<intptr_t>(first);
    }

    template <typename T>
    constexpr T* Memory::Align(T* address, size_t alignment)
    {
        return reinterpret_cast<T*>(Math::Ceil(reinterpret_cast<uintptr_t>(address), alignment));
    }

    template <typename T>
    constexpr T* Memory::AlignDown(T* address, size_t alignment)
    {
        return reinterpret_cast<T*>(Math::Floor(reinterpret_cast<uintptr_t>(address), alignment));
    }

    constexpr bool Memory::IsAlignedTo(void* address, size_t alignment)
    {
        return reinterpret_cast<uintptr_t>(address) % alignment == 0;
    }

}

