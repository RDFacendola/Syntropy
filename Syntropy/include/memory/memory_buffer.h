
/// \file memory_buffer.h
/// \brief This header is part of the syntropy memory management system. It contains classes and definitions for memory buffers.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "memory/memory_range.h"
#include "memory/allocators/allocator.h"

namespace syntropy
{
    /************************************************************************/
    /* MEMORY BUFFER                                                        */
    /************************************************************************/

    /// \brief Represents a raw memory buffer.
    /// The buffer must be allocated via an explicit allocator.
    /// \author Raffaele D. Facendola - February 2017
    class MemoryBuffer
    {
    public:

        /// \brief Create a new empty buffer.
        MemoryBuffer();

        /// \brief Create a new buffer.
        /// \param size Size of the buffer, in bytes.
        /// \param allocator Allocator used to allocate the memory.
        MemoryBuffer(Bytes size, Allocator& allocator);

        /// \brief Copy constructor.
        /// Copy the content of another buffer to this one.
        /// \param other Buffer to copy.
        MemoryBuffer(const MemoryBuffer& other);

        /// \brief Move constructor.
        /// Assign the memory buffer of another instance to this one.
        /// \param other Buffer to move.
        MemoryBuffer(MemoryBuffer&& other);

        /// \brief Destructor.
        ~MemoryBuffer();

        /// \brief Unified assignment operator.
        MemoryBuffer& operator=(MemoryBuffer other);

        /// \brief Dereferencing operator. Access the base address of the buffer.
        /// \return Returns the base address of the buffer.
        void* operator*() const;

        /// \brief Access an element in the buffer.
        /// \param offset Offset with respect to the first element of the buffer.
        /// \return Returns a pointer to the element (buffer+offset).
        void* operator[](Bytes offset) const;

        /// \brief Get the size of the buffer, in bytes.
        /// \return Returns the size of the buffer, in bytes.
        Bytes GetSize() const;

        /// \brief Get the buffer memory range.
        operator MemoryRange() const;

        /// \brief Swap the content of this buffer with another one.
        void Swap(MemoryBuffer& other) noexcept;

    private:

        MemoryRange range_;     ///< \brief Buffer memory range.

        Allocator* allocator_;  ///< \brief Allocator used to allocate\deallocate memory. Non-owning pointer.
    };

}

namespace std
{
    /// \brief Swap specialization for memory buffers.
    template<>
    void swap(syntropy::MemoryBuffer& first, syntropy::MemoryBuffer& second);
}

