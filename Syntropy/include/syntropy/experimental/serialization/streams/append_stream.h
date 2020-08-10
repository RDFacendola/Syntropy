
/// \file append_stream.h
/// \brief This header is part of the Syntropy serialization module. It contains definitions for high-level sequential output streams.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/core/smart_pointers.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/experimental/serialization/append_stream_buffer.h"

namespace Syntropy
{
    /************************************************************************/
    /* APPEND STREAM <T ENCODER, T STREAM BUFFER>                           */
    /************************************************************************/

    /// \brief Exposes methods to sequentially write structured data to a stream buffer.
    /// \tparam TDecoder Type of a decoder used to encode data before being written.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TEncoder, typename TStreamBuffer>
    class AppendStream
    {
    public:

        /// \brief Create a new stream bound to a stream buffer.
        AppendStream(TStreamBuffer& stream_buffer);

        /// \brief Default copy constructor.
        AppendStream(const AppendStream&) = default;

        /// \brief Default move constructor.
        AppendStream(AppendStream&&) = default;

        /// \brief Default copy-assignment constructor.
        AppendStream& operator=(const AppendStream&) = default;

        /// \brief Default move-assignment constructor.
        AppendStream& operator=(AppendStream&&) = default;

        /// \brief Default destructor.
        ~AppendStream() = default;

        /// \brief Write data sequentially to the underlying stream buffer.
        template <typename TType>
        AppendStream& operator<<(const TType& data);

    private:

        /// \brief Underlying stream buffer.
        Pointer<TStreamBuffer> stream_buffer_{ nullptr };

    };

    /************************************************************************/
    /* RAW APPEND STREAM ENCODER                                            */
    /************************************************************************/

    /// \brief Represents a simple encoder that encodes values to their raw object-representation.
    /// \author Raffaele D. Facendola - June 2020.
    struct RawAppendStreamEncoder
    {
        /// \brief Encode bytes from rhs and write the result to lhs.
        template <typename TStreamBuffer, typename TType>
        void operator()(TStreamBuffer& lhs, const TType& rhs) const;
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new AppendStream by deducing template types from arguments.
    template <typename TEncoder, typename TStreamBuffer>
    AppendStream<TEncoder, TStreamBuffer> MakeAppendStream(TStreamBuffer& stream_buffer);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // AppendStream<TEncoder, TStreamBuffer>.

    template <typename TEncoder, typename TStreamBuffer>
    inline AppendStream<TEncoder, TStreamBuffer>::AppendStream(TStreamBuffer& stream_buffer)
        : stream_buffer_(&stream_buffer)
    {

    }

    template <typename TEncoder, typename TStreamBuffer>
    template <typename TType>
    inline AppendStream<TEncoder, TStreamBuffer>& AppendStream<TEncoder, TStreamBuffer>::operator<<(const TType& data)
    {
        TEncoder{}(*stream_buffer_, data);

        return *this;
    }

    // RawAppendStreamEncoder.

    template <typename TStreamBuffer, typename TType>
    inline void RawAppendStreamEncoder::operator()(TStreamBuffer& lhs, const TType& rhs) const
    {
        lhs.Append(MakeConstMemoryRange(rhs));
    }

    // Non-member functions.

    template <typename TEncoder, typename TStreamBuffer>
    inline AppendStream<TEncoder, TStreamBuffer> MakeAppendStream(TStreamBuffer& stream_buffer)
    {
        return AppendStream<TEncoder, TStreamBuffer>(stream_buffer);
    }

}