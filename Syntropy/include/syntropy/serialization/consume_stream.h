
/// \file consume_stream.h
/// \brief This header is part of the Syntropy serialization module. It contains definition for sequential input streams.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/language/type_traits.h"
#include "syntropy/memory/bytes.h"

namespace syntropy
{
    /************************************************************************/
    /* CONSUME STREAM                                                       */
    /************************************************************************/

    /// \brief Interface for a class which binds to a input stream-like object and exposes sequential input functionalities.
    /// \author Raffaele D. Facendola - June 2020
    class ConsumeStream
    {
    public:

        /// \brief Default virtual destructor.
        virtual ~ConsumeStream() = default;

        /// \brief Consume stream data.
        template <typename TData>
        ConsumeStream& operator>>(TData& data);

        /// \brief Consume stream data.
        ConsumeStream& operator>>(const MemoryRange& data);

        /// \brief Check whether the underlying stream is empty.
        virtual Bool IsEmpty() const = 0;

        /// \brief Get the stream content size, in bytes.
        virtual Bytes GetSize() const = 0;

    private:

        /// \brief Consume data sequentially from the stream.
        /// \return Returns the range containing read data.
        virtual MemoryRange Consume(const MemoryRange& data) = 0;

    };

    /************************************************************************/
    /* CONSUME STREAM T <T STREAM>                                          */
    /************************************************************************/

    /// \brief Adapter class which binds to a input stream-like object and exposes sequential input functionalities.
    /// Bound streams must outlive instances of this class.
    /// \author Raffaele D. Facendola - June 2020
    template <typename TStream>
    class ConsumeStreamT : public ConsumeStream
    {
    public:

        /// \brief Create a new stream bound to a input stream object.
        ConsumeStreamT(TStream& stream);

        /// \brief Default copy constructor.
        ConsumeStreamT(const ConsumeStreamT& other) = default;

        /// \brief Default move constructor.
        ConsumeStreamT(ConsumeStreamT&& other) = default;

        /// \brief Default copy-assignment operator.
        ConsumeStreamT& operator=(const ConsumeStreamT& other) = default;

        /// \brief Default move-assignment operator.
        ConsumeStreamT& operator=(ConsumeStreamT&& other) = default;

        /// \brief Default destructor.
        virtual ~ConsumeStreamT() = default;

        virtual Bool IsEmpty() const override;

        virtual Bytes GetSize() const override;

    private:

        virtual MemoryRange Consume(const MemoryRange& data) override;

        /// \brief Underlying stream.
        ObserverPtr<TStream> stream_{ nullptr };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a consume stream deducing templates from arguments.
    template <typename TStream>
    ConsumeStreamT<TStream> MakeConsumeStream(TStream& stream);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ConsumeStream.

    template <typename TData>
    inline ConsumeStream& ConsumeStream::operator>>(TData& data)
    {
        return (*this) >> MakeMemoryRange(data);
    }

    inline ConsumeStream& ConsumeStream::operator>>(const MemoryRange& data)
    {
        Consume(data);

        return *this;
    }

    // ConsumeStreamT<TStream>.

    template <typename TStream>
    inline ConsumeStreamT<TStream>::ConsumeStreamT(TStream& stream)
        : stream_(&stream)
    {

    }

    template <typename TStream>
    inline MemoryRange ConsumeStreamT<TStream>::Consume(const MemoryRange& data)
    {
        return stream_->Consume(data);
    }

    template <typename TStream>
    inline Bool ConsumeStreamT<TStream>::IsEmpty() const
    {
        return stream_->IsEmpty();
    }

    template <typename TStream>
    inline Bytes ConsumeStreamT<TStream>::GetSize() const
    {
        return stream_->GetSize();
    }

    // Non-member functions.

    template <typename TStream>
    inline ConsumeStreamT<TStream> MakeConsumeStream(TStream& stream)
    {
        return ConsumeStreamT<TStream>(stream);
    }

}

