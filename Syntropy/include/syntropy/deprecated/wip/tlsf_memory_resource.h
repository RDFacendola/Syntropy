
/// \file tlsf_memory_resource.h
/// \brief This header is part of the syntropy memory management system. It contains memory resources based on two-level segregated-fit allocator.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <tuple>

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_span.h"

#include "syntropy/allocators/memory_resource.h"

#include "syntropy/containers/vector.h"

#include "syntropy/math/math.h"

namespace syntropy
{
    /************************************************************************/
    /* TLSF MEMORY RESOURCE <TMEMORY RESOURCE>                              */
    /************************************************************************/

    /// \brief Tier 2, high-performances, low-fragmentation memory resource to handle allocation of any size.
    /// The allocator allocates pages on demand but uses a no-deallocation policy to avoid kernel calls.
    ///
    /// Based on: http://www.gii.upv.es/tlsf/files/jrts2008.pdf
    ///
    /// \author Raffaele D. Facendola - January 2017
    template <typename TMemoryResource>
    class TLSFMemoryResource
    {
    public:

        /// \brief Create a new allocator.
        /// \param first_levels Number of first-dimension levels. Each first-level splits free-blocks ranges exponentially.
        /// \param second_level_bits Number of bits needed to encode second-dimension levels. Each second-level splits first-level ranges linearly.
        /// \param arguments Arguments used to construct the underlying memory resource.
        template <typename... TArguments>
        TLSFMemoryResource(std::int64_t first_levels, std::int64_t second_level_bits, TArguments&&... arguments);

        /// \brief No copy constructor.
        TLSFMemoryResource(const TLSFMemoryResource&) = delete;

        /// \brief Move constructor.
        TLSFMemoryResource(TLSFMemoryResource&& rhs);

        /// \brief No assignment operator.
        TLSFMemoryResource& operator=(const TLSFMemoryResource&) = delete;

        /// \brief Destructor.
        /// \remarks The allocator is expected to outlive every other allocation performed on it.
        ~TLSFMemoryResource() = default;

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
        void Deallocate(const MemoryRange& block) noexcept;

        /// \brief Deallocate an aligned memory block.
        void Deallocate(const MemoryRange& block, Alignment alignment) noexcept;

        /// \brief Check whether this memory resource owns the provided memory block.
        /// \param block Block to check the ownership of.
        /// \return Returns true if the provided memory range was allocated by this memory resource, returns false otherwise.
        bool Owns(const MemoryRange& block) const noexcept;

        /// \brief Get the maximum allocation size that can be handled by this memory resource.
        /// The returned value shall not be used to determine whether a call to "Allocate" will fail.
        /// \return Returns the maximum allocation size that can be handled by this memory resource.
        Bytes GetMaxAllocationSize() const noexcept;

    private:

        struct FreeBlock
        {

        };

        /// \brief Map a block size to its first-level and second-level indexes.
        std::tuple<std::int64_t, std::int64_t> MappingInsert(Bytes size) const;

        /// \brief Maps a block size to the first-level and second-level to start the search of a free block from. The block size is rounded up to the next list size.
        std::tuple<Bytes, std::int64_t, std::int64_t> MappingSearch(Bytes size) const;

        /// \brief Find a free block which is at least as big as the size identified by the provided first-level and second-level indexes.
        FreeBlock** SearchSuitableBlock(std::int64_t first_index, std::int64_t second_index) const;

        /// \brief Get the head of the free-list identified by the provided first-index second-index pair.
        FreeBlock** HeadList(std::int64_t first_index, std::int64_t second_index) const;

        /// \brief Underlying memory resource, wrapped into polymorphic interface needed for internal state dynamic allocations.
        MemoryResourceT<TMemoryResource> memory_resource_;

        /// \brief Number of first-level ranges. Each range i is [2^i; 2^(i+1)).
        std::int64_t first_levels_{ 16 };

        /// \brief Number of second-level ranges.
        std::int64_t second_levels_{ 8 };

        /// \brief Number of bits needed to represent second-classes ranges. Each range splits a first-level range in equal widths.
        std::int64_t second_level_bits_{ 3 };

        /// \brief Bitmap used to identify non-empty, first-level, free lists.
        std::int64_t first_bitmap_{ 0 };

        /// \brief Bitmap used to identify non-empty, second-level, free lists. One bitmap per each first-level range.
        Vector<std::int64_t> second_bitmaps_;

        /// \brief Linked list of free blocks for each first-level second-level index pair.
        Vector<FreeBlock*> free_lists_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // TLSFMemoryResource<TMemoryResource>.

    template <typename TMemoryResource>
    template <typename... TArguments>
    TLSFMemoryResource<TMemoryResource>::TLSFMemoryResource(std::int64_t first_levels, std::int64_t second_level_bits, TArguments&&... arguments)
        : memory_resource_(std::forward<TArguments>(arguments)...)
        , first_levels_(first_levels)
        , second_levels_(std::int64_t{ 1 } << second_level_bits)
        , second_level_bits_(second_level_bits)
        , second_bitmaps_(memory_resource_)
        , free_lists_(memory_resource_)
    {
        // Dynamic memory allocated directly on the underlying memory resource.

        second_bitmaps_.resize(first_levels_);
        free_lists_.resize(first_levels_ * second_levels_);
    }

    template <typename TMemoryResource>
    TLSFMemoryResource<TMemoryResource>::TLSFMemoryResource(TLSFMemoryResource&& rhs)
        : memory_resource_(std::move(rhs.memory_resource_))
        , first_levels_(rhs.first_levels_)
        , second_levels_(rhs.second_levels_)
        , second_level_bits_(rhs.second_level_bits_)
        , second_bitmaps_(std::move(rhs.second_bitmaps_))
        , free_lists_(std::move(rhs.free_lists_))
    {
        rhs.second_bitmaps_ = {};
        rhs.free_lists_ = {};
    }

    template <typename TMemoryResource>
    MemoryRange TLSFMemoryResource<TMemoryResource>::Allocate(Bytes size) noexcept
    {
        return {};
    }

    template <typename TMemoryResource>
    MemoryRange TLSFMemoryResource<TMemoryResource>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        return {};
    }

    template <typename TMemoryResource>
    void TLSFMemoryResource<TMemoryResource>::Deallocate(const MemoryRange& block) noexcept
    {

    }

    template <typename TMemoryResource>
    void TLSFMemoryResource<TMemoryResource>::Deallocate(const MemoryRange& block, Alignment alignment) noexcept
    {

    }

    template <typename TMemoryResource>
    bool TLSFMemoryResource<TMemoryResource>::Owns(const MemoryRange& block) const noexcept
    {
        return false;
    }

    template <typename TMemoryResource>
    Bytes TLSFMemoryResource<TMemoryResource>::GetMaxAllocationSize() const noexcept
    {
        return {};
    }

    template <typename TMemoryResource>
    std::tuple<std::int64_t, std::int64_t> TLSFMemoryResource<TMemoryResource>::MappingInsert(Bytes size) const
    {
        auto first_index = FloorLog2(*size);

        auto second_index = (*size >> (first_index - second_level_bits_)) - second_levels_;

        return { first_index, second_index };
    }

    template <typename TMemoryResource>
    std::tuple<Bytes, std::int64_t, std::int64_t> TLSFMemoryResource<TMemoryResource>::MappingSearch(Bytes size) const
    {
        auto rounded_size = Bytes{ *size + (1 << (FloorLog2(*size) - second_level_bits_)) - 1 };

        auto[first_index, second_index] = MappingInsert(rounded_size);

        return { rounded_size, first_index, second_index };
    }

    template <typename TMemoryResource>
    typename TLSFMemoryResource<TMemoryResource>::FreeBlock** TLSFMemoryResource<TMemoryResource>::SearchSuitableBlock(std::int64_t first_index, std::int64_t second_index) const
    {
        static const auto kMask = ~std::int64_t{ 0 };

        if (auto bitmap = second_bitmaps_[first_index] & (kMask << second_index))
        {
            second_index = platform::BuiltIn::GetLeastSignificantBit(static_cast<std::uint64_t>(bitmap));

            return HeadList(first_index, second_index);
        }
        else if (bitmap = first_bitmap_ & (kMask << (first_index + 1)))
        {
            first_index = platform::BuiltIn::GetLeastSignificantBit(static_cast<std::uint64_t>(bitmap));
            second_index = platform::BuiltIn::GetLeastSignificantBit(static_cast<std::uint64_t>(second_bitmaps_[first_index]));

            return HeadList(first_index, second_index);
        }

        return nullptr;
    }

    template <typename TMemoryResource>
    inline typename  TLSFMemoryResource<TMemoryResource>::FreeBlock** TLSFMemoryResource<TMemoryResource>::HeadList(std::int64_t first_index, std::int64_t second_index) const
    {
        return &free_lists_[first_index * (1 << second_level_bits_) + second_index];
    }




}
