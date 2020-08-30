
/// \file consume_stream.h
/// \brief This header is part of the Syntropy serialization module. It contains definitions for high-level sequential input streams.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/foundation.h"
#include "syntropy/memory/smart_pointers.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/experimental/serialization/consume_stream_buffer.h"

namespace Syntropy
{
    /************************************************************************/
    /* CONSUME STREAM T <T DECODER, T STREAM BUFFER>                        */
    /************************************************************************/

    /// \brief Exposes methods to sequentially read structured data from a stream buffer.
    /// \tparam TDecoder Type of a decoder used to decode data after being read.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TDecoder, typename TStreamBuffer>
    class ConsumeStream
    {
    public:

        /// \brief Create a new stream bound to a stream buffer.
        ConsumeStream(TStreamBuffer& stream_buffer);

        /// \brief Default copy constructor.
        ConsumeStream(const ConsumeStream&) = default;

        /// \brief Default move constructor.
        ConsumeStream(ConsumeStream&&) = default;

        /// \brief Default copy-assignment constructor.
        ConsumeStream& operator=(const ConsumeStream&) = default;

        /// \brief Default move-assignment constructor.
        ConsumeStream& operator=(ConsumeStream&&) = default;

        /// \brief Default destructor.
        ~ConsumeStream() = default;

        /// \brief Read data sequentially from the underlying stream buffer.
        template <typename TType>
        ConsumeStream& operator>>(TType& data);

    private:

        /// \brief Underlying stream buffer.
        RWPointer<TStreamBuffer> stream_buffer_{ nullptr };

    };

    /************************************************************************/
    /* RAW APPEND STREAM DECODER                                            */
    /************************************************************************/

    /// \brief Represents a simple decoder that decodes values from their raw object-representation.
    /// \author Raffaele D. Facendola - June 2020.
    struct RawAppendStreamDecoder
    {
        /// \brief Decode bytes from lhs and write the result to rhs.
        template <typename TStreamBuffer, typename TType>
        void operator()(TStreamBuffer& lhs, TType& rhs) const;
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new AppendStream by deducing template types from arguments.
    template <typename TDecoder, typename TStreamBuffer>
    ConsumeStream<TDecoder, TStreamBuffer> MakeConstumeStream(TStreamBuffer& stream_buffer);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ConsumeStream<TDecoder>.

    template <typename TDecoder, typename TStreamBuffer>
    inline ConsumeStream<TDecoder, TStreamBuffer>::ConsumeStream(TStreamBuffer& stream_buffer)
        : stream_buffer_(&stream_buffer)
    {

    }

    template <typename TDecoder, typename TStreamBuffer>
    template <typename TType>
    inline ConsumeStream<TDecoder, TStreamBuffer>& ConsumeStream<TDecoder, TStreamBuffer>::operator>>(TType& data)
    {
        TDecoder{}(*stream_buffer_, data);

        return *this;
    }

    // RawAppendStreamDecoder.

    template <typename TStreamBuffer, typename TType>
    inline void RawAppendStreamDecoder::operator()(TStreamBuffer& lhs, TType& rhs) const
    {
        lhs.Consume(MakeMemoryRange(rhs));
    }

    // Non-member functions.

    template <typename TDecoder, typename TStreamBuffer>
    inline ConsumeStream<TDecoder, TStreamBuffer> MakeConstumeStream(TStreamBuffer& stream_buffer)
    {
        return ConsumeStream<TDecoder, TStreamBuffer>(stream_buffer);
    }

}