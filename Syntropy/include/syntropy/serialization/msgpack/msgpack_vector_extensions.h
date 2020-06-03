
/// \file msgpack_vector_extension.h
/// \brief This header is part of the syntropy serialization system. It contains traits used to encode vectors extension types using the msgpack specification.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/math/vector.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/serialization/msgpack/msgpack_format.h"
#include "syntropy/serialization/msgpack/msgpack_reader.h"
#include "syntropy/serialization/msgpack/msgpack_writer.h"

namespace syntropy
{
    /************************************************************************/
    /* MSGPACK VECTOR EXTENSION FORMAT                                      */
    /************************************************************************/

    /// \brief Vector types supported by the vector extension type.
    /// \author Raffaele D. Facendola - November 2019.
    enum class MsgpackVectorExtensionFormat : Int
    {
        /// \brief 32-bit big-endian single precision floating point vector.
        kFloat32 = 0x00,

        /// \brief 32-bit big-endian signed integer vector.
        kInt32 = 0x10,

        /// \brief 64-bit big-endian signed integer vector.
        kInt64 = 0x20,
    };

    /************************************************************************/
    /* MSGPACK SELECT ENCODED TYPE                                          */
    /************************************************************************/

    /// \brief Trait used to associate each type with an encoded data type.
    /// \author Raffaele D. Facendola - December 2019.
    template <typename TType>
    using MsgpackSelectEncodedTypeT = ConditionalT<sizeof(TType) == sizeof(Byte), Byte,
        ConditionalT<sizeof(TType) == sizeof(std::int16_t), std::int16_t,
        ConditionalT<sizeof(TType) == sizeof(std::int32_t), std::int32_t,
        ConditionalT<sizeof(TType) == sizeof(Int), Int,
        void>>>>;

    /************************************************************************/
    /* MSGPACK VECTOR EXTENSION TYPE                                        */
    /************************************************************************/

    /// \brief Base class for vector extension types up to rank 15.
    template <typename T, Int kRank, MsgpackVectorExtensionFormat kFormat>
    struct MsgpackVectorExtensionType
    {
        static_assert(kRank <= 0x0Fu, "Unsupported vector rank.");

        /// \brief Get the memory footprint of rhs, in bytes.
        static constexpr Bytes GetSize(const Math::VectorN<T, kRank>& rhs);

        /// \brief Get the extension type id.
        static constexpr Byte GetType();

        /// \brief Get the format associated to this vector extension type.
        static constexpr Byte GetFormat();

        /// \brief Encode rhs inside the provided stream.
        static void Encode(MsgpackWriter::TOutputStream& stream, const Math::VectorN<T, kRank>& rhs);

        /// \brief Decode rhs from the provided stream.
        static void Decode(MsgpackReader::TInputStream& stream, Bytes size, Math::VectorN<T, kRank>& rhs);
    };

    /************************************************************************/
    /* MSGPACK EXTENSION TYPE                                               */
    /************************************************************************/

    /// \brief Template specialization for Float vectors.
    template <Int kRank>
    struct MsgpackExtensionType<Math::VectorN<Float, kRank>> : MsgpackVectorExtensionType<Float, kRank, MsgpackVectorExtensionFormat::kFloat32> {};

    /// \brief Template specialization for 32-bit signed integer vectors.
    template <Int kRank>
    struct MsgpackExtensionType<Math::VectorN<Fix32, kRank>> : MsgpackVectorExtensionType<Fix32, kRank, MsgpackVectorExtensionFormat::kInt32> {};

    /// \brief Template specialization for 64-bit signed integer vectors.
    template <Int kRank>
    struct MsgpackExtensionType<Math::VectorN<Int, kRank>> : MsgpackVectorExtensionType<Int, kRank, MsgpackVectorExtensionFormat::kInt64> {};

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    //MsgpackVectorExtensionType<T, kRank, kFormat>.

    template <typename T, Int kRank, MsgpackVectorExtensionFormat kFormat>
    constexpr Bytes MsgpackVectorExtensionType<T, kRank, kFormat>::GetSize(const Math::VectorN<T, kRank>& rhs)
    {
        return BytesOf<MsgpackVectorExtensionFormat>() + kRank * BytesOf<T>();
    }

    template <typename T, Int kRank, MsgpackVectorExtensionFormat kFormat>
    constexpr Byte MsgpackVectorExtensionType<T, kRank, kFormat>::GetType()
    {
        return 0x00;
    }

    template <typename T, Int kRank, MsgpackVectorExtensionFormat kFormat>
    constexpr Byte MsgpackVectorExtensionType<T, kRank, kFormat>::GetFormat()
    {
        return Byte(kFormat) | (Byte(kRank) & 0b00001111);
    }

    template <typename T, Int kRank, MsgpackVectorExtensionFormat kFormat>
    void MsgpackVectorExtensionType<T, kRank, kFormat>::Encode(MsgpackWriter::TOutputStream& stream, const Math::VectorN<T, kRank>& rhs)
    {
        stream.put(GetFormat());

        for (auto&& element : rhs)
        {
            auto encoded_element = Msgpack::Encode(element);

            stream.write(reinterpret_cast<Byte*>(&encoded_element), sizeof(encoded_element));
        }
    }

    template <typename T, Int kRank, MsgpackVectorExtensionFormat kFormat>
    void MsgpackVectorExtensionType<T, kRank, kFormat>::Decode(MsgpackReader::TInputStream& stream, Bytes size, Math::VectorN<T, kRank>& rhs)
    {
        auto format = Byte(stream.get());

        if (format == GetFormat() && size == GetSize(rhs))
        {
            for (auto&& element : rhs)
            {
                auto encoded_element = MsgpackSelectEncodedTypeT<T>{};

                stream.read(reinterpret_cast<Byte*>(&encoded_element), sizeof(encoded_element));

                element = Msgpack::Decode<T>(encoded_element);
            }
        }
        else
        {
            stream.setstate(stream.rdstate() | std::ios::failbit);
        }
    }





}

