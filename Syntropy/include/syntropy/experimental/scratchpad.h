
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

/************************************************************************/
/* CLI TEXT ALIGNMENT                                                   */
/************************************************************************/

/// \brief Alignment of a CLI text line.
enum class CLITextAlignment : Fix8
{
    /// \brief Left alignment.
    kLeft = 0,

    /// \brief Center alignment.
    kCenter = 1,

    /// \brief Right alignment.
    kRight = 2,
};


inline String DefaultCLIStyle::PrintCenter(const String& text, char fill) const
{
    LineOut([&](auto& line)
    {
        auto text_size = text.size();

        auto left_pad = (line_size_ - text_size) / 2;

        auto right_pad = (line_size_ - text_size - left_pad);

        line << String(left_pad, fill) << text << String(right_pad, fill);
    });
}

inline String DefaultCLIStyle::PrintLeft(const String& text, char fill) const
{
    LineOut([&](auto& line)
    {
        auto text_size = text.size();

        auto right_pad = line_size_ - text_size;

        line << text << String(right_pad, fill);
    });
}

inline String DefaultCLIStyle::PrintRight(const String& text, char fill) const
{
    LineOut([&](auto& line)
    {
        auto text_size = text.size();

        auto left_pad = line_size_ - text_size;

        line << String(left_pad, fill) << text;
    });
}

inline String DefaultCLIStyle::PrintLine(char fill) const
{
    PrintLine(fill, line_size_);
}

inline String DefaultCLIStyle::PrintLine(char fill, Int count) const
{
    LineOut([&](auto& line)
    {
        line << String(count, fill);
    });
}

template <typename TLineFunction>
inline String DefaultCLIStyle::LineOut(TLineFunction line_function) const
{
    auto line = StringStream{};

    line_function(line);

    line << '\n';

    std::cout << line.str();
}


//     struct SpanTestFixture
//     {
// 
//     };
// 
//     auto SpanTestSuite2 = MakeInlineTestSuite<SpanTestFixture>("SpanTestSuite")
//         .After([](auto& ctx)
//         {
//     
//         })
//         .Before([](auto& ctx)
//         {
//         
//         })
//         .TestCase("DefaultSpanIsEmpty", [](auto& ctx)
//         {
//             
//         })
//         .TestCase("Another", [](auto& ctx)
//         {
// 
//         });