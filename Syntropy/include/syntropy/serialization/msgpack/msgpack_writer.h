
/// \file msgpack_stream.h
/// \brief This header is part of the Syntropy serialization module. It contains definition for streams encoding data using the Msgpack specification.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include "syntropy/language/type_traits.h"
#include "syntropy/language/utility.h"
#include "syntropy/core/types.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/memory/memory.h"
#include "syntropy/containers/vector.h"
#include "syntropy/containers/map.h"
#include "syntropy/platform/endianness.h"
#include "syntropy/serialization/output_stream.h"
#include "syntropy/serialization/memory_stream_buffer.h"
#include "syntropy/serialization/stream_writer.h"
#include "syntropy/serialization/msgpack/msgpack.h"

namespace syntropy
{
    /************************************************************************/
    /* MSGPACK ENCODER                                                      */
    /************************************************************************/

    /// \brief Represents a functor that encodes values using the Msgpack format.
    /// \see https://github.com/msgpack/msgpack/blob/master/spec.md
    /// \author Raffaele D. Facendola - June 2020.
    struct MsgpackStreamEncoder
    {
        /// \brief Encode rhs and write the result to an output stream.
        void operator()(OutputStream& lhs, Null rhs) const;

        /// \brief Encode rhs and write the result to an output stream.
        void operator()(OutputStream& lhs, Boolean rhs) const;

        /// \brief Encode rhs and write the result to an output stream.Integer
        void operator()(OutputStream& lhs, Integer rhs) const;

        /// \brief Encode rhs and write the result to an output stream.
        void operator()(OutputStream& lhs, Floating rhs) const;

        /// \brief Encode rhs and write the result to an output stream.
        void operator()(OutputStream& lhs, const String& rhs) const;

        /// \brief Encode rhs and write the result to an output stream.
        void operator()(OutputStream& lhs, const ConstMemoryRange& rhs) const;

        /// \brief Encode rhs and write the result to an output stream.
        template <typename TElement>
        void operator()(OutputStream& lhs, const Vector<TElement>& rhs) const;

        template <typename TKey, typename TValue>
        void operator()(OutputStream& lhs, const Map<TKey, TValue>& rhs) const;

        /// \brief Encode rhs and write the result to an output stream.
        template <typename TExtension, typename = EnableIfValidExpressionT<HasMsgpackExtensionEncoder, TExtension>>
        void operator()(OutputStream& lhs, const TExtension& rhs) const;

        /// \brief Pack together a fixed format and a 8-bit size using a mask.
        Byte Pack(MsgpackFormat format, Fix8 value, MsgpackFormatMask mask) const;

    };

    /************************************************************************/
    /* MSGPACK WRITER                                                       */
    /************************************************************************/

    /// \brief Writer for data streams encoded via Msgpack specification.
    /// \author Raffaele D. Facendola - May 2020.
    using MsgpackWriter = StreamWriterT<MsgpackStreamEncoder>;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MsgpackStreamEncoder.

    inline void MsgpackStreamEncoder::operator()(OutputStream& lhs, Null rhs) const
    {
        lhs << MsgpackFormat::kNil;
    }

    inline void MsgpackStreamEncoder::operator()(OutputStream& lhs, Boolean rhs) const
    {
        lhs << (rhs ? MsgpackFormat::kTrue : MsgpackFormat::kFalse);
    }

    inline void MsgpackStreamEncoder::operator()(OutputStream& lhs, Floating rhs) const
    {
        auto bytes = Endianness::ToBigEndian(Memory::BitCast<Fix32>(rhs));

        lhs << MsgpackFormat::kFloat32 << bytes;
    }

    template <typename TElement>
    inline void MsgpackStreamEncoder::operator()(OutputStream& lhs, const Vector<TElement>& rhs) const
    {
        // Type format and size.

        if (Msgpack::IsFixArray(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix8(rhs.size()));

            lhs << Pack(MsgpackFormat::kFixArray, encoded_size, MsgpackFormatMask::kFixArray);
        }
        else if (Msgpack::IsArray16(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix16(rhs.size()));

            lhs << MsgpackFormat::kArray16 << encoded_size;
        }
        else if (Msgpack::IsArray32(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix32(rhs.size()));

            lhs << MsgpackFormat::kArray32 << encoded_size;
        }

        // Encode each element individually.

        for (auto&& element : rhs)
        {
            (*this)(lhs, element);
        }
    }

    template <typename TKey, typename TValue>
    inline void MsgpackStreamEncoder::operator()(OutputStream& lhs, const Map<TKey, TValue>& rhs) const
    {
        // Type format and size.

        if (Msgpack::IsFixMap(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix8(rhs.size()));

            lhs << Pack(MsgpackFormat::kFixMap, encoded_size, MsgpackFormatMask::kFixMap);
        }
        else if (Msgpack::IsMap16(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix16(rhs.size()));

            lhs << MsgpackFormat::kMap16 << encoded_size;
        }
        else if (Msgpack::IsMap32(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix32(rhs.size()));

            lhs << MsgpackFormat::kMap32 << encoded_size;
        }

        // Encode each key-value pair individually.

        for (auto&& element : rhs)
        {
            (*this)(lhs, element.first);
            (*this)(lhs, element.second);
        }
    }

    template <typename TExtension, typename>
    inline void MsgpackStreamEncoder::operator()(OutputStream& lhs, const TExtension& rhs) const
    {
        using namespace Literals;

        // Serialize the extension on a temporary buffer. #TODO Allocate on thread-local stack memory resource.

        auto payload = MemoryStreamBuffer{};

        auto payload_writer = MakeOutputStream(payload);

        auto extension_type = MsgpackExtensionType{ MsgpackExtensionEncoder<TExtension>{}(payload_writer, rhs) };

        auto payload_size = payload.GetSize();

        // Extension format, payload size and extension type.

        if (payload_size == 1_Bytes)
        {
            lhs << MsgpackFormat::kFixExt1;
        }
        else if (payload_size == 2_Bytes)
        {
            lhs << MsgpackFormat::kFixExt2;
        }
        else if (payload_size == 4_Bytes)
        {
            lhs << MsgpackFormat::kFixExt4;
        }
        else if (payload_size == 8_Bytes)
        {
            lhs << MsgpackFormat::kFixExt8;
        }
        else if (payload_size == 16_Bytes)
        {
            lhs << MsgpackFormat::kFixExt16;
        }
        else if (payload_size <= 0xFF_Bytes)
        {
            auto size = Endianness::ToBigEndian(ToFix8(*payload_size));

            lhs << MsgpackFormat::kExt8 <<  size;
        }
        else if (payload_size <= 0xFFFF_Bytes)
        {
            auto size = Endianness::ToBigEndian(ToFix16(*payload_size));

            lhs << MsgpackFormat::kExt16 << size;
        }
        else if (payload_size <= 0xFFFFFFFF_Bytes)
        {
            auto size = Endianness::ToBigEndian(ToFix32(*payload_size));

            lhs << MsgpackFormat::kExt32 << size;
        }
        else
        {
            SYNTROPY_ASSERT(false);     // #TODO Signal error on the output stream.
        }

        // Payload.

        //lhs << extension_type << payload.;

    }

    inline Byte MsgpackStreamEncoder::Pack(MsgpackFormat format, Fix8 value, MsgpackFormatMask mask) const
    {
        return ToByte(format) | (ToByte(value) & ~ToByte(mask));
    }

}

