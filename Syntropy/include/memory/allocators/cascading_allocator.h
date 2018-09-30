
/// \file cascading_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains allocators that sits on top of an underlying allocator and are constructed by need.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "memory/bytes.h"
#include "memory/alignment.h"
#include "memory/memory_address.h"
#include "memory/memory_range.h"
#include "memory/allocators/counting_allocator.h"

#include <type_traits>
#include <functional>

namespace syntropy
{
    /************************************************************************/
    /* CASCADING ALLOCATOR                                                  */
    /************************************************************************/

    /// \brief Allocator that spawns many sub-allocators sitting on top of a single underlying allocator.
    /// Sub-allocators are spawned by need and returned to the underlying allocator when empty.
    /// \tparam TAllocator Type of the underlying allocator.
    /// \tparam TCascade Type of the sub-allocators. Must be move-constructible.
    /// \author Raffaele D. Facendola - August 2018
    template <typename TAllocator, typename TCascade>
    class CascadingAllocator
    {
    public:

        /// \brief Create a new allocator.
        /// \param cascade_capacity Capacity of each cascade, in bytes.
        /// \param cascade_constructor Functor used to construct a cascade. Must be of the form: MemoryRange -> TCascade.
        /// \param arguments Arguments to pass to the underlying allocator.
        template <typename TCascadeConstructor, typename... TArguments>
        CascadingAllocator(Bytes cascade_capacity, TCascadeConstructor&& cascade_constructor, TArguments&&... arguments);

        /// \brief No copy constructor.
        CascadingAllocator(const CascadingAllocator&) = delete;

        /// \brief Move constructor.
        CascadingAllocator(CascadingAllocator&& rhs) noexcept;

        /// \brief Default destructor.
        /// Cascades are returned to the underlying allocator when the allocation count reaches 0.
        /// When this allocator goes out of scope it is expected that each block allocated via this allocator was already deallocated
        /// by the user. No cascade is expected to be alive when this happens.
        ~CascadingAllocator() = default;

        /// \brief Unified assignment operator.
        CascadingAllocator& operator=(CascadingAllocator rhs) noexcept;

        /// \brief Allocate a new memory block.
        /// \param size Size of the memory block to allocate.
        /// \return Returns a range representing the requested memory block. If no allocation could be performed returns an empty range.
        MemoryRange Allocate(Bytes size) noexcept;

        /// \brief Allocate a new aligned memory block.
        /// \param size Size of the memory block to allocate.
        /// \param alignment Block alignment.
        /// \return Returns a range representing the requested aligned memory block. If no allocation could be performed returns an empty range.
        MemoryRange Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \param block Block to deallocate.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size).
        void Deallocate(const MemoryRange& block);

        /// \brief Deallocate an aligned memory block.
        /// \param block Block to deallocate. Must refer to any allocation performed via Allocate(size, alignment).
        /// \param alignment Block alignment.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const MemoryRange& block, Alignment alignment);

        /// \brief Check whether this allocator owns the provided memory block.
        /// \param block Block to check the ownership of.
        /// \return Returns true if the provided memory range was allocated by this allocator, returns false otherwise.
        bool Owns(const MemoryRange& block) const noexcept;

        /// \brief Swap this allocator with the provided instance.
        void Swap(CascadingAllocator& rhs) noexcept;

    private:

        /// \brief Type of the function used to construct cascades.
        using CascadeConstructor = std::function<TCascade(const MemoryRange&)>;

        /// \brief Represents a cascade allocator linked to other cascades.
        struct Cascade
        {
            /// \brief Create a new cascade from an explicit allocator.
            Cascade(TCascade&& allocator);

            Cascade* next_{ nullptr };                      ///< \brief Next cascade.

            Cascade* previous_{ nullptr };                  ///< \brief Previous cascade.

            CountingAllocator<TCascade> allocator_;         ///< \brief Cascade allocator.
        };

        /// \brief Perform an allocation on the first available cascade. If no such cascade exists spawn a new one.
        /// \param allocate Allocation functor.
        /// \return Returns the allocated memory block. If the allocation could not be performed, returns an empty range.
        template <typename TAllocation>
        MemoryRange AllocateOnCascade(TAllocation&& allocate) noexcept;

        /// \brief Perform a deallocation on the proper cascade.
        /// \param block Block to deallocate.
        /// \param deallocate Deallocation functor.
        template <typename TDeallocation>
        void DeallocateOnCascade(const MemoryRange& block, TDeallocation&& deallocate) noexcept;

        /// \brief Create a new cascade.
        /// \return Returns the new cascade.
        Cascade* CreateCascade() noexcept;

        /// \brief Check whether the provided cascade is linked to the free list.
        /// \return Returns tue if the cascade is linked to the free list, returns false otherwise.
        bool IsLinked(Cascade& cascade) const noexcept;

        /// \brief Link the provided cascade to the free list.
        /// \param cascade Cascade to link.
        void LinkCascade(Cascade& cascade) noexcept;

        /// \brief Unlink the provided cascade from the free list.
        /// \param cascade Cascade to unlink.
        void UnlinkCascade(Cascade& cascade) noexcept;

        Bytes cascade_capacity_;                                ///< \brief Capacity of each cascade. The capacity is shared among the space used to perform allocations and the actual allocator state.

        Alignment cascade_alignment_;                           ///< \brief Alignment of each cascade. Used to quickly determine which cascade a block belongs to.

        Cascade* cascade_{ nullptr };                           ///< \brief Available cascade list.

        TAllocator allocator_;                                  ///< \brief Underlying allocator used to allocate cascades.

        CascadeConstructor cascade_constructor_;                ///< \brief Function used to create a cascade given a storage and a memory range to work on.
    };

}

/// \brief Swaps two syntropy::CascadingAllocator<> instances.
template <typename TAllocator, typename TCascade>
void swap(syntropy::CascadingAllocator<TAllocator, TCascade>& lhs, syntropy::CascadingAllocator<TAllocator, TCascade>& rhs) noexcept;

namespace syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    template <typename TAllocator, typename TCascade>
    template <typename TCascadeConstructor, typename... TArguments>
    inline CascadingAllocator<TAllocator, TCascade>::CascadingAllocator(Bytes cascade_capacity, TCascadeConstructor&& cascade_constructor, TArguments&&... arguments)
        : cascade_capacity_(cascade_capacity)
        , cascade_alignment_(Alignment(Math::NextPow2(cascade_capacity)))
        , allocator_(std::forward<TArguments>(arguments)...)
        , cascade_constructor_(std::forward<TCascadeConstructor>(cascade_constructor))
    {
        
    }

    template <typename TAllocator, typename TCascade>
    inline CascadingAllocator<TAllocator, TCascade>::CascadingAllocator(CascadingAllocator&& rhs) noexcept
        : cascade_capacity_(std::move(rhs.cascade_capacity_))
        , cascade_alignment_(std::move(rhs.cascade_alignment_))
        , cascade_(std::move(rhs.cascade_))
        , allocator_(std::move(rhs.allocator_))
        , cascade_constructor_(std::move(rhs.cascade_constructor_))
    {

    }

    template <typename TAllocator, typename TCascade>
    inline CascadingAllocator<TAllocator, TCascade>& CascadingAllocator<TAllocator, TCascade>::operator=(CascadingAllocator rhs) noexcept
    {
        rhs.Swap(*this);
        return *this;
    }

    template <typename TAllocator, typename TCascade>
    inline MemoryRange CascadingAllocator<TAllocator, TCascade>::Allocate(Bytes size) noexcept
    {
        return AllocateOnCascade([size](auto& allocator) { return allocator.Allocate(size); });
    }

    template <typename TAllocator, typename TCascade>
    inline MemoryRange CascadingAllocator<TAllocator, TCascade>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        return AllocateOnCascade([size, alignment](auto& allocator) { return allocator.Allocate(size, alignment); });
    }

    template <typename TAllocator, typename TCascade>
    inline void CascadingAllocator<TAllocator, TCascade>::Deallocate(const MemoryRange& block)
    {
        return DeallocateOnCascade(block, [&block](auto& allocator) { allocator.Deallocate(block); });
    }

    template <typename TAllocator, typename TCascade>
    inline void CascadingAllocator<TAllocator, TCascade>::Deallocate(const MemoryRange& block, Alignment alignment)
    {
        return DeallocateOnCascade(block, [&block, alignment](auto& allocator) { allocator.Deallocate(block, alignment); });
    }

    template <typename TAllocator, typename TCascade>
    inline bool CascadingAllocator<TAllocator, TCascade>::Owns(const MemoryRange& block) const noexcept
    {
        return allocator_.Owns(block);
    }

    template <typename TAllocator, typename TCascade>
    inline void CascadingAllocator<TAllocator, TCascade>::Swap(CascadingAllocator& rhs) noexcept
    {
        using std::swap;

        swap(cascade_capacity_, rhs.cascade_capacity_);
        swap(cascade_alignment_, rhs.cascade_alignment_);
        swap(cascade_, rhs.cascade_);
        swap(allocator_, rhs.allocator_);
        swap(cascade_constructor_, rhs.cascade_constructor_);
    }

    template <typename TAllocator, typename TCascade>
    template <typename TAllocation>
    MemoryRange CascadingAllocator<TAllocator, TCascade>::AllocateOnCascade(TAllocation&& allocate) noexcept
    {
        // Search the first cascade able to fit the requested allocation.

        for (auto cascade = cascade_; cascade; cascade = cascade->next_)
        {
            if (auto block = allocate(cascade->allocator_))                     // Attempt to allocate on a free cascade.
            {
                return block;
            }
            else if (cascade->allocator_.GetAllocationCount() == 0u)            // If the allocation failed on a pristine allocator it means the allocation cannot be handled at all.
            {
                return {};
            }
            else                                                                // Assumes the allocator is about to run out-of-memory. Unlink the cascade in order to avoid future allocation attempts and keep the response time low.
            {
                UnlinkCascade(*cascade);
            }
        }

        // None of the existing cascades was able to handle the request: spawn a new cascade.

        if (auto cascade = CreateCascade())
        {
            return allocate(cascade->allocator_);
        }

        // The underlying allocator ran out of memory.

        return {};
    }

    template <typename TAllocator, typename TCascade>
    template <typename TDeallocation>
    void CascadingAllocator<TAllocator, TCascade>::DeallocateOnCascade(const MemoryRange& block, TDeallocation&& deallocate) noexcept
    {
        SYNTROPY_ASSERT(allocator_.Owns(block));

        auto cascade = block.Begin().GetAlignedDown(cascade_alignment_).As<Cascade>();

        deallocate(cascade->allocator_);                                                                    // Deallocate the block on the proper cascade.

        if (cascade->allocator_.GetAllocationCount() == 0u)                                                 // If the cascade becomes empty return it to the underlying allocator.
        {
            UnlinkCascade(*cascade);

            cascade->~Cascade();                                                                            // Destroy the cascade.

            auto cascade_range = MemoryRange({ cascade, MemoryAddress(cascade) + cascade_capacity_ });

            allocator_.Deallocate(cascade_range, cascade_alignment_);
        }
        else if (!IsLinked(*cascade))                                                                       // If the cascade wasn't already linked to the free list, link it now.
        {
            LinkCascade(*cascade);
        }
    }

    template <typename TAllocator, typename TCascade>
    typename CascadingAllocator<TAllocator, TCascade>::Cascade* CascadingAllocator<TAllocator, TCascade>::CreateCascade() noexcept
    {
        if (auto block = allocator_.Allocate(cascade_capacity_, cascade_alignment_))
        {
            auto cascade = block.Begin().As<Cascade>();

            auto cascade_range = MemoryRange(cascade + 1, block.End());                             // The cascade range is adjacent to the cascade data.

            new (cascade) Cascade(cascade_constructor_(cascade_range));                             // Construct the cascade in-place.

            LinkCascade(*cascade);

            return cascade;
        }

        return nullptr;                                                                             // The underlying allocator could not handle the request.
    }

    template <typename TAllocator, typename TCascade>
    inline bool CascadingAllocator<TAllocator, TCascade>::IsLinked(Cascade& cascade) const noexcept
    {
        return cascade_ == &cascade || cascade.previous_;           // Either the cascade is the head of the free list or it is accessible via some previous cascade.
    }

    template <typename TAllocator, typename TCascade>
    inline void CascadingAllocator<TAllocator, TCascade>::LinkCascade(Cascade& cascade) noexcept
    {
        cascade.next_ = cascade_;

        if (cascade_)
        {
            cascade_->previous_ = &cascade;
        }

        cascade_ = &cascade;
    }

    template <typename TAllocator, typename TCascade>
    inline void CascadingAllocator<TAllocator, TCascade>::UnlinkCascade(Cascade& cascade) noexcept
    {
        if (cascade.next_)
        {
            cascade.next_->previous_ = cascade.previous_;
        }

        if (cascade.previous_)                                      // The cascade was somewhere in the middle of the free list.
        {
            cascade.previous_->next_ = cascade.next_;
        }
        else                                                        // The cascade was the head of the free list.
        {
            cascade_ = cascade.next_;
        }

        cascade.previous_ = nullptr;                                // The cascade is no longer accessible from the predecessor.
    }

    template <typename TAllocator, typename TCascade>
    inline CascadingAllocator<TAllocator, TCascade>::Cascade::Cascade(TCascade&& allocator)
        : allocator_(std::move(allocator))
    {

    }

}

template <typename TAllocator, typename TCascade>
inline void swap(syntropy::CascadingAllocator<TAllocator, TCascade>& lhs, syntropy::CascadingAllocator<TAllocator, TCascade>& rhs) noexcept
{
    lhs.Swap(rhs);
}
