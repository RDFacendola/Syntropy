
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

namespace syntropy
{
    /************************************************************************/
    /* CONSOLE LINE BUILDER                                                 */
    /************************************************************************/

    /// \brief Builder object used to construct command line interface text lines.
    /// \author Raffaele D. Facendola - June 2020.
    class ConsoleLineBuilder
    {
    public:

        /// \brief Set the fixed length of a line.
        ConsoleLineBuilder& LineSize(Int size);

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

        /// \brief Create a fixed-width line filled with a repeating text.
        String NewLine(const StringView& fill) const;

        String& Copy(String& destination, const StringView& source, Int padding);

        /// \brief Line stream.
        StringStream line_;

        /// \brief Line size.
        Int line_size_{ 80 };

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ConsoleLineBuilder.

    inline ConsoleLineBuilder& ConsoleLineBuilder::LineSize(Int size)
    {
        line_size_ = size;

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
        auto line = NewLine(fill);

        auto padding = 0;

        line_ << Copy(line, text, padding) << kNewLine;

        return *this;
    }

    inline ConsoleLineBuilder& ConsoleLineBuilder::Right(const StringView& text, const StringView& fill)
    {
        auto line = NewLine(fill);

        auto padding = line_size_ - text.size();

        line_ << Copy(line, text, padding) << kNewLine;

        return *this;
    }

    inline ConsoleLineBuilder& ConsoleLineBuilder::Center(const StringView& text, const StringView& fill)
    {
        //         for (auto text_range = GetConstRange(text); text_range;)
        //         {
        //             auto next = text_range = Find(text_range, kNewLine);
        // 
        //             auto line_range = MakeConstRange(text_range.Begin(), next.End());
        //         }

        // #TODO StringView instead of String.

        // #TODO Chop the text for each '\n' found.
        //  Chop each line found this way every "line_size_" positions (actually find a blank space before line size to avoid chopping words). When a single world is more than line_size, kill the last one and chop.
        //  Handle each line independently.

        auto line = NewLine(fill);

        auto padding = (line_size_ - text.size()) / 2;

        line_ << Copy(line, text, padding) << kNewLine;

        return *this;
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

    inline String ConsoleLineBuilder::NewLine(const StringView& fill) const
    {
        auto line = String(line_size_, ' ');

        //Memory::Repeat(GetRange(line), GetRange(fill));

        return line;
    }

    inline String& ConsoleLineBuilder::Copy(String& destination, const StringView& source, Int padding)
    {
        padding = Math::Clamp(padding, ToInt(0), ToInt(destination.size()));

        //Memory::Copy(GetRange(destination).PopFront(padding), GetRange(source));

        return destination;
    }

}