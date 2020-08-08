
/// \file stack_allocator.h
/// \brief This header is part of the Syntropy memory module. It contains definitions for stack allocators (aka linear allocators).
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/math/math.h"
#include "syntropy/core/types.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* STACK ALLOCATOR <ALLOCATOR>                                          */
    /************************************************************************/

    /// \brief Tier 1 allocator that use an underlying allocator to allocate over a contiguous range of memory addresses.
    /// Memory is allocated sequentially and divided into chunks. Pointer-level deallocation is not supported.
    /// When the current allocation chunk is exhausted a new chunk is requested from the underlying allocator automatically.
    /// \author Raffaele D. Facendola - January 2017, August 2018
    template <typename TAllocator>
    class StackAllocator
    {
    public:

        /// \brief A checkpoint used to restore the allocator status.
        class TCheckpoint;

        /// \brief Create a new allocator.
        /// \param granularity Allocation granularity for each allocated chunk.
        template <typename... TArguments>
        StackAllocator(Bytes granularity, TArguments&&... arguments) noexcept;

        /// \brief No copy constructor.
        StackAllocator(const StackAllocator&) = delete;

        /// \brief Move constructor.
        StackAllocator(StackAllocator&& rhs) noexcept;

        /// \brief Destructor.
        ~StackAllocator();

        /// \brief Unified assignment operator.
        StackAllocator& operator=(StackAllocator rhs) noexcept;

        /// \brief Allocate a new memory block.
        /// If a memory block could not be allocated, returns an empty block.
        RWByteSpan Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const RWByteSpan& block, Alignment alignment) noexcept;

        /// \brief Deallocate every allocation performed on this allocator so far, invalidating all outstanding checkpoints.
        void DeallocateAll() noexcept;

        /// \brief Check whether this allocator owns a memory block.
        Bool Owns(const ByteSpan& block) const noexcept;

        /// \brief Swap this memory resource with the provided instance.
        void Swap(StackAllocator& rhs) noexcept;

        /// \brief Get the current state of the allocator.
        TCheckpoint Checkpoint() const;

        /// \brief Restore the allocator to a previous state.
        /// If the provided checkpoint wasn't obtained by means of ::Checkpoint(), the behavior of this method is undefined.
        /// This method invalidates all checkpoints obtained after the provided checkpoint. Rewinding to an invalid checkpoint results in undefined behavior.
        void Rewind(const TCheckpoint& checkpoint);

    private:

        /// \brief A chunk in the allocation chain.
        struct Chunk;

        /// \brief Underlying memory resource.
        TAllocator allocator_;

        /// \brief Size of each chunk.
        Bytes chunk_size_;

        /// \brief Current active chunk.
        Chunk* chunk_{ nullptr };

    };

    /************************************************************************/
    /* LINEAR MEMORY RESOURCE <ALLOCATOR> :: CHECKPOINT                     */
    /************************************************************************/

    /// \brief Represents a checkpoint used to rewind a virtual stack allocator back to a previous state.
    template <typename TAllocator>
    class StackAllocator<TAllocator>::TCheckpoint
    {
        friend class StackAllocator<TAllocator>;

        /// \brief Chunk when the checkpoint was initially created.
        Pointer<Chunk> chunk_;

        /// \brief Unallocated storage when the checkpoint was initially created.
        RWByteSpan unallocated_;
    };

    /************************************************************************/
    /* LINEAR MEMORY RESOURCE <ALLOCATOR> :: CHUNK                          */
    /************************************************************************/

    /// \brief A chunk in the allocation chain.
    template <typename TAllocator>
    struct StackAllocator<TAllocator>::Chunk
    {
        /// \brief Pointer to the previous chunk.
        Pointer<Chunk> previous_;

        /// \brief Chunk span.
        RWByteSpan chunk_;

        /// \brief Available memory storage in the chunk.
        RWByteSpan storage_;

        /// \brief Unallocated span in the chunk.
        RWByteSpan unallocated_;
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Swaps two SequentialMemoryResource.
    template <typename TAllocator>
    void swap(syntropy::StackAllocator<TAllocator>& lhs, syntropy::StackAllocator<TAllocator>& rhs) noexcept;
 
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // StackAllocator<TAllocator>.
    // ======================================

    template <typename TAllocator>
    template <typename... TArguments>
    inline StackAllocator<TAllocator>::StackAllocator(Bytes granularity, TArguments&&... arguments) noexcept
        : allocator_(std::forward<TArguments>(arguments)...)
        , granularity_(granularity)
    {

    }

    template <typename TAllocator>
    inline StackAllocator<TAllocator>::StackAllocator(StackAllocator&& rhs) noexcept
        : allocator_(std::move(rhs.allocator_))
        , granularity_(rhs.granularity_)
        , chunk_(rhs.chunk_)
    {
        rhs.chunk_ = nullptr;
    }

    template <typename TAllocator>
    inline StackAllocator<TAllocator>::~StackAllocator()
    {
        DeallocateAll();
    }

    template <typename TAllocator>
    inline StackAllocator<TAllocator>& StackAllocator<TAllocator>::operator=(StackAllocator rhs) noexcept
    {
        rhs.Swap(*this);
        return *this;
    }

    template <typename TAllocator>
    RWByteSpan StackAllocator<TAllocator>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        // Attempt to allocate on the current chunk. Fast-path.

        if(chunk_) 
        {
            if (auto unallocated = Memory::Align(chunk_->unallocated_, alignment); Size(unallocated) >= size)
            {
                auto block = Front(unallocated, ToInt(size));

                chunk_->unallocated = DifferenceFront(chunk_->storage_, block);

                return block;
            }
        }

        // Allocate a new chunk accounting for Chunk header and alignment requirements. Performance cost depends on the underlying memory resource.

        auto chunk_size = Ceil(SizeOf<Chunk>() + size + alignment - ToBytes(1), chunk_size_);

        if (auto block = allocator_.Allocate(chunk_size))
        {
            // Link to the previous chunk.

            auto chunk = FromTypeless<Chunk>(Begin(block));

            chunk->previous_ = chunk_;
            chunk->chunk_ = block;
            chunk->storage_ = PopFront(block, ToInt(SizeOf<Chunk>()));
            chunk->unallocated_ = chunk_->storage_;

            chunk_ = chunk;

            // Allocate the block from the new chunk.

            if (auto unallocated = Memory::Align(chunk_->unallocated_, alignment); Size(unallocated) >= size)
            {
                auto block = Front(unallocated, ToInt(size));

                chunk_->unallocated = DifferenceFront(chunk_->storage_, block);

                return block;
            }
        }

        return {};              // Out-of-memory.
    }

    template <typename TAllocator>
    inline void StackAllocator<TAllocator>::Deallocate(const RWByteSpan& block, Alignment /*alignment*/) noexcept
    {
        SYNTROPY_ASSERT(Owns(block));
    }

    template <typename TAllocator>
    inline void StackAllocator<TAllocator>::DeallocateAll() noexcept
    {
        for (; chunk_ != nullptr;)
        {
            auto previous = chunk_->previous_;

            allocator_.Deallocate(chunk_->chunk_);

            chunk_ = previous;
        }

        head_ = nullptr;
    }

    template <typename TAllocator>
    inline Bool StackAllocator<TAllocator>::Owns(const ByteSpan& block) const noexcept
    {
        // Can't query the underlying memory resource directly since it might be shared with other allocators.

        for (auto chunk = chunk_; chunk; chunk = chunk->previous_)
        {
            if (chunk->chunk_.Contains(block))
            {
                return true;
            }
        }

        return false;
    }

    template <typename TAllocator>
    inline void StackAllocator<TAllocator>::Swap(StackAllocator& rhs) noexcept
    {
        using std::swap;

        swap(allocator_, rhs.allocator_);
        swap(chunk_size_, rhs.chunk_size_);
        swap(chunk_, rhs.chunk_);
        swap(head_, rhs.head_);
    }

    template <typename TAllocator>
    inline StackAllocator<TAllocator>::TCheckpoint StackAllocator<TAllocator>::Checkpoint() const
    {
        auto checkpoint = TCheckpoint{};

        checkpoint.chunk_ = chunk_;
        checkpoint.checkpoint_ = chunk_ ? chunk_->unallocated_ : nullptr;

        return checkpoint;
    }

    template <typename TAllocator>
    inline void StackAllocator<TAllocator>::Rewind(const TCheckpoint& checkpoint)
    {
        // Deallocate chunks until the checkpoint chunk becomes the active one.

        for (auto chunk = chunk_; chunk && (checkpoint_->chunk_ != chunk_);)
        {
            auto previous = chunk->previous_;

            allocator_.Deallocate(chunk->chunk_);

            chunk = previous;
        }

        // Rewind the current chunk state.

        chunk_ = checkpoint_->chunk_;
        
        if (chunk_)
        {
            chunk_->unallocated_ = checkpoint_.unallocated_;
        }
    }

    // Non-member functions.

    template <typename TAllocator>
    inline void swap(syntropy::StackAllocator<TAllocator>& lhs, syntropy::StackAllocator<TAllocator>& rhs) noexcept
    {
        lhs.Swap(rhs);
    }

}
