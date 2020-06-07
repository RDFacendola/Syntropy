
/// \brief Write binary data to the underlying buffer.
void WriteRaw(const ConstMemoryRange& data);


/// \ brief Write a packed byte formed by a format and a payload.
/// Both format and payload are expected to be convertible to Byte. If payload and format overlap the behavior of this method is undefined
/// If payload bits spill into format's the behavior of this method is undefined.
template <typename TFormat, typename TPayload>
void Pack(const TFormat& format, const TPayload& payload);



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
    buffer_->Discard();
}