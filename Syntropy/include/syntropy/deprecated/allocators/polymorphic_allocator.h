
/// \file polymorphic_allocator.h
/// \brief This header is part of the syntropy memory management. It contains definitions of polymorphic allocators.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <tuple>

#include "syntropy/type_traits.h"
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
        PolymorphicAllocator(MemoryResource& memory_resource) noexcept;

        /// \brief Default copy constructor.
        PolymorphicAllocator(const PolymorphicAllocator&) = default;

        /// \brief Construct a new polymorphic allocator using rhs's memory resource.
        template<class UType>
        PolymorphicAllocator(const PolymorphicAllocator<UType>& rhs) noexcept;

        /// \brief No assignment operator.
        PolymorphicAllocator& operator=(const PolymorphicAllocator&) = delete;

        /// \brief Get the underlying memory resource.
        [[nodiscard]] MemoryResource& GetMemoryResource() const noexcept;

        /// \brief Allocate storage for count objects of type TType using the underlying memory resource.
        [[nodiscard]] TType* allocate(std::size_t count);

        /// \brief Deallocate the storage pointed to by storage, which must have been allocated from a MemoryResorce that compares equal to the underlying resource.
        void deallocate(TType* storage, size_t count) noexcept;

        /// \brief Construct an object in a given storage.
        template <typename UType, typename... UArguments, typename = std::enable_if_t<!is_specialization_v<UType, std::pair>>>
        void construct(UType* storage, UArguments&&... arguments);

        /// \brief Construct an object.
        template <typename UType1, typename UType2, typename... UArguments1, typename... UArguments2>
        void construct(std::pair<UType1, UType2>* storage, std::piecewise_construct_t, std::tuple<UArguments1...> x, std::tuple<UArguments2...> y);

        /// \brief Construct an object.
        template <typename UType1, typename UType2>
        void construct(std::pair<UType1, UType2>* storage);

        /// \brief Construct an object.
        template <typename UType1, typename UType2, typename UType3, typename UType4>
        void construct(std::pair<UType1, UType2>* storage, UType3&& x, UType4&& y);

        /// \brief Construct an object.
        template <typename UType1, typename UType2, typename UType3, typename UType4>
        void construct(std::pair<UType1, UType2>* storage, const std::pair<UType3, UType4>& xy);

        /// \brief Construct an object.
        template <typename UType1, typename UType2, typename UType3, typename UType4>
        void construct(std::pair<UType1, UType2>* storage, std::pair<UType3, UType4>&& xy);

        /// \brief Return a default-constructed PolymorphicAllocator object.
        [[nodiscard]] PolymorphicAllocator<TType> select_on_container_copy_construction() const noexcept;

    private:

        /// \brief Underlying memory resource.
        MemoryResource* memory_resource_ = &GetDefaultMemoryResource();

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
    inline PolymorphicAllocator<TType>::PolymorphicAllocator(MemoryResource& memory_resource) noexcept
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
    inline MemoryResource& PolymorphicAllocator<TType>::GetMemoryResource() const noexcept
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
    template <typename UType, typename... TArguments, typename>
    inline void PolymorphicAllocator<TType>::construct(UType* storage, TArguments&&... arguments)
    {
        using TOuterAlloc = std::allocator<char>;
        using TInnerAlloc = PolymorphicAllocator<TType>;

        auto outer_allocator = TOuterAlloc{};

        // Uses-allocator construction: propagate *this if UType uses a PolymorphicAllocator, otherwise ignore it.

        if constexpr (std::uses_allocator_v<UType, TInnerAlloc> && std::is_constructible_v<UType, std::allocator_arg_t, TInnerAlloc&, TArguments...>)
        {
            // Leading-allocator convention.

            std::allocator_traits<TOuterAlloc>::construct(outer_allocator, storage, std::allocator_arg, *this, std::forward<TArguments>(arguments)...);
        }
        else if constexpr (std::uses_allocator_v<UType, TInnerAlloc>)
        {
            // Trailing-allocator convention.

            std::allocator_traits<TOuterAlloc>::construct(outer_allocator, storage, std::forward<TArguments>(arguments)..., *this);
        }
        else
        {
            // UType doesn't use this allocator.

            std::allocator_traits<TOuterAlloc>::construct(outer_allocator, storage, std::forward<TArguments>(arguments)...);
        }
    }
 
    template <typename TType>
    template <typename UType1, typename UType2, typename... UArguments1, typename... UArguments2>
    inline void PolymorphicAllocator<TType>::construct(std::pair<UType1, UType2>* storage, std::piecewise_construct_t, std::tuple<UArguments1...> x, std::tuple<UArguments2...> y)
    {
        constexpr auto kType1AllocatorAware = std::uses_allocator_v<UType1, PolymorphicAllocator>;
        constexpr auto kType2AllocatorAware = std::uses_allocator_v<UType2, PolymorphicAllocator>;




        //::new (static_cast<void*>(storage)) std::pair<UType1, UType2>(std::piecewise_construct, std::move(x_prime), std::move(y_prime));
    }

    template <typename TType>
    template <typename UType1, typename UType2>
    inline void PolymorphicAllocator<TType>::construct(std::pair<UType1, UType2>* storage)
    {
        construct(storage, std::piecewise_construct, std::tuple<>(), std::tuple<>());
    }

    template <typename TType>
    template <typename UType1, typename UType2, typename UType3, typename UType4>
    inline void PolymorphicAllocator<TType>::construct(std::pair<UType1, UType2>* storage, UType3&& x, UType4&& y)
    {
        construct(storage, std::piecewise_construct, std::forward_as_tuple(std::forward<UType3>(x)), std::forward_as_tuple(std::forward<UType4>(y)));
    }

    template <typename TType>
    template <typename UType1, typename UType2, typename UType3, typename UType4>
    inline void PolymorphicAllocator<TType>::construct(std::pair<UType1, UType2>* storage, const std::pair<UType3, UType4>& xy)
    {
        construct(storage, std::piecewise_construct, std::forward_as_tuple(xy.first), std::forward_as_tuple(xy.second));
    }

    template <typename TType>
    template <typename UType1, typename UType2, typename UType3, typename UType4>
    inline void PolymorphicAllocator<TType>::construct(std::pair<UType1, UType2>* storage, std::pair<UType3, UType4>&& xy)
    {
        construct(storage, std::piecewise_construct, std::forward_as_tuple(std::forward<UType3>(xy.first)), std::forward_as_tuple<UType4>(std::forward<UType4>(xy.second)));
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
