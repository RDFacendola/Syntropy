
/// \file output_stream_buffer.h
/// \brief This header is part of the Syntropy serialization module. It contains definition for output stream buffers.
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
    /* OUTPUT STREAM BUFFER                                                 */
    /************************************************************************/

    /// \brief Interface for a class which binds to a stream buffer-like object and exposes sequential output functionalities only.
    /// \author Raffaele D. Facendola - June 2020
    class OutputStreamBuffer
    {
    public:

        /// \brief Default virtual destructor.
        virtual ~OutputStreamBuffer() = default;

        /// \brief Write data sequentially to the stream, causing it to grow.
        /// \return Returns the range containing unwritten data.
        virtual ConstMemoryRange WriteSequential(const ConstMemoryRange& data) = 0;

        /// \brief Increase the underlying buffer allocation size.
        /// This method may cause buffer reallocation.
        /// If the underlying buffer doesn't support this operation, this method behaves as no-op.
        virtual void Reserve(Bytes capacity) = 0;

        /// \brief Shrink the allocation size up to the current buffer size.
        /// This method preserve the buffer content and may reallocate the underlying buffer.
        /// If the underlying buffer doesn't support this operation, this method behaves as no-op.
        virtual void Shrink() = 0;

        /// \brief Discard buffer content.
        /// If the underlying buffer doesn't support this operation, this method behaves as no-op.
        virtual void Discard() = 0;

    };

    /************************************************************************/
    /* OUTPUT STREAM BUFFER T <TSTREAM BUFFER>                              */
    /************************************************************************/

    /// \brief Adapter class which binds to a stream buffer-like object and exposes output functionalities only.
    /// \author Raffaele D. Facendola - June 2020
    template <typename TStreamBuffer>
    class OutputStreamBufferT : public OutputStreamBuffer
    {
    public:

        /// \brief Create a new output stream bound to a stream buffer object.
        /// The provided stream buffer must outlive this object, otherwise the behavior of the class is undefined.
        OutputStreamBufferT(TStreamBuffer& stream_buffer);

        /// \brief Default copy constructor.
        OutputStreamBufferT(const OutputStreamBufferT& other) = default;

        /// \brief Default move constructor.
        OutputStreamBufferT(OutputStreamBufferT&& other) = default;

        /// \brief Default copy-assignment operator.
        OutputStreamBufferT& operator=(const OutputStreamBufferT& other) = default;

        /// \brief Default move-assignment operator.
        OutputStreamBufferT& operator=(OutputStreamBufferT&& other) = default;

        /// \brief Default destructor.
        virtual ~OutputStreamBufferT() = default;

        virtual ConstMemoryRange WriteSequential(const ConstMemoryRange& data) override;

        virtual void Reserve(Bytes capacity) override;

        virtual void Shrink() override;

        virtual void Discard() override;

    private:

        /// \brief Predicate used to detect whether the underlying stream buffer supports the "Reserve(Byte)" method.
        template <typename TStreamBuffer>
        using IsReserveSupported = decltype(std::declval<TStreamBuffer>().Reserve(Bytes{}));

        /// \brief Predicate used to detect whether the underlying stream buffer supports the "Shrink()" method.
        template <typename TStreamBuffer>
        using IsShrinkSupported = decltype(std::declval<TStreamBuffer>().Shrink());

        /// \brief Predicate used to detect whether the underlying stream buffer supports the "Discard()" method.
        template <typename TStreamBuffer>
        using IsDiscardSupported = decltype(std::declval<TStreamBuffer>().Discard());

        /// \brief Underlying stream buffer.
        ObserverPtr<TStreamBuffer> stream_buffer_{ nullptr };

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // OutputStreamBufferT<TStreamBuffer>.

    template <typename TStreamBuffer>
    inline OutputStreamBufferT<TStreamBuffer>::OutputStreamBufferT(TStreamBuffer& stream_buffer)
        : stream_buffer_(&stream_buffer)
    {

    }

    template <typename TStreamBuffer>
    inline ConstMemoryRange OutputStreamBufferT<TStreamBuffer>::WriteSequential(const ConstMemoryRange& data)
    {
        return stream_buffer_->WriteSequential(data);
    }

    template <typename TStreamBuffer>
    inline void OutputStreamBufferT<TStreamBuffer>::Reserve(Bytes capacity)
    {
        if constexpr (IsValidExpressionV<IsReserveSupported, TStreamBuffer>)
        {
            stream_buffer_->Reserve(capacity);
        }
    }

    template <typename TStreamBuffer>
    inline void OutputStreamBufferT<TStreamBuffer>::Shrink()
    {
        if constexpr (IsValidExpressionV<IsShrinkSupported, TStreamBuffer>)
        {
            stream_buffer_->Shrink();
        }
    }

    template <typename TStreamBuffer>
    inline void OutputStreamBufferT<TStreamBuffer>::Discard()
    {
        if constexpr (IsValidExpressionV<IsDiscardSupported, TStreamBuffer>)
        {
            stream_buffer_->Discard();
        }
    }


}

