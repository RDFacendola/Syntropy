
/// \file stack_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains stack-based and frame-based allocators.
///
/// \author Raffaele D. Facendola - March 2017

#pragma once

#include <mutex>

#include "memory/alignment.h"
#include "memory/memory_address.h"
#include "memory/virtual_memory_range.h"

namespace syntropy
{
    /************************************************************************/
    /* STACK ALLOCATOR                                                      */
    /************************************************************************/

    /// \brief Used to allocate memory on a preallocated contiguous memory block.
    /// Allocations are performed on the head of the stack. Pointer-level deallocations are not supported.
    /// The stack can be rewound to a previous state, undoing all the allocations that were performed from that point on.
    /// Use this allocator to group together allocations having the same lifespan and when high performances are needed.
    /// Memory is allocated upfront to avoid kernel calls while allocating.
    /// \author Raffaele D. Facendola - January 2017
    class StackAllocator
    {
    public:

        /// \brief Create a new allocator.
        /// \param capacity Amount of memory reserved by the allocator.
        /// \param alignment Memory alignment.
        StackAllocator(Bytes capacity, Alignment alignment);

        /// \brief Create a new allocator from a non-owned memory range.
        /// \param memory_range Memory range used by the allocator.
        /// \param alignment Memory alignment.
        /// \remarks The allocator doesn't take ownership of the memory range provided as input.
        StackAllocator(const MemoryRange& memory_range, Alignment alignment = Alignment(1_Bytes));

        /// \brief No copy constructor.
        StackAllocator(const StackAllocator&) = delete;

        /// \brief Default destructor.
        ~StackAllocator() = default;

        /// \brief No assignment operator.
        StackAllocator& operator=(const StackAllocator&) = delete;

        /// \brief Allocate a new memory block on the allocator's head.
        /// \param size Size of the memory block to allocate, in bytes.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate(Bytes size);

        /// \brief Allocate a new aligned memory block on the allocator's head.
        /// \param size Size of the memory block to allocate, in bytes.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate(Bytes size, Alignment alignment);

        /// \brief Free all the allocations performed so far.
        void Free();

        /// \brief Save the current status of the allocator.
        /// To restore it use RestoreStatus().
        void SaveStatus();

        /// \brief Restore the last saved status.
        /// Calling this method multiple times causes older status to be restored.
        void RestoreStatus();

        /// \brief Get the current allocation size, in bytes.
        /// \return Returns the total amount of allocations performed so far by this allocator, in bytes.
        Bytes GetAllocationSize() const;

        /// \brief Get the amount of system memory committed by the allocator, in bytes.
        /// Note that the stack allocator allocates all the memory it needs upfront.
        /// \return Returns the amount of system memory committed by the allocator, in bytes.
        Bytes GetCommitSize() const;

    private:

        VirtualMemoryRange memory_pool_;        ///< \brief Virtual memory range owned by this allocator. Empty if the allocator owns no virtual memory.

        MemoryRange memory_range_;              ///< \brief Memory range managed by the allocator. May refer to memory_pool_ or to a range owned by someone else.

        MemoryAddress head_;                    ///< \brief Pointer to the first unallocated memory block.

        MemoryAddress status_;                  ///< \brief Points to the last saved status. Grows backwards from the top of the allocator range.

        std::mutex mutex_;                      ///< \brief Used for thread-safety purposes.
    };

    /// \brief Utility allocator that sits on top of a stack allocator and handles concrete object construction and destruction via RAII.
    ///
    /// \usage ScopeAllocator s(stack_allocator);
    ///        auto foo = s.New<Foo>(arg0, arg1);
    ///        (foo gets destroyed when s goes out of scope)
    ///
    /// \author Raffaele D. Facendola - January 2017
    /// \see Based on http://www.frostbite.com/wp-content/uploads/2013/05/scopestacks_public.pdf
    /// \remarks Be careful while mixing allocation made by ScopeAllocators and the underlying LinearAllocator since these allocation are not tracked.
    class ScopeAllocator
    {
    public:

        /// \brief Create a new scope allocator.
        /// \param allocator Actual linear allocator to use.
        ScopeAllocator(StackAllocator& allocator);

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

        StackAllocator& allocator_;                     ///< \brief Actual allocator.

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
        DoubleBufferedAllocator(Bytes capacity, Alignment alignment);

        /// \brief Allocate a new memory block on the current allocator.
        /// \param size Size of the memory block to allocate, in bytes.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate(Bytes size);

        /// \brief Allocate a new aligned memory block on the current allocator.
        /// \param size Size of the memory block to allocate, in bytes.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate(Bytes size, Alignment alignment);

        /// \brief Free all the memory blocks allocated so far in the current allocator.
        void Free();

        /// \brief Flips the linear allocators, activating the next one.
        /// This method causes the inactive allocator to become active and vice-versa. The new active allocator is freed.
        void Flip();

        /// \brief Get the current linear allocator.
        /// \return Returns the current linear allocator.
        StackAllocator& GetCurrentAllocator();

        /// \brief Save the status of the current allocator.
        /// To restore the status use RestoreStatus().
        void SaveStatus();

        /// \brief Restore the last saved status of the current allocator.
        /// Calling this method multiple times causes older status to be restored as well.
        void RestoreStatus();

        /// \brief Get the current allocation size, in bytes.
        /// \return Returns the total amount of allocations performed so far by this allocator, in bytes.
        Bytes GetAllocationSize() const;

        /// \brief Get the amount of system memory committed by the allocator, in bytes.
        /// Note that the stack allocator allocates all the memory it needs upfront.
        /// \return Returns the amount of system memory committed by the allocator, in bytes.
        Bytes GetCommitSize() const;

    private:

        // TODO: Use a double-ended stack for better memory management.

        StackAllocator allocators_[2];             ///< \brief Linear allocators pair.

        StackAllocator* current_;                  ///< \brief Current allocator.

    };

}

// Implementation

namespace syntropy
{

    /************************************************************************/
    /* SCOPE ALLOCATOR                                                      */
    /************************************************************************/

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
        // Do nothing
    }

}

