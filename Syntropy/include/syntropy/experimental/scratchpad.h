
/// \brief Write binary data to the underlying buffer.
void WriteRaw(const ConstMemoryRange& data);


/// \ brief Write a packed byte formed by a format and a payload.
/// Both format and payload are expected to be convertible to Byte. If payload and format overlap the behavior of this method is undefined
/// If payload bits spill into format's the behavior of this method is undefined.
template <typename TFormat, typename TPayload>
void Pack(const TFormat& format, const TPayload& payload);
