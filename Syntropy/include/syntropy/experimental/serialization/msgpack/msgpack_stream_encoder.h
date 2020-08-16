
/// \file msgpack_stream_encoder.h
/// \brief This header is part of the Syntropy serialization module. It contains definitions for Msgpack stream encoders.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include "syntropy/language/type_traits.h"
#include "syntropy/language/language_types.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/core/string.h"
#include "syntropy/containers/vector.h"
#include "syntropy/containers/map.h"
#include "syntropy/experimental/serialization/msgpack/msgpack.h"
#include "syntropy/experimental/serialization/memory_stream_buffer.h"

#include "syntropy/language/utility.h"
#include "syntropy/memory/memory.h"
#include "syntropy/platform/endianness.h"

namespace syntropy
{
    /************************************************************************/
    /* MSGPACK STREAM ENCODER                                               */
    /************************************************************************/

    /// \brief Represents a functor that encodes values using the Msgpack format.
    /// \see https://github.com/msgpack/msgpack/blob/master/spec.md
    /// \author Raffaele D. Facendola - June 2020.
    struct MsgpackStreamEncoder
    {
        /// \brief Encode rhs and write the result to an output stream.
        template <typename TAppendStreamBuffer>
        void operator()(TAppendStreamBuffer&& lhs, Null rhs) const;

        /// \brief Encode rhs and write the result to an output stream.
        template <typename TAppendStreamBuffer>
        void operator()(TAppendStreamBuffer&& lhs, Boolean rhs) const;

        /// \brief Encode rhs and write the result to an output stream.Integer
        template <typename TAppendStreamBuffer>
        void operator()(TAppendStreamBuffer&& lhs, Integer rhs) const;

        /// \brief Encode rhs and write the result to an output stream.
        template <typename TAppendStreamBuffer>
        void operator()(TAppendStreamBuffer&& lhs, Floating rhs) const;

        /// \brief Encode rhs and write the result to an output stream.
        template <typename TAppendStreamBuffer>
        void operator()(TAppendStreamBuffer&& lhs, const String& rhs) const;

        /// \brief Encode rhs and write the result to an output stream.
        template <typename TAppendStreamBuffer>
        void operator()(TAppendStreamBuffer&& lhs, const ConstMemoryRange& rhs) const;

        /// \brief Encode rhs and write the result to an output stream.
        template <typename TAppendStreamBuffer, typename TElement>
        void operator()(TAppendStreamBuffer&& lhs, const Vector<TElement>& rhs) const;

        template <typename TAppendStreamBuffer, typename TKey, typename TValue>
        void operator()(TAppendStreamBuffer&& lhs, const Map<TKey, TValue>& rhs) const;

        /// \brief Encode rhs and write the result to an output stream.
        template <typename TAppendStreamBuffer, typename TExtension, typename = EnableIfValidExpressionT<HasMsgpackExtensionEncoder, TExtension>>
        void operator()(TAppendStreamBuffer&& lhs, const TExtension& rhs) const;

        /// \brief Append a value rhs to lhs using its object-representation.
        template <typename TAppendStreamBuffer, typename TType>
        void Append(TAppendStreamBuffer&& lhs, const TType& rhs) const;

        /// \brief Pack together a fixed format and a 8-bit size using a mask.
        Byte Pack(MsgpackFormat format, Fix8 value, MsgpackFormatMask mask) const;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MsgpackStreamEncoder.

    template <typename TAppendStreamBuffer>
    inline void MsgpackStreamEncoder::operator()(TAppendStreamBuffer&& lhs, Null rhs) const
    {
        Append(lhs, MsgpackFormat::kNil);
    }

    template <typename TAppendStreamBuffer>
    inline void MsgpackStreamEncoder::operator()(TAppendStreamBuffer&& lhs, Boolean rhs) const
    {
        Append(lhs, rhs ? MsgpackFormat::kTrue : MsgpackFormat::kFalse);
    }

    template <typename TAppendStreamBuffer>
    void MsgpackStreamEncoder::operator()(TAppendStreamBuffer&& lhs, Integer rhs) const
    {
        if (Msgpack::IsPositiveFixInt(rhs))
        {
            auto value = Endianness::ToBigEndian(ToFix8(rhs));

            auto bytes = Pack(MsgpackFormat::kPositiveFixInt, value, MsgpackFormatMask::kPositiveFixInt);

            Append(lhs, bytes);
        }
        else if (Msgpack::IsNegativeFixInt(rhs))
        {
            auto value = Endianness::ToBigEndian(ToFix8(-rhs));

            auto bytes = Pack(MsgpackFormat::kNegativeFixInt, value, MsgpackFormatMask::kNegativeFixInt);

            Append(lhs, bytes);
        }
        else if (Msgpack::IsInt8(rhs))
        {
            auto bytes = Endianness::ToBigEndian(ToFix8(rhs));

            Append(lhs, MsgpackFormat::kInt8);
            Append(lhs, bytes);
        }
        else if (Msgpack::IsInt16(rhs))
        {
            auto bytes = Endianness::ToBigEndian(ToFix16(rhs));

            Append(lhs, MsgpackFormat::kInt16);
            Append(lhs, bytes);
        }
        else if (Msgpack::IsInt32(rhs))
        {
            auto bytes = Endianness::ToBigEndian(ToFix32(rhs));

            Append(lhs, MsgpackFormat::kInt32);
            Append(lhs, bytes);
        }
        else
        {
            auto bytes = Endianness::ToBigEndian(ToFix64(rhs));

            Append(lhs, MsgpackFormat::kInt64);
            Append(lhs, bytes);
        }
    }

    template <typename TAppendStreamBuffer>
    inline void MsgpackStreamEncoder::operator()(TAppendStreamBuffer&& lhs, Floating rhs) const
    {
        auto bytes = Endianness::ToBigEndian(Memory::BitCast<Fix32>(rhs));

        Append(lhs, MsgpackFormat::kFloat32);
        Append(lhs, bytes);
    }

    template <typename TAppendStreamBuffer>
    void MsgpackStreamEncoder::operator()(TAppendStreamBuffer&& lhs, const String& rhs) const
    {
        // Type format and size.

        if (Msgpack::IsFixStr(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix8(rhs.size()));

            auto bytes = Pack(MsgpackFormat::kFixStr, encoded_size, MsgpackFormatMask::kFixStr);

            Append(lhs, bytes);
        }
        else if (Msgpack::IsStr8(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix8(rhs.size()));

            Append(lhs, MsgpackFormat::kStr8);
            Append(lhs, encoded_size);
        }
        else if (Msgpack::IsStr16(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix16(rhs.size()));

            Append(lhs, MsgpackFormat::kStr16);
            Append(lhs, encoded_size);
        }
        else if (Msgpack::IsStr32(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix32(rhs.size()));

            Append(lhs, MsgpackFormat::kStr32);
            Append(lhs, encoded_size);
        }
        else
        {
            SYNTROPY_ASSERT(false);     // #TODO Signal error on the output stream.
        }

        // Payload.

        auto payload = MakeConstMemoryRange(rhs.data(), ToBytes(rhs.size()));

        lhs.Append(payload);
    }

    template <typename TAppendStreamBuffer>
    void MsgpackStreamEncoder::operator()(TAppendStreamBuffer&& lhs, const ConstMemoryRange& rhs) const
    {
        // Type format and size.

        auto size = *rhs.GetSize();

        if (Msgpack::IsBin8(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix8(size));

            Append(lhs, MsgpackFormat::kBin8);
            Append(lhs, encoded_size);
        }
        else if (Msgpack::IsBin16(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix16(size));

            Append(lhs, MsgpackFormat::kBin16);
            Append(lhs, encoded_size);
        }
        else if (Msgpack::IsBin32(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix32(size));

            Append(lhs, MsgpackFormat::kBin32);
            Append(lhs, encoded_size);
        }
        else
        {
            SYNTROPY_ASSERT(false);     // #TODO Signal error on the output stream.
        }

        // Payload.

        lhs.Append(rhs);
    }

    template <typename TAppendStreamBuffer,typename TElement>
    inline void MsgpackStreamEncoder::operator()(TAppendStreamBuffer&& lhs, const Vector<TElement>& rhs) const
    {
        // Type format and size.

        if (Msgpack::IsFixArray(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix8(rhs.size()));

            auto bytes = Pack(MsgpackFormat::kFixArray, encoded_size, MsgpackFormatMask::kFixArray);

            Append(lhs, bytes);
        }
        else if (Msgpack::IsArray16(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix16(rhs.size()));

            Append(lhs, MsgpackFormat::kArray16);
            Append(lhs, encoded_size);
        }
        else if (Msgpack::IsArray32(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix32(rhs.size()));

            Append(lhs, MsgpackFormat::kArray32);
            Append(lhs, encoded_size);
        }
        else
        {
            SYNTROPY_ASSERT(false);     // #TODO Signal error on the output stream.
        }

        // Encode each element individually.

        for (auto&& element : rhs)
        {
            (*this)(lhs, element);
        }
    }

    template <typename TAppendStreamBuffer, typename TKey, typename TValue>
    inline void MsgpackStreamEncoder::operator()(TAppendStreamBuffer&& lhs, const Map<TKey, TValue>& rhs) const
    {
        // Type format and size.

        if (Msgpack::IsFixMap(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix8(rhs.size()));

            auto bytes = Pack(MsgpackFormat::kFixMap, encoded_size, MsgpackFormatMask::kFixMap);

            Append(lhs, bytes);
        }
        else if (Msgpack::IsMap16(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix16(rhs.size()));

            Append(lhs, MsgpackFormat::kMap16);
            Append(lhs, encoded_size);
        }
        else if (Msgpack::IsMap32(rhs))
        {
            auto encoded_size = Endianness::ToBigEndian(ToFix32(rhs.size()));

            Append(lhs, MsgpackFormat::kMap32);
            Append(lhs, encoded_size);
        }
        else
        {
            SYNTROPY_ASSERT(false);     // #TODO Signal error on the output stream.
        }

        // Encode each key-value pair individually.

        for (auto&& element : rhs)
        {
            (*this)(lhs, element.first);
            (*this)(lhs, element.second);
        }
    }

    template <typename TAppendStreamBuffer, typename TExtension, typename>
    inline void MsgpackStreamEncoder::operator()(TAppendStreamBuffer&& lhs, const TExtension& rhs) const
    {
        using namespace literals;

        // Serialize the extension on a temporary buffer so that total size is known in advance. #TODO Allocate on thread-local stack memory resource.

        auto payload = MemoryStreamBuffer{};

        auto payload_writer = AppendStream(payload);

        auto extension_type = MsgpackExtensionType{ MsgpackExtensionEncoder<TExtension>{}(payload_writer, rhs) };

        auto payload_size = payload.GetSize();

        // Extension format, payload size and extension type.

        if (payload_size == 1_Bytes)
        {
            Append(lhs, MsgpackFormat::kFixExt1);
        }
        else if (payload_size == 2_Bytes)
        {
            Append(lhs, MsgpackFormat::kFixExt2);
        }
        else if (payload_size == 4_Bytes)
        {
            Append(lhs, MsgpackFormat::kFixExt4);
        }
        else if (payload_size == 8_Bytes)
        {
            Append(lhs, MsgpackFormat::kFixExt8);
        }
        else if (payload_size == 16_Bytes)
        {
            Append(lhs, MsgpackFormat::kFixExt16);
        }
        else if (payload_size <= 0xFF_Bytes)
        {
            auto size = Endianness::ToBigEndian(ToFix8(*payload_size));

            Append(lhs, MsgpackFormat::kExt8);
            Append(lhs, size);
        }
        else if (payload_size <= 0xFFFF_Bytes)
        {
            auto size = Endianness::ToBigEndian(ToFix16(*payload_size));

            Append(lhs, MsgpackFormat::kExt16);
            Append(lhs, size);
        }
        else if (payload_size <= 0xFFFFFFFF_Bytes)
        {
            auto size = Endianness::ToBigEndian(ToFix32(*payload_size));

            Append(lhs, MsgpackFormat::kExt32);
            Append(lhs, size);
        }
        else
        {
            SYNTROPY_ASSERT(false);     // #TODO Signal error on the output stream.
        }

        // Extension type and payload.

        Append(lhs, extension_type);

        lhs.Append(payload.Release().GetConstData());

    }

    template <typename TAppendStreamBuffer, typename TType>
    inline void MsgpackStreamEncoder::Append(TAppendStreamBuffer&& lhs, const TType& rhs) const
    {
        lhs.Append(MakeConstMemoryRange(rhs));
    }

    inline Byte MsgpackStreamEncoder::Pack(MsgpackFormat format, Fix8 value, MsgpackFormatMask mask) const
    {
        return ToByte(format) | (ToByte(value) & ~ToByte(mask));
    }

}

