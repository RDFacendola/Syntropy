
/// \file stream_writer.h
/// \brief This header is part of the Syntropy serialization module. It contains for stream writer classes.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/core/smart_pointers.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/serialization/output_stream.h"

namespace syntropy
{
    /************************************************************************/
    /* STREAM WRITER T <T ENCODER>                                          */
    /************************************************************************/

    /// \brief Exposes methods to sequentially write structured data to an output stream.
    /// \tparam TEncoder Type of an encoder used to encode data before writing them to the output stream.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TEncoder>
    class StreamWriterT
    {
    public:

        /// \brief Create a new writer bound to an output stream.
        StreamWriterT(OutputStream& output_stream);

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

        /// \brief Underlying output stream.
        ObserverPtr<OutputStream> output_stream_;

    };

    /************************************************************************/
    /* RAW STREAM ENCODER                                                   */
    /************************************************************************/

    /// \brief Represents a simple encoder that encodes values to their raw object-representation.
    /// Encoders are used to encode data before sending them to an output stream.
    /// \author Raffaele D. Facendola - June 2020.
    struct RawStreamEncoder
    {
        /// \brief Encode rhs and write the result to an output stream.
        template <typename TType>
        void operator()(OutputStream& lhs, const TType& rhs) const;
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
    inline StreamWriterT<TEncoder>::StreamWriterT(OutputStream& output_stream)
        : output_stream_(&output_stream)
    {

    }

    template <typename TEncoder>
    template <typename TType>
    inline StreamWriterT<TEncoder>& StreamWriterT<TEncoder>::operator<<(const TType& data)
    {
        TEncoder{}(*output_stream_, data);

        return *this;
    }

    // RawStreamEncoder.

    template <typename TType>
    inline void RawStreamEncoder::operator()(OutputStream& lhs, const TType& rhs) const
    {
        lhs << rhs;
    }

}