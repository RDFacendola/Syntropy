
/// \file consume_stream_buffer.h
/// \brief This header is part of the Syntropy serialization module. It contains definitions for low-level sequential input streams buffers.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/language/type_traits.h"
#include "syntropy/memory/bytes.h"

namespace syntropy
{
    /************************************************************************/
    /* CONSUME STREAM BUFFER                                                */
    /************************************************************************/

    /// \brief Interface for a class which binds to a stream buffer-like object and exposes low-level sequential input functionalities.
    /// \author Raffaele D. Facendola - June 2020
    class ConsumeStreamBuffer
    {
    public:

        /// \brief Default virtual destructor.
        virtual ~ConsumeStreamBuffer() = default;

        /// \brief Consume data sequentially from the stream buffer.
        /// \return Returns the range containing read data.
        virtual MemoryRange Consume(const MemoryRange& data) = 0;

        /// \brief Check whether the underlying stream buffer is empty.
        virtual Bool IsEmpty() const = 0;

        /// \brief Get the stream buffer content size, in bytes.
        virtual Bytes GetSize() const = 0;

    };

    /************************************************************************/
    /* CONSUME STREAM BUFFER T <T STREAM BUFFER>                            */
    /************************************************************************/

    /// \brief Adapter class which binds to a stream buffer-like object and exposes low-level sequential input functionalities.
    /// Bound stream buffer must outlive instances of this class.
    /// \author Raffaele D. Facendola - June 2020
    template <typename TStreamBuffer>
    class ConsumeStreamBufferT : public ConsumeStreamBuffer
    {
    public:

        /// \brief Create a new stream buffer bound to a stream buffer object.
        ConsumeStreamBufferT(TStreamBuffer& stream_buffer);

        /// \brief Default copy constructor.
        ConsumeStreamBufferT(const ConsumeStreamBufferT& other) = default;

        /// \brief Default move constructor.
        ConsumeStreamBufferT(ConsumeStreamBufferT&& other) = default;

        /// \brief Default copy-assignment operator.
        ConsumeStreamBufferT& operator=(const ConsumeStreamBufferT& other) = default;

        /// \brief Default move-assignment operator.
        ConsumeStreamBufferT& operator=(ConsumeStreamBufferT&& other) = default;

        /// \brief Default destructor.
        virtual ~ConsumeStreamBufferT() = default;

        virtual MemoryRange Consume(const MemoryRange& data) override;

        virtual Bool IsEmpty() const override;

        virtual Bytes GetSize() const override;

    private:

        /// \brief Underlying stream buffer.
        Pointer<TStreamBuffer> stream_buffer_{ nullptr };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a consume stream buffer deducing templates from arguments.
    template <typename TStreamBuffer>
    ConsumeStreamBufferT<TStreamBuffer> MakeConsumeStreamBuffer(TStreamBuffer& stream_buffer);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ConsumeStreamT<TStreamBuffer>.

    template <typename TStreamBuffer>
    inline ConsumeStreamBufferT<TStreamBuffer>::ConsumeStreamBufferT(TStreamBuffer& stream_buffer)
        : stream_buffer_(&stream_buffer)
    {

    }

    template <typename TStreamBuffer>
    inline MemoryRange ConsumeStreamBufferT<TStreamBuffer>::Consume(const MemoryRange& data)
    {
        return stream_buffer_->Consume(data);
    }

    template <typename TStreamBuffer>
    inline Bool ConsumeStreamBufferT<TStreamBuffer>::IsEmpty() const
    {
        return stream_buffer_->IsEmpty();
    }

    template <typename TStreamBuffer>
    inline Bytes ConsumeStreamBufferT<TStreamBuffer>::GetSize() const
    {
        return stream_buffer_->GetSize();
    }

    // Non-member functions.

    template <typename TStreamBuffer>
    inline ConsumeStreamBufferT<TStreamBuffer> MakeConsumeStreamBuffer(TStreamBuffer& stream_buffer)
    {
        return ConsumeStreamBufferT<TStreamBuffer>(stream_buffer);
    }

}

