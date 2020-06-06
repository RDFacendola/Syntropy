
/// \file stream_writer.h
/// \brief This header is part of the Syntropy serialization module. It contains definitions for classes used to write on streams.
///
/// \author Raffaele D. Facendola - June 2020.

#include "syntropy/core/types.h"

namespace syntropy
{
    /************************************************************************/
    /* STREAM WRITER                                                        */
    /************************************************************************/

    /// \brief Exposes methods to write to a stream.
    /// \author Raffaele D. Facendola - June 2020.
    class StreamWriter
    {
    public:

        /// \brief Type of the underlying output stream.
        using TStream = BasicStringStream<Byte>;

        /// \brief Type of the underlying output string.
        using TString = BasicString<Byte>;

        /// \brief Create a new stream writer bound to a stream.
        StreamWriter(TStream& stream);

        /// \brief Default copy constructor.
        StreamWriter(const StreamWriter&) = default;

        /// \brief Default move constructor.
        StreamWriter(StreamWriter&&) = default;

        /// \brief Default copy-assignment constructor.
        StreamWriter& operator=(const StreamWriter&) = default;

        /// \brief Default move-assignment constructor.
        StreamWriter& operator=(StreamWriter&&) = default;

        /// \brief Default destructor.
        ~StreamWriter() = default;

        /// \ brief Write a packed byte formed by a format and a payload.
        /// Both format and payload are expected to be convertible to Byte. If payload and format overlap the behavior of this method is undefined
        /// If payload bits spill into format's the behavior of this method is undefined.
        template <typename TFormat, typename TPayload>
        void Pack(const TFormat& format, const TPayload& payload);

        /// \brief Write one or more values inside the underlying stream.
        template <typename... TTypes>
        void Write(TTypes&&... values);

        /// \brief Write data to the underlying stream.
        void WriteRaw(const ConstMemoryRange& data);

        /// \brief Clear the underlying stream.
        void Clear();

        /// \brief Get the underlying string.
        TString ToString() const;

        /// \brief Get the total amount of bytes written to the stream.
        Bytes GetWrittenCount() const;

    private:

        /// \brief Underlying stream.
        TStream stream_;

        /// \brief Number of written bytes.
        Bytes written_count_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // StreamWriter.

    template <typename TFormat, typename TPayload>
    inline void StreamWriter::Pack(const TFormat& format, const TPayload& payload)
    {
        static_assert(sizeof(format) == sizeof(Byte), "Format type must be 8-bit wide.");
        static_assert(sizeof(payload) == sizeof(Byte), "Payload type must be 8-bit wide.");

        auto pack = ToByte(format) | ToByte((payload));

        static_assert(sizeof(pack) == sizeof(Byte));

        Write(pack);
    }

    template <typename... TTypes>
    inline void StreamWriter::Write(TTypes&&... values)
    {
        (WriteRaw(MakeConstMemoryRange(values)), ...);
    }

    inline void StreamWriter::WriteRaw(const ConstMemoryRange& data)
    {
        stream_ << data;

        written_count_ += data.GetSize();
    }

    inline void StreamWriter::Clear()
    {
        stream_.str({});
    }

    inline MsgpackWriter::TString StreamWriter::ToString() const
    {
        return stream_.str();
    }

    inline Bytes StreamWriter::GetWrittenCount() const
    {
        return written_count_;
    }

}