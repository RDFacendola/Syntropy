
/// \file pool_allocator.h
/// \brief This header is part of the Syntropy allocators module. It contains allocators used to allocate fixed-size blocks.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/language/foundation.h"
#include "syntropy/experimental/memory/smart_pointers.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/memory/new.h"
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
        Memory::RWByteSpan Allocate(Memory::Bytes size, Memory::Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const Memory::RWByteSpan& block, Memory::Alignment alignment);

        /// \brief Check whether this allocator owns a memory block.
        Bool Owns(const Memory::ByteSpan& block) const noexcept;

        /// \brief Deallocate every allocation performed on this allocator so far, invalidating all outstanding checkpoints.
        void DeallocateAll() noexcept;

        /// \brief Swap this allocator with another one.
        void Swap(PoolAllocator& rhs) noexcept;

    private:

        /// \brief A chunk allocated on the underlying allocator.
        struct Chunk;

        /// \brief Free block.
        struct FreeBlock;

        /// \brief Allocate a new chunk and replace the active one.
        RWPointer<Chunk> AllocateChunk() noexcept;

        /// \brief Allocate a new block in a chunk.
        Memory::RWByteSpan AllocateBlock(Chunk& chunk) noexcept;

        /// \brief Allocate a block on the current chunk or, if unavailable, allocate a new chunk.
        Memory::RWByteSpan AllocateBlock() noexcept;

        /// \brief Link a chunk to another chunk.
        void Link(Chunk& chunk, RWPointer<Chunk> next) noexcept;

        /// \brief Unlink a chunk.
        void Unlink(Chunk& chunk) noexcept;

        /// \brief Underlying allocator. Deallocated blocks are sent to the free list and never deallocated by this allocator.
        TAllocator allocator_;

        /// \brief Size of each chunk.
        Memory::Bytes chunk_size_;

        /// \brief Size of each block in a chunk.
        Memory::Bytes block_size_;

        /// \brief List of chunks with at least one free block.
        RWPointer<Chunk> available_chunks_{ nullptr };

        /// \brief List of chunks with no free block.
        RWPointer<Chunk> unavailable_chunks_{ nullptr };

    };

    /************************************************************************/
    /* POOL ALLOCATOR <ALLOCATOR> :: CHUNK                                  */
    /************************************************************************/

    /// \brief A chunk in the allocation chain.
    template <typename TAllocator>
    struct PoolAllocator<TAllocator>::Chunk
    {
        /// \brief A span covering the entire chunk.
        Memory::RWByteSpan self_;

        /// \brief Pointer to the previous chunk.
        RWPointer<Chunk> previous_{ nullptr };

        /// \brief Pointer to the next chunk.
        RWPointer<Chunk> next_{ nullptr };

        /// \brief Pointer to the first free block in the chunk.
        RWPointer<FreeBlock> free_{ nullptr };

        /// \brief Number of active allocations.
        Int allocation_count_{ 0 };

        /// \brief A span covering the chunk payload.
        Memory::RWByteSpan payload_;

        /// \brief List of blocks (either free or allocated).
        Memory::RWByteSpan blocks_;
    };

    /************************************************************************/
    /* POOL ALLOCATOR <ALLOCATOR> :: FREE BLOCK                             */
    /************************************************************************/

    /// \brief A free block.
    template <typename TAllocator>
    struct PoolAllocator<TAllocator>::FreeBlock
    {
        ///< \brief Next free block in the chunk.
        RWPointer<FreeBlock> next_{ nullptr };
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // PoolAllocator<TAllocator>.

    template <typename TAllocator>
    template <typename... TArguments>
    inline PoolAllocator<TAllocator>::PoolAllocator(Memory::Bytes block_size, Memory::Bytes chunk_size, TArguments&&... arguments) noexcept
        : allocator_(Forward<TArguments>(arguments)...)
        , chunk_size_(chunk_size)
        , block_size_(Math::Max(block_size, Memory::SizeOf<FreeBlock>()))               // Free blocks in a chunk are chained together: each block must be large enough to fit a pointer.
    {
        SYNTROPY_ASSERT((chunk_size - Memory::SizeOf<Chunk>()) > block_size_);          // Chunks are expected to fit two or more blocks, otherwise the first allocation will make them unavailable right away.
    }

    template <typename TAllocator>
    inline PoolAllocator<TAllocator>::PoolAllocator(PoolAllocator&& rhs) noexcept
        : allocator_(Move(rhs.allocator_))
        , chunk_size_(rhs.chunk_size_)
        , block_size_(rhs.block_size_)
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
    Memory::RWByteSpan PoolAllocator<TAllocator>::Allocate(Memory::Bytes size, Memory::Alignment alignment) noexcept
    {
        if ((size <= block_size_) && (alignment <= ToAlignment(block_size_)))
        {
            if (auto block = AllocateBlock())
            {
                return Memory::Front(block, size);
            }
        }

        return {};
    }

    template <typename TAllocator>
    void PoolAllocator<TAllocator>::Deallocate(const Memory::RWByteSpan& block, Memory::Alignment alignment)
    {
        SYNTROPY_UNDEFINED_BEHAVIOR((Size(block) <= block_size_) && (alignment <= Memory::Alignment(block_size_)), "The provided block doesn't belong to this allocator instance");

        auto chunk = FromTypeless<Chunk>(Memory::AlignDown(Begin(block)));

        auto is_available = chunk->free_ || chunk->blocks_;

        chunk->allocation_count_--;

        // Link to chunk free list.

        auto free = FromTypeless<FreeBlock>(Begin(block));

        free->next_ = chunk->free_;
        chunk->free_ = free;

        // Fast-path : if the chunk was already available and it has other active allocations no action is required.

        if (is_available && (chunk->allocation_count_ > 0))
        {
            return;
        }

        // Either the chunk become available (most likely, fast path) or, if the last block was deallocated, the entire chunk is deallocated.

        if (chunk->allocation_count_ > 0)
        {
            Link(*chunk, available_chunks_);
        }
        else
        {
            Unlink(chunk);

            allocator_.Deallocate(Memory::RWByteSpan{ chunk, chunk_size_ }, ToAlignment(chunk_size_));
        }
    }

    template <typename TAllocator>
    inline Bool PoolAllocator<TAllocator>::Owns(const Memory::ByteSpan& block) const noexcept
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
        Swap(allocator_, rhs.allocator_);
        Swap(chunk_size_, rhs.chunk_size_);
        Swap(block_size_, rhs.block_size_);
        Swap(available_chunks_, rhs.available_chunks_);
        Swap(unavailable_chunks_, rhs.unavailable_chunks_);
    }

    template <typename TAllocator>
    inline RWPointer<typename PoolAllocator<TAllocator>::Chunk> PoolAllocator<TAllocator>::AllocateChunk() noexcept
    {
        if (auto chunk_storage = allocator_.Allocate(chunk_size_, ToAlignment(chunk_size_)))            // Chunks are aligned at their own size.
        {
            auto chunk = new (chunk_storage) Chunk{};

            chunk->self_ = chunk_storage;
            chunk->payload_ = Memory::PopFront(chunk_storage, Memory::SizeOf<Chunk>());                 // Discard header footprint.
            chunk->blocks_ = Memory::Align(chunk->payload_, block_size_, ToAlignment(block_size_));     // Align the payload to block size and alignment.

            return chunk_;
        }

        return nullptr;
    }

    template <typename TAllocator>
    Memory::RWByteSpan PoolAllocator<TAllocator>::AllocateBlock(Chunk& chunk) noexcept
    {
        chunk.allocation_count_++;

        // Fast-path: recycle a free block (keeps allocations close together).

        if (chunk.free_)
        {
            auto block = RWByteSpan{ chunk.free_, block_size_ };

            auto next_free = FromTypeless<FreeBlock>(Begin(block))->next_;

            chunk.free_ = next_free;

            return block;
        }

        // Fast-path: allocate at the end of the chunk.

        auto [block, blocks] = Memory::SliceFront(chunk.blocks_, block_size_);

        chunk.blocks_ = blocks;

        return block;
    }

    template <typename TAllocator>
    Memory::RWByteSpan PoolAllocator<TAllocator>::AllocateBlock() noexcept
    {
        auto chunk = (available_chunks_ ? available_chunks_ : AllocateChunk());                         // Either the first available chunk (fast path) or a new one.

        if (chunk)
        {
            auto block = AllocateBlock(*chunk);

            auto chunks = (chunk->free_ || chunk_->blocks_) ? available_chunks_ : unavailable_chunks_;  

            Link(*chunk, chunks);

            return block;
        }

        return {};
    }

    template <typename TAllocator>
    inline void PoolAllocator<TAllocator>::Link(Chunk& chunk, RWPointer<Chunk> next) noexcept
    {
        if (&chunk != next)
        {
            Unlink(chunk);

            chunk.next_ = next;

            if (next)
            {
                next.previous_ = &chunk;
            }

            // Make sure to update allocation chains eventually.

            if (next == available_chunks_)
            {
                available_chunks_ = &chunk;
            }
            
            if (next == unavailable_chunks_)
            {
                unavailable_chunks_ = &chunk;
            }
        }
    }

    template <typename TAllocator>
    void PoolAllocator<TAllocator>::Unlink(Chunk& chunk) noexcept
    {
        // Make sure not to break allocation chains.

        if (&chunk == unavailable_chunks_)
        {
            unavailable_chunks_ = chunk->next_;
        }

        if (&chunk == available_chunks_)
        {
            available_chunks_ = chunk->next_;
        }

        // Unlink.

        if (chunk.previous_)
        {
            chunk.previous_->next_ = chunk.next_;
            chunk.previous_ = nullptr;
        }

        if (chunk.next_)
        {
            chunk.next_->previous_ = chunk.previous_;
            chunk.next_ = nullptr;
        }
    }

}
