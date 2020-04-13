
/// \file polymorphic_allocator.h
/// \brief This header is part of the syntropy memory management. It contains definitions of polymorphic allocators.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/allocators/polymorphic_memory_resource.h"

#include "syntropy/allocators/memory_resource.h"

namespace syntropy
{
    /************************************************************************/
    /* POLYMORPHIC ALLOCATOR <TTYPE>                                        */
    /************************************************************************/

    /// \brief Allocator whose allocation behavior depends on the memory resource it is constructed with.
    /// This class is meant to mimic and replace the usage of std::pmr::polymorphic_allocator when consistent multithread behavior is required.
    /// The standard defines a global, atomic default memory resource which can be changed via std::pmr::set_default_resource. In multithreaded environment
    /// this method is deemed to be harmful since it changes the memory resource of every other thread.
    /// \author Raffaele D. Facendola - April 2020.
    template <typename TType>
    class PolymorphicAllocator
    {
    public:

        template<typename>
        friend class PolymorphicAllocator;

        using value_type = TType;

        /// \brief Default constructor.
        PolymorphicAllocator() noexcept = default;

        /// \brief Create a new polymorphic allocator with explicit memory resource.
        PolymorphicAllocator(PolymorphicMemoryResource& memory_resource) noexcept;

        /// \brief Default copy constructor.
        PolymorphicAllocator(const PolymorphicAllocator&) = default;

        /// \brief Construct a new polymorphic allocator using rhs's memory resource.
        template<class UType>
        PolymorphicAllocator(const PolymorphicAllocator<UType>& rhs) noexcept;

        /// \brief No assignment operator.
        PolymorphicAllocator& operator=(const PolymorphicAllocator&) = delete;

        /// \brief Get the underlying memory resource.
        [[nodiscard]] PolymorphicMemoryResource& GetMemoryResource() const noexcept;

        /// \brief Allocate storage for count objects of type TType using the underlying memory resource.
        [[nodiscard]] TType* allocate(std::size_t count);

        /// \brief Deallocate the storage pointed to by storage, which must have been allocated from a MemoryResorce that compares equal to the underlying resource.
        void deallocate(TType* storage, size_t count) noexcept;

        /// \brief Construct an object in a given storage.
        template <typename UType, typename... TArguments>
        void construct(UType* storage, TArguments&&... arguments);

        /// \brief Return a default-constructed PolymorphicAllocator object.
        [[nodiscard]] PolymorphicAllocator<TType> select_on_container_copy_construction() const noexcept;

    private:

        /// \brief Underlying memory resource.
        PolymorphicMemoryResource* memory_resource_ = &MemoryResource::GetDefaultResource();

    };

    template <typename TType, typename UType>
    [[nodiscard]] bool operator==(const PolymorphicAllocator<TType>& lhs, const PolymorphicAllocator<UType>& rhs) noexcept;

    template <typename TType, typename UType>
    [[nodiscard]] bool operator!=(const PolymorphicAllocator<TType>& lhs, const PolymorphicAllocator<UType>& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // PolymorphicAllocator<TType>.

    template <typename TType>
    inline PolymorphicAllocator<TType>::PolymorphicAllocator(PolymorphicMemoryResource& memory_resource) noexcept
        : memory_resource_{ std::addressof(memory_resource) }
    {

    }

    template <typename TType>
    template<class UType>
    inline PolymorphicAllocator<TType>::PolymorphicAllocator(const PolymorphicAllocator<UType>& rhs) noexcept
            : memory_resource_{ rhs.memory_resource_ }
    {

    }

    template <typename TType>
    inline PolymorphicMemoryResource& PolymorphicAllocator<TType>::GetMemoryResource() const noexcept
    {
        return *memory_resource_;
    }

    template <typename TType>
    inline TType* PolymorphicAllocator<TType>::allocate(std::size_t count)
    {
        auto block = memory_resource_->Allocate(BytesOf<TType>() * count, AlignmentOf<TType>());

        return block.Begin().As<TType>();
    }

    template <typename TType>
    inline void PolymorphicAllocator<TType>::deallocate(TType* storage, size_t count) noexcept
    {
        auto block = MemoryRange{ storage, BytesOf<TType>() * count };

        memory_resource_->Deallocate(block, AlignmentOf<TType>());
    }

    template <typename TType>
    template <typename UType, typename... TArguments>
    inline void PolymorphicAllocator<TType>::construct(UType* storage, TArguments&&... arguments)
    {
        new (storage) TType(std::forward<TArguments>(arguments)...);
    }
 
    template <typename TType>
    inline PolymorphicAllocator<TType> PolymorphicAllocator<TType>::select_on_container_copy_construction() const noexcept
    {
        return {};
    }

    template <typename TType, typename UType>
    inline bool operator==(const PolymorphicAllocator<TType>& lhs, const PolymorphicAllocator<UType>& rhs) noexcept
    {
        return (&lhs.GetMemoryResource() == &rhs.GetMemoryResource());
    }

    template <typename TType, typename UType>
    inline bool operator!=(const PolymorphicAllocator<TType>& lhs, const PolymorphicAllocator<UType>& rhs) noexcept
    {
        return !(lhs == rhs);
    }

}
