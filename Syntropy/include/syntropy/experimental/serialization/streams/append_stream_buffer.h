
/// \file append_stream_buffer.h
/// \brief This header is part of the Syntropy serialization module. It contains definition for sequential output streams buffers.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/language/type_traits.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/memory/bytes.h"

namespace syntropy
{
    /************************************************************************/
    /* APPEND STREAM BUFFER                                                 */
    /************************************************************************/

    /// \brief Interface for a class which binds to a stream buffer-like object and exposes low-level sequential output functionalities.
    /// \author Raffaele D. Facendola - June 2020
    class AppendStreamBuffer
    {
    public:

        /// \brief Default virtual destructor.
        virtual ~AppendStreamBuffer() = default;

        /// \brief Append data sequentially to the stream buffer.
        /// \return Returns the range containing unwritten data.
        virtual ConstMemoryRange Append(const ConstMemoryRange& data) = 0;

        /// \brief Increase the underlying buffer allocation size.
        /// \param size Amount of bytes to reserve.
        /// \remark If the underlying stream buffer doesn't provide this interface this method does nothing.
        virtual void Grow(Bytes size) = 0;

    };

    /************************************************************************/
    /* APPEND STREAM BUFFER T <T STREAM>                                    */
    /************************************************************************/

    /// \brief Adapter class which binds to a stream buffer-like object and exposes low-level sequential output functionalities.
    /// Bound stream buffer must outlive instances of this class.
    /// \author Raffaele D. Facendola - June 2020
    template <typename TStreamBuffer>
    class AppendStreamBufferT : public AppendStreamBuffer
    {
    public:

        /// \brief Create a new stream buffer bound to a stream buffer object.
        AppendStreamBufferT(TStreamBuffer& stream);

        /// \brief Default copy constructor.
        AppendStreamBufferT(const AppendStreamBufferT& other) = default;

        /// \brief Default move constructor.
        AppendStreamBufferT(AppendStreamBufferT&& other) = default;

        /// \brief Default copy-assignment operator.
        AppendStreamBufferT& operator=(const AppendStreamBufferT& other) = default;

        /// \brief Default move-assignment operator.
        AppendStreamBufferT& operator=(AppendStreamBufferT&& other) = default;

        /// \brief Default destructor.
        virtual ~AppendStreamBufferT() = default;

        virtual ConstMemoryRange Append(const ConstMemoryRange& data) override;

        virtual void Grow(Bytes size) override;

    private:

        /// \brief Predicate used to detect whether the underlying stream buffer supports the "Grow(Byte)" interface.
        template <typename UStreamBuffer>
        using IsGrowSupported = decltype(std::declval<UStreamBuffer>().Grow(Bytes{}));

        /// \brief Underlying stream buffer.
        ObserverPtr<TStreamBuffer> stream_buffer_{ nullptr };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create an append stream buffer wrapping a generic stream buffer-like object whose type is deduced by arguments.
    template <typename TStreamBuffer>
    AppendStreamBufferT<TStreamBuffer> MakeAppendStreamBuffer(TStreamBuffer& stream_buffer);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // AppendStreamBufferT<TStreamBuffer>.

    template <typename TStreamBuffer>
    inline AppendStreamBufferT<TStreamBuffer>::AppendStreamBufferT(TStreamBuffer& stream_buffer)
        : stream_buffer_(&stream_buffer)
    {

    }

    template <typename TStreamBuffer>
    inline ConstMemoryRange AppendStreamBufferT<TStreamBuffer>::Append(const ConstMemoryRange& data)
    {
        return stream_buffer_->Append(data);
    }

    template <typename TStreamBuffer>
    inline void AppendStreamBufferT<TStreamBuffer>::Grow(Bytes capacity)
    {
        if constexpr (IsValidExpressionV<IsGrowSupported, TStreamBuffer>)
        {
            stream_buffer_->Grow(capacity);
        }
    }

    // Non-member functions.

    template <typename TStreamBuffer>
    inline AppendStreamBufferT<TStreamBuffer> MakeAppendStreamBuffer(TStreamBuffer& stream_buffer)
    {
        return AppendStreamBufferT<TStreamBuffer>(stream_buffer);
    }

}

