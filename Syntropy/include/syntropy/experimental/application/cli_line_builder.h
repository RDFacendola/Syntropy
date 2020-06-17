
/// \file cli_line_builder.h
/// \brief This header is part of Syntropy application module. It contains definitions for classes used to construct lines.
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
    /* CLI LINE BUILDER                                                     */
    /************************************************************************/

    /// \brief Builder object used to construct CLI text lines.
    /// \author Raffaele D. Facendola - June 2020.
    class CLILineBuilder
    {
    public:

        /// \brief Set the fixed length of a line.
        CLILineBuilder& LineSize(Int size);

        /// \brief Repeat a filler text.
        CLILineBuilder& Fill(const StringView& fill);

        /// \brief Print zero or more lines filled with a filler text.
        CLILineBuilder& Fill(const StringView& fill, Int lines);

        /// \brief Print a left-aligned text.
        CLILineBuilder& Left(const StringView& text);

        /// \brief Print a right-aligned text.
        CLILineBuilder& Right(const StringView& text);

        /// \brief Print a center-aligned text.
        CLILineBuilder& Center(const StringView& text);

        /// \brief Print a left-aligned text and fill the remaining space with a filler sequence.
        CLILineBuilder& Left(const StringView& text, const StringView& fill);

        /// \brief Print a right-aligned text and fill the remaining space with a filler sequence.
        CLILineBuilder& Right(const StringView& text, const StringView& fill);

        /// \brief Print a center-aligned text and fill the remaining space with a filler sequence.
        CLILineBuilder& Center(const StringView& text, const StringView& fill);

        /// \brief Print am empty line.
        CLILineBuilder& Blank();

        /// \brief Print zero or more empty lines.
        CLILineBuilder& Blank(Int count);

        /// \brief Build the CLI line and clear the builder status.
        String Build();

    private:

        /// \brief Token for the end of a line (carriage return and line feed).
        static constexpr auto kNewLine = "\n";
        
        /// \brief Token for a blank character.
        static constexpr auto kBlank = " ";

        /// \brief Create a fixed-width line with a fill.
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

    // CLILineBuilder.

    inline CLILineBuilder& CLILineBuilder::LineSize(Int size)
    {
        line_size_ = size;

        return *this;
    }

    inline CLILineBuilder& CLILineBuilder::Fill(const StringView& fill)
    {
        auto line = NewLine(fill);

        line_ << line << kNewLine;

        return *this;
    }

    inline CLILineBuilder& CLILineBuilder::Fill(const StringView& fill, Int lines)
    {
        for (; lines > 0; --lines)
        {
            Fill(fill);
        }

        return *this;
    }

    inline CLILineBuilder& CLILineBuilder::Left(const StringView& text)
    {
        return Left(text, kBlank);
    }

    inline CLILineBuilder& CLILineBuilder::Right(const StringView& text)
    {
        return Right(text, kBlank);
    }

    inline CLILineBuilder& CLILineBuilder::Center(const StringView& text)
    {
        return Center(text, kBlank);
    }

    inline CLILineBuilder& CLILineBuilder::Left(const StringView& text, const StringView& fill)
    {
        auto line = NewLine(fill);

        auto padding = 0;

        line_ << Copy(line, text, padding) << kNewLine;

        return *this;
    }

    inline CLILineBuilder& CLILineBuilder::Right(const StringView& text, const StringView& fill)
    {
        auto line = NewLine(fill);

        auto padding = line_size_ - text.size();

        line_ << Copy(line, text, padding) << kNewLine;

        return *this;
    }

    inline CLILineBuilder& CLILineBuilder::Center(const StringView& text, const StringView& fill)
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

    inline CLILineBuilder& CLILineBuilder::Blank()
    {
        line_ << kNewLine;

        return *this;
    }

    inline CLILineBuilder& CLILineBuilder::Blank(Int count)
    {
        for (; count > 0; --count)
        {
            Blank();
        }

        return *this;
    }

    inline String CLILineBuilder::Build()
    {
        auto line = line_.str();

        line_.str("");

        return line;
    }

    inline String CLILineBuilder::NewLine(const StringView& fill) const
    {
        auto line = String(line_size_, ' ');

        //Memory::Repeat(GetRange(line), GetRange(fill));

        return line;
    }

    inline String& CLILineBuilder::Copy(String& destination, const StringView& source, Int padding)
    {
        padding = Math::Clamp(padding, ToInt(0), ToInt(destination.size()));

        //Memory::Copy(GetRange(destination).PopFront(padding), GetRange(source));

        return destination;
    }

}