
/// \file byte_span.h
/// \brief This header is part of Syntropy memory module. It contains definitions for byte spans.
///
/// \author Raffaele D. Facendola - July 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/support/swap.h"

#include "syntropy/core/concepts/range.h"

#include "syntropy/math/math.h"

#include "syntropy/memory/size.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte.h"

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* BASE BYTE SPAN                                                       */
    /************************************************************************/

    /// \brief Represents a contiguous, non-owning, range of bytes.
    /// \author Raffaele D. Facendola - December 2020.
    template <typename TTraits>
    class BaseByteSpan
    {
        template <typename TTraits>
        friend constexpr Bytes Count(Immutable<BaseByteSpan<TTraits>> rhs) noexcept;

        template <typename TTraits>
        friend constexpr typename TTraits::TPointer Data(Immutable<BaseByteSpan<TTraits>> rhs) noexcept;

    public:

        /// \brief Pointer type.
        using TPointer = typename TTraits::TPointer;

        /// \brief Reference type.
        using TReference = typename TTraits::TReference;

        /// \brief Create an empty byte span.
        constexpr BaseByteSpan() noexcept = default;

        /// \brief Create an empty byte span.
        constexpr BaseByteSpan(Null) noexcept;

        /// \brief Create a byte span given a pointer to the first byte and span size.
        constexpr BaseByteSpan(TPointer begin, Immutable<Bytes> size) noexcept;

        /// \brief Create a byte span given a pointer to both the first and past the last byte.
        constexpr BaseByteSpan(TPointer begin, TPointer end) noexcept;

        /// \brief Default copy constructor.
        constexpr BaseByteSpan(Immutable<BaseByteSpan> rhs) noexcept = default;

        /// \brief Converting copy constructor.
        template <typename UTraits>
        requires (!Concepts::SameAs<TTraits, UTraits>)
        constexpr BaseByteSpan(Immutable<BaseByteSpan<UTraits>> rhs) noexcept;

        /// \brief Default destructor.
        ~BaseByteSpan() noexcept = default;

        /// \brief Copy assignment operator.
        template <typename UTraits>
        constexpr Mutable<BaseByteSpan> operator=(Immutable<BaseByteSpan<UTraits>> rhs) noexcept;

        ///  \brief Check whether the byte span is non-empty.
        [[nodiscard]] constexpr explicit operator Bool() const noexcept;

        /// \brief Access a byte by offset.
        /// If the provided offset is not within the byte span the behavior of this method is undefined.
        [[nodiscard]] constexpr TReference operator[](Immutable<Bytes> offset) const noexcept;

    private:

        /// \brief Pointer to the first element.
        TPointer data_{ nullptr };

        /// \brief Span size.
        Bytes size_{ 0 };

    };

    /************************************************************************/
    /* BYTE SPAN                                                            */
    /************************************************************************/

    /// \brief Tag for read-only byte spans.
    struct ByteSpanTypeTraits
    {
        /// \brief Pointer type.
        using TPointer = BytePtr;

        /// \brief Reference type.
        using TReference = Immutable<Byte>;
    };

    /// \brief Represents a span of read-only bytes.
    using ByteSpan = BaseByteSpan<ByteSpanTypeTraits>;

    /************************************************************************/
    /* RW BYTE SPAN                                                         */
    /************************************************************************/

    /// \brief Tag for read-write byte spans.
    struct RWByteSpanTypeTraits
    {
        /// \brief Pointer type.
        using TPointer = RWBytePtr;

        /// \brief Reference type.
        using TReference = Mutable<Byte>;
    };

    /// \brief Represents a span of read-write bytes.
    using RWByteSpan = BaseByteSpan<RWByteSpanTypeTraits>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Comparison.
    // ===========

    /// \brief Check whether lhs and rhs are equivalent.
    template <typename TTraits, typename UTraits>
    [[nodiscard]] constexpr Bool operator==(Immutable<BaseByteSpan<TTraits>> lhs, Immutable<BaseByteSpan<UTraits>> rhs) noexcept;

    /// \brief Compare two spans lexicographically.
    template <typename TTraits, typename UTraits>
    [[nodiscard]] constexpr Ordering operator<=>(Immutable<BaseByteSpan<TTraits>> lhs, Immutable<BaseByteSpan<UTraits>> rhs) noexcept;

    // Forward range.
    // ==============

    /// \brief Access the first byte in a byte span.
    /// \remarks Accessing the first byte of an empty span results in undefined behavior.
    template <typename TTraits>
    [[nodiscard]] constexpr typename TTraits::TReference Front(Immutable<BaseByteSpan<TTraits>> rhs) noexcept;

    /// \brief Discard the first bytes in a byte span and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TTraits>
    [[nodiscard]] constexpr BaseByteSpan<TTraits> PopFront(Immutable<BaseByteSpan<TTraits>> rhs) noexcept;

    /// \brief Check whether a byte span is empty.
    /// \return Returns true if the span is empty, returns false otherwise.
    [[nodiscard]] constexpr Bool IsEmpty(Immutable<ByteSpan> rhs) noexcept;

    // Sized range.
    // ============

    /// \brief Get the size of a byte span.
    template <typename TTraits>
    [[nodiscard]] constexpr Bytes Count(Immutable<BaseByteSpan<TTraits>> rhs) noexcept;

    // Bidirectional range.
    // ====================

    /// \brief Access the last byte in a byte span.
    /// \remarks Accessing the last byte of an empty span results in undefined behavior.
    template <typename TTraits>
    [[nodiscard]] constexpr typename TTraits::TReference Back(Immutable<BaseByteSpan<TTraits>> rhs) noexcept;

    /// \brief Discard the last bytes in a byte span and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TTraits>
    [[nodiscard]] constexpr BaseByteSpan<TTraits> PopBack(Immutable<BaseByteSpan<TTraits>> rhs) noexcept;

    // Random access range.
    // ====================

    /// \brief Obtain a sub-span given an offset and a size.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    template <typename TTraits>
    [[nodiscard]] constexpr BaseByteSpan<TTraits> Select(Immutable<BaseByteSpan<TTraits>> rhs, Immutable<Bytes> offset, Immutable<Bytes> size) noexcept;

    /// \brief Obtain a span element at given index.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    template <typename TTraits>
    [[nodiscard]] constexpr typename TTraits::TReference Select(Immutable<BaseByteSpan<TTraits>> rhs, Immutable<Bytes> offset) noexcept;

    // Contiguous range.
    // =================

    /// \brief Access underlying span data.
    /// \remarks Accessing data of an empty span is allowed but the returned value is unspecified.
    template <typename TTraits>
    [[nodiscard]] constexpr typename TTraits::TPointer Data(Immutable<BaseByteSpan<TTraits>> rhs) noexcept;

    // Alignment.
    // ==========

    /// \brief Consume lhs from the front until its first byte is aligned to rhs or lhs is exhausted.
    template <typename TTraits>
    [[nodiscard]] BaseByteSpan<TTraits> Align(Immutable<BaseByteSpan<TTraits>> lhs, Immutable<Alignment> alignment) noexcept;

    /// \brief Consume lhs from the back until its size is a multiple of size or lhs is exhausted.
    template <typename TTraits>
    [[nodiscard]] BaseByteSpan<TTraits> Floor(Immutable<BaseByteSpan<TTraits>> lhs, Immutable<Bytes> size) noexcept;

    // Conversions.
    // ============

    /// \brief Get the read-only object representation of rhs.
    /// An object representation is the sequence of bytes starting from the object address.
    /// If rhs type is not exactly TObject, the behavior of this method is undefined.
    template <typename TObject>
    [[nodiscard]] ByteSpan BytesOf(Immutable<TObject> rhs) noexcept;

    /// \brief Get the read-write object representation of rhs.
    /// An object representation is the sequence of bytes starting from the object address.
    /// If rhs type is not exactly TObject, the behavior of this method is undefined.
    template <typename TObject>
    [[nodiscard]] RWByteSpan BytesOf(Mutable<TObject> rhs) noexcept;

    /// \brief Get an object TObject from its object representation.
    /// \remarks If rhs is not exactly TObject, accessing the returned value results in undefined behavior.
    template <typename TObject, typename TTraits>
    [[nodiscard]] Reference<TObject> FromBytesOf(Immutable<BaseByteSpan<TTraits>> rhs) noexcept;

    /// \brief Get the object representation of bytes in the contiguous range rhs.
    template <Concepts::ContiguousRange TRange>
    [[nodiscard]] auto RangeBytesOf(Immutable<TRange> rhs) noexcept;

    /// \brief Get the object representation of bytes in the contiguous range rhs.
    /// \remarks The range-byte representation of a BaseByteSpan is the span itself.
    template <typename TTraits>
    [[nodiscard]] BaseByteSpan<TTraits> RangeBytesOf(Immutable<BaseByteSpan<TTraits>> rhs) noexcept;

    /// \brief Get a contiguous range of strongly-typed elements from its range object representation.
    /// \remarks If rhs is not exactly a range TRange, accessing the returned value results in undefined behavior.
    template <Concepts::ContiguousRange TRange, typename TTraits>
    [[nodiscard]] TRange FromRangeBytesOf(Immutable<BaseByteSpan<TTraits>> rhs) noexcept;

    // Access.
    // =======

    /// \brief Convert rhs to a read-only byte span.
    [[nodiscard]] ByteSpan ToReadOnly(Immutable<ByteSpan> rhs) noexcept;

    /// \brief Convert rhs to a read-write byte span.
    /// \remarks If the original memory location is not read-writable, accessing the returned values results in undefined behavior.
    [[nodiscard]] RWByteSpan ToReadWrite(Immutable<ByteSpan> rhs) noexcept;

    // Utilities.
    // ==========

    /// \brief Create a new byte span by deducing template from arguments.
    [[nodiscard]] constexpr ByteSpan MakeByteSpan(BytePtr begin, Immutable<Bytes> size) noexcept;

    /// \brief Create a new byte span by deducing template from arguments.
    [[nodiscard]] constexpr ByteSpan MakeByteSpan(BytePtr begin, BytePtr end) noexcept;

    /// \brief Create a new byte span by deducing template from arguments.
    [[nodiscard]] constexpr RWByteSpan MakeByteSpan(RWBytePtr begin, Immutable<Bytes> size) noexcept;

    /// \brief Create a new byte span by deducing template from arguments.
    [[nodiscard]] constexpr RWByteSpan MakeByteSpan(RWBytePtr begin, RWBytePtr end) noexcept;

}

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* RANGE TRAITS                                                         */
    /************************************************************************/

    /// \brief Specialization for byte spans.
    template <typename TTraits>
    struct RangeElementReferenceTypeTraits<Memory::BaseByteSpan<TTraits>> : Alias<typename TTraits::TReference> {};

    /// \brief Specialization for byte spans.
    template <typename TTraits>
    struct RangeElementPointerTypeTraits<Memory::BaseByteSpan<TTraits>> : Alias<typename TTraits::TPointer> {};

    /// \brief Specialization for byte spans.
    template <typename TTraits>
    struct RangeElementCountTypeTraits<Memory::BaseByteSpan<TTraits>> : Alias<Memory::Bytes> {};
}

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // BaseByteSpan.

    template <typename TTraits>
    constexpr BaseByteSpan<TTraits>::BaseByteSpan(Null) noexcept
    {

    }

    template <typename TTraits>
    constexpr BaseByteSpan<TTraits>::BaseByteSpan(BaseByteSpan<TTraits>::TPointer begin, Immutable<Bytes> size) noexcept
        : data_(begin)
        , size_(size)
    {

    }

    template <typename TTraits>
    constexpr BaseByteSpan<TTraits>::BaseByteSpan(BaseByteSpan<TTraits>::TPointer begin, BaseByteSpan<TTraits>::TPointer end) noexcept
        : BaseByteSpan(begin, ToBytes(end - begin))
    {

    }

    template <typename TTraits>
    template <typename UTraits>
    requires (!Concepts::SameAs<TTraits, UTraits>)
    constexpr BaseByteSpan<TTraits>::BaseByteSpan(Immutable<BaseByteSpan<UTraits>> rhs) noexcept
        : data_(Data(rhs))
        , size_(Count(rhs))
    {

    }

    template <typename TTraits>
    template <typename UTraits>
    constexpr Mutable<BaseByteSpan<TTraits>> BaseByteSpan<TTraits>::operator=(Immutable<BaseByteSpan<UTraits>> rhs) noexcept
    {
        data_ = Data(rhs);
        size_ = Count(rhs);

        return *this;
    }

     template <typename TTraits>
     [[nodiscard]] constexpr BaseByteSpan<TTraits>::operator Bool() const noexcept
     {
         return size_ > ToBytes(0);
     }

    template <typename TTraits>
    [[nodiscard]] constexpr typename BaseByteSpan<TTraits>::TReference BaseByteSpan<TTraits>::operator[](Immutable<Bytes> offset) const noexcept
    {
        return data_[offset];
    }

    // Non-member functions.
    // =====================

    // Comparison.

    template <typename TTraits, typename UTraits>
    [[nodiscard]] constexpr Bool operator==(Immutable<BaseByteSpan<TTraits>> lhs, Immutable<BaseByteSpan<UTraits>> rhs) noexcept
    {
        using namespace Ranges;

        return AreEqual(lhs, rhs) || AreEquivalent(lhs, rhs);
    }

    template <typename TTraits, typename UTraits>
    [[nodiscard]] constexpr Ordering operator<=>(Immutable<BaseByteSpan<TTraits>> lhs, Immutable<BaseByteSpan<UTraits>> rhs) noexcept
    {
        using namespace Ranges;

        return Compare(lhs, rhs);
    }

    // Forward Range.

    template <typename TTraits>
    [[nodiscard]] constexpr typename TTraits::TReference Front(Immutable<BaseByteSpan<TTraits>> rhs) noexcept
    {
        return *Data(rhs);
    }

    template <typename TTraits>
    [[nodiscard]] constexpr BaseByteSpan<TTraits> PopFront(Immutable<BaseByteSpan<TTraits>> rhs) noexcept
    {
        using Syntropy::ToInt;

        return { Data(rhs) + ToInt(1), Data(rhs) + Count(rhs) };
    }

    [[nodiscard]] constexpr Bool IsEmpty(Immutable<ByteSpan> rhs) noexcept
    {
        return Count(rhs) == ToBytes(0);
    }

    // Sized range.

    template <typename TTraits>
    [[nodiscard]] constexpr Bytes Count(Immutable<BaseByteSpan<TTraits>> rhs) noexcept
    {
        return rhs.size_;
    }

    // Bidirectional range.

    template <typename TTraits>
    [[nodiscard]] constexpr typename TTraits::TReference Back(Immutable<BaseByteSpan<TTraits>> rhs) noexcept
    {
        return *(Data(rhs) + Count(rhs) - ToBytes(1));
    }

    template <typename TTraits>
    [[nodiscard]] constexpr BaseByteSpan<TTraits> PopBack(Immutable<BaseByteSpan<TTraits>> rhs) noexcept
    {
        return { Data(rhs), Data(rhs) + Count(rhs) - ToBytes(1) };
    }

    // Random access range.

    template <typename TTraits>
    [[nodiscard]] constexpr BaseByteSpan<TTraits> Select(Immutable<BaseByteSpan<TTraits>> rhs, Immutable<Bytes> offset, Immutable<Bytes> size) noexcept
    {
        return { Data(rhs) + offset, size };
    }

    template <typename TTraits>
    [[nodiscard]] constexpr typename TTraits::TReference Select(Immutable<BaseByteSpan<TTraits>> rhs, Immutable<Bytes> offset) noexcept
    {
        using Syntropy::ToInt;

        return rhs[ToInt(offset)];
    }

    // Contiguous range.

    template <typename TTraits>
    [[nodiscard]] constexpr typename TTraits::TPointer Data(Immutable<BaseByteSpan<TTraits>> rhs) noexcept
    {
        return rhs.data_;
    }

    // Alignment.

    template <typename TTraits>
    [[nodiscard]] inline BaseByteSpan<TTraits> Align(Immutable<BaseByteSpan<TTraits>> lhs, Immutable<Alignment> alignment) noexcept
    {
        auto begin = Align(Data(lhs), alignment);
        auto end = Data(lhs) + Count(lhs);

        auto min_begin_end = Math::Min(begin, end);

        return { min_begin_end, end };
    }

    template <typename TTraits>
    [[nodiscard]] inline BaseByteSpan<TTraits> Floor(Immutable<BaseByteSpan<TTraits>> lhs, Immutable<Bytes> size) noexcept
    {
        auto floor_size = Math::Floor(Count(lhs), size);

        return Front(lhs, floor_size);
    }

    // Conversion.

    template <typename TObject>
    [[nodiscard]] inline ByteSpan BytesOf(Immutable<TObject> rhs) noexcept
    {
        auto data = ToBytePtr(&rhs);
        auto size = SizeOf<TObject>();

        return { data, size };
    }

    template <typename TObject>
    [[nodiscard]] inline RWByteSpan BytesOf(Mutable<TObject> rhs) noexcept
    {
        auto data = ToBytePtr(&rhs);
        auto size = SizeOf<TObject>();

        return { data, size };
    }

    template <typename TObject, typename TTraits>
    [[nodiscard]] inline Reference<TObject> FromBytesOf(Immutable<BaseByteSpan<TTraits>> rhs) noexcept
    {
        return *FromTypelessPtr<TObject>(Data(rhs));
    }

    template <Concepts::ContiguousRange TRange>
    [[nodiscard]] inline auto RangeBytesOf(Immutable<TRange> rhs) noexcept
    {
        using RangeElement = Templates::RangeElementPointer<TRange>;

        auto data = ToBytePtr(Data(rhs));
        auto size = SizeOf<RangeElement>() * Count(rhs);

        return MakeByteSpan( data, size );
    }

    template <typename TTraits>
    [[nodiscard]] inline BaseByteSpan<TTraits> RangeBytesOf(Immutable<BaseByteSpan<TTraits>> rhs) noexcept
    {
        return rhs;
    }

    template <Concepts::ContiguousRange TRange, typename TTraits>
    [[nodiscard]] inline TRange FromRangeBytesOf(Immutable<BaseByteSpan<TTraits>> rhs) noexcept
    {
        if constexpr (Concepts::SameAs<TRange, BaseByteSpan<TTraits>>)
        {
            return rhs;
        }
        else
        {
            using RangeElement = Templates::RangeElementPointer<TRange>;

            auto data = FromTypelessPtr<RangeElement>(Data(rhs));
            auto count = Count(rhs) / SizeOf<RangeElement>();

            return TRange{ data, count };
        }
    }

    // Access.

    [[nodiscard]] inline ByteSpan ToReadOnly(Immutable<ByteSpan> rhs) noexcept
    {
        return rhs;
    }

    [[nodiscard]] inline RWByteSpan ToReadWrite(Immutable<ByteSpan> rhs) noexcept
    {
        using Syntropy::ToReadWrite;

        return { ToReadWrite(Data(rhs)), Count(rhs) };
    }

    // Utilities.

    [[nodiscard]] constexpr ByteSpan MakeByteSpan(BytePtr begin, Immutable<Bytes> size) noexcept
    {
        return { begin, size };
    }

    [[nodiscard]] constexpr ByteSpan MakeByteSpan(BytePtr begin, BytePtr end) noexcept
    {
        return { begin, end };
    }

    [[nodiscard]] constexpr RWByteSpan MakeByteSpan(RWBytePtr begin, Immutable<Bytes> size) noexcept
    {
        return { begin, size };
    }

    [[nodiscard]] constexpr RWByteSpan MakeByteSpan(RWBytePtr begin, RWBytePtr end) noexcept
    {
        return { begin, end };
    }

}

// ===========================================================================
