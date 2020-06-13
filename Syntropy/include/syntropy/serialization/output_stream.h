
/// \file output_stream.h
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
    class OutputStream
    {
    public:

        /// \brief Default virtual destructor.
        virtual ~OutputStream() = default;

        /// \brief Append data to the output stream.
        template <typename TData>
        OutputStream& operator<<(const TData& data);

        /// \brief Append data to the output stream.
        OutputStream& operator<<(const ConstMemoryRange& data);

        /// \brief Increase the underlying buffer allocation size.
        /// \param size Amount of bytes to reserve.
        /// \return Returns true if the reservation request was successful, returns false otherwise.
        /// \remark Streams are not required to provide this interface, if that's the case this method is no-op.
        virtual bool Reserve(Bytes size) = 0;

    private:

        /// \brief Write data sequentially to the stream.
        /// \return Returns the range containing unwritten data.
        virtual ConstMemoryRange Append(const ConstMemoryRange& data) = 0;

    };

    /************************************************************************/
    /* OUTPUT STREAM T <T STREAM>                                           */
    /************************************************************************/

    /// \brief Adapter class which binds to an output stream-like object and exposes output functionalities only.
    /// \author Raffaele D. Facendola - June 2020
    template <typename TStream>
    class OutputStreamT : public OutputStream
    {
    public:

        /// \brief Create a new output stream bound to a stream object.
        /// The provided stream must outlive this object, otherwise the behavior of the class is undefined.
        OutputStreamT(TStream& stream);

        /// \brief Default copy constructor.
        OutputStreamT(const OutputStreamT& other) = default;

        /// \brief Default move constructor.
        OutputStreamT(OutputStreamT&& other) = default;

        /// \brief Default copy-assignment operator.
        OutputStreamT& operator=(const OutputStreamT& other) = default;

        /// \brief Default move-assignment operator.
        OutputStreamT& operator=(OutputStreamT&& other) = default;

        /// \brief Default destructor.
        virtual ~OutputStreamT() = default;

    private:

        /// \brief Predicate used to detect whether the underlying stream supports the "Reserve(Byte)" and "GetSize()" methods.
        template <typename TStream>
        using IsReserveSupported = decltype(std::declval<TStream>().Reserve(std::declval<TStream>().GetSize() + Bytes{}));

        virtual ConstMemoryRange Append(const ConstMemoryRange& data) override;

        virtual bool Reserve(Bytes size) override;

        /// \brief Underlying stream.
        ObserverPtr<TStream> stream_{ nullptr };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create an output stream wrapping a given stream-like object.
    template <typename TStream>
    OutputStreamT<TStream> MakeOutputStream(TStream& stream);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // OutputStream.

            /// \brief Append data to the output stream.
    template <typename TData>
    inline OutputStream& OutputStream::operator<<(const TData& data)
    {
        return (*this) << MakeConstMemoryRange(data);
    }

    inline OutputStream& OutputStream::operator<<(const ConstMemoryRange& data)
    {
        Append(data);

        return *this;
    }

    // OutputStreamT<TStream>.

    template <typename TStream>
    inline OutputStreamT<TStream>::OutputStreamT(TStream& stream)
        : stream_(&stream)
    {

    }

    template <typename TStream>
    inline ConstMemoryRange OutputStreamT<TStream>::Append(const ConstMemoryRange& data)
    {
        return stream_->Append(data);
    }

    template <typename TStream>
    inline bool OutputStreamT<TStream>::Reserve(Bytes capacity)
    {
        if constexpr (IsValidExpressionV<IsReserveSupported, TStream>)
        {
            stream_->Reserve(stream_->GetSize() + capacity);
            return true;
        }

        return false;
    }

    // Non-member functions.

    template <typename TStream>
    inline OutputStreamT<TStream> MakeOutputStream(TStream& stream)
    {
        return OutputStreamT<TStream>(stream);
    }

}

