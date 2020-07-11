
/// \file memory_span.h
/// \brief This header is part of Syntropy memory module. It contains definitions for memory spans.
///
/// \author Raffaele D. Facendola - July 2020

#pragma once

#include "syntropy/language/type_traits.h"
#include "syntropy/core/types.h"
#include "syntropy/core/span.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"

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
        using TPointer = typename TTraits::TPointer;

        /// \brief Underlying reference type.
        using TReference = typename TTraits::TReference;

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
        constexpr typename TTraits::TPointer GetData() const noexcept;

    private:

        /// \brief Pointer to the first element in the memory region.
        typename TTraits::TPointer data_{ nullptr };

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

        /// \brief Reference to a byte.
        using TReference = Byte&;
    };

    /// \brief Traits for a memory span in a read only memory region.
    struct ReadOnlyMemorySpanTraits
    {
        /// \brief Address of the underlying memory region.
        using TPointer = ReadOnlyBytePtr;

        /// \brief Reference to a byte.
        using TReference = const Byte&;
    };

    /************************************************************************/
    /* MEMORY SPAN ALIAS                                                    */
    /************************************************************************/

    /// \brief Type alias for a memory span to a read-write memory region.
    using MemorySpan = MemorySpanT<MemorySpanTraits>;

    /// \brief Type alias for a memory span to a read-only memory region.
    using ReadOnlyMemorySpan = MemorySpanT<ReadOnlyMemorySpanTraits>;

    /// \brief Type of a memory span which may contain elements of type TElement.
    template <typename TElement>
    using SelectMemorySpanT = ConditionalT<IsConstV<TElement>, ReadOnlyMemorySpan, MemorySpan>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Get a pointer to the first byte in a memory span.
    template <typename TTraits>
    constexpr typename MemorySpanT<TTraits>::TPointer begin(const MemorySpanT<TTraits>& memory_span) noexcept;

    /// \brief Get a pointer past the last byte in a memory span.
    template <typename TTraits>
    constexpr typename MemorySpanT<TTraits>::TPointer end(const MemorySpanT<TTraits>& memory_span) noexcept;

    /// \brief Check whether two memory spans' contents are equivalent.
    template <typename TTraits, typename UTraits>
    constexpr Bool operator==(const MemorySpanT<TTraits>& lhs, const MemorySpanT<UTraits>& rhs) noexcept;

    /// \brief Check whether two memory spans' contents are not equivalent.
    template <typename TTraits, typename UTraits>
    constexpr Bool operator!=(const MemorySpanT<TTraits>& lhs, const MemorySpanT<UTraits>& rhs) noexcept;

    /// \brief Check whether a memory span is empty.
    template <typename TTraits>
    constexpr Bool IsEmpty(const MemorySpanT<TTraits>& memory_span) noexcept;

    /// \brief Get the size of a memory span.
    template <typename TTraits>
    constexpr Bytes Size(const MemorySpanT<TTraits>& memory_span) noexcept;

    /// \brief Access the first byte in a memory span.
    /// If the memory span is empty the behavior of this method is undefined.
    template <typename TTraits>
    constexpr typename MemorySpanT<TTraits>::TReference& Front(const MemorySpanT<TTraits>& memory_span) noexcept;

    /// \brief Discard the first element in a memory span and return the resulting subspan.
    /// If the memory span is empty the behavior of this method is undefined.
    template <typename TTraits>
    constexpr MemorySpanT<TTraits> PopFront(const MemorySpanT<TTraits>& memory_span) noexcept;

    /// \brief Discard some bytes in memory span and return the resulting memory span.
    /// If this method would cause the new span to exceed the original one, the behavior of this method is undefined.
    template <typename TTraits>
    constexpr MemorySpanT<TTraits> PopFront(const MemorySpanT<TTraits>& memory_span, Bytes bytes) noexcept;

    /// \brief Obtain a memory span consisting of the first elements of a memory span.
    /// If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TTraits>
    constexpr MemorySpanT<TTraits> First(const MemorySpanT<TTraits>& memory_span, Bytes bytes) noexcept;

    /// \brief Check whether two spans are identical (ie: they both refer to the same memory region).
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

    /// \brief Check whether a memory span is aligned to a given alignment value.
    /// This method only accounts for the first element in the span.
    template <typename TTraits>
    constexpr Bool IsAlignedTo(const MemorySpanT<TTraits>& memory_span, Alignment alignment) noexcept;

    /// \brief Consume a memory span until its first byte is aligned to a given boundary or the span is exhausted.
    template <typename TTraits>
    constexpr MemorySpanT<TTraits> Align(const MemorySpanT<TTraits>& memory_span, Alignment alignment) noexcept;

    /// \brief Convert a memory span to a strongly-typed span.
    /// If the underlying memory span doesn't refer to TElements or it has non-integer number of elements, the behavior of this method is undefined.
    template <typename TElement, typename TTraits>
    constexpr Span<TElement> ToSpan(const MemorySpanT<TTraits>& memory_span) noexcept;

    /// \brief Convert a strongly-typed span to a memory span.
    template <typename TElement>
    constexpr SelectMemorySpanT<TElement> ToMemorySpan(const Span<TElement>& rhs) noexcept;

    /// \brief Convert a strongly-typed span to a read-only memory span.
    template <typename TElement>
    constexpr ReadOnlyMemorySpan ToReadOnlyMemorySpan(const Span<TElement>& rhs) noexcept;

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
        using namespace literals;

        return Size(memory_span) <= 0_Bytes;
    }

    template <typename TTraits>
    constexpr Bytes Size(const MemorySpanT<TTraits>& memory_span) noexcept
    {
        return memory_span.GetSize();
    }

    template <typename TTraits>
    constexpr typename MemorySpanT<TTraits>::TReference& Front(const MemorySpanT<TTraits>& memory_span) noexcept
    {
        return *memory_span.GetData();
    }

    template <typename TTraits>
    constexpr MemorySpanT<TTraits> PopFront(const MemorySpanT<TTraits>& memory_span) noexcept
    {
        using namespace literals;

        return PopFront(memory_span, 1_Bytes);
    }

    template <typename TTraits>
    constexpr MemorySpanT<TTraits> PopFront(const MemorySpanT<TTraits>& memory_span, Bytes bytes) noexcept
    {
        return { memory_span.GetData() + bytes, Size(memory_span) - bytes };
    }

    template <typename TTraits>
    constexpr MemorySpanT<TTraits> First(const MemorySpanT<TTraits>& memory_span, Bytes bytes) noexcept
    {
        return { memory_span.GetData(), bytes };
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
        // Empty spans do not contain any other span.

        if (!lhs)
        {
            return false;
        }

        // Empty spans are contained in any other set, except empty spans.

        if (!rhs)
        {
            return true;
        }

        // Test span boundaries.

        auto lhs_begin = lhs.GetData();
        auto lhs_end = lhs_begin + Size(lhs);

        auto rhs_begin = rhs.GetData();
        auto rhs_end = rhs_begin + Size(rhs);

        return (lhs_begin <= rhs_begin) && (lhs_end >= rhs_end);
    }

    template <typename TTraits, typename UTraits>
    constexpr Bool Overlaps(const MemorySpanT<TTraits>& lhs, const MemorySpanT<UTraits>& rhs) noexcept
    {
        auto lhs_begin = lhs.GetData();
        auto lhs_end = lhs_begin + Size(lhs);

        auto rhs_begin = rhs.GetData();
        auto rhs_end = rhs_begin + Size(rhs);

        return (lhs_begin < rhs_end) && (rhs_begin < lhs_end);
    }

    template <typename TTraits>
    constexpr Bool IsAlignedTo(const MemorySpanT<TTraits>& memory_span, Alignment alignment) noexcept
    {
        auto address = reinterpret_cast<Int>(memory_span.GetData());

        return (address & (ToInt(alignment) - 1)) == 0;
    }

    template <typename TTraits>
    constexpr MemorySpanT<TTraits> Align(const MemorySpanT<TTraits>& memory_span, Alignment alignment) noexcept
    {
        using TPointer = typename MemorySpanT<TTraits>::TPointer;

        auto begin_address = reinterpret_cast<Int>(memory_span.GetData());
        auto end_address = begin_address + ToInt(Size(memory_span));

        auto alignment_mask = ToInt(alignment) - 1;

        begin_address = Math::Min((begin_address + alignment_mask) & ~alignment_mask, end_address);

        auto begin_ptr = reinterpret_cast<TPointer>(begin_address);
        auto end_ptr = reinterpret_cast<TPointer>(end_address);

        return { begin_ptr, end_ptr };
    }

    template <typename TElement, typename TTraits>
    constexpr Span<TElement> ToSpan(const MemorySpanT<TTraits>& rhs) noexcept
    {
        auto begin = reinterpret_cast<ObserverPtr<TElement>>(rhs.GetData());
        auto size = Size(rhs) / BytesOf<TElement>();

        return { begin, size };
    }

    template <typename TElement>
    constexpr SelectMemorySpanT<TElement> ToMemorySpan(const Span<TElement>& rhs) noexcept
    {
        auto begin = reinterpret_cast<typename SelectMemorySpanT<TElement>::TPointer>(rhs.GetData());

        auto size = rhs.GetCount() * BytesOf<TElement>();

        return { begin, size };
    }

    template <typename TElement>
    constexpr ReadOnlyMemorySpan ToReadOnlyMemorySpan(const Span<TElement>& rhs) noexcept
    {
        auto begin = reinterpret_cast<const Byte*>(rhs.GetData());

        auto size = rhs.GetCount() * BytesOf<TElement>();

        return { begin, size };
    }

}
