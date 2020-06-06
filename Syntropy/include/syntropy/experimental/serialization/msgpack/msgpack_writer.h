
/// \file msgpack_stream.h
/// \brief This header is part of the Syntropy serialization module. It contains definition for streams encoding data using the Msgpack specification.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <ostream>

#include "syntropy/language/type_traits.h"
#include "syntropy/language/utility.h"
#include "syntropy/containers/vector.h"
#include "syntropy/containers/map.h"
#include "syntropy/core/types.h"
#include "syntropy/core/string.h"
#include "syntropy/core/string_stream.h"
#include "syntropy/platform/endianness.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/memory.h"
#include "syntropy/math/math.h"

#include "syntropy/serialization/msgpack/msgpack.h"

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
    public:

        /// \brief Type of the underlying string.
        using TString = MsgpackStreamWriter::TString;
        
        /// \brief Create a writer.
        MsgpackWriter() = default;

        /// \brief Insert a null value.
        MsgpackWriter& operator<<(Null rhs);

        /// \brief Insert a boolean value.
        MsgpackWriter& operator<<(Boolean rhs);

        /// \brief Insert an integer value.
        MsgpackWriter& operator<<(Integer rhs);

        /// \brief Insert a floating point value.
        MsgpackWriter& operator<<(Floating rhs);

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

        /// \brief Underlying stream writer.
        MsgpackStreamWriter stream_writer_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MsgpackWriter.
    
    inline MsgpackWriter& MsgpackWriter::operator<<(Null rhs)
    {
        stream_writer_.Write(MsgpackFormat::kNil);

        return *this;
    }

    inline MsgpackWriter& MsgpackWriter::operator<<(Boolean rhs)
    {
        stream_writer_.Write(rhs ? MsgpackFormat::kTrue : MsgpackFormat::kFalse);

        return *this;
    }

    inline MsgpackWriter& MsgpackWriter::operator<<(Floating rhs)
    {
        auto bytes = Endianness::ToBigEndian(Memory::BitCast<Fix32>(rhs));

        stream_writer_.Write(MsgpackFormat::kFloat32, bytes);

        return *this;
    }

    template <typename TElement>
    MsgpackWriter& MsgpackWriter::operator<<(const Vector<TElement>& rhs)
    {
        // Type format and size.

        if (Msgpack::IsFixArray(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix8(rhs.size()));

            stream_writer_.Pack(MsgpackFormat::kFixArray, size);
        }
        else if (Msgpack::IsArray16(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix16(rhs.size()));

            stream_writer_.Write(MsgpackFormat::kArray16, size);
        }
        else if (Msgpack::IsArray32(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix32(rhs.size()));

            stream_writer_.Write(MsgpackFormat::kArray32, size);
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

            stream_writer_.Pack(MsgpackFormat::kFixMap, size);
        }
        else if (Msgpack::IsMap16(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix16(rhs.size()));

            stream_writer_.Write(MsgpackFormat::kMap16, size);
        }
        else if (Msgpack::IsMap32(rhs))
        {
            auto size = Endianness::ToBigEndian(ToFix32(rhs.size()));

            stream_writer_.Write(MsgpackFormat::kMap32, size);
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
        using namespace Literals;

        using TMsgpackExtension = MsgpackExtension<TExtension>;

        // Serialize the extension on a temporary buffer.

        auto payload_writer = MsgpackStreamWriter();

        auto extension_type = MsgpackExtensionType{ TMsgpackExtension::Serialize(payload_writer, rhs) };

        auto payload_size = payload_writer.GetWrittenCount();

        // Extension format, payload size and extension type.

        if (payload_size == 1_Bytes)
        {
            stream_writer_.Write(MsgpackFormat::kFixExt1, extension_type);
        }
        else if (payload_size == 2_Bytes)
        {
            stream_writer_.Write(MsgpackFormat::kFixExt2, extension_type);
        }
        else if (payload_size == 4_Bytes)
        {
            stream_writer_.Write(MsgpackFormat::kFixExt4, extension_type);
        }
        else if (payload_size == 8_Bytes)
        {
            stream_writer_.Write(MsgpackFormat::kFixExt8, extension_type);
        }
        else if (payload_size == 16_Bytes)
        {
            stream_writer_.Write(MsgpackFormat::kFixExt16, extension_type);
        }
        else if (payload_size <= 0xFF_Bytes)
        {
            auto size = Endianness::ToBigEndian(ToFix8(*payload_size));

            stream_writer_.Write(MsgpackFormat::kExt8, size, extension_type);
        }
        else if (payload_size <= 0xFFFF_Bytes)
        {
            auto size = Endianness::ToBigEndian(ToFix16(*payload_size));

            stream_writer_.Write(MsgpackFormat::kExt16, size, extension_type);
        }
        else if (payload_size <= 0xFFFFFFFF_Bytes)
        {
            auto size = Endianness::ToBigEndian(ToFix32(*payload_size));

            stream_writer_.Write(MsgpackFormat::kExt32, size, extension_type);
        }

        // Payload.

        auto payload = payload_writer.ToString();

        auto payload_data = MakeConstMemoryRange(payload.data(), Bytes(payload.size()));

        stream_writer_.WriteRaw(payload_data);

        return *this;
    }

    inline void MsgpackWriter::Clear()
    {
        stream_writer_.Clear();
    }

    inline MsgpackWriter::TString MsgpackWriter::ToString() const
    {
        return stream_writer_.ToString();
    }

}

