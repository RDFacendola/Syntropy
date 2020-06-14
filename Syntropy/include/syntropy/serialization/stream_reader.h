
/// \file stream_reader.h
/// \brief This header is part of the Syntropy serialization module. It contains for stream reader classes.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/core/smart_pointers.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/serialization/consume_stream.h"

namespace syntropy
{
    /************************************************************************/
    /* STREAM READER T <T DECODER>                                          */
    /************************************************************************/

    /// \brief Exposes methods to sequentially read structured data from a  stream.
    /// \tparam TDecoder Type of a decoder used to decode data after being read from the stream.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TDecoder>
    class StreamReaderT
    {
    public:

        /// \brief Create a new writer bound to a stream.
        StreamReaderT(ConsumeStream& stream);

        /// \brief Default copy constructor.
        StreamReaderT(const StreamReaderT&) = default;

        /// \brief Default move constructor.
        StreamReaderT(StreamReaderT&&) = default;

        /// \brief Default copy-assignment constructor.
        StreamReaderT& operator=(const StreamReaderT&) = default;

        /// \brief Default move-assignment constructor.
        StreamReaderT& operator=(StreamReaderT&&) = default;

        /// \brief Default destructor.
        ~StreamReaderT() = default;

        /// \brief Read data sequentially from the underlying stream.
        template <typename TType>
        StreamReaderT& operator>>(TType& data);

    private:

        /// \brief Underlying stream.
        ObserverPtr<ConsumeStream> stream_;

    };

    /************************************************************************/
    /* RAW STREAM DECODER                                                   */
    /************************************************************************/

    /// \brief Represents a simple decoder that decodes values from their raw object-representation.
    /// \author Raffaele D. Facendola - June 2020.
    struct RawStreamDecoder
    {
        /// \brief Decode bytes from lhs and write the result to rhs.
        /// If the data were encoded using an encoder different than RawStreamEncoder, the behavior of this method is undefined.
        template <typename TType>
        void operator()(ConsumeStream& lhs, TType& rhs) const;
    };

    /************************************************************************/
    /* TYPE ALIASES                                                         */
    /************************************************************************/

    /// \brief Type alias for a raw stream reader.
    using StreamReader = StreamReaderT<RawStreamDecoder>;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // StreamReaderT.

    template <typename TDecoder>
    inline StreamReaderT<TDecoder>::StreamReaderT(ConsumeStream& stream)
        : stream_(&stream)
    {

    }

    template <typename TDecoder>
    template <typename TType>
    inline StreamReaderT<TDecoder>& StreamReaderT<TDecoder>::operator>>(TType& data)
    {
        TDecoder{}(*stream_, data);

        return *this;
    }

    // RawStreamDecoder.

    template <typename TType>
    inline void RawStreamDecoder::operator()(ConsumeStream& lhs, TType& rhs) const
    {
        lhs >> rhs;
    }

}