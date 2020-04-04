
/// \file memory_resource.h
/// \brief This header is part of the syntropy memory management. It contains definitions and interfaces for memory resource.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <cstddef>

namespace syntropy
{
    /************************************************************************/
    /* MEMORY RESOURCE                                                      */
    /************************************************************************/

    /// \brief Represents an abstract interface to an unbounded set of classes encapsulating memory resources.
    /// This class mimics and replaces standard's std::memory_resource. Standard's global default memory resoures are deemed
    /// to be harmful since it may cause non-coherent allocation behavior in the same scope.
    /// \author Raffaele D. Facendola - April 2020.
    class MemoryResource
    {
    public:

        /// \brief Get a memory resource that uses the global operator new and operator delete to allocate memory.
        static MemoryResource& GetNewDeleteResource() noexcept;

        /// \brief Get the thread-local default memory resource.
        /// \remarks The local default memory resource is used by certain facilities when an explicit memory resource is not supplied.
        static MemoryResource& GetDefaultResource() noexcept;

        /// \brief Set the thread-local default memory resource.
        /// \return Returns the previous value of the local default memory resource.
        /// \remarks The local default memory resource is used by certain facilities when an explicit memory resource is not supplied.
        static MemoryResource& SetDefaultResource(MemoryResource& memory_resource) noexcept;

        /// \brief Default constructor.
        MemoryResource() = default;

        /// \brief Default assignment operator.
        MemoryResource(const MemoryResource& rhs) = default;

        /// \brief Default virtual destructor.
        virtual ~MemoryResource() = default;

        /// \brief Default assignment operator.
        MemoryResource& operator=(const MemoryResource& rhs) = default;

        /// \brief Allocate storage with a size of at least bytes bytes.
        /// The returned storage is aligned to the specified alignment if such alignment is supported, and to alignof(std::max_align_t) otherwise.
        [[nodiscard]] void* Allocate(std::size_t bytes, std::size_t alignment = alignof(std::max_align_t));

        /// \brief Deallocate the storage pointed by storage. 
        /// storage shall have been returned by a prior call to Allocate(bytes, alignment) on a MemoryResource that compares equal to *this, and 
        /// the storage it points to shall not yet have been deallocated.
        void Deallocate(void* storage, std::size_t bytes, std::size_t alignment = alignof(std::max_align_t));

        /// \brief Compares *this for equality with rhs.
        /// Two MemoryResources compare equal if and only if memory allocated from one MemoryResource can be deallocated from the other and vice-versa.
        bool IsEqual(const MemoryResource& rhs) const noexcept;

    private:

        /// \brief Thread-local default memory resource.
        static thread_local MemoryResource* default_memory_resource_;

        /// \brief Allocates storage with a size of at least bytes bytes, aligned to the specified alignment.
        /// alignment shall be a power of two.
        virtual void* DoAllocate(std::size_t bytes, std::size_t alignment) = 0;

        /// \brief Deallocates the storage pointed to by storage.
        /// storage must have been returned by a prior call to Allocate(bytes, alignment) on a memory_resource that compares equal to *this, and
        /// the storage it points to must not yet have been deallocated, otherwise the behavior is undefined.
        virtual void DoDeallocate(void* storage, std::size_t bytes, std::size_t alignment) = 0;

        /// \brief Compares *this for equality with rhs.
        /// Two MemoryResources compare equal if and only if memory allocated from one MemoryResource can be deallocated from the other and vice-versa.
        virtual bool DoIsEqual(const MemoryResource& rhs) const noexcept = 0;

    };

    /// \brief Compare the MemoryResources for equality.
    /// Two MemoryResources compare equal if and only if memory allocated from one memory_resource can be deallocated from the other and vice versa.
    bool operator==(const MemoryResource& lhs, const MemoryResource& rhs) noexcept;

    /// \brief Compare the MemoryResources for inequality.
    /// Two MemoryResources compare equal if and only if memory allocated from one memory_resource can be deallocated from the other and vice versa.
    bool operator!=(const MemoryResource& lhs, const MemoryResource& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MemoryResource.

    inline void* MemoryResource::Allocate(std::size_t bytes, std::size_t alignment)
    {
        return DoAllocate(bytes, alignment);
    }

    inline void MemoryResource::Deallocate(void* storage, std::size_t bytes, std::size_t alignment)
    {
        return DoDeallocate(storage, bytes, alignment);
    }

    inline bool MemoryResource::IsEqual(const MemoryResource& rhs) const noexcept
    {
        return DoIsEqual(rhs);
    }

    inline MemoryResource& MemoryResource::GetDefaultResource() noexcept
    {
        return *default_memory_resource_;
    }

    inline MemoryResource& MemoryResource::SetDefaultResource(MemoryResource& memory_resource) noexcept
    {
        auto& previous_memory_resource = GetDefaultResource();

        default_memory_resource_ = &memory_resource;

        return previous_memory_resource;
    }

    inline bool operator==(const MemoryResource& lhs, const MemoryResource& rhs) noexcept
    {
        return (&lhs == &rhs) || lhs.IsEqual(rhs);
    }

    inline bool operator!=(const MemoryResource& lhs, const MemoryResource& rhs) noexcept
    {
        return !(lhs == rhs);
    }

}
