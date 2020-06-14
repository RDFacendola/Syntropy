
/// \file append_stream.h
/// \brief This header is part of the Syntropy serialization module. It contains definition for output streams.
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
    /* OUTPUT STREAM                                                        */
    /************************************************************************/

    /// \brief Interface for a class which binds to an output stream-like object and exposes sequential output functionalities.
    /// \author Raffaele D. Facendola - June 2020
    class AppendStream
    {
    public:

        /// \brief Default virtual destructor.
        virtual ~AppendStream() = default;

        /// \brief Append data to the stream.
        template <typename TData>
        AppendStream& operator<<(const TData& data);

        /// \brief Append data to the stream.
        AppendStream& operator<<(const ConstMemoryRange& data);

        /// \brief Increase the underlying buffer allocation size.
        /// \param size Amount of bytes to reserve.
        /// \remark If the underlying stream doesn't provide this interface this method does nothing.
        virtual void Grow(Bytes size) = 0;

    private:

        /// \brief Append data sequentially to the stream.
        /// \return Returns the range containing unwritten data.
        virtual ConstMemoryRange Append(const ConstMemoryRange& data) = 0;

    };

    /************************************************************************/
    /* APPEND STREAM T <T STREAM>                                           */
    /************************************************************************/

    /// \brief Adapter class which binds to an output stream-like object and exposes sequential output functionalities.
    /// Bound streams must outlive instances of this class.
    /// \author Raffaele D. Facendola - June 2020
    template <typename TStream>
    class AppendStreamT : public AppendStream
    {
    public:

        /// \brief Create a new stream bound to an output stream object.
        AppendStreamT(TStream& stream);

        /// \brief Default copy constructor.
        AppendStreamT(const AppendStreamT& other) = default;

        /// \brief Default move constructor.
        AppendStreamT(AppendStreamT&& other) = default;

        /// \brief Default copy-assignment operator.
        AppendStreamT& operator=(const AppendStreamT& other) = default;

        /// \brief Default move-assignment operator.
        AppendStreamT& operator=(AppendStreamT&& other) = default;

        /// \brief Default destructor.
        virtual ~AppendStreamT() = default;

        virtual void Grow(Bytes size) override;

    private:

        /// \brief Predicate used to detect whether the underlying stream supports the "Grow(Byte)" interface.
        template <typename TStream>
        using IsGrowSupported = decltype(std::declval<TStream>().Grow(Bytes{}));

        virtual ConstMemoryRange Append(const ConstMemoryRange& data) override;

        /// \brief Underlying stream.
        ObserverPtr<TStream> stream_{ nullptr };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create an append stream wrapping a given stream-like object.
    template <typename TStream>
    AppendStreamT<TStream> MakeAppendStream(TStream& stream);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // AppendStream.

    template <typename TData>
    inline AppendStream& AppendStream::operator<<(const TData& data)
    {
        return (*this) << MakeConstMemoryRange(data);
    }

    inline AppendStream& AppendStream::operator<<(const ConstMemoryRange& data)
    {
        Append(data);

        return *this;
    }

    // AppendStreamT<TStream>.

    template <typename TStream>
    inline AppendStreamT<TStream>::AppendStreamT(TStream& stream)
        : stream_(&stream)
    {

    }

    template <typename TStream>
    inline ConstMemoryRange AppendStreamT<TStream>::Append(const ConstMemoryRange& data)
    {
        return stream_->Append(data);
    }

    template <typename TStream>
    inline void AppendStreamT<TStream>::Grow(Bytes capacity)
    {
        if constexpr (IsValidExpressionV<IsGrowSupported, TStream>)
        {
            stream_->Grow(capacity);
        }
    }

    // Non-member functions.

    template <typename TStream>
    inline AppendStreamT<TStream> MakeAppendStream(TStream& stream)
    {
        return AppendStreamT<TStream>(stream);
    }

}

