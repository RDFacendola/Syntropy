
/// \file standard_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains an allocator used to allocate memory on the stack.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <type_traits>

#include "memory/bytes.h"
#include "memory/alignment.h"
#include "memory/memory_address.h"
#include "memory/memory_range.h"

namespace syntropy
{
    /************************************************************************/
    /* STACK ALLOCATOR                                                      */
    /************************************************************************/

    /// \brief Allocator used to allocate a single block of memory on the stack.
    /// This allocator is intended to be composed with some other allocator such as syntropy::LinearAllocator.
    /// \author Raffaele D. Facendola - August 2018
    template <std::size_t kSize, std::size_t kAlignment>
    class StackAllocator
    {
    public:

        /// \brief Default constructor.
        StackAllocator() noexcept = default;

        /// \brief Default copy constructor.
        StackAllocator(const StackAllocator&) noexcept = default;

        /// \brief Default destructor.
        ~StackAllocator() noexcept = default;

        /// \brief Default assignment operator.
        StackAllocator& operator=(const StackAllocator&) noexcept = default;

        /// \brief Allocate a new memory block.
        /// \param size Size of the memory block to allocate.
        /// \return Returns the entire storage held by the allocator. If the requested size is greater than the allocator storage returns an empty range.
        MemoryRange Allocate(Bytes size) noexcept;

        /// \brief Allocate a new aligned memory block.
        /// \param size Size of the memory block to allocate.
        /// \param alignment Block alignment.
        /// \return Returns the entire storage held by the allocator. If the requested size is greater than the allocator storage returns an empty range.
        MemoryRange Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \param block Block to deallocate.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size).
        void Deallocate(const MemoryRange& block) noexcept;

        /// \brief Deallocate an aligned memory block.
        /// \param block Block to deallocate. Must refer to any allocation performed via Allocate(size, alignment).
        /// \param alignment Block alignment.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const MemoryRange& block, Alignment alignment) noexcept;

        /// \brief Check whether this allocator owns the provided memory block.
        /// \param block Block to check the ownership of.
        /// \return Returns true if the provided memory range was allocated by this allocator, returns false otherwise.
        bool Owns(const MemoryRange& block) const noexcept;

    private:

        std::aligned_storage_t<kSize, kAlignment> storage_;         ///< \brief Allocator storage.

        MemoryRange allocated_range_;                               ///< \brief Range that was handed over during the allocation.
    };

}

/************************************************************************/
/* IMPLEMENTATION                                                       */
/************************************************************************/

namespace syntropy
{
    template <std::size_t kSize, std::size_t kAlignment>
    inline MemoryRange StackAllocator<kSize, kAlignment>::Allocate(Bytes size) noexcept
    {
        if (size > 0_Bytes && !allocated_range_ && size <= Bytes{ kSize })
        {
            auto block = MemoryAddress{ &storage_ };

            allocated_range_ = { block, block + size };

            return allocated_range_;
        }

        return {};
    }

    template <std::size_t kSize, std::size_t kAlignment>
    inline MemoryRange StackAllocator<kSize, kAlignment>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (size > 0_Bytes && !allocated_range_)
        {
            auto block = MemoryAddress{ &storage_ };

            auto aligned_block = block.GetAligned(alignment);

            if (aligned_block + size <= block + Bytes{ kSize })
            {
                allocated_range_ = { aligned_block, aligned_block + size };

                return allocated_range_;
            }
        }

        return {};
    }

    template <std::size_t kSize, std::size_t kAlignment>
    inline void StackAllocator<kSize, kAlignment>::Deallocate(const MemoryRange& block) noexcept
    {
        SYNTROPY_ASSERT(block == allocated_range_);

        allocated_range_ = {};
    }

    template <std::size_t kSize, std::size_t kAlignment>
    inline void StackAllocator<kSize, kAlignment>::Deallocate(const MemoryRange& block, Alignment alignment) noexcept
    {
        SYNTROPY_ASSERT(block == allocated_range_);

        allocated_range_ = {};
    }

    template <std::size_t kSize, std::size_t kAlignment>
    bool StackAllocator<kSize, kAlignment>::Owns(const MemoryRange& block) const noexcept
    {
        return block == allocated_range_;
    }
}


