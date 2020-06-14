
/// \file consume_stream.h
/// \brief This header is part of the Syntropy serialization module. It contains definitions for high-level sequential input streams.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/core/smart_pointers.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/serialization/consume_stream_buffer.h"

namespace syntropy
{
    /************************************************************************/
    /* CONSUME STREAM T <T DECODER>                                         */
    /************************************************************************/

    /// \brief Exposes methods to sequentially read structured data from a stream buffer.
    /// \tparam TDecoder Type of a decoder used to decode data after being read.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TDecoder>
    class ConsumeStreamT
    {
    public:

        /// \brief Create a new stream bound to a stream buffer.
        ConsumeStreamT(ConsumeStreamBuffer& stream_buffer);

        /// \brief Default copy constructor.
        ConsumeStreamT(const ConsumeStreamT&) = default;

        /// \brief Default move constructor.
        ConsumeStreamT(ConsumeStreamT&&) = default;

        /// \brief Default copy-assignment constructor.
        ConsumeStreamT& operator=(const ConsumeStreamT&) = default;

        /// \brief Default move-assignment constructor.
        ConsumeStreamT& operator=(ConsumeStreamT&&) = default;

        /// \brief Default destructor.
        ~ConsumeStreamT() = default;

        /// \brief Read data sequentially from the underlying stream buffer.
        template <typename TType>
        ConsumeStreamT& operator>>(TType& data);

    private:

        /// \brief Underlying stream buffer.
        ObserverPtr<ConsumeStreamBuffer> stream_buffer_{ nullptr };

    };

    /************************************************************************/
    /* RAW STREAM DECODER                                                   */
    /************************************************************************/

    /// \brief Represents a simple decoder that decodes values from their raw object-representation.
    /// \author Raffaele D. Facendola - June 2020.
    struct RawStreamDecoder
    {
        /// \brief Decode bytes from lhs and write the result to rhs.
        template <typename TType>
        void operator()(ConsumeStreamBuffer& lhs, TType& rhs) const;
    };

    /************************************************************************/
    /* TYPE ALIASES                                                         */
    /************************************************************************/

    /// \brief Type alias for a raw consume stream.
    using ConsumeStream = ConsumeStreamT<RawStreamDecoder>;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ConsumeStreamT<TDecoder>.

    template <typename TDecoder>
    inline ConsumeStreamT<TDecoder>::ConsumeStreamT(ConsumeStreamBuffer& stream_buffer)
        : stream_buffer_(&stream_buffer)
    {

    }

    template <typename TDecoder>
    template <typename TType>
    inline ConsumeStreamT<TDecoder>& ConsumeStreamT<TDecoder>::operator>>(TType& data)
    {
        TDecoder{}(*stream_buffer_, data);

        return *this;
    }

    // RawStreamDecoder.

    template <typename TType>
    inline void RawStreamDecoder::operator()(ConsumeStreamBuffer& lhs, TType& rhs) const
    {
        lhs.Consume(MakeMemoryRange(rhs));
    }

}