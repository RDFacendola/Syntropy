
/// \file console_line_builder.h
/// \brief This header is part of Syntropy application module. It contains definitions for classes used to construct text lines.
///
/// \author Raffaele D. Facendola - June 2020.

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/core/string.h"
#include "syntropy/core/string_stream.h"
#include "syntropy/memory/memory.h"
#include "syntropy/language/algorithm.h"

#include "syntropy/math/math.h"

namespace Syntropy
{
    /************************************************************************/
    /* CONSOLE LINE BUILDER                                                 */
    /************************************************************************/

    /// \brief Builder object used to build fixed-length console lines.
    /// \author Raffaele D. Facendola - June 2020.
    class ConsoleLineBuilder
    {
    public:

        /// \brief Create a new fixed-length console line builder.
        ConsoleLineBuilder(Int max_line_size);

        /// \brief Set a new line size.
        ConsoleLineBuilder& LineSize(Int line_size);

        /// \brief Fill a line with a repeating text.
        ConsoleLineBuilder& Fill(const StringView& fill);

        /// \brief Fill zero or more lines with a repeating text.
        ConsoleLineBuilder& Fill(const StringView& fill, Int lines);

        /// \brief Print a left-aligned text, wrapping exceeding text to new lines.
        ConsoleLineBuilder& Left(const StringView& text);

        /// \brief Print a right-aligned text, wrapping exceeding text to new lines.
        ConsoleLineBuilder& Right(const StringView& text);

        /// \brief Print a center-aligned text, wrapping exceeding text to new lines.
        ConsoleLineBuilder& Center(const StringView& text);

        /// \brief Print a left-aligned text and fill the remaining space with a filler sequence.
        /// If the provided text exceeds the length of a line, it gets truncated.
        ConsoleLineBuilder& Left(const StringView& text, const StringView& fill);

        /// \brief Print a right-aligned text and fill the remaining space with a filler sequence.
        /// If the provided text exceeds the length of a line, it gets truncated.
        ConsoleLineBuilder& Right(const StringView& text, const StringView& fill);

        /// \brief Print a center-aligned text and fill the remaining space with a filler sequence.
        /// If the provided text exceeds the length of a line, it gets truncated.
        ConsoleLineBuilder& Center(const StringView& text, const StringView& fill);

        /// \brief Print am empty line.
        ConsoleLineBuilder& Blank();

        /// \brief Print zero or more empty lines.
        ConsoleLineBuilder& Blank(Int count);

        /// \brief Build the text line and clear the builder status.
        String Build();

    private:

        /// \brief Token for the end of a line (carriage return and line feed).
        static constexpr auto kNewLine = "\n";
        
        /// \brief Token for a blank character.
        static constexpr auto kBlank = " ";

        /// \brief Execute an operation on each sentence in the provided text.
        template <typename TOperation>
        ConsoleLineBuilder& ForEachLine(const StringView& text, const TOperation& operation);

        /// \brief Create a fixed-width line filled with a repeating text.
        String NewLine(const StringView& fill) const;

        /// \brief Copy a source string onto a destination after being padded left to right.
        String& Copy(String& destination, const StringView& source, Int padding) const;

        /// \brief Line stream.
        StringStream line_;

        /// \brief Current line size.
        Int line_size_{ 80 };

        /// \brief Maximum line size.
        Int max_line_size_{ line_size_ };

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ConsoleLineBuilder.

    inline ConsoleLineBuilder::ConsoleLineBuilder(Int max_line_size)
        : line_size_(max_line_size)
        , max_line_size_(max_line_size)
    {

    }

    inline ConsoleLineBuilder& ConsoleLineBuilder::LineSize(Int line_size)
    {
        line_size_ = Math::Min(line_size, max_line_size_);

        return *this;
    }

    inline ConsoleLineBuilder& ConsoleLineBuilder::Fill(const StringView& fill)
    {
        auto line = NewLine(fill);

        line_ << line << kNewLine;

        return *this;
    }

    inline ConsoleLineBuilder& ConsoleLineBuilder::Fill(const StringView& fill, Int lines)
    {
        for (; lines > 0; --lines)
        {
            Fill(fill);
        }

        return *this;
    }

    inline ConsoleLineBuilder& ConsoleLineBuilder::Left(const StringView& text)
    {
        return Left(text, kBlank);
    }

    inline ConsoleLineBuilder& ConsoleLineBuilder::Right(const StringView& text)
    {
        return Right(text, kBlank);
    }

    inline ConsoleLineBuilder& ConsoleLineBuilder::Center(const StringView& text)
    {
        return Center(text, kBlank);
    }

    inline ConsoleLineBuilder& ConsoleLineBuilder::Left(const StringView& text, const StringView& fill)
    {
        return ForEachLine(text, [&fill, this](const StringView& text_line)
        {
            auto out_line = NewLine(fill);

            constexpr auto padding = ToInt(0);

            line_ << Copy(out_line, text_line, padding) << kNewLine;
        });
    }

    inline ConsoleLineBuilder& ConsoleLineBuilder::Right(const StringView& text, const StringView& fill)
    {
        return ForEachLine(text, [&fill, this](const StringView& text_line)
        {
            auto out_line = NewLine(fill);

            auto padding = line_size_ - text_line.size();

            line_ << Copy(out_line, text_line, padding) << kNewLine;
        });
    }

    inline ConsoleLineBuilder& ConsoleLineBuilder::Center(const StringView& text, const StringView& fill)
    {
        return ForEachLine(text, [&fill, this](const StringView& text_line)
        {
            auto out_line = NewLine(fill);

            auto padding = (line_size_ - text_line.size()) / 2;

            line_ << Copy(out_line, text_line, padding) << kNewLine;
        });
    }

    inline ConsoleLineBuilder& ConsoleLineBuilder::Blank()
    {
        line_ << kNewLine;

        return *this;
    }

    inline ConsoleLineBuilder& ConsoleLineBuilder::Blank(Int count)
    {
        for (; count > 0; --count)
        {
            Blank();
        }

        return *this;
    }

    inline String ConsoleLineBuilder::Build()
    {
        auto line = line_.str();

        line_.str("");

        return line;
    }

    template <typename TOperation>
    ConsoleLineBuilder& ConsoleLineBuilder::ForEachLine(const StringView& text, const TOperation& operation)
    {
        for (auto source_text = text; source_text.size() > 0;)
        {
            auto line_size = source_text.find_first_of(kNewLine);                       // Terminate the sentence on the first new-line character found, up to maximum line size.

            line_size = Math::Min(line_size, source_text.size());                       // Behaves as if the new-line was found past the end of the source text.

            auto consume_size = line_size + 1;                                          // Consume the new-line character.

            if (ToInt(line_size) > line_size_)
            {
                line_size = source_text.find_last_of(kBlank, line_size_ + 1);           // Terminate the sentence on the first blank-character found, up to maximum line size.

                line_size = Math::Min(line_size, source_text.size());                   // Behaves as if the first blank-character was found past the end of the source text.

                consume_size = line_size + 1;                                           // Consume the blank character.
            }

            if ((ToInt(line_size) > line_size_) || (line_size == String::npos))
            {
                line_size = line_size_;                                                 // Terminate the sentence to maximum line size.

                consume_size = line_size_;                                              // The sentence was forcibly terminated at maximum line size.
            }

            operation(source_text.substr(0, line_size));                                // Process line.

            consume_size = Math::Min(consume_size, source_text.size());

            source_text = source_text.substr(consume_size);                             // Consume.
        }

        return *this;
    }

    inline String ConsoleLineBuilder::NewLine(const StringView& fill) const
    {
        auto line = String(line_size_, ' ');

        auto destination_span = RWByteSpan(reinterpret_cast<RWBytePtr>(line.data()), ToInt(line.size()));
        auto source_span = ByteSpan(reinterpret_cast<BytePtr>(fill.data()), ToInt(fill.size()));

        Memory::Repeat(destination_span, source_span);

        return line;
    }

    inline String& ConsoleLineBuilder::Copy(String& destination, const StringView& source, Int padding) const
    {
        auto destination_span = RWByteSpan(reinterpret_cast<RWBytePtr>(destination.data()), ToInt(destination.size()));
        auto source_span = ByteSpan(reinterpret_cast<BytePtr>(source.data()), ToInt(source.size()));

        destination_span = PopFront(destination_span, padding);

        Memory::Copy(destination_span, source_span);

        return destination;
    }

}
