
/// \file pool_allocator.h
/// \brief This header is part of the Syntropy allocators module. It contains allocators used to allocate fixed-size blocks.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/core/smart_pointers.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/diagnostics/assert.h"

namespace Syntropy
{
    /************************************************************************/
    /* POOL ALLOCATOR <ALLOCATOR>                                           */
    /************************************************************************/

    /// \brief Tier 1 allocator that uses an underlying allocator to allocate fixed-size blocks.
    /// Blocks are aligned to their own size and organized into chunks.
    /// Chunks are allocated on demand and never deallocated. Free chunks and free blocks are recycled when possible.
    /// \author Raffaele D. Facendola - August 2018
    template <typename TAllocator>
    class PoolAllocator
    {
    public:

        /// \brief Create a new allocator.
        /// \param block_size Size of each allocated block.
        /// \param chunk_size Size of each chunk.
        /// \param Arguments used to construct the underlying allocator.
        template <typename... TArguments>
        PoolAllocator(Bytes block_size, Bytes chunk_size, TArguments&&... arguments) noexcept;

        /// \brief No copy constructor.
        PoolAllocator(const PoolAllocator&) = delete;

        /// \brief Move constructor.
        PoolAllocator(PoolAllocator&& rhs) noexcept;

        /// \brief Destructor.
        ~PoolAllocator();

        /// \brief Unified assignment operator.
        PoolAllocator& operator=(PoolAllocator rhs) noexcept;

        /// \brief Allocate a new memory block.
        RWByteSpan Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const RWByteSpan& block, Alignment alignment);

        /// \brief Check whether this allocator owns a memory block.
        Bool Owns(const ByteSpan& block) const noexcept;

        /// \brief Deallocate every allocation performed on this allocator so far, invalidating all outstanding checkpoints.
        void DeallocateAll() noexcept;

        /// \brief Swap this allocator with another one.
        void Swap(PoolAllocator& rhs) noexcept;

    private:

        /// \brief A chunk allocated on the underlying allocator.
        struct Chunk;

        /// \brief Free block.
        struct FreeBlock;

        /// \brief Get the chunk containing a block.
        /// If the provided block doesn't belong to this allocator, the behavior of this method is undefined.
        Chunk& BlockToChunk(const ByteSpan& block) const noexcept;

        /// \brief Link a chunk to a list and get the head of the list.
        Pointer<Chunk> Link(Chunk& chunk, Pointer<Chunk> list) const noexcept;

        /// \brief Allocate a new chunk and replace the active one.
        Pointer<Chunk> AllocateChunk() noexcept;

        /// \brief Allocate a block on the current chunk.
        /// \remarks If the current chunk cannot fit the allocation or there's no active chunk, the behavior of this function is undefined.
        RWByteSpan AllocateBlock() noexcept;

        /// \brief Underlying allocator. Deallocated blocks are sent to the free list and never deallocated by this allocator.
        TAllocator allocator_;

        /// \brief Size of each block.
        Bytes block_size_;

        /// \brief Size of each chunk.
        Bytes chunk_size_;

        /// \brief Linked list of chunks with at least one free block.
        Pointer<Chunk> available_chunks_{ nullptr };

        /// \brief Linked list of chunks with no free block.
        Pointer<Chunk> unavailable_chunks_{ nullptr };

    };

    /************************************************************************/
    /* POOL ALLOCATOR <ALLOCATOR> :: CHUNK                                  */
    /************************************************************************/

    /// \brief A chunk in the allocation chain.
    template <typename TAllocator>
    struct PoolAllocator<TAllocator>::Chunk
    {
        /// \brief A span covering the entire chunk.
        RWByteSpan self_;

        /// \brief A span covering the chunk payload.
        RWByteSpan payload_;

        /// \brief List of blocks (either free or allocated).
        RWByteSpan blocks_;

        /// \brief Pointer to the next chunk.
        Pointer<Chunk> next_{ nullptr };

        /// \brief Pointer to the previous chunk.
        Pointer<Chunk> previous_{ nullptr };

        /// \brief Pointer to the first free block in the chunk.
        Pointer<FreeBlock> free_{ nullptr };
    };

    /************************************************************************/
    /* POOL ALLOCATOR <ALLOCATOR> :: FREE BLOCK                             */
    /************************************************************************/

    /// \brief A free block.
    template <typename TAllocator>
    struct PoolAllocator<TAllocator>::FreeBlock
    {
        ///< \brief Next free block in the chunk.
        Pointer<FreeBlock> next_{ nullptr };
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // PoolAllocator<TAllocator>.

    template <typename TAllocator>
    template <typename... TArguments>
    inline PoolAllocator<TAllocator>::PoolAllocator(Bytes block_size, Bytes chunk_size, TArguments&&... arguments) noexcept
        : allocator_(std::forward<TArguments>(arguments)...)
        , block_size_(block_size)
        , chunk_size_(chunk_size)
    {

    }

    template <typename TAllocator>
    inline PoolAllocator<TAllocator>::PoolAllocator(PoolAllocator&& rhs) noexcept
        : allocator_(std::move(rhs.allocator_))
        , block_size_(rhs.block_size_)
        , chunk_size_(rhs.chunk_size_)
        , available_chunks_(rhs.available_chunks_)
        , unavailable_chunks_(rhs.unavailable_chunks_)
    {
        rhs.available_chunks_ = nullptr;
        rhs.unavailable_chunks_ = nullptr;
    }

    template <typename TAllocator>
    inline PoolAllocator<TAllocator>::~PoolAllocator()
    {
        DeallocateAll();
    }

    template <typename TAllocator>
    inline PoolAllocator<TAllocator>& PoolAllocator<TAllocator>::operator=(PoolAllocator rhs) noexcept
    {
        rhs.Swap(*this);

        return *this;
    }

    template <typename TAllocator>
    RWByteSpan PoolAllocator<TAllocator>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if ((size <= block_size_) && (alignment <= ToAlignment(block_size_)))
        {
            if (auto block = AllocateBlock())
            {
                return Front(block, ToInt(size));
            }
        }

        return {};
    }

    template <typename TAllocator>
    void PoolAllocator<TAllocator>::Deallocate(const RWByteSpan& block, Alignment alignment)
    {
        SYNTROPY_UNDEFINED_BEHAVIOR(Owns(block));
        SYNTROPY_UNDEFINED_BEHAVIOR(Size(block) <= block_size_);
        SYNTROPY_UNDEFINED_BEHAVIOR(alignment <= Alignment(block_size_));

        // Find the chunk containing the provided block.

        auto chunk = BlockToChunk(block);

        // Link the block with other free blocks in the chunk.

        auto free = FromTypeless<FreeBlock>(Begin(block));

        free->next_ = chunk_->free_;

        chunk_->free_ = free;

        // If the chunk was unavailable, it becomes available now.

        if (!free->next_)
        {
            available_chunks_ = Link(*free, available_chunks_);
        }

        // #TODO Release the entire chunk if it was the last allocation!
    }

    template <typename TAllocator>
    inline Bool PoolAllocator<TAllocator>::Owns(const ByteSpan& block) const noexcept
    {
        auto owns = [](auto chunk, auto block)
        {
            for (; chunk; chunk = chunk->next_)
            {
                if (Contains(chunk->payload_, block));
                {
                    return true;
                }
            }

            return false;
        }

        return owns(available_chunks_, block) || owns(unavailable_chunks_, block);
    }

    template <typename TAllocator>
    inline void PoolAllocator<TAllocator>::DeallocateAll() noexcept
    {
        auto deallocate = [](auto& chunk)
        {
            for (; chunk;)
            {
                auto next = chunk->next_;

                allocator_.Deallocate(chunk->self_, ToAlignment(chunk_size_));

                chunk = next;
            }
        };

        deallocate(available_chunks_);
        deallocate(unavailable_chunks_);
    }

    template <typename TAllocator>
    inline void PoolAllocator<TAllocator>::Swap(PoolAllocator& rhs) noexcept
    {
        using std::swap;

        swap(allocator_, rhs.allocator_);
        swap(chunk_size_, rhs.chunk_size_);
        swap(block_size_, rhs.block_size_);
        swap(available_chunks_, rhs.available_chunks_);
        swap(unavailable_chunks_, rhs.unavailable_chunks_);
    }

    template <typename TAllocator>
    inline typename PoolAllocator<TAllocator>::Chunk& PoolAllocator<TAllocator>::BlockToChunk(const ByteSpan& block) const noexcept
    {

    }

    template <typename TAllocator>
    inline Pointer<typename PoolAllocator<TAllocator>::Chunk> Link(Chunk& chunk, Pointer<Chunk> list) const noexcept
    {

    }

    template <typename TAllocator>
    inline Pointer<typename PoolAllocator<TAllocator>::Chunk> PoolAllocator<TAllocator>::AllocateChunk() noexcept
    {
        if (auto storage = allocator_.Allocate(chunk_size_))
        {
            auto chunk = FromTypeless<Chunk>(Begin(storage));

            chunk->next_ = chunk_;
            chunk->storage_ = PopFront(storage, ToInt(SizeOf<Chunk>()));

            chunk_ = chunk;

            return chunk_;
        }

        return nullptr;
    }

    template <typename TAllocator>
    inline RWByteSpan PoolAllocator<TAllocator>::AllocateBlock() noexcept
    {
        // Attempt to recycle a free block. Fast-path.

        if (free_)
        {
            auto block = RWByteSpan{ ToRWBytePtr(free_), ToInt(size) };

            free_ = free_->next_;

            return block;
        }

        // Either allocate on the current chunk (fast path) or allocate a new chunk (speed depends on the underlying allocator).

        if ((chunk_ && (Size(chunk_->storage_) >= block_size_)) || AllocateChunk())
        {
            auto [block, storage] = SliceFront(chunk_->storage_, ToInt(block_size_));

            chunk_->storage_ = storage;

            return block_;
        }

        return {};
    }



}
