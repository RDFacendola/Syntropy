
/// \file byte_writer.h
/// \brief This header is part of the Syntropy core module. It contains definition for byte-string writers.
///
/// \author Raffaele D. Facendola - 2020

#include "syntropy/core/types.h"
#include "syntropy/core/byte_string.h"

namespace syntropy
{
    /************************************************************************/
    /* BYTE WRITER                                                          */
    /************************************************************************/

    /// \brief Exposes methods to write to a stream.
    /// \author Raffaele D. Facendola - June 2020.
    class ByteWriter
    {
    public:

        /// \brief Create a new writer bound to a buffer.
        ByteWriter(ByteString& buffer);

        /// \brief Default copy constructor.
        ByteWriter(const ByteWriter&) = default;

        /// \brief Default move constructor.
        ByteWriter(ByteWriter&&) = default;

        /// \brief Default copy-assignment constructor.
        ByteWriter& operator=(const ByteWriter&) = default;

        /// \brief Default move-assignment constructor.
        ByteWriter& operator=(ByteWriter&&) = default;

        /// \brief Default destructor.
        ~ByteWriter() = default;

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
        ByteString* buffer_;

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