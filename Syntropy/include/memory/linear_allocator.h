
/// \file frame_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains linear, frame and stack-based allocators.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <memory>
#include <iterator>
#include <algorithm>

#include "memory/memory.h"

namespace syntropy
{

    /// \brief Base allocator used to allocate sequential memory blocks over a contiguous range of virtual memory addresses.
    /// Memory is committed and decommited on demand. Allocations are always rounded to the next memory page boundary.
    /// \author Raffaele D. Facendola - January 2017
    class SequentialAllocator
    {
    public:

        /// \brief Create a new sequential allocator.
        /// \param capacity Amount of memory reserved by the allocator.
        /// \param alignment Memory alignment.
        SequentialAllocator(size_t capacity, size_t alignment);

        /// \brief No copy constructor.
        SequentialAllocator(const SequentialAllocator&) = delete;

        /// \brief Default destructor.
        ~SequentialAllocator();

        /// \brief No assignment operator.
        SequentialAllocator& operator=(const SequentialAllocator&) = delete;

        /// \brief Allocate a new memory block on the allocator's head.
        /// \param size Size of the memory block to allocate, in bytes.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate(size_t size);

        /// \brief Free a memory block from the allocator's head.
        void Free(size_t size);

        /// \brief Get the total amount of memory that was allocated on this allocator.
        /// \return Returns the total amount of memory that was allocated on this allocator, in bytes.
        size_t GetSize() const;

        /// \brief Get the effective memory footprint of this allocator.
        /// \return Returns the amount of memory effectively allocated by this allocator, in bytes.
        size_t GetCapacity() const;

        /// \brief Get the base pointer of this allocator.
        /// \return Returns the base pointer of this allocator.
        int8_t* GetBasePointer();

    private:

        MemoryRange memory_;        ///< \brief Reserved virtual memory range.

        int8_t* head_;              ///< \brief Points to the first unmapped memory page.

    };

    /// \brief Used to allocate memory on a pre-allocated contiguous memory block.
    /// Use this allocator to group together allocations having the same lifespan and when high performances are needed.
    /// Memory is allocated upfront to avoid kernel calls while allocating. Pointer-level deallocation is not supported.
    /// \author Raffaele D. Facendola - January 2017
    class LinearAllocator
    {
    public:

        /// \brief Create a new allocator.
        /// \param capacity Amount of memory reserved by the allocator.
        /// \param alignment Memory alignment.
        LinearAllocator(size_t capacity, size_t alignment);

        /// \brief No copy constructor.
        LinearAllocator(const LinearAllocator&) = delete;

        /// \brief Default destructor.
        ~LinearAllocator();

        /// \brief No assignment operator.
        LinearAllocator& operator=(const LinearAllocator&) = delete;

        /// \brief Allocate a new memory block.
        /// \param size Size of the memory block to allocate, in bytes.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate(size_t size);

        /// \brief Allocate a new aligned memory block.
        /// \param size Size of the memory block to allocate, in bytes.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate(size_t size, size_t alignment);

        /// \brief Free all the memory blocks allocated so far.
        void Free();

        /// \brief Save the status of the allocator.
        /// To restore it use RestoreStatus().
        void SaveStatus();

        /// \brief Restore the last saved status.
        /// Calling this method multiple times causes older status to be restored.
        void RestoreStatus();

        /// \brief Get the total amount of memory that was allocated on this allocator.
        /// \return Returns the total amount of memory that was allocated on this allocator, in bytes.
        size_t GetSize() const;

        /// \brief Get the effective memory footprint of this allocator.
        /// \return Returns the amount of memory effectively allocated by this allocator, in bytes.
        size_t GetCapacity() const;

        /// \brief Get the base pointer of this allocator.
        /// \return Returns the base pointer of this allocator.
        int8_t* GetBasePointer();

    private:

        MemoryRange memory_;            ///< \brief Reserved virtual memory range.

        int8_t* head_;                  ///< \brief Pointer to the first unallocated memory block.

        int8_t* status_;                ///< \brief Points to the last saved status. Grows backwards from the top of the allocator range.

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

        /// \brief Get the total amount of memory that was allocated on this allocator.
        /// \return Returns the total amount of memory that was allocated on this allocator, in bytes.
        size_t GetSize() const;

        /// \brief Get the effective memory footprint of this allocator.
        /// \return Returns the amount of memory effectively allocated by this allocator, in bytes.
        size_t GetCapacity() const;

    private:

        /// \brief Increase the size of the vector by one.
        void IncreaseSize();

        /// \brief Decrease the size of the vector by one.
        void DecreaseSize();

        /// \brief Get the minimum size allowed for this allocator.
        /// \return Returns the minimum size allowed for this allocator, in bytes.
        size_t GetMinSize() const;

        size_t count_;                          ///< \brief Number of elements in the allocator.

        size_t max_count_;                      ///< \brief Maximum amount of elements in the allocator.

        SequentialAllocator allocator_;         ///< \brief Actual underlying allocator.

    };

    /// \brief Utility allocator that sits on top of a linear allocator and handles concrete object construction and destruction via RAII.
    ///
    /// \usage ScopeAllocator scope(linear_allocator);
    ///        auto foo = scope.New<Foo>(arg0, arg1);
    ///        (foo gets destroyed when scope goes out of... scope)
    ///
    /// \author Raffaele D. Facendola - January 2017
    /// \see Based on http://www.frostbite.com/wp-content/uploads/2013/05/scopestacks_public.pdf
    /// \remarks Be careful while mixing allocation made by ScopeAllocators and the underlying LinearAllocator since these allocation are not tracked.
    class ScopeAllocator
    {
    public:

        /// \brief Create a new scope allocator.
        /// \param allocator Actual linear allocator to use.
        ScopeAllocator(LinearAllocator& allocator);

        /// \brief Default destructor.
        /// Destroys all the objects that were allocated from this allocator and rewinds the allocator status.
        ~ScopeAllocator();

        /// \brief Create a new object.
        /// \tparam T Type of the object to create.
        /// \param arguments Arguments to pass to T's constructor.
        /// \return Returns a pointer to a new instance of T.
        template <typename T, typename... TArgs>
        T* New(TArgs&&... arguments);

        /// \brief Create a new aligned object.
        /// \tparam T Type of the object to create.
        /// \param alignment Alignment of the object, in bytes.
        /// \param arguments Arguments to pass to T's constructor.
        /// \return Returns a pointer to a new instance of T.
        template <typename T, typename... TArgs>
        T* AlignedNew(size_t alignment, TArgs&&... arguments);

    private:

        /// \brief Handles explicit destruction of objects.
        /// \param instance Pointer to the instance to destroy. Must be of type T.
        template <typename T>
        static void Destructor(void* instance);

        /// \brief Allocate a finalizer object on the linear allocator.
        /// This overload does not participate in overload resolution if T is trivially destructible. In that case no finalizer object is required.
        /// \param instance Instance the finalizer refers to.
        template <typename T, typename = std::enable_if_t<!std::is_trivially_destructible<T>::value>>
        void AllocateFinalizer(T* instance);

        /// \brief Overload used for trivially destructible object. This method does nothing.
        /// The ellipsis conversion sequence here makes this overload the worst one (so we can't accidentally call this method when the other overload is also eligible).
        void AllocateFinalizer(...);

        /// \brief Finalizer object used to destroy allocated objects.
        /// Finalizers are allocated just after the object they are supposed to destroy.
        struct Finalizer
        {

            /// \brief Destroy the object.
            void operator()() const;

            void(*destructor_)(void* instance);         ///< \brief Pointer to the destruction function. std::function uses more than 8 bytes of memory.

            Finalizer* next_;                           ///< \brief Next finalizer.

            void* object_;                              ///< \brief Object to finalize. The pointer is needed since there's no guarantee that aligned objects and their finalizers are contiguous in memory.
        };

        LinearAllocator& allocator_;                    ///< \brief Actual allocator.

        Finalizer* finalizer_list_;                     ///< \brief First finalizer functor.

    };

    /// \brief Packs a pair of LinearAllocator together such that allocations performed during a frame are available to the next frame as well.
    /// All the actions are performed only of the currently active allocator. After a flip the inactive allocators become active and vice-versa.
    /// \author Raffaele D. Facendola - January 2017
    class DoubleBufferedAllocator
    {
    public:

        /// \brief Create a new allocator.
        /// \param capacity Amount of memory reserved by each allocator.
        /// \param alignment Memory alignment.
        DoubleBufferedAllocator(size_t capacity, size_t alignment);

        /// \brief Allocate a new memory block on the current allocator.
        /// \param size Size of the memory block to allocate, in bytes.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate(size_t size);

        /// \brief Allocate a new aligned memory block on the current allocator.
        /// \param size Size of the memory block to allocate, in bytes.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate(size_t size, size_t alignment);

        /// \brief Free all the memory blocks allocated so far in the current allocator.
        void Free();

        /// \brief Flips the linear allocators, activating the next one.
        /// This method causes the inactive allocator to become active and vice-versa. The new active allocator is freed.
        void Flip();

        /// \brief Get the current linear allocator.
        /// \return Returns the current linear allocator.
        LinearAllocator& GetCurrentAllocator();

        /// \brief Save the status of the current allocator.
        /// To restore the status use RestoreStatus().
        void SaveStatus();

        /// \brief Restore the last saved status of the current allocator.
        /// Calling this method multiple times causes older status to be restored as well.
        void RestoreStatus();

        /// \brief Get the total amount of memory that was allocated on this allocator.
        /// \return Returns the total amount of memory that was allocated on this allocator, in bytes.
        size_t GetSize() const;

        /// \brief Get the effective memory footprint of this allocator.
        /// \return Returns the amount of memory effectively allocated by this allocator, in bytes.
        size_t GetCapacity() const;

    private:

        LinearAllocator allocators_[2];             ///< \brief Linear allocators pair.

        LinearAllocator* current_;                  ///< \brief Current allocator.

    };

}

namespace syntropy
{
    // Implementation

    //////////////// SCOPE ALLOCATOR ////////////////

    template <typename T, typename... TArgs>
    T* ScopeAllocator::New(TArgs&&... arguments)
    {
        auto storage = allocator_.Allocate(sizeof(T));                              // Storage for the instance

        auto instance = new (storage) T(std::forward<TArgs>(arguments)...);         // Create the new instance

        AllocateFinalizer(instance);                                                // Either allocates a finalizer or does nothing if T doesn't require a destructor.

        return instance;
    }

    template <typename T, typename... TArgs>
    T* ScopeAllocator::AlignedNew(size_t alignment, TArgs&&... arguments)
    {
        auto storage = allocator_.Allocate(sizeof(T), alignment);                   // Aligned storage for the instance

        auto instance = new (storage) T(std::forward<TArgs>(arguments)...);         // Create the new instance

        AllocateFinalizer(instance);                                                // Either allocates a finalizer or does nothing if T doesn't require a destructor.

        return instance;
    }

    template <typename T>
    void ScopeAllocator::Destructor(void* instance)
    {
        static_cast<T*>(instance)->~T();
    }

    template <typename T, typename>
    void ScopeAllocator::AllocateFinalizer(T* instance)
    {
        auto finalizer = reinterpret_cast<Finalizer*>(allocator_.Allocate(sizeof(Finalizer)));

        finalizer->next_ = finalizer_list_;
        finalizer->destructor_ = &Destructor<T>;
        finalizer->object_ = instance;

        finalizer_list_ = finalizer;
    }

    inline void ScopeAllocator::AllocateFinalizer(...)
    {

    }

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
        return reinterpret_cast<T*>(allocator_.GetBasePointer());
    }

    template <typename T>
    T* VectorAllocator<T>::end()
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
    size_t VectorAllocator<T>::GetSize() const
    {
        return allocator_.GetSize();
    }

    template <typename T>
    size_t VectorAllocator<T>::GetCapacity() const
    {
        return allocator_.GetCapacity();
    }

    template <typename T>
    void VectorAllocator<T>::IncreaseSize()
    {
        ++count_;

        auto size = GetSize();                                      // Allocated space, in bytes. Always a multiple of the page size.

        auto capacity = size / sizeof(T);                           // Elements that can fit within the current allocated space.

        if (count_ > capacity)
        {
            // Double the allocation size.

            size = std::min(size, GetCapacity() - size);            // Prevents the new allocation from exceeding the total capacity.

            allocator_.Allocate(size);                              // Kernel call.
        }
    }

    template <typename T>
    void VectorAllocator<T>::DecreaseSize()
    {
        --count_;

        auto size = GetSize();                                                  // Allocated space, in bytes. Always a multiple of the page size.

        auto min_size = GetMinSize();

        SYNTROPY_ASSERT(size >= min_size);

        auto capacity = size / sizeof(T);                                       // Elements that can fit within the current allocated space.

        if (size > min_size && count_ < capacity / 4)
        {
            // Halve the allocation size.
            // At the end of this call the slack size is at least equal to count (hence the 4 above). This prevents memory page trashing.

            size -= std::max(Memory::CeilToPageSize(size / 2), min_size);       // Extra space to free.

            allocator_.Free(size);                                              // Kernel call.
        }
    }

    template <typename T>
    size_t VectorAllocator<T>::GetMinSize() const
    {
        return Memory::CeilToPageSize(sizeof(T) * kInitialCapacity);
    }

}