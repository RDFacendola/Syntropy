
/// \file memory_range.h
/// \brief This header is part of the syntropy memory management system. It contains classes and definitions for memory ranges.
///
/// \author Raffaele D. Facendola - August 2018

#pragma once

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/memory_address.h"

#include "syntropy/types/range.h"

namespace syntropy
{
    /************************************************************************/
    /* MEMORY RANGE                                                         */
    /************************************************************************/

    /// \brief Type alias for a range of contiguous memory addresses.
    using MemoryRange = Range<MemoryAddress>;

    /// \brief Type alias for a range of constant contiguous memory addresses.
    using ConstMemoryRange = Range<ConstMemoryAddress>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

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

    template <typename TType>
    MemoryRange MakeMemoryRange(TType& data) noexcept
    {
        return { &data, &data + 1 };
    }

    template <typename TType>
    ConstMemoryRange MakeMemoryRange(const TType& data) noexcept
    {
        return { &data, &data + 1 };
    }

    template <typename TType>
    ConstMemoryRange MakeConstMemoryRange(const TType& data) noexcept
    {
        return MakeMemoryRange(data);
    }

}