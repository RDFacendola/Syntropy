
/// \file append_stream.h
/// \brief This header is part of the Syntropy serialization module. It contains definitions for high-level sequential output streams.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/core/smart_pointers.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/serialization/append_stream_buffer.h"

namespace syntropy
{
    /************************************************************************/
    /* APPEND STREAM T <T ENCODER, T APPEND STREAM BUFFER>                  */
    /************************************************************************/

    /// \brief Exposes methods to sequentially write structured data to a stream buffer.
    /// \tparam TDecoder Type of a decoder used to encode data before being written.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TEncoder, typename TAppendStreamBuffer = AppendStreamBuffer>
    class AppendStreamT
    {
    public:

        /// \brief Create a new stream bound to a stream buffer.
        AppendStreamT(TAppendStreamBuffer& stream_buffer);

        /// \brief Default copy constructor.
        AppendStreamT(const AppendStreamT&) = default;

        /// \brief Default move constructor.
        AppendStreamT(AppendStreamT&&) = default;

        /// \brief Default copy-assignment constructor.
        AppendStreamT& operator=(const AppendStreamT&) = default;

        /// \brief Default move-assignment constructor.
        AppendStreamT& operator=(AppendStreamT&&) = default;

        /// \brief Default destructor.
        ~AppendStreamT() = default;

        /// \brief Write data sequentially to the underlying stream buffer.
        template <typename TType>
        AppendStreamT& operator<<(const TType& data);

    private:

        /// \brief Underlying stream buffer.
        ObserverPtr<TAppendStreamBuffer> stream_buffer_{ nullptr };

    };

    /************************************************************************/
    /* RAW STREAM ENCODER                                                   */
    /************************************************************************/

    /// \brief Represents a simple encoder that encodes values to their raw object-representation.
    /// \author Raffaele D. Facendola - June 2020.
    struct RawAppendStreamEncoder
    {
        /// \brief Encode bytes from rhs and write the result to lhs.
        template <typename TAppendStreamBuffer, typename TType>
        void operator()(TAppendStreamBuffer& lhs, const TType& rhs) const;
    };

    /************************************************************************/
    /* TYPE ALIASES                                                         */
    /************************************************************************/

    /// \brief Type alias for a raw stream writer.
    using AppendStream = AppendStreamT<RawAppendStreamEncoder>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new AppendStreamT by deducing template types from arguments.
    template <typename TEncoder, typename TAppendStreamBuffer>
    AppendStreamT<TEncoder, TAppendStreamBuffer> MakeAppendStream(TAppendStreamBuffer& stream_buffer);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // AppendStreamT<TEncoder, TAppendStreamBuffer>.

    template <typename TEncoder, typename TAppendStreamBuffer>
    inline AppendStreamT<TEncoder, TAppendStreamBuffer>::AppendStreamT(TAppendStreamBuffer& stream_buffer)
        : stream_buffer_(&stream_buffer)
    {

    }

    template <typename TEncoder, typename TAppendStreamBuffer>
    template <typename TType>
    inline AppendStreamT<TEncoder, TAppendStreamBuffer>& AppendStreamT<TEncoder, TAppendStreamBuffer>::operator<<(const TType& data)
    {
        TEncoder{}(*stream_buffer_, data);

        return *this;
    }

    // RawAppendStreamEncoder.

    template <typename TAppendStreamBuffer, typename TType>
    inline void RawAppendStreamEncoder::operator()(TAppendStreamBuffer& lhs, const TType& rhs) const
    {
        lhs.Append(MakeConstMemoryRange(rhs));
    }

    // Non-member functions.

    template <typename TEncoder, typename TAppendStreamBuffer>
    inline AppendStreamT<TEncoder, TAppendStreamBuffer> MakeAppendStream(TAppendStreamBuffer& stream_buffer)
    {
        return AppendStreamT<TEncoder, TAppendStreamBuffer>(stream_buffer);
    }

}