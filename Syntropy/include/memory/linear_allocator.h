
/// \file frame_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains sequential and linear allocators.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "memory/memory.h"

#include <algorithm>

namespace syntropy
{

    /// \brief Base allocator used to allocate sequential memory blocks over a contiguous range of virtual memory addresses.
    /// Memory is committed and decommited on demand: the allocator allocates the minimum amount of system memory pages.
    /// Memory is allocated and freed on the allocator's head.
    /// \author Raffaele D. Facendola - January 2017
    class LinearAllocator
    {
    public:

        /// \brief Create a new allocator.
        /// \param capacity Amount of memory reserved by the allocator.
        /// \param alignment Memory alignment.
        LinearAllocator(size_t capacity, size_t alignment);

        /// \brief Create a new allocator.
        /// \param memory_range Memory range used by the allocator.
        /// \param alignment Memory alignment.
        /// \remarks The allocator doesn't take ownership of the memory range provided as input.
        LinearAllocator(const MemoryRange& memory_range, size_t alignment);

        /// \brief No copy constructor.
        LinearAllocator(const LinearAllocator&) = delete;

        /// \brief Default destructor.
        ~LinearAllocator() = default;

        /// \brief No assignment operator.
        LinearAllocator& operator=(const LinearAllocator&) = delete;

        /// \brief Allocate a new memory block on the allocator's head.
        /// \param size Size of the memory block to allocate, in bytes.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate(size_t size);

        /// \brief Free a memory block on the allocator's head.
        /// \param size Size of the memory block to free, in bytes.
        void Free(size_t size);

        /// \brief Get the current allocation size, in bytes.
        /// \return Returns the total amount of allocations performed so far by this allocator, in bytes.
        size_t GetAllocationSize() const;

        /// \brief Get the amount of system memory committed by the allocator, in bytes.
        /// Note that the stack allocator allocates all the memory it needs upfront.
        /// \return Returns the amount of system memory committed by the allocator, in bytes.
        size_t GetCommitSize() const;

        /// \brief Get the memory range managed by this allocator.
        /// \return Returns the memory range managed by this allocator.
        const MemoryRange& GetRange() const;

    private:

        MemoryPool memory_pool_;        ///< \brief Virtual memory range owned by this allocator. Empty if the allocator owns no virtual memory.

        MemoryRange memory_range_;      ///< \brief Memory range managed by the allocator. May refer to memory_pool_ or to a range owned by someone else.

        void* head_;                    ///< \brief Points to the first unallocated memory address.

        void* page_head_;               ///< \brief Points to the first unmapped memory page.

    };

    /// \brief Allocator that behaves like a vector that grows and shrinks over the virtual memory to avoid reallocations.
    /// Use this allocator to store an unknown number of objects when only the upper bound is known and no memory waste is allowed.
    /// Generally used as a sub-allocator for dynamic allocations of another allocator.
    /// \author Raffaele D. Facendola - January 2017
    template <typename T>
    class VectorAllocator
    {
    public:

        /// \brief Initial element capacity.
        static const size_t kInitialCapacity = 16;

        /// \brief Create a new vector allocator.
        /// \param max_count Maximum amount of elements that can be stored.
        VectorAllocator(size_t max_count);

        /// \brief No copy constructor.
        VectorAllocator(const VectorAllocator&) = delete;

        /// \brief Destructor.
        ~VectorAllocator() = default;

        /// \brief No assignment operator.
        VectorAllocator& operator=(const VectorAllocator&) = delete;

        /// \brief Get an iterator pointing to the first element in the allocator.
        /// \return Returns an iterator pointing to the first element in the allocator.
        T* begin();

        /// \brief Get an iterator pointing to the past-the-end element in the allocator.
        /// \return Returns an iterator pointing to the past-the-end element in the allocator.
        T* end();

        /// \brief Get an iterator pointing to the first element in the allocator.
        /// \return Returns an iterator pointing to the first element in the allocator.
        const T* begin() const;

        /// \brief Get an iterator pointing to the past-the-end element in the allocator.
        /// \return Returns an iterator pointing to the past-the-end element in the allocator.
        const T* end() const;

        /// \brief Access an element on the allocator.
        /// \param index Index of the element to access.
        /// \return Returns the index-th element on the allocator.
        T& operator[](size_t index);

        /// \brief Access the first element.
        /// \return Returns a reference to the first element.
        T& Front();

        /// \brief Access the last element.
        /// \return Returns a reference to the last element.
        T& Back();

        /// \brief Insert a new element at the end of the allocator, after its current last element.
        /// The value is copied to the new element.
        /// \param element Element to copy.
        void PushBack(const T& element);

        /// \brief Insert a new element at the end of the allocator, after its current last element.
        /// The value is moved to the new element.
        /// \param element Element to move.
        void PushBack(T&& element);

        /// \brief Insert a new element at the end of the allocator, after its current last element.
        /// \param args Arguments used to construct the element in place.
        template <class... TArgs>
        void EmplaceBack(TArgs&&... args);

        /// \brief Remove the last element in the allocator.
        /// The removed element is destroyed.
        void PopBack();

        /// \brief Get the number of elements stored inside the allocator.
        /// \return Returns the number of elements stored inside the allocator.
        size_t GetCount() const;

        /// \brief Check whether the allocator is empty.
        /// \return Returns true if the allocator is empty, returns false otherwise.
        bool IsEmpty() const;

        /// \brief Get the maximum number of elements that can be store inside the allocator.
        /// \return Returns the maximum number of elements that can be store inside the allocator.
        size_t GetMaxCount() const;

        /// \brief Get the current allocation size, in bytes.
        /// \return Returns the total amount of allocations performed so far by this allocator, in bytes.
        size_t GetAllocationSize() const;

        /// \brief Get the amount of system memory committed by the allocator, in bytes.
        /// Note that the stack allocator allocates all the memory it needs upfront.
        /// \return Returns the amount of system memory committed by the allocator, in bytes.
        size_t GetCommitSize() const;

        /// \brief Get the memory range managed by this allocator.
        /// \return Returns the memory range managed by this allocator.
        const MemoryRange& GetRange() const;

    private:

        /// \brief Increase the size of the vector.
        void IncreaseSize(size_t amount = 1);

        /// \brief Decrease the size of the vector.
        void DecreaseSize(size_t amount = 1);

        /// \brief Get the minimum size allowed for this allocator.
        /// \return Returns the minimum size allowed for this allocator, in bytes.
        size_t GetMinSize() const;

        size_t count_;                          ///< \brief Number of elements in the allocator.

        size_t max_count_;                      ///< \brief Maximum amount of elements in the allocator.

        LinearAllocator allocator_;             ///< \brief Actual underlying allocator.

    };

}

namespace syntropy
{
    // Implementation

    //////////////// VECTOR ALLOCATOR ////////////////

    template <typename T>
    VectorAllocator<T>::VectorAllocator(size_t max_count)
        : count_(0)
        , max_count_(max_count)
        , allocator_(Memory::CeilToPageSize(sizeof(T) * max_count_), alignof(T))
    {
        // Allocate the initial slack
        allocator_.Allocate(GetMinSize());
    }

    template <typename T>
    T* VectorAllocator<T>::begin()
    {
        return reinterpret_cast<T*>(*allocator_.GetRange());
    }

    template <typename T>
    T* VectorAllocator<T>::end()
    {
        return begin() + count_;
    }

    template <typename T>
    const T* VectorAllocator<T>::begin() const
    {
        return reinterpret_cast<const T*>(allocator_.GetRange().GetTop());
    }

    template <typename T>
    const T* VectorAllocator<T>::end() const
    {
        return begin() + count_;
    }

    template <typename T>
    T& VectorAllocator<T>::operator[](size_t index)
    {
        SYNTROPY_ASSERT(index < count_);
        return *(begin() + index);
    }

    template <typename T>
    T& VectorAllocator<T>::Front()
    {
        return *begin();
    }

    template <typename T>
    T& VectorAllocator<T>::Back()
    {
        return *(begin() + count_ - 1);
    }

    template <typename T>
    void VectorAllocator<T>::PushBack(const T& element)
    {
        SYNTROPY_ASSERT(count_ < max_count_);

        IncreaseSize();

        Back() = element;
    }

    template <typename T>
    void VectorAllocator<T>::PushBack(T&& element)
    {
        SYNTROPY_ASSERT(count_ < max_count_);

        IncreaseSize();

        Back() = std::move(element);
    }

    template <typename T>
    template <class... TArgs>
    void VectorAllocator<T>::EmplaceBack(TArgs&&... args)
    {
        SYNTROPY_ASSERT(count_ < max_count_);

        IncreaseSize();

        new (std::addressof(Back())) T(std::forward<TArgs>(args)...);
    }

    template <typename T>
    void VectorAllocator<T>::PopBack()
    {
        SYNTROPY_ASSERT(!IsEmpty());

        std::addressof(Back())->~T();       // Destroy the element.

        DecreaseSize();
    }

    template <typename T>
    size_t VectorAllocator<T>::GetCount() const
    {
        return count_;
    }

    template <typename T>
    bool VectorAllocator<T>::IsEmpty() const
    {
        return count_ == 0;
    }

    template <typename T>
    size_t VectorAllocator<T>::GetMaxCount() const
    {
        return max_count_;
    }

    template <typename T>
    size_t VectorAllocator<T>::GetAllocationSize() const
    {
        return allocator_.GetAllocationSize();
    }

    template <typename T>
    size_t VectorAllocator<T>::GetCommitSize() const
    {
        return allocator_.GetCommitSize();
    }

    template <typename T>
    const MemoryRange& VectorAllocator<T>::GetRange() const
    {
        return allocator_.GetRange();
    }

    template <typename T>
    void VectorAllocator<T>::IncreaseSize(size_t amount)
    {
        count_ += amount;

        auto size = GetAllocationSize();                            // Allocated space, in bytes. Always a multiple of the page size.
        auto capacity = size / sizeof(T);                           // Elements that can fit within the current allocated space.

        while (count_ > capacity)
        {
            // Double the allocation size.

            size = std::min(size, GetRange().GetSize() - size);     // Prevents the new allocation from exceeding the total capacity.

            allocator_.Allocate(size);                              // Kernel call.

            // Refresh the current capacity.

            size = GetAllocationSize();
            capacity = size / sizeof(T);
        }

    }

    template <typename T>
    void VectorAllocator<T>::DecreaseSize(size_t amount)
    {
        count_ -= amount;

        auto min_size = GetMinSize();
        auto size = GetAllocationSize();                                // Allocated space, in bytes. Always a multiple of the page size.
        auto capacity = size / sizeof(T);                               // Elements that can fit within the current allocated space.

        while (size > min_size && count_ < capacity / 4)
        {
            // Halve the allocation size.
            // At the end of this call the slack size is at least equal to count (hence the 4 above). This prevents memory page trashing.

            size -= std::max(Memory::CeilToPageSize(size / 2), min_size);       // Extra space to free.

            allocator_.Free(size);                                              // Kernel call.

            // Refresh the current capacity.

            size = GetAllocationSize();
            capacity = size / sizeof(T);
        }
    }

    template <typename T>
    size_t VectorAllocator<T>::GetMinSize() const
    {
        return Memory::CeilToPageSize(sizeof(T) * kInitialCapacity);
    }

}