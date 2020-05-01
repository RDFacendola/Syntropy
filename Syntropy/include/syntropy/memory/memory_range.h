
/// \file memory_range.h
/// \brief This header is part of the Syntropy memory module. It contains classes and definitions for memory ranges.
///
/// \author Raffaele D. Facendola - August 2018

#pragma once

#include <type_traits>

#include "syntropy/core/range.h"

#include "syntropy/memory/memory_address.h"

namespace syntropy
{
    /************************************************************************/
    /* MEMORY RANGE                                                         */
    /************************************************************************/

    /// \brief Type alias for a range of non-constant contiguous memory addresses.
    using MemoryRange = Range<MemoryAddress>;

    /// \brief Type alias for a range of constant contiguous memory addresses.
    using ConstMemoryRange = Range<ConstMemoryAddress>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new range from a pair of contiguous iterators.
    template <typename TIterator>
    auto MakeMemoryRange(TIterator begin, TIterator end);

    /// \brief Create a new memory range from an iterator and a size.
    template <typename TIterator>
    auto MakeMemoryRange(TIterator begin, Bytes size);

    /// \brief Create a new constant memory range from a pair of contiguous iterators.
    template <typename TIterator>
    ConstMemoryRange MakeConstMemoryRange(TIterator begin, TIterator end);

    /// \brief Create a new constant memory range from an iterator and a size.
    template <typename TIterator>
    ConstMemoryRange MakeConstMemoryRange(TIterator begin, Bytes size);

    /// \brief Create a memory range containing the provided object.
    template <typename TCollection>
    MemoryRange MakeMemoryRange(TCollection& data) noexcept;

    /// \brief Create a memory range containing the provided object.
    template <typename TType>
    MemoryRange MakeMemoryRange(TType& data) noexcept;

    /// \brief Create a memory range containing the provided object.
    template <typename TType>
    ConstMemoryRange MakeMemoryRange(const TType& data) noexcept;

    /// \brief Create a constant memory range containing the provided object.
    template <typename TType>
    ConstMemoryRange MakeConstMemoryRange(const TType& data) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.

    template <typename TIterator>
    inline auto MakeMemoryRange(TIterator begin, TIterator end)
    {
        using std::distance;

        // 'begin' and 'end' may not be dereferenced: use the size overload.

        using TElement = std::remove_reference_t<decltype(*begin)>;

        return MakeMemoryRange(begin, distance(begin, end) * BytesOf<TElement>());
    }

    template <typename TIterator>
    inline auto MakeMemoryRange(TIterator begin, Bytes size)
    {
        using namespace literals;

        // Compute range size since 'end' cannot be dereferenced.

        auto address = (size > 0_Bytes) ? MakeMemoryAddress(&(*begin)) : nullptr;           // Either const or non-const.

        return MakeRange(address, size);
    }

    template <typename TIterator>
    inline ConstMemoryRange MakeConstMemoryRange(TIterator begin, TIterator end)
    {
        using std::distance;

        // 'begin' and 'end' may not be dereferenced: use the size overload.

        using TElement = std::remove_reference_t<decltype(*begin)>;

        return MakeConstMemoryRange(begin, distance(begin, end) * BytesOf<TElement>());
    }

    template <typename TIterator>
    inline ConstMemoryRange MakeConstMemoryRange(TIterator begin, Bytes size)
    {
        using namespace literals;

        // Compute range size since 'end' cannot be dereferenced.

        auto address = (size > 0_Bytes) ? MakeConstMemoryAddress(&(*begin)) : nullptr;      // Enforce const-ness.

        return MakeConstRange(address, size);
    }

    template <typename TType>
    inline MemoryRange MakeMemoryRange(TType& data) noexcept
    {
        return { &data, &data + 1 };
    }

    template <typename TType>
    inline ConstMemoryRange MakeMemoryRange(const TType& data) noexcept
    {
        return { &data, &data + 1 };
    }

    template <typename TType>
    inline ConstMemoryRange MakeConstMemoryRange(const TType& data) noexcept
    {
        return MakeMemoryRange(data);
    }

}