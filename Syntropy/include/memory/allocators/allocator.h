
/// \file allocator.h
/// \brief This header is part of the syntropy memory management system. It contains definition of allocator interfaces and global allocation functionalities.
///
/// IMPORTANT: Array allocation\deallocation is not supported. 
/// Placement delete for arrays cannot work with the standard syntax new[] since there's no way of interpreting bookkeeping data (or *where* it is stored)
/// auto p = new (Arena) Foo[5];
/// delete[] p;                         // ERROR: Arena is not used.
/// operator delete[](p, Arena);        // ERROR: This will only deallocate the memory without calling the dtor of Foo. Furthermore we don't know where the bookkeeping data are stored.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "memory/memory.h"

#include "containers/hashed_string.h"

#include "diagnostics/diagnostics.h"

/// \brief Instantiate a new object via custom allocator.
/// \usage auto foo = SYNTROPY_NEW(allocator) Foo();
#define SYNTROPY_NEW(allocator) \
    new (allocator, SYNTROPY_HERE)

/// \brief Delete an object created via custom allocator.
/// \usage SYNTROPY_DELETE(allocator, pointer);
#define SYNTROPY_DELETE(allocator, ptr) \
    syntropy::Allocator::Delete(ptr, allocator, SYNTROPY_HERE);

/// \brief Allocate a new buffer via custom allocator.
/// \usage void* buffer = SYNTROPY_ALLOC(allocator, size);
#define SYNTROPY_ALLOC(allocator, size) \
    operator new(size, allocator, SYNTROPY_HERE)

/// \brief Free a buffer allocated via custom allocator.
/// \usage SYNTROPY_FREE(allocator, buffer);
#define SYNTROPY_FREE(allocator, ptr) \
    operator delete(ptr, allocator, SYNTROPY_HERE)

namespace syntropy
{

    /// \brief Base interface for allocators.
    /// \author Raffaele D. Facendola - February 2017
    class Allocator
    {
    public:

        /// \brief Delete an object allocated via a custom allocator.
        /// \param ptr Pointer to the object to delete.
        /// \param allocator Allocator used to allocate the object.
        template <typename T>
        static void Delete(T* ptr, Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace);

        /// \brief Create a new named allocator.
        /// \param name Name of the allocator.
        Allocator(const HashedString& name);

        /// \brief Copy constructor
        Allocator(const Allocator& other) = default;

        /// \brief Move constructor
        Allocator(Allocator&& other) = default;

        /// \brief Virtual destructor.
        virtual ~Allocator() = default;

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
        /// The caller must ensure that the block belongs to the allocator otherwise the behaviour is undefined.
        /// \param block Pointer to the block to free.
        virtual void Free(void* block) = 0;

        /// \brief Check whether the allocator owns the provided block.
        /// \param block Address of the block to check.
        /// \return Returns true if address belongs to this allocator, return false otherwise.
        /// \remarks Addresses owned by an allocator may not be actually allocated, but they can in future.
        virtual bool Owns(void* block) const = 0;

        /// \brief Get the biggest allocation that can be performed by this allocator.
        virtual size_t GetMaxAllocationSize() const = 0;

        /// \brief Get a symbolic name for the allocator.
        /// \return Returns a symbolic name for the allocator.
        const HashedString& GetName() const;

        /// \brief Get the context associated to this allocator instance.
        operator diagnostics::Context() const;

    private:

        HashedString name_;                             ///< \brief Name of the allocator.

        diagnostics::Context context_;                  ///< \brief Context associated to the allocator.

    };

}

// Allocation

/// \brief New operator overload for custom allocators.
void* operator new (std::size_t size, syntropy::Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace);

/// \brief Delete expression for custom allocators.
/// Used to *deallocate* object allocated via custom allocators.
void operator delete (void* ptr, syntropy::Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace);

// Implementation

namespace syntropy
{

    /************************************************************************/
    /* ALLOCATOR                                                            */
    /************************************************************************/

    template <typename T>
    void Allocator::Delete(T* ptr, Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace)
    {
        ptr->~T();
        operator delete(ptr, allocator, stack_trace);
    }

}