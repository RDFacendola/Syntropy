
/// \file pool_allocator.h
/// \brief This header is part of the Syntropy memory module. It contains pooled allocators.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/core/smart_pointers.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/diagnostics/assert.h"

namespace syntropy
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

        /// \brief Swap this allocator with another one.
        void Swap(PoolAllocator& rhs) noexcept;

    private:

        /// \brief A chunk allocated on the underlying allocator.
        struct Chunk;

        /// \brief Free block.
        struct FreeBlock;

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

        /// \brief Next free block in the pool.
        Pointer<FreeBlock> free_{ nullptr };

        /// \brief Current active chunk.
        Pointer<Chunk> chunk_{ nullptr };

    };

    /************************************************************************/
    /* POOL ALLOCATOR <ALLOCATOR> :: CHUNK                                  */
    /************************************************************************/

    /// \brief A chunk in the allocation chain.
    template <typename TAllocator>
    struct PoolAllocator<TAllocator>::Chunk
    {
        /// \brief Pointer to the next chunk.
        Pointer<PoolAllocator::Chunk> next_{ nullptr };

        /// \brief Unallocated memory storage.
        RWByteSpan storage_;
    };

    /************************************************************************/
    /* POOL ALLOCATOR <ALLOCATOR> :: FREE BLOCK                             */
    /************************************************************************/

    /// \brief A free block.
    template <typename TAllocator>
    struct PoolAllocator<TAllocator>::FreeBlock
    {
        ///< \brief Next free block in the pool.
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
        // Each chunk shall have enough storage to fit the chunk header and at least one block.

        SYNTROPY_ASSERT((SizeOf<Chunk>() + block_size_) <= chunk_size_);
    }

    template <typename TAllocator>
    inline PoolAllocator<TAllocator>::PoolAllocator(PoolAllocator&& rhs) noexcept
        : allocator_(std::move(rhs.allocator_))
        , block_size_(rhs.block_size_)
        , chunk_size_(rhs.chunk_size_)
        , free_(rhs.free_)
        , chunk_(rhs.chunk_)
    {
        rhs.free_ = nullptr;
        rhs.chunk_ = nullptr;
    }

    template <typename TAllocator>
    inline PoolAllocator<TAllocator>::~PoolAllocator()
    {
        for (; chunk_ != nullptr;)
        {
            auto next = chunk_->next_;

            allocator_.Deallocate({ chunk_, chunk_size_ });

            chunk_ = next;
        }
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
        if ((size <= block_size_) && (alignment <= Alignment(block_size_)))
        {
            if (auto block = AllocateBlock())
            {
                return Front(block, ToInt(size));               // Clamp to requested size.
            }
        }

        return {};
    }

    template <typename TAllocator>
    inline void PoolAllocator<TAllocator>::Deallocate(const RWByteSpan& block, Alignment alignment)
    {
        SYNTROPY_UNDEFINED_BEHAVIOR(Owns(block));
        SYNTROPY_UNDEFINED_BEHAVIOR(Size(block) <= block_size_);
        SYNTROPY_UNDEFINED_BEHAVIOR(alignment <= Alignment(block_size_));

        auto free = FromTypeless<FreeBlock>(Begin(block));      // Use block storage to contain a pointer to the next free block.

        free->next_ = free_;

        free_ = free;
    }

    template <typename TAllocator>
    inline Bool PoolAllocator<TAllocator>::Owns(const ByteSpan& block) const noexcept
    {
        // Can't query the underlying allocator directly since it might be shared.

        for (auto chunk = chunk_; chunk; chunk = chunk->next_)
        {
            if (Contains({ ToBytePtr(chunk), chunk_size_ }, block));
            {
                return true;
            }
        }

        return false;
    }

    template <typename TAllocator>
    inline void PoolAllocator<TAllocator>::Swap(PoolAllocator& rhs) noexcept
    {
        using std::swap;

        swap(allocator_, rhs.allocator_);
        swap(chunk_size_, rhs.chunk_size_);
        swap(head_, rhs.head_);
        swap(block_size_, rhs.block_size_);
        swap(free_, rhs.free_);
        swap(chunk_, rhs.chunk_);
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
