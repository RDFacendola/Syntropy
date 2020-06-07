
/// \file input_stream_buffer.h
/// \brief This header is part of the Syntropy serialization module. It contains definition for input stream buffers.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/type_traits.h"
#include "syntropy/serialization/memory_stream_buffer.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/memory/bytes.h"

namespace syntropy
{
    /************************************************************************/
    /* INPUT STREAM BUFFER                                                  */
    /************************************************************************/

    /// \brief Interface for a class which binds to a stream buffer-like object and exposes sequential input functionalities only.
    /// \author Raffaele D. Facendola - June 2020
    class InputStreamBuffer
    {
    public:

        /// \brief Default virtual destructor.
        virtual ~InputStreamBuffer() = default;

        /// \brief Read data sequentially from the stream, causing it to shrink.
        /// \return Returns the range containing read data.
        virtual MemoryRange ReadSequential(const MemoryRange& data) = 0;

        /// \brief Check whether the underlying buffer is empty.
        virtual Bool IsEmpty() const = 0;

        /// \brief Get the stream content size, in bytes.
        virtual Bytes GetSize() const = 0;

    };

    /************************************************************************/
    /* INPUT STREAM BUFFER T <TSTREAM BUFFER>                               */
    /************************************************************************/

    /// \brief Adapter class which binds to a stream buffer-like object and exposes input functionalities only.
    /// \author Raffaele D. Facendola - June 2020
    template <typename TStreamBuffer>
    class InputStreamBufferT : public InputStreamBuffer
    {
    public:

        /// \brief Create a new output stream bound to a stream buffer object.
        /// The provided stream buffer must outlive this object, otherwise the behavior of the class is undefined.
        InputStreamBufferT(TStreamBuffer& stream_buffer);

        /// \brief Default copy constructor.
        InputStreamBufferT(const InputStreamBufferT& other) = default;

        /// \brief Default move constructor.
        InputStreamBufferT(InputStreamBufferT&& other) = default;

        /// \brief Default copy-assignment operator.
        InputStreamBufferT& operator=(const InputStreamBufferT& other) = default;

        /// \brief Default move-assignment operator.
        InputStreamBufferT& operator=(InputStreamBufferT&& other) = default;

        /// \brief Default destructor.
        virtual ~InputStreamBufferT() = default;

        virtual MemoryRange ReadSequential(const MemoryRange& data) override;

        virtual Bool IsEmpty() const override;

        virtual Bytes GetSize() const override;

    private:

        /// \brief Underlying stream buffer.
        ObserverPtr<TStreamBuffer> stream_buffer_{ nullptr };

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // InputStreamBufferT<TStreamBuffer>.

    template <typename TStreamBuffer>
    inline InputStreamBufferT<TStreamBuffer>::InputStreamBufferT(TStreamBuffer& stream_buffer)
        : stream_buffer_(&stream_buffer)
    {

    }

    template <typename TStreamBuffer>
    inline MemoryRange InputStreamBufferT<TStreamBuffer>::ReadSequential(const MemoryRange& data)
    {
        return stream_buffer_->ReadSequential(data);
    }

    template <typename TStreamBuffer>
    inline Bool InputStreamBufferT<TStreamBuffer>::IsEmpty() const
    {
        return stream_buffer_->IsEmpty();
    }

    template <typename TStreamBuffer>
    inline Bytes InputStreamBufferT<TStreamBuffer>::GetSize() const
    {
        return stream_buffer_->GetSize();
    }

}

