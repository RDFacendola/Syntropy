
/// \file pool_memory_resource.h
/// \brief This header is part of the Syntropy allocators module. It contains pooled memory resources.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/core/types.h"
#include "syntropy/diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* POOL MEMORY RESOURCE <TMEMORY RESOURCE>                              */
    /************************************************************************/

    /// \brief Tier 1 memory resource that uses an underlying memory resource to allocate fixed-size blocks.
    /// Blocks are aligned to their own size.
    /// Deallocated blocks are kept around and recycled when possible.
    /// \author Raffaele D. Facendola - August 2018
    template <typename TMemoryResource>
    class PoolMemoryResource
    {
    public:

        /// \brief Create a new memory resource.
        /// \param block_size Size of each allocated block.
        /// \param chunk_size Size of each chunk allocated from the underlying memory resource.
        /// \param Arguments used to construct the underlying memory resource.
        template <typename... TArguments>
        PoolMemoryResource(Bytes block_size, Bytes chunk_size, TArguments&&... arguments) noexcept;

        /// \brief No copy constructor.
        PoolMemoryResource(const PoolMemoryResource&) = delete;

        /// \brief Move constructor.
        PoolMemoryResource(PoolMemoryResource&& rhs) noexcept;

        /// \brief Destructor.
        ~PoolMemoryResource();

        /// \brief Unified assignment operator.
        PoolMemoryResource& operator=(PoolMemoryResource rhs) noexcept;

        /// \brief Allocate a new aligned memory block.
        /// \param size Size of the memory block to allocate.
        /// \param alignment Block alignment.
        /// \return Returns a range representing the requested aligned memory block. If no allocation could be performed returns an empty range.
        MemoryRange Allocate(Bytes size, Alignment alignment = MaxAlignmentOf()) noexcept;

        /// \brief Deallocate an aligned memory block.
        /// \param block Block to deallocate. Must refer to any allocation performed via Allocate(size, alignment).
        /// \param alignment Block alignment.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const MemoryRange& block, Alignment alignment = MaxAlignmentOf());

        /// \brief Deallocate every allocation performed so far.
        /// \remarks This method returns every allocation to the underlying memory resource.
        void DeallocateAll() noexcept;

        /// \brief Check whether this memory resource owns the provided memory block.
        /// \param block Block to check the ownership of.
        /// \return Returns true if the provided memory range was allocated by this memory resource, returns false otherwise.
        Bool Owns(const MemoryRange& block) const noexcept;

        /// \brief Swap this memory resource with the provided instance.
        void Swap(PoolMemoryResource& rhs) noexcept;

    private:

        struct Chunk;
        struct FreeBlock;

        ///< \brief Underlying memory resource. Deallocated blocks are sent to the free list and never deallocated by this memory resource.
        TMemoryResource memory_resource_;

        /// \brief Size of each chunk in the allocation chain.
        Bytes chunk_size_;

        /// \brief Pointer past the last allocated address in the active chunk.
        MemoryAddress head_;

        ///< \brief Size of each allocated block.
        Bytes block_size_;

        ///< \brief Next free block in the pool.
        FreeBlock* free_{ nullptr };

        ///< \brief Current active chunk.
        Chunk* chunk_{ nullptr };

    };

    /************************************************************************/
    /* POOL MEMORY RESOURCE <MEMORY RESOURCE> :: CHUNK                      */
    /************************************************************************/

    /// \brief A chunk in the allocation chain.
    template <typename TMemoryResource>
    struct PoolMemoryResource<TMemoryResource>::Chunk
    {
        /// \brief Pointer to the previous chunk.
        PoolMemoryResource::Chunk* previous_;

        /// \brief Pointer past the last allocable address in the chunk.
        MemoryAddress end_;
    };

    /************************************************************************/
    /* POOL MEMORY RESOURCE <MEMORY RESOURCE> :: FREE BLOCK                 */
    /************************************************************************/

    /// \brief Represents a free block: the memory block itself is used to store a pointer to the next free block in the list.
    template <typename TMemoryResource>
    struct PoolMemoryResource<TMemoryResource>::FreeBlock
    {
        ///< \brief Next free block in the pool.
        FreeBlock* next_{ nullptr };
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Swaps two syntropy::PoolMemoryResource.
    template <typename TMemoryResource>
    void swap(syntropy::PoolMemoryResource<TMemoryResource>& lhs, syntropy::PoolMemoryResource<TMemoryResource>& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // PoolMemoryResource<TMemoryResource>.

    template <typename TMemoryResource>
    template <typename... TArguments>
    inline PoolMemoryResource<TMemoryResource>::PoolMemoryResource(Bytes block_size, Bytes chunk_size, TArguments&&... arguments) noexcept
        : memory_resource_(std::forward<TArguments>(arguments)...)
        , chunk_size_(chunk_size)
        , block_size_(block_size)
    {
        SYNTROPY_ASSERT(block_size >= MaxAlignmentOf());
    }

    template <typename TMemoryResource>
    inline PoolMemoryResource<TMemoryResource>::PoolMemoryResource(PoolMemoryResource&& rhs) noexcept
        : memory_resource_(std::move(rhs.memory_resource_))
        , chunk_size_(rhs.chunk_size_)
        , head_(rhs.head_)
        , block_size_(rhs.block_size_)
        , free_(rhs.free_)
        , chunk_(chunk_)
    {
        rhs.free_ = nullptr;
        rhs.chunk_ = nullptr;
    }

    template <typename TMemoryResource>
    inline PoolMemoryResource<TMemoryResource>::~PoolMemoryResource()
    {
        DeallocateAll();
    }

    template <typename TMemoryResource>
    inline PoolMemoryResource<TMemoryResource>& PoolMemoryResource<TMemoryResource>::operator=(PoolMemoryResource rhs) noexcept
    {
        rhs.Swap(*this);
        return *this;
    }

    template <typename TMemoryResource>
    MemoryRange PoolMemoryResource<TMemoryResource>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if ((size <= block_size_) && (alignment <= Alignment(block_size_)))
        {
            // Attempt to recycle a free block. Fast-path.

            if (free_)
            {
                auto block = MemoryAddress(free_);

                free_ = free_->next_;

                return { block, size };
            }

            // Attempt to allocate on the current chunk. Fast-path.

            {
                auto block = MemoryRange{ head_, block_size_ };

                if (chunk_ && (block.End() <= chunk_->end_))
                {
                    head_ = block.End();

                    return { block.Begin(), size };
                }
            }

            // Allocate a new chunk accounting for Chunk header and alignment requirements. Performance cost depends on the underlying memory resource.

            if (auto block = memory_resource_.Allocate(chunk_size_))
            {
                // Link to the previous chunk.

                auto chunk = block.Begin().As<Chunk>();

                chunk->previous_ = chunk_;
                chunk->end_ = block.End();

                chunk_ = chunk;

                // Allocate the block from the new chunk.

                auto head = (block.Begin() + BytesOf<Chunk>()).GetAligned(Alignment(block_size_));

                head_ = head + block_size_;

                return { head, size };
            }
        }

        return {};
    }

    template <typename TMemoryResource>
    inline void PoolMemoryResource<TMemoryResource>::Deallocate(const MemoryRange& block, Alignment alignment)
    {
        SYNTROPY_ASSERT(alignment <= Alignment(block_size_));
        SYNTROPY_ASSERT(memory_resource_.Owns(block));

        // Send the block to the free list, making it eligible for recycling. Fast-path.

        auto free = free_;

        free_ = block.Begin().As<FreeBlock>();

        free_->next_ = free;
    }

    template <typename TMemoryResource>
    inline void PoolMemoryResource<TMemoryResource>::DeallocateAll() noexcept
    {
        for (; chunk_ != nullptr;)
        {
            auto previous = chunk_->previous_;

            memory_resource_.Deallocate({ chunk_, chunk_->end_ });

            chunk_ = previous;
        }

        head_ = nullptr;
        free_ = nullptr;
    }

    template <typename TMemoryResource>
    inline Bool PoolMemoryResource<TMemoryResource>::Owns(const MemoryRange& block) const noexcept
    {
        // Can't query the underlying memory resource directly since it might be shared with other allocators.

        for (auto chunk = chunk_; chunk; chunk = chunk->previous_)
        {
            if (MemoryRange{ chunk, chunk->end_ }.Contains(block))
            {
                return true;
            }
        }

        return false;
    }

    template <typename TMemoryResource>
    inline void PoolMemoryResource<TMemoryResource>::Swap(PoolMemoryResource& rhs) noexcept
    {
        using std::swap;

        swap(memory_resource_, rhs.memory_resource_);
        swap(chunk_size_, rhs.chunk_size_);
        swap(head_, rhs.head_);
        swap(block_size_, rhs.block_size_);
        swap(free_, rhs.free_);
        swap(chunk_, rhs.chunk_);
    }

    // Non-member functions.

    template <typename TMemoryResource>
    void swap(syntropy::PoolMemoryResource<TMemoryResource>& lhs, syntropy::PoolMemoryResource<TMemoryResource>& rhs) noexcept
    {
        lhs.Swap(rhs);
    }

}
