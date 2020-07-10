
/// \file memory_span.h
/// \brief This header is part of Syntropy memory module. It contains definitions for memory spans.
///
/// \author Raffaele D. Facendola - July 2020

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/memory/bytes.h"

namespace syntropy
{
    /************************************************************************/
    /* TYPE ALIAS                                                           */
    /************************************************************************/

    /// \brief Type alias for a pointer to a read-write memory region.
    using BytePtr = ObserverPtr<Byte>;

    /// \brief Type alias for a pointer to a read-only memory region.
    using ReadOnlyBytePtr = ObserverPtr<const Byte>;

    /************************************************************************/
    /* MEMORY SPAN                                                          */
    /************************************************************************/

    /// \brief Represents a contiguous memory region.
    /// \author Raffaele D. Facendola - July 2020.
    template <typename TTraits>
    class MemorySpanT
    {
    public:

        /// \brief Underlying pointer type.
        template <typename TType>
        using TPointer = TTraits::TPointer;

        /// \brief Create an empty memory span.
        constexpr MemorySpanT() noexcept = default;

        /// \brief Create a memory span given a pointer to the first byte and its size.
        template <typename UPointer>
        constexpr MemorySpanT(UPointer begin, Bytes size) noexcept;

        /// \brief Create a memory span given pointer to the first byte and past the last one.
        template <typename UPointer>
        constexpr MemorySpanT(UPointer begin, UPointer end) noexcept;

        /// \brief Copy constructor.
        template <typename UTraits>
        constexpr MemorySpanT(const MemorySpanT<UTraits>& rhs) noexcept;

        /// \brief Default destructor.
        ~MemorySpanT() noexcept = default;

        /// \brief Copy assignment operator.
        template <typename UTraits>
        constexpr MemorySpanT& operator=(const MemorySpanT<UTraits>& rhs) noexcept;

        /// \brief Check whether the memory span is non-empty.
        constexpr operator Bool() const noexcept;

        /// \brief Get the size of the memory span.
        constexpr Bytes GetSize() const noexcept;

        /// \brief Access the underlying buffer.
        constexpr TTraits::TPointer GetData() const noexcept;

    private:

        /// \brief Pointer to the first element in the memory region.
        TTraits::TPointer data_{ nullptr };

        /// \brief Span size.
        Bytes size_ = Bytes{ 0 };

    };

    /************************************************************************/
    /* MEMORY SPAN TRAITS                                                   */
    /************************************************************************/

    /// \brief Traits for a memory span in a read\write memory region.
    struct MemorySpanTraits
    {
        /// \brief Address of the underlying memory region.
        using TPointer = BytePtr;
    };

    /// \brief Traits for a memory span in a read only memory region.
    struct ReadOnlyMemorySpanTag
    {
        /// \brief Address of the underlying memory region.
        using TPointer = ReadOnlyBytePtr;
    };

    /************************************************************************/
    /* MEMORY SPAN ALIAS                                                    */
    /************************************************************************/

    /// \brief Type alias for a memory memory span to a read-write memory region.
    using MemorySpan = MemorySpanT<MemorySpanTraits>;

    /// \brief Type alias for a memory memory span to a read-only memory region.
    using ReadOnlyMemorySpan = MemorySpanT<ReadOnlyMemorySpanTag>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Get a pointer to the first byte in a memory memory span.
    template <typename TTraits>
    constexpr typename MemorySpanT<TTraits>::TPointer begin(const MemorySpanT<TTraits>& memory_span) noexcept;

    /// \brief Get a pointer past the last byte in a memory memory span.
    template <typename TTraits>
    constexpr typename MemorySpanT<TTraits>::TPointer end(const MemorySpanT<TTraits>& memory_span) noexcept;

    /// \brief Check whether two memory spans' contents are equivalent.
    template <typename TTraits, typename UTraits>
    constexpr Bool operator==(const MemorySpanT<TTraits>& lhs, const MemorySpanT<UTraits>& rhs) noexcept;

    /// \brief Check whether two memory spans' contents are not equivalent.
    template <typename TTraits, typename UTraits>
    constexpr Bool operator!=(const MemorySpanT<TTraits>& lhs, const MemorySpanT<UTraits>& rhs) noexcept;

    /// \brief Check whether a memory memory span is empty.
    template <typename TTraits>
    constexpr Bool IsEmpty(const MemorySpanT<TTraits>& memory_span) noexcept;

    /// \brief Get the size of a memory memory span.
    template <typename TTraits>
    constexpr Bytes Size(const MemorySpanT<TTraits>& memory_span) noexcept;

    /// \brief Access the first byte in a memory memory span.
    /// If the memory span is empty the behavior of this method is undefined.
    template <typename TTraits>
    constexpr typename MemorySpanT<TTraits>::TPointer& Front(const MemorySpanT<TTraits>& memory_span) noexcept;

    /// \brief Discard the first element in a memory span and return the resulting subspan.
    /// If the memory span is empty the behavior of this method is undefined.
    template <typename TTraits>
    constexpr MemorySpanT<TTraits> PopFront(const MemorySpanT<TTraits>& memory_span) noexcept;

    /// \brief Check whether two spans are identical (ie: they both refer to the same memory).
    template <typename TTraits, typename UTraits>
    constexpr Bool AreIdentical(const MemorySpanT<TTraits>& lhs, const MemorySpanT<UTraits>& rhs) noexcept;

    /// \brief Check whether two spans are element-wise equivalent.
    template <typename TTraits, typename UTraits>
    constexpr Bool AreEquivalent(const MemorySpanT<TTraits>& lhs, const MemorySpanT<UTraits>& rhs) noexcept;

    /// \brief Check whether rhs refers to a memory location identical or contained inside lhs.
    template <typename TTraits, typename UTraits>
    constexpr Bool Contains(const MemorySpanT<TTraits>& lhs, const MemorySpanT<UTraits>& rhs) noexcept;

    /// \brief Check whether lhs and lhs overlaps the same memory location.
    /// Empty spans are not considered to be overlapping with any other memory_span.
    template <typename TTraits, typename UTraits>
    constexpr Bool Overlaps(const MemorySpanT<TTraits>& lhs, const MemorySpanT<UTraits>& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MemorySpanT<TTraits>.

    template <typename TTraits>
    template <typename UPointer>
    constexpr MemorySpanT<TTraits>::MemorySpanT(UPointer begin, Bytes size) noexcept
        : data_(begin)
        , size_(size)
    {

    }

    template <typename TTraits>
    template <typename UPointer>
    constexpr MemorySpanT<TTraits>::MemorySpanT(UPointer begin, UPointer end) noexcept
        : data_(begin)
        , size_(ToBytes(end - begin))
    {

    }

    template <typename TTraits>
    template <typename UTraits>
    constexpr MemorySpanT<TTraits>::MemorySpanT(const MemorySpanT<UTraits>& rhs) noexcept
        : data_(rhs.GetData())
        , size_(rhs.GetSize())
    {

    }

    template <typename TTraits>
    template <typename UTraits>
    constexpr MemorySpanT<TTraits>& MemorySpanT<TTraits>::operator=(const MemorySpanT<UTraits>& rhs) noexcept
    {
        data_ = rhs.GetData();
        size_ = rhs.GetSize();

        return *this;
    }

    template <typename TTraits>
    constexpr MemorySpanT<TTraits>::operator Bool() const noexcept
    {
        return size_ > Bytes{ 0 };
    }

    template <typename TTraits>
    constexpr Bytes MemorySpanT<TTraits>::GetSize() const noexcept
    {
        return size_;
    }

    template <typename TTraits>
    constexpr typename TTraits::TPointer MemorySpanT<TTraits>::GetData() const noexcept
    {
        return data_;
    }

    // Non-member functions.

    template <typename TTraits>
    constexpr typename MemorySpanT<TTraits>::TPointer begin(const MemorySpanT<TTraits>& memory_span) noexcept
    {
        return memory_span.GetData();
    }

    template <typename TTraits>
    constexpr typename MemorySpanT<TTraits>::TPointer end(const MemorySpanT<TTraits>& memory_span) noexcept
    {
        return memory_span.GetData() + memory_span.GetSize();
    }

    template <typename TTraits, typename UTraits>
    constexpr Bool operator==(const MemorySpanT<TTraits>& lhs, const MemorySpanT<UTraits>& rhs) noexcept
    {
        return AreEquivalent(lhs, rhs);
    }

    template <typename TTraits, typename UTraits>
    constexpr Bool operator!=(const MemorySpanT<TTraits>& lhs, const MemorySpanT<UTraits>& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <typename TTraits>
    constexpr Bool IsEmpty(const MemorySpanT<TTraits>& memory_span) noexcept
    {
        return Size(memory_span) <= Bytes{ 0 };
    }

    template <typename TTraits>
    constexpr Bytes Size(const MemorySpanT<TTraits>& memory_span) noexcept
    {
        return memory_span.GetSize();
    }

    template <typename TTraits>
    constexpr typename MemorySpanT<TTraits>::TPointer& Front(const MemorySpanT<TTraits>& memory_span) noexcept
    {
        return *memory_span.GetData();
    }

    template <typename TTraits>
    constexpr MemorySpanT<TTraits> PopFront(const MemorySpanT<TTraits>& memory_span) noexcept
    {
        return { memory_span.GetData() + 1, Size(memory_span) - 1 };
    }

    template <typename TTraits, typename UTraits>
    constexpr Bool AreIdentical(const MemorySpanT<TTraits>& lhs, const MemorySpanT<UTraits>& rhs) noexcept
    {
        return (lhs.GetData() == rhs.GetData()) && (Size(lhs) == Size(rhs));
    }

    template <typename TTraits, typename UTraits>
    constexpr Bool AreEquivalent(const MemorySpanT<TTraits>& lhs, const MemorySpanT<UTraits>& rhs) noexcept
    {
        auto count = ToInt(Math::Min(Size(lhs), Size(rhs)));

        return std::memcmp(lhs.GetData(), rhs.GetData(), count) == 0;
    }

    template <typename TTraits, typename UTraits>
    constexpr Bool Contains(const MemorySpanT<TTraits>& lhs, const MemorySpanT<UTraits>& rhs) noexcept
    {
        auto lhs_begin = lhs.GetData();
        auto lhs_end = lhs_begin + Count(lhs);

        auto rhs_begin = rhs.GetData();
        auto rhs_end = rhs_begin + Count(rhs);

        return (!rhs) || ((lhs_begin <= rhs_begin) && (lhs_end >= rhs_end));
    }

    template <typename TTraits, typename UTraits>
    constexpr Bool Overlaps(const MemorySpanT<TTraits>& lhs, const MemorySpanT<UTraits>& rhs) noexcept
    {
        auto lhs_begin = lhs.GetData();
        auto lhs_end = lhs_begin + Count(lhs);

        auto rhs_begin = rhs.GetData();
        auto rhs_end = rhs_begin + Count(rhs);

        return (lhs_begin < rhs_end) && (rhs_begin < lhs_end);
    }

}
