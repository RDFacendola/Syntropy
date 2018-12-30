
/// \file memory_manager.h
/// \brief This header is part of the syntropy memory management system. It contains classes used to handle context-based dynamic allocations and
/// application-level allocator lifecycles.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <vector>
#include <memory>
#include <type_traits>

#include "syntropy/memory/allocators/allocator.h"

#include "syntropy/diagnostics/assert.h"

/// \brief Instantiate a new object on the active syntropy::MemoryManager allocator.
/// \usage auto foo = SYNTROPY_MM_NEW Foo();
#define SYNTROPY_MM_NEW \
    SYNTROPY_NEW(syntropy::GetMemoryManager().GetAllocator())

/// \brief Delete an object that was allocated via an allocator registered to syntropy::MemoryManager.
/// Searches for any allocator owned by MemoryManager that can delete the object, regardless of the current active allocator and the allocator who made the allocation.
/// \usage SYNTROPY_MM_DELETE(pointer);
#define SYNTROPY_MM_DELETE(ptr) \
    SYNTROPY_DELETE(*syntropy::GetMemoryManager().GetAllocator(ptr), ptr)

/// \brief Allocate a new buffer on the active syntropy::MemoryManager allocator.
/// \usage void* buffer = SYNTROPY_MM_ALLOC(size);
#define SYNTROPY_MM_ALLOC(size) \
    SYNTROPY_ALLOC(syntropy::GetMemoryManager().GetAllocator(), size)

/// \brief Free a buffer that was allocated via an allocator registered to syntropy::MemoryManager.
/// Searches for any allocator owned by MemoryManager that can deallocate the object, regardless of the current active allocator and the allocator who made the allocation.
/// \usage SYNTROPY_MM_FREE(buffer);
#define SYNTROPY_MM_FREE(ptr) \
    SYNTROPY_FREE(*syntropy::GetMemoryManager().GetAllocator(ptr), ptr)

namespace syntropy
{

    /// \brief Class used to manage application allocators and allocation contexts.
    /// This singleton is meant to handle allocators that exist for application's entire duration.
    /// The manager can take ownership of existing allocators: once taken those allocator lasts until the application is closed.
    /// Each thread has its own stack of allocators: whenever a new dynamic allocation request is performed (see SYNTROPY_MM_NEW \ SYNTROPY_MM_ALLOC),
    /// the request is forwarded to the allocator on top of the stack. If the stack is empty, the request is handled by the default allocator.
    /// Use syntropy::MemoryContext to push new allocators.
    /// Use ::SetDefaultAllocator to set a new default allocator. If no default allocator is specified, the first allocator added to the manager becomes the default allocator.
    /// 
    /// \author Raffaele D. Facendola - March 2017
    class MemoryManager
    {
    public:

        /// \brief Get the singleton instance.
        /// \return Returns the singleton instance.
        static MemoryManager& GetInstance();

        /// \brief No copy constructor.
        MemoryManager(const MemoryManager&) = delete;

        /// \brief No assignment operator.
        MemoryManager& operator=(const MemoryManager&) = delete;

        /// \brief Default destructor.
        ~MemoryManager() = default;

        /// \brief Add an allocator to the memory manager.
        /// The memory manager takes ownership of the allocator.
        /// \return Returns a reference to the allocator.
        template <typename TAllocator>
        TAllocator& AcquireAllocator(std::unique_ptr<TAllocator> allocator);

        /// \brief Set the default allocator.
        /// The default allocator is the allocator that is used when the allocator stack is empty.
        /// \param allocator_name Name of the new default allocator.
        /// \return Returns true if the default allocator was set successfully, returns false otherwise.
        /// \remarks The default allocator must have been created via CreateAllocator method.
        bool SetDefaultAllocator(const HashedString& allocator_name);

        /// \brief Get the default allocator.
        /// \return Returns the default allocator.
        Allocator& GetDefaultAllocator();

        /// \brief Get the current active allocator.
        /// \return Returns the allocator on top of the allocator stack.
        Allocator& GetAllocator();

        /// \brief Get the allocator owning a memory block.
        /// \return Returns the allocator owning the specified memory block. If no such allocator exists, returns nullptr.
        /// \remarks This method searches only inside the allocators owned by the MemoryManager.
        Allocator* GetAllocator(void* block);

    private:

        friend class MemoryContext;

        /// \brief Push a named allocator on top of the current allocator stack.
        /// If no matching allocator was found, pushes the default allocator on top of the stack.
        /// The allocator may or may not be owned by the manager: any named allocator can be chosen. See Allocator.
        /// \param allocator_name Name of the allocator to push on top of the stack.
        void PushContext(const HashedString& allocator_name);

        /// \brief Pop the last allocator from the allocator stack.
        void PopContext();

        /// \brief Private constructor.
        MemoryManager();

        std::vector<std::unique_ptr<Allocator>> allocators_;                ///< \brief List of allocators in this manager. The first element is the default allocator.

        static thread_local std::vector<Allocator*> allocator_stack_;       ///< \brief Current stack of allocators.

    };

    /// \brief Get a reference to the MemoryManager singleton.
    MemoryManager& GetMemoryManager();

    /// \brief Import a memory manager configuration from JSON file.
    /// Existing allocators are preserved, default allocator may change.
    /// \param Path of the file containing the configuration.
    /// \return Returns true if the configuration could be imported successfully, returns false otherwise.
    bool ImportMemoryConfigurationFromJSON(const std::string& path);

    /// \brief Class used to push and pop a memory context via RAII paradigm.
    ///
    /// A memory context defines which allocator will handle dynamic allocation requests performed via SYNTROPY_MM_NEW and SYNTROPY_MM_ALLOC
    ///
    /// An allocation context is valid only within the current thread, different threads may have different contexts at any given time.
    /// Allocation contexts may be nested but not overlapped.
    ///
    /// If no matching context is found the default context is used instead. This ensures that, in case of a missing context, allocations are not spread
    /// on other allocators.
    ///
    /// \usage: 
    /// {
    ///     MemoryContext ctx("foo_allocator")
    ///
    ///     ... allocation performed here via SYNTROPY_MM_NEW are now forwarded to "foo_allocator" ...
    ///
    /// }   // The context is no longer active here
    ///
    /// \author Raffaele D. Facendola - March 2017
    class MemoryContext
    {
    public:

        /// \brief Push a new memory context.
        /// \param context_name Name of the context to push (allocator name).
        MemoryContext(const HashedString& context_name);

        /// \brief Destructor. Pop the previously pushed context.
        ~MemoryContext();

    };

}

// Implementation

namespace syntropy
{

    /************************************************************************/
    /* MEMORY MANAGER                                                       */
    /************************************************************************/

    template <typename TAllocator>
    TAllocator& MemoryManager::AcquireAllocator(std::unique_ptr<TAllocator> allocator)
    {
        static_assert(std::is_base_of_v<Allocator, TAllocator>, "TAllocator must derive from syntropy::Allocator");

        allocators_.push_back(std::move(allocator));                    // Acquire allocator's ownership.

        return *static_cast<TAllocator*>(allocators_.back().get());
    }

}