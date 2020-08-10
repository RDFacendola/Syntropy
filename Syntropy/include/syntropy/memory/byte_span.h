
/// \file byte_span.h
/// \brief This header is part of Syntropy memory module. It contains definitions for byte spans and related utility functions.
///
/// \author Raffaele D. Facendola - July 2020

#pragma once

#include <typeinfo>

#include "syntropy/diagnostics/assert.h"

#include "syntropy/core/types.h"
#include "syntropy/core/span.h"

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"

namespace Syntropy
{
    /************************************************************************/
    /* BYTE SPAN                                                            */
    /************************************************************************/

    /// \brief Represents a span of read-only bytes.
    using ByteSpan = Span<Byte>;

    /// \brief Represents a span of read-write bytes.
    using RWByteSpan = RWSpan<Byte>;

    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    /// \brief Exposes byte span-related definitions.
    namespace Memory
    {
        /// \brief Get the memory footprint of rhs.
        template <typename TElement>
        constexpr Bytes Size(const SpanT<TElement>& rhs) noexcept;

        /// \brief Get the read-only object representation of rhs.
        /// An object representation is the sequence of bytes starting from the object address.
        /// If rhs type is not exactly TObject, the behavior of this method is undefined.
        template <typename TObject>
        ByteSpan BytesOf(const TObject& rhs) noexcept;

        /// \brief Get the read-write object representation of rhs.
        /// An object representation is the sequence of bytes starting from the object address.
        /// If rhs type is not exactly TObject, the behavior of this method is undefined.
        template <typename TObject>
        RWByteSpan RWBytesOf(TObject& rhs) noexcept;

        /// \brief Check whether the first byte in lhs is aligned to alignment
        /// If the provided span is empty the behavior of this method is undefined.
        Bool IsAlignedTo(const ByteSpan& lhs, Alignment alignment) noexcept;

        /// \brief Consume lhs from the front until its first byte is aligned to alignment or lhs is exhausted.
        ByteSpan Align(const ByteSpan& lhs, Alignment alignment) noexcept;

        /// \brief Consume lhs from the front until its first byte is aligned to alignment or lhs is exhausted.
        RWByteSpan Align(const RWByteSpan& lhs, Alignment alignment) noexcept;

        /// \brief Reduce lhs from the back until its size if multiple of size or lhs is exhausted.
        ByteSpan Floor(const ByteSpan& lhs, Bytes size) noexcept;

        /// \brief Reduce lhs from the back until its size if multiple of size or lhs is exhausted.
        RWByteSpan Floor(const RWByteSpan& lhs, Bytes size) noexcept;

    }

    /************************************************************************/
    /* TYPE CAST                                                            */
    /************************************************************************/

    /// \brief Convert rhs to a read-only typed span.
    /// If rhs doesn't refer to a span of TElements or it has a non-integer number of elements, the behavior of this method is undefined.
    template <typename TElement>
    Span<TElement> ToSpan(const ByteSpan& rhs) noexcept;

    /// \brief Convert rhs to a read-write typed span.
    /// If rhs doesn't refer to a span of TElements or it has a non-integer number of elements, the behavior of this method is undefined.
    template <typename TElement>
    RWSpan<TElement> ToRWSpan(const RWByteSpan& rhs) noexcept;

    /// \brief Convert an read-only span to a read-only byte span.
    template <typename TElement>
    ByteSpan ToByteSpan(const SpanT<TElement>& rhs) noexcept;

    /// \brief Convert a read-write span to a read-write byte span.
    template <typename TElement>
    RWByteSpan ToRWByteSpan(const SpanT<TElement>& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Memory.
    // =======

    template <typename TElement>
    constexpr Bytes Memory::Size(const SpanT<TElement>& rhs) noexcept
    {
        return Count(rhs) * SizeOf<TElement>();
    }

    template <typename TObject>
    inline ByteSpan Memory::BytesOf(const TObject& rhs) noexcept
    {
        auto& type_from = typeid(rhs);
        auto& type_to = typeid(TObject);

        SYNTROPY_UNDEFINED_BEHAVIOR(type_from == type_to, "Dynamic type mismatch.");

        return ByteSpan{ ToBytePtr(&rhs), ToInt(SizeOf<TObject>()) };
    }

    template <typename TObject>
    inline RWByteSpan Memory::RWBytesOf(TObject& rhs) noexcept
    {
        auto& type_from = typeid(rhs);
        auto& type_to = typeid(TObject);

        SYNTROPY_UNDEFINED_BEHAVIOR(type_from == type_to, "Dynamic type mismatch.");

        return RWByteSpan{ ToRWBytePtr(&rhs), ToInt(SizeOf<TObject>()) };
    }

    inline Bool Memory::IsAlignedTo(const ByteSpan& lhs, Alignment alignment) noexcept
    {
        SYNTROPY_UNDEFINED_BEHAVIOR(!IsEmpty(lhs), "Empty spans don't have a well-defined alignment.");

        return IsAlignedTo(Begin(lhs), alignment);
    }

    inline ByteSpan Memory::Align(const ByteSpan& lhs, Alignment alignment) noexcept
    {
        auto begin = Align(lhs.GetData(), alignment);
        auto end = End(lhs);

        return { Math::Min(begin, end), end };
    }

    inline RWByteSpan Memory::Align(const RWByteSpan& lhs, Alignment alignment) noexcept
    {
        return ReadWrite(Align(ReadOnly(lhs), alignment));
    }

    inline ByteSpan Memory::Floor(const ByteSpan& lhs, Bytes size) noexcept
    {
        auto floor_size = Math::Floor(Memory::Size(lhs), size);

        return Front(lhs, ToInt(floor_size));
    }

    inline RWByteSpan Memory::Floor(const RWByteSpan& lhs, Bytes size) noexcept
    {
        return ReadWrite(Floor(ReadOnly(lhs), size));
    }

    // Type cast.
    // ==========

    template <typename TElement>
    inline Span<TElement> ToSpan(const ByteSpan& rhs) noexcept
    {
        using TPointer = Pointer<AddConstT<TElement>>;

        auto begin = reinterpret_cast<TPointer>(Begin(rhs));
        auto end = reinterpret_cast<TPointer>(End(rhs));

        return { begin, end };
    }

    template <typename TElement>
    inline RWSpan<TElement> ToRWSpan(const RWByteSpan& rhs) noexcept
    {
        using TRWPointer = Pointer<RemoveConstT<TElement>>;

        auto begin = reinterpret_cast<TRWPointer>(Begin(rhs));
        auto end = reinterpret_cast<TRWPointer>(End(rhs));

        return { begin, end };
    }

    template <typename TElement>
    inline ByteSpan ToByteSpan(const SpanT<TElement>& rhs) noexcept
    {
        auto begin = ToBytePtr(Begin(rhs));
        auto end = ToBytePtr(End(rhs));

        return { begin, end };
    }

    template <typename TElement>
    inline RWByteSpan ToRWByteSpan(const SpanT<TElement>& rhs) noexcept
    {
        auto begin = ToRWBytePtr(Begin(rhs));
        auto end = ToRWBytePtr(End(rhs));

        return { begin, end };
    }

}
