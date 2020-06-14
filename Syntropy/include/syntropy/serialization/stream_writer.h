
/// \file stream_writer.h
/// \brief This header is part of the Syntropy serialization module. It contains for stream writer classes.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/core/smart_pointers.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/serialization/append_stream.h"

namespace syntropy
{
    /************************************************************************/
    /* STREAM WRITER T <T ENCODER>                                          */
    /************************************************************************/

    /// \brief Exposes methods to sequentially write structured data to a stream.
    /// \tparam TEncoder Type of an encoder used to encode data prior to append operation.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TEncoder>
    class StreamWriterT
    {
    public:

        /// \brief Create a new writer bound to a stream.
        StreamWriterT(AppendStream& stream);

        /// \brief Default copy constructor.
        StreamWriterT(const StreamWriterT&) = default;

        /// \brief Default move constructor.
        StreamWriterT(StreamWriterT&&) = default;

        /// \brief Default copy-assignment constructor.
        StreamWriterT& operator=(const StreamWriterT&) = default;

        /// \brief Default move-assignment constructor.
        StreamWriterT& operator=(StreamWriterT&&) = default;

        /// \brief Default destructor.
        ~StreamWriterT() = default;

        /// \brief Write data sequentially to the underlying stream.
        template <typename TType>
        StreamWriterT& operator<<(const TType& data);

    private:

        /// \brief Underlying stream.
        ObserverPtr<AppendStream> stream_{ nullptr };

    };

    /************************************************************************/
    /* RAW STREAM ENCODER                                                   */
    /************************************************************************/

    /// \brief Represents a simple encoder that encodes values to their raw object-representation.
    /// \author Raffaele D. Facendola - June 2020.
    struct RawStreamEncoder
    {
        /// \brief Encode rhs and write the result to an stream.
        template <typename TType>
        void operator()(AppendStream& lhs, const TType& rhs) const;
    };

    /************************************************************************/
    /* TYPE ALIASES                                                         */
    /************************************************************************/

    /// \brief Type alias for a raw stream writer.
    using StreamWriter = StreamWriterT<RawStreamEncoder>;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // StreamWriterT.

    template <typename TEncoder>
    inline StreamWriterT<TEncoder>::StreamWriterT(AppendStream& stream)
        : stream_(&stream)
    {

    }

    template <typename TEncoder>
    template <typename TType>
    inline StreamWriterT<TEncoder>& StreamWriterT<TEncoder>::operator<<(const TType& data)
    {
        TEncoder{}(*stream_, data);

        return *this;
    }

    // RawStreamEncoder.

    template <typename TType>
    inline void RawStreamEncoder::operator()(AppendStream& lhs, const TType& rhs) const
    {
        lhs << rhs;
    }

}