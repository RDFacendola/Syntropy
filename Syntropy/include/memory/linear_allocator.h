
/// \file frame_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains linear, frame and stack-based allocators.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>

namespace syntropy
{

    /// \brief Used to allocate memory on a pre-allocated contiguous memory block.
    /// Use this allocator to group together allocations having the same lifespan and when high performances are needed and no particular deallocation handling is required.
    /// Memory is allocated upfront to avoid kernel calls while allocating. Pointer-level deallocation is not supported.
    /// \author Raffaele D. Facendola - January 2017
    class LinearAllocator
    {
    public:

        /// \brief Create a new allocator.
        /// \param capacity Amount of memory reserved by the allocator.
        LinearAllocator(size_t capacity);

        /// \brief Default destructor.
        ~LinearAllocator();

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

    private:

        int8_t* base_;                              ///< \brief Base pointer to the memory chunk reserved for this allocator.

        int8_t* head_;                              ///< \brief Pointer to the first unallocated memory block.

        int8_t* status_;                            ///< \brief Points to the last saved status. Grows backwards from the top of the allocator range.

        size_t capacity_;                           ///< \brief Maximum capacity of the allocator.

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
        DoubleBufferedAllocator(size_t capacity);

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

        AllocateFinalizer(reinterpret_cast<T*>(storage));                           // Either allocates a finalizer or does nothing if T doesn't require a destructor.

        return new (storage) T(std::forward<TArgs>(arguments)...);                  // ctor
    }

    template <typename T, typename... TArgs>
    T* ScopeAllocator::AlignedNew(size_t alignment, TArgs&&... arguments)
    {
        auto storage = allocator_.Allocate(sizeof(T), alignment);                   // Aligned storage for the instance

        AllocateFinalizer(reinterpret_cast<T*>(storage));                           // Either allocates a finalizer or does nothing if T doesn't require a destructor.

        return new (storage) T(std::forward<TArgs>(arguments)...);                  // ctor
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
}