
/// \file buffer.inl
///
/// \author Raffaele D. Facendola - February 2021.

#pragma once

#include "syntropy/memory/foundation/memory.h"

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* BASE BUFFER                                                          */
    /************************************************************************/

    template <typename TTraits>
    template <typename TThis>
    inline BaseBuffer<TTraits>
    ::BaseBuffer(Mutable<BaseAllocator> allocator) noexcept
        : BaseBuffer(ToBytes(0), MaxAlignment(), allocator)
    {

    }

    template <typename TTraits>
    template <typename TThis>
    inline BaseBuffer<TTraits>
    ::BaseBuffer(Bytes size, Mutable<BaseAllocator> allocator) noexcept
        : BaseBuffer(size, MaxAlignment(), allocator)
    {

    }

    template <typename TTraits>
    template <typename TThis>
    inline BaseBuffer<TTraits>
    ::BaseBuffer(Bytes size,
                 Alignment alignment,
                 Mutable<BaseAllocator> allocator) noexcept
        : allocator_(&allocator)
        , data_(allocator.Allocate(size, alignment))
        , alignment_(alignment)
    {
        SYNTROPY_ASSERT(Size(data_) == size);       // Out of memory?
    }

    template <typename TTraits>
    template <typename UTraits>
    inline BaseBuffer<TTraits>
    ::BaseBuffer(Immutable<BaseBuffer<UTraits>> rhs) noexcept
        : BaseBuffer(rhs, rhs.GetAllocator())
    {

    }

    template <typename TTraits>
    template <typename UTraits>
    inline BaseBuffer<TTraits>
    ::BaseBuffer(Immutable<BaseBuffer<UTraits>> rhs,
                 Mutable<BaseAllocator> allocator) noexcept
        : BaseBuffer(rhs.GetCount(), rhs.GetAlignment(), allocator)
    {
        Copy(data_, rhs.data_);
    }

    template <typename TTraits>
    template <typename UTraits>
    inline BaseBuffer<TTraits>
    ::BaseBuffer(Movable<BaseBuffer<UTraits>> rhs) noexcept
        : allocator_(rhs.allocator_)
        , alignment_(rhs.alignment_)
    {
        if constexpr(Concepts::SameAs<decltype(data_), decltype(rhs.data_)>)
        {
            Syntropy::Swap(data_, rhs.data_);
        }
        else
        {
            data_ = Move(rhs.data_);
            rhs.data_ = {};
        }
    }

    template <typename TTraits>
    template <typename UTraits>
    inline BaseBuffer<TTraits>
    ::BaseBuffer(Movable<BaseBuffer<UTraits>> rhs,
                 Mutable<BaseAllocator> allocator) noexcept
        : allocator_(allocator_)
        , alignment_(rhs.alignment_)
    {
        // Either move or copy.

        if (allocator_ == rhs.allocator_)
        {
            Syntropy::Swap(data_, rhs.data_);
        }
        else
        {
            Copy(data_, rhs.data_);
        }
    }

    template <typename TTraits>
    template <typename UTraits>
    inline Mutable<BaseBuffer<TTraits>> BaseBuffer<TTraits>
    ::operator=(Immutable<BaseBuffer<UTraits>> rhs) noexcept
    {
        if (this != &rhs)
        {
            if (Size(*this) != Size(rhs))
            {
                data_ = allocator_->Allocate(Size(rhs), rhs.GetAlignment());
            }

            Copy(data_, rhs.GetData());
        }

        return *this;
    }

    template <typename TTraits>
    template <typename UTraits>
    inline Mutable<BaseBuffer<TTraits>> BaseBuffer<TTraits>
    ::operator=(Movable<BaseBuffer<UTraits>> rhs) noexcept
    {
        if (allocator_ == rhs.allocator_)
        {
            Swap(rhs);                          // Move by swap.
        }
        else
        {
            *this = Copy(rhs);                  // Copy reference.
        }

        return *this;
    }

    template <typename TTraits>
    inline BaseBuffer<TTraits>
    ::~BaseBuffer() noexcept
    {
        allocator_->Deallocate(data_, alignment_);
    }

    template <typename TTraits>
    [[nodiscard]] inline
    Immutable<typename BaseBuffer<TTraits>::TData> BaseBuffer<TTraits>
    ::GetData() const noexcept
    {
        return data_;
    }

    template <typename TTraits>
    [[nodiscard]] inline Bytes BaseBuffer<TTraits>
    ::GetCount() const noexcept
    {
        return data_.GetCount();
    }

    template <typename TTraits>
    [[nodiscard]] inline Immutable<Alignment> BaseBuffer<TTraits>
    ::GetAlignment() const noexcept
    {
        return alignment_;
    }

    template <typename TTraits>
    [[nodiscard]] inline Mutable<BaseAllocator> BaseBuffer<TTraits>
    ::GetAllocator() const noexcept
    {
        return *allocator_;
    }

    template <typename TTraits>
    [[nodiscard]] inline Immutable<typename BaseBuffer<TTraits>::TSpan>
    BaseBuffer<TTraits>
    ::GetRange() const noexcept
    {
        return data_;
    }

    template <typename TTraits>
    inline void BaseBuffer<TTraits>
    ::Swap(Mutable<BaseBuffer> rhs) noexcept
    {
        SYNTROPY_UNDEFINED_BEHAVIOR(allocator_ == rhs.allocator_,
                                    "Buffers must share the same allocator.");

        Syntropy::Swap(data_, rhs.data_);
        Syntropy::Swap(alignment_, rhs.alignment_);
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    template <typename TTraits>
    [[nodiscard]] Buffer
    ToReadOnly(Movable<BaseBuffer<TTraits>> rhs) noexcept
    {
        return Buffer(Move(rhs));
    }

}

// ===========================================================================
