
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
    /* APPEND STREAM T <T ENCODER>                                          */
    /************************************************************************/

    /// \brief Exposes methods to sequentially write structured data to a stream buffer.
    /// \tparam TDecoder Type of a decoder used to encode data before being written.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TEncoder>
    class AppendStreamT
    {
    public:

        /// \brief Create a new stream bound to a stream buffer.
        AppendStreamT(AppendStreamBuffer& stream_buffer);

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
        ObserverPtr<AppendStreamBuffer> stream_buffer_{ nullptr };

    };

    /************************************************************************/
    /* RAW STREAM ENCODER                                                   */
    /************************************************************************/

    /// \brief Represents a simple encoder that encodes values to their raw object-representation.
    /// \author Raffaele D. Facendola - June 2020.
    struct RawStreamEncoder
    {
        /// \brief Encode bytes from rhs and write the result to lhs.
        template <typename TType>
        void operator()(AppendStreamBuffer& lhs, const TType& rhs) const;
    };

    /************************************************************************/
    /* TYPE ALIASES                                                         */
    /************************************************************************/

    /// \brief Type alias for a raw stream writer.
    using AppendStream = AppendStreamT<RawStreamEncoder>;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // AppendStreamT.

    template <typename TEncoder>
    inline AppendStreamT<TEncoder>::AppendStreamT(AppendStreamBuffer& stream_buffer)
        : stream_buffer_(&stream_buffer)
    {

    }

    template <typename TEncoder>
    template <typename TType>
    inline AppendStreamT<TEncoder>& AppendStreamT<TEncoder>::operator<<(const TType& data)
    {
        TEncoder{}(*stream_buffer_, data);

        return *this;
    }

    // RawStreamEncoder.

    template <typename TType>
    inline void RawStreamEncoder::operator()(AppendStreamBuffer& lhs, const TType& rhs) const
    {
        lhs.Append(MakeConstMemoryRange(rhs));
    }

}