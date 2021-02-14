
/// \file buffer.inl
///
/// \author Raffaele D. Facendola - February 2021.

#pragma once

#include "syntropy/memory/foundation/memory.h"

#include "syntropy/language/support/swap.h"

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* BASE BUFFER                                                          */
    /************************************************************************/

    inline Buffer
    ::Buffer(Mutable<BaseAllocator> allocator) noexcept
        : Buffer(ToBytes(0), MaxAlignment(), allocator)
    {

    }

    inline Buffer
    ::Buffer(Bytes size, Mutable<BaseAllocator> allocator) noexcept
        : Buffer(size, MaxAlignment(), allocator)
    {

    }

    inline Buffer
    ::Buffer(Bytes size,
             Alignment alignment,
             Mutable<BaseAllocator> allocator) noexcept
        : allocator_(&allocator)
        , data_(allocator.Allocate(size, alignment))
        , alignment_(alignment)
    {
        SYNTROPY_ASSERT(data_.GetCount() == size);       // Out of memory?
    }

    inline Buffer
    ::Buffer(Immutable<Buffer> rhs) noexcept
        : Buffer(rhs, rhs.GetAllocator())
    {

    }

    inline Buffer
    ::Buffer(Immutable<Buffer> rhs,
             Mutable<BaseAllocator> allocator) noexcept
        : Buffer(rhs.GetCount(), rhs.GetAlignment(), allocator)
    {
        Copy(data_, rhs.data_);
    }

    inline Buffer
    ::Buffer(Movable<Buffer> rhs) noexcept
        : allocator_(rhs.allocator_)
        , alignment_(rhs.alignment_)
    {
        Algorithm::Swap(data_, rhs.data_);
    }

    inline Mutable<Buffer> Buffer
    ::operator=(Immutable<Buffer> rhs) noexcept
    {
        if (this != &rhs)
        {
            if (GetCount() != rhs.GetCount())
            {
                data_ = allocator_->Allocate(rhs.GetCount(),
                                             rhs.GetAlignment());
            }

            Copy(data_, rhs.data_);
        }

        return *this;
    }

    inline Mutable<Buffer> Buffer
    ::operator=(Movable<Buffer> rhs) noexcept
    {
        if (allocator_ == rhs.allocator_)
        {
            Algorithm::Swap(data_, rhs.data_);
            Algorithm::Swap(alignment_, rhs.alignment_);
        }
        else
        {
            *this = rhs;
        }

        return *this;
    }

    inline Buffer
    ::~Buffer() noexcept
    {
        if(allocator_)
        {
            allocator_->Deallocate(data_, alignment_);
        }
    }

    [[nodiscard]] inline BytePtr Buffer
    ::GetData() const noexcept
    {
        return data_.GetData();
    }

    [[nodiscard]] inline RWBytePtr Buffer
    ::GetData() noexcept
    {
        return data_.GetData();
    }

    [[nodiscard]] inline Bytes Buffer
    ::GetCount() const noexcept
    {
        return data_.GetCount();
    }

    [[nodiscard]] inline Immutable<Alignment> Buffer
    ::GetAlignment() const noexcept
    {
        return alignment_;
    }

    [[nodiscard]] inline Mutable<BaseAllocator> Buffer
    ::GetAllocator() const noexcept
    {
        return *allocator_;
    }

}

// ===========================================================================
