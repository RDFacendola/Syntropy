
/// \file polymorphic_allocator.h
/// \brief This header is part of the Syntropy allocators module. It contains definitions of polymorphic allocators.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <tuple>

#include "syntropy/language/type_traits.h"
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
        template <typename UType, typename... UArguments>
        void construct(UType* storage, UArguments&&... arguments);

        /// \brief Return a default-constructed PolymorphicAllocator object.
        [[nodiscard]] PolymorphicAllocator<TType> select_on_container_copy_construction() const noexcept;

    private:

        /// \brief Construct a pair piecewise.
        template <typename UType1, typename UType2, typename... UArguments1, typename... UArguments2>
        void ConstructPiecewise(std::pair<UType1, UType2>* storage, std::piecewise_construct_t, std::tuple<UArguments1...> x, std::tuple<UArguments2...> y);

        /// \brief Construct a pair piecewise.
        template <typename UType1, typename UType2>
        void ConstructPiecewise(std::pair<UType1, UType2>* storage);

        /// \brief Construct a pair piecewise.
        template <typename UType1, typename UType2, typename UType3, typename UType4>
        void ConstructPiecewise(std::pair<UType1, UType2>* storage, UType3&& x, UType4&& y);

        /// \brief Construct a pair piecewise.
        template <typename UType1, typename UType2, typename UType3, typename UType4>
        void ConstructPiecewise(std::pair<UType1, UType2>* storage, const std::pair<UType3, UType4>& xy);

        /// \brief Construct a pair piecewise.
        template <typename UType1, typename UType2, typename UType3, typename UType4>
        void ConstructPiecewise(std::pair<UType1, UType2>* storage, std::pair<UType3, UType4>&& xy);

        /// \brief Forward a tuple either by using this as additional element if UType is allocator-aware (as first or last tuple element) or by leaving the tuple unmodified.
        template <typename UType, typename... UArguments>
        auto ForwardTuple(std::tuple<UArguments...>&& tuple);

        /// \brief Underlying memory resource.
        MemoryResource* memory_resource_ = &GetDefaultMemoryResource();

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

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
    template <typename UType, typename... UArguments>
    inline void PolymorphicAllocator<TType>::construct(UType* storage, UArguments&&... arguments)
    {
        if constexpr (IsSpecializationV<UType, std::pair>)
        {
            ConstructPiecewise(storage, std::forward<UArguments>(arguments)...);
        }
        else if constexpr(!std::uses_allocator_v<UType, PolymorphicAllocator<TType>>)
        {
            // UType doesn't use this allocator: proceed to object construction stopping this allocator from propagating.

            ::new (static_cast<void*>(storage)) UType(std::forward<UArguments>(arguments)...);
        }
        else if constexpr (std::is_constructible_v<UType, std::allocator_arg_t, PolymorphicAllocator<TType>&, UArguments...>)
        {
            // Leading-allocator convention: propagate this as first argument of UType ctor (after the allocator_tag).

            ::new (static_cast<void*>(storage)) UType(std::allocator_arg, *this, std::forward<UArguments>(arguments)...);
        }
        else if constexpr (std::is_constructible_v<UType, UArguments..., PolymorphicAllocator<TType>>)
        {
            // Trailing-allocator convention: pass this as last argument of UType ctor.

            ::new (static_cast<void*>(storage)) UType(std::forward<UArguments>(arguments)..., *this);
        }
        else
        {
            static_assert(AlwaysFalseV<UType>, "Uses-allocator construction failed: UType is allocator-aware but doesn't use neither leading nor trailing allocator convention.");
        }
    }


    template <typename TType>
    inline PolymorphicAllocator<TType> PolymorphicAllocator<TType>::select_on_container_copy_construction() const noexcept
    {
        return {};
    }

    template <typename TType>
    template <typename UType1, typename UType2, typename... UArguments1, typename... UArguments2>
    inline void PolymorphicAllocator<TType>::ConstructPiecewise(std::pair<UType1, UType2>* storage, std::piecewise_construct_t, std::tuple<UArguments1...> x, std::tuple<UArguments2...> y)
    {
        ::new (static_cast<void*>(storage)) std::pair<UType1, UType2>(std::piecewise_construct, ForwardTuple<UType1>(std::move(x)), ForwardTuple<UType2>(std::move(y)));
    }

    template <typename TType>
    template <typename UType1, typename UType2>
    inline void PolymorphicAllocator<TType>::ConstructPiecewise(std::pair<UType1, UType2>* storage)
    {
        ConstructPiecewise(storage, std::piecewise_construct, std::tuple<>(), std::tuple<>());
    }

    template <typename TType>
    template <typename UType1, typename UType2, typename UType3, typename UType4>
    inline void PolymorphicAllocator<TType>::ConstructPiecewise(std::pair<UType1, UType2>* storage, UType3&& x, UType4&& y)
    {
        ConstructPiecewise(storage, std::piecewise_construct, std::forward_as_tuple(std::forward<UType3>(x)), std::forward_as_tuple(std::forward<UType4>(y)));
    }

    template <typename TType>
    template <typename UType1, typename UType2, typename UType3, typename UType4>
    inline void PolymorphicAllocator<TType>::ConstructPiecewise(std::pair<UType1, UType2>* storage, const std::pair<UType3, UType4>& xy)
    {
        ConstructPiecewise(storage, std::piecewise_construct, std::forward_as_tuple(xy.first), std::forward_as_tuple(xy.second));
    }

    template <typename TType>
    template <typename UType1, typename UType2, typename UType3, typename UType4>
    inline void PolymorphicAllocator<TType>::ConstructPiecewise(std::pair<UType1, UType2>* storage, std::pair<UType3, UType4>&& xy)
    {
        ConstructPiecewise(storage, std::piecewise_construct, std::forward_as_tuple(std::forward<UType3>(xy.first)), std::forward_as_tuple<UType4>(std::forward<UType4>(xy.second)));
    }

    template <typename TType>
    template <typename UType, typename... UArguments>
    inline auto PolymorphicAllocator<TType>::ForwardTuple(std::tuple<UArguments...>&& tuple)
    {
        if constexpr (!std::uses_allocator_v<UType, PolymorphicAllocator<TType>>)
        {
            // UType doesn't use PolymorphicAllocator<TType> allocator.

            return std::move(tuple);
        }
        else if constexpr (std::is_constructible_v<UType, std::allocator_arg_t, PolymorphicAllocator<TType>&, UArguments...>)
        {
            // Leading-allocator convention.

            return std::tuple_cat(std::make_tuple(std::allocator_arg, *this), std::move(tuple));
        }
        else if constexpr (std::is_constructible_v<UType, UArguments..., PolymorphicAllocator<TType>>)
        {
            // Trailing-allocator convention.

            return std::tuple_cat(std::move(tuple), std::make_tuple(*this));
        }
        else
        {
            static_assert(AlwaysFalseV<UType>, "Uses-allocator construction failed: UType is allocator-aware but doesn't use neither leading nor trailing allocator convention.");
        }
    }

    // Non-member functions.

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
