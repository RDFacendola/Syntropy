
/// \file msgpack_vector_extension.h
/// \brief This header is part of the syntropy serialization system. It contains traits used to encode vectors extension types using the msgpack specification.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include "syntropy/language/tuple.h"
#include "syntropy/language/language.h"
#include "syntropy/platform/endianness.h"
#include "syntropy/math/vector.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/serialization/msgpack/msgpack.h"
#include "syntropy/serialization/msgpack/msgpack_reader.h"
#include "syntropy/serialization/msgpack/msgpack_writer.h"

namespace syntropy
{
    /************************************************************************/
    /* MSGPACK VECTOR EXTENSION TYPE                                        */
    /************************************************************************/

    /// \brief Msgpack vector extension types.
    /// \author Raffaele D. Facendola - November 2019.
    enum class MsgpackVectorExtensionType : Fix8
    {
        /// \brief Vector extension format.
        kVector = ToFix8(0x00),
    };

    /************************************************************************/
    /* MSGPACK VECTOR EXTENSION FORMAT                                      */
    /************************************************************************/

    /// \brief Msgpack vector extension format.
    /// \author Raffaele D. Facendola - June 2020.
    enum class MsgpackVectorExtensionFormat : Fix8
    {
        /// \brief Format of a 32-bit floating point vector up to 4 elements.
        kFloat32 = ToFix8(0b00000000),

        /// \brief Format of a 64-bit integer vector up to 4 elements.
        kInt64 = ToFix8(0b00000100),

        /// \brief Format of a 32-bit integer vector up to 4 elements.
        kInt32 = ToFix8(0b00001000),

        /// \brief Format of a 16-bit integer vector up to 4 elements.
        kInt16 = ToFix8(0b00001100),

        /// \brief Format of a 8-bit integer vector up to 4 elements.
        kInt8 = ToFix8(0b00010000)
    };

    /// \brief Msgpack vector extension subtype formats.
    /// \author Raffaele D. Facendola - June 2020.
    enum class MsgpackVectorExtensionFormatMask : Fix8
    {
        /// \brief No bit set.
        kNone = ToFix8(0b00000000),

        /// \brief 2-bit vector rank.
        kRank = ToFix8(0b11111100),

        /// \brief All bit set.
        kAll = ToFix8(0b11111111)
    };

    /************************************************************************/
    /* MSGPACK EXTENSION                                                    */
    /************************************************************************/

    /// \brief Partial template specialization for floating-point vectors.
    template <Int kRank>
    struct MsgpackExtension<Math::VectorN<Float, kRank>>
    {
        static_assert(kRank <= 4, "Unsupported vector rank.");

        /// \brief Type of a vector.
        using TFloatVector = Math::VectorN<Float, kRank>;

        /// \brief Serialize rhs inside a stream.
        static MsgpackExtensionType Serialize(MsgpackStreamWriter& writer, const TFloatVector& rhs);

        /// \brief Decode rhs from the provided stream.
        static void Deserialize(MsgpackStreamWriter& writer, Bytes size, TFloatVector& rhs);
    };
 
    /// \brief Partial template specialization for integer vectors.
    template <Int kRank>
    struct MsgpackExtension<Math::VectorN<Int, kRank>>
    {
        static_assert(kRank <= 4, "Unsupported vector rank.");

        /// \brief Type of a vector.
        using TIntVector = Math::VectorN<Int, kRank>;

        /// \brief Serialize rhs inside a stream.
        static MsgpackExtensionType Serialize(MsgpackStreamWriter& writer, const TIntVector& rhs);

        /// \brief Decode rhs from the provided stream.
        static void Deserialize(MsgpackStreamWriter& writer, Bytes size, TIntVector& rhs);
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MsgpackExtension<Math::VectorN<Float, kRank>>

    template <Int kRank>
    MsgpackExtensionType MsgpackExtension<Math::VectorN<Float, kRank>>::Serialize(MsgpackStreamWriter& writer, const TFloatVector& rhs)
    {
        writer.Pack(MsgpackVectorExtensionFormat::kFloat32, kRank);

        auto SerializeElement = [&writer](Float element)
        {
            auto bytes = Endianness::ToBigEndian(Memory::BitCast<Fix32>(element));

            writer.Write(bytes);
        };

        LockstepApply(SerializeElement, rhs);

        return MsgpackExtensionType{ MsgpackVectorExtensionType::kVector };
    }

    template <Int kRank>
    void MsgpackExtension<Math::VectorN<Float, kRank>>::Deserialize(MsgpackStreamWriter& stream, Bytes size, TFloatVector& rhs)
    {
        //         auto format = Byte(stream.get());
        // 
        //         if (format == GetFormat() && size == GetSize(rhs))
        //         {
        //             for (auto&& element : rhs)
        //             {
        //                 auto encoded_element = MsgpackSelectEncodedTypeT<T>{};
        // 
        //                 stream.read(reinterpret_cast<Byte*>(&encoded_element), sizeof(encoded_element));
        // 
        //                 element = Msgpack::Decode<T>(encoded_element);
        //             }
        //         }
        //         else
        //         {
        //             stream.setstate(stream.rdstate() | std::ios::failbit);
        //         }
    }

    // MsgpackExtension<Math::VectorN<Int, kRank>>







}

