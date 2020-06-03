
/// \file msgpack_stream.h
/// \brief This header is part of the Syntropy serialization module. It contains definition for streams encoding data using the Msgpack specification.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <ostream>

#include "syntropy/core/types.h"
#include "syntropy/core/string.h"
#include "syntropy/core/string_stream.h"
#include "syntropy/platform/endianness.h"
#include "syntropy/containers/vector.h"
#include "syntropy/containers/map.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/memory.h"
#include "syntropy/math/math.h"
#include "syntropy/language/type_traits.h"

#include "syntropy/serialization/msgpack/msgpack_format.h"

namespace syntropy
{
    /************************************************************************/
    /* MSGPACK WRITER                                                       */
    /************************************************************************/

    /// \brief Writer for data streams encoded via Msgpack specification.
    /// \author Raffaele D. Facendola - May 2020.
    /// \see https://github.com/msgpack/msgpack/blob/master/spec.md
    class MsgpackWriter
    {
        /// \brief Predicate used to determine whether TType is a valid extension type for Msgpack.
        template <typename TType>
        using IsMsgpackExtension = decltype(MsgpackExtensionType<TType>);

    public:

        /// \brief Type of the underlying string.
        using TString = BasicString<Byte>;

        /// \brief Type of the underlying stream.
        using TStream = BasicStringStream<Byte>;
        
        /// \brief Type of the underlying output stream.
        using TOutputStream = BasicOStringStream<Byte>;

        /// \brief Create a writer.
        MsgpackWriter() = default;

        /// \brief Insert a null value.
        MsgpackWriter& operator<<(Null rhs);

        /// \brief Insert a boolean value.
        MsgpackWriter& operator<<(Bool rhs);

        /// \brief Insert an integer value.
        MsgpackWriter& operator<<(Int rhs);

        /// \brief Insert a floating point value.
        MsgpackWriter& operator<<(Float rhs);

        /// \brief Insert a string.
        MsgpackWriter& operator<<(const String& rhs);

        /// \brief Insert a byte-array.
        MsgpackWriter& operator<<(const ConstMemoryRange& rhs);

        /// \brief Insert an array.
        template <typename TElement>
        MsgpackWriter& operator<<(const Vector<TElement>& rhs);

        /// \brief Insert a map.
        template <typename TKey, typename TValue>
        MsgpackWriter& operator<<(const Map<TKey, TValue>& rhs);

        /// \brief Insert an extension-type value.
        template <typename TExtension, typename = EnableIfValidExpressionT<IsMsgpackExtension, TExtension>>
        MsgpackWriter& operator<<(const TExtension& rhs);

        /// \brief Clear the underlying stream.
        void Clear();

        /// \brief Access the underlying string.
        TString ToString() const;

    private:

        /// \ brief Write a packed byte formed by a format and a payload.
        /// If payload bits spill into format's the behavior of this method is undefined.
        MsgpackWriter& Pack(MsgpackFormat format, Fix8 payload);

        /// \brief Write one or more values inside the underlying stream.
        template <typename... TTypes>
        MsgpackWriter& Write(TTypes&&... values);

        /// \brief Write data inside the underlying stream.
        MsgpackWriter& Write(const ConstMemoryRange& data);

        /// \brief Underlying stream.
        TStream stream_;
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MsgpackWriter.

    inline MsgpackWriter& MsgpackWriter::operator<<(Null rhs)
    {
        return Write(MsgpackFormat::kNil);
    }

    inline MsgpackWriter& MsgpackWriter::operator<<(Bool rhs)
    {
        return Write(rhs ? MsgpackFormat::kTrue : MsgpackFormat::kFalse);
    }

    inline MsgpackWriter& MsgpackWriter::operator<<(Int rhs)
    {
        if (Msgpack::IsPositiveFixInt(rhs))
        {
            auto bytes = Endianness::ToBigEndian(ToFix8(rhs));

            Pack(MsgpackFormat::kPositiveFixInt, bytes);
        }
        else if (Msgpack::IsNegativeFixInt(rhs))
        {
            auto bytes = Endianness::ToBigEndian(ToFix8(-rhs));

            Pack(MsgpackFormat::kNegativeFixInt, bytes);
        }
        else if (Msgpack::IsInt8(rhs))
        {
            auto bytes = Endianness::ToBigEndian(ToFix8(rhs));

            Write(MsgpackFormat::kInt8, bytes);
        }
        else if (Msgpack::IsInt16(rhs))
        {
            auto bytes = Endianness::ToBigEndian(ToFix16(rhs));

            Write(MsgpackFormat::kInt16, bytes);
        }
        else if (Msgpack::IsInt32(rhs))
        {
            auto bytes = Endianness::ToBigEndian(ToFix32(rhs));

            Write(MsgpackFormat::kInt32, bytes);
        }
        else
        {
            auto bytes = Endianness::ToBigEndian(ToFix64(rhs));

            Write(MsgpackFormat::kInt64, bytes);
        }

        return *this;
    }

    inline MsgpackWriter& MsgpackWriter::operator<<(Float rhs)
    {
        auto bytes = Endianness::ToBigEndian(Memory::BitCast<Fix32>(rhs));

        return Write(MsgpackFormat::kFloat32, bytes);
    }

    template <typename TElement>
    MsgpackWriter& MsgpackWriter::operator<<(const Vector<TElement>& rhs)
    {
        // Type format and size.

        if (Msgpack::IsFixArray(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix8(rhs.size()));

            Pack(MsgpackFormat::kFixArray, size);
        }
        else if (Msgpack::IsArray16(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix16(rhs.size()));

            Write(MsgpackFormat::kArray16, size);
        }
        else if (Msgpack::IsArray32(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix32(rhs.size()));

            Write(MsgpackFormat::kArray32, size);
        }

        // Payload.

        for (auto&& element : rhs)
        {
            operator<<(element);
        }

        return *this;
    }

    template <typename TKey, typename TValue>
    MsgpackWriter& MsgpackWriter::operator<<(const Map<TKey, TValue>& rhs)
    {
        // Type format and size.

        if (Msgpack::IsFixMap(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix8(rhs.size()));

            Pack(MsgpackFormat::kFixMap, size);
        }
        else if (Msgpack::IsMap16(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix16(rhs.size()));

            Write(MsgpackFormat::kMap16, size);
        }
        else if (Msgpack::IsMap32(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix32(rhs.size()));

            Put(MsgpackFormat::kMap32, size);
        }

        // Payload.

        for (auto&& element : rhs)
        {
            operator<<(element.first);
            operator<<(element.second);
        }

        return *this;
    }

    template <typename TExtension, typename>
    MsgpackWriter& MsgpackWriter::operator<<(const TExtension& rhs)
    {
        using TMsgpackExtensionType = MsgpackExtensionType<TExtension>;

        auto type_id = TMsgpackExtensionType::GetType();
        auto payload_size = MsgpackExtensionType<TExtension>::GetSize(rhs);

        static_assert(sizeof(type_id) == sizeof(Fix8), "Extension type-id must be 8-bit wide.");

        // Type format, payload size and type-id.

        if (Msgpack::IsFixExt1(rhs))
        {
            Write(MsgpackFormat::kFixExt1, type_id);
        }
        else if (Msgpack::IsFixExt2(rhs))
        {
            Write(MsgpackFormat::kFixExt2, type_id);
        }
        else if (Msgpack::IsFixExt4(rhs))
        {
            Write(MsgpackFormat::kFixExt4, type_id);
        }
        else if (Msgpack::IsFixExt8(rhs))
        {
            Write(MsgpackFormat::kFixExt8, type_id);
        }
        else if (Msgpack::IsFixExt16(rhs))
        {
            Write(MsgpackFormat::kFixExt16, type_id);
        }
        else if (Msgpack::IsExt8(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix8(*payload_size));

            Write(MsgpackFormat::kExt8, size, type_id);
        }
        else if (Msgpack::IsExt16(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix16(*payload_size));

            Write(MsgpackFormat::kExt16, size, type_id);
        }
        else if (Msgpack::IsExt32(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix32(*payload_size));

            Write(MsgpackFormat::kExt32, size, type_id);
        }

        // Payload.

        TMsgpackExtensionType::Encode(static_cast<TOutputStream&>(stream_), rhs);

        return *this;
    }

    inline void MsgpackWriter::Clear()
    {
        stream_.str({});
    }

    inline MsgpackWriter::TString MsgpackWriter::ToString() const
    {
        return stream_.str();
    }

    inline MsgpackWriter& MsgpackWriter::Pack(MsgpackFormat format, Fix8 payload)
    {
        Write(ToFix8(format) | payload);

        return *this;
    }

    template <typename... TTypes>
    inline MsgpackWriter& MsgpackWriter::Write(TTypes&&... values)
    {
        ((stream_ << MakeConstMemoryRange(values)), ...);

        return *this;
    }

    inline MsgpackWriter& MsgpackWriter::Write(const ConstMemoryRange& data)
    {
        stream_ << data;

        return *this;
    }

}

