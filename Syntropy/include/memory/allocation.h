
/// \file allocation.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "diagnostics/log.h"
#include "diagnostics/debug.h"

#include "containers/hashed_string.h"

/// \brief Allocate a new object via custom allocator.
/// \usage SYNTROPY_NEW(allocator) Foo();
#define SYNTROPY_NEW(allocator) \
    new (allocator, SYNTROPY_HERE)

/// \brief Delete an object create via custom allocator.
/// \usage SYNTROPY_DELETE(allocator, pointer);
#define SYNTROPY_DELETE(allocator, ptr) \
    syntropy::Delete(ptr, allocator, SYNTROPY_HERE);

namespace syntropy
{

    /// \brief Base interface for allocators.
    /// \author Raffaele D. Facendola - February 2017
    class Allocator
    {
    public:

        /// \brief Allocate a new memory block.
        /// \param size Size of the memory block to allocate, in bytes.
        /// \return Returns a pointer to the allocated memory block.
        virtual void* Allocate(size_t size) = 0;

        /// \brief Allocate a new aligned memory block.
        /// \param size Size of the memory block to allocate, in bytes.
        /// \param alignment Alignment of the allocated block. Must be a multiple of the minimum allocation size.
        /// \return Returns a pointer to the allocated memory block.
        virtual void* Allocate(size_t size, size_t alignment) = 0;

        /// \brief Free a memory block.
        /// \param block Pointer to the block to free.
        virtual void Free(void* block) = 0;

        /// \brief Get a symbolic name for the allocator.
        /// \return Returns a symbolic name for the allocator.
        virtual const HashedString& GetName() const = 0;

    };

    /// \brief Represents a raw memory buffer.
    /// \author Raffaele D. Facendola - February 2017
    class MemoryBuffer
    {
    public:

        /// \brief Create a new empty buffer.
        MemoryBuffer();

        /// \brief Create a new buffer.
        /// \param base Base address of the buffer.
        /// \param size Size of the buffer, in bytes.
        /// \param allocator Allocator used to allocate the memory.
        MemoryBuffer(size_t size, Allocator& allocator);

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
        void* operator[](size_t offset) const;

        /// \brief Get the size of the buffer, in bytes.
        /// \return Returns the size of the buffer, in bytes.
        size_t GetSize() const;

        /// \brief Swap the content of this buffer with another one.
        void Swap(MemoryBuffer& other) noexcept;

    private:

        void* base_;            ///< \brief First address in the buffer. Owning pointer.

        size_t size_;           ///< \brief Size of the buffer, in bytes.

        Allocator* allocator_;  ///< \brief Allocator used to allocate\deallocate memory. Non-owning pointer.

    };

    /// \brief Delete an object allocated using a custom allocator.
    /// \param ptr Pointer to the object to delete.
    /// \param allocator Allocator used to allocate the object.
    template <typename T>
    void Delete(T* ptr, Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace);

}

/// \brief New operator overload for custom allocators.
void* operator new (std::size_t size, syntropy::Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace);

/// \brief Delete expression for custom allocators.
/// Used to *deallocate* object allocated via custom allocators.
void operator delete (void* ptr, syntropy::Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace);

namespace std
{
    /// \brief Swap specialization for memory buffers.
    template<>
    void swap(syntropy::MemoryBuffer& first, syntropy::MemoryBuffer& second);
}

namespace syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    template <typename T>
    void Delete(T* ptr, Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace)
    {
        ptr->~T();
        operator delete(ptr, allocator, stack_trace);
    }
}