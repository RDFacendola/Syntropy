
/// \file default_console_style.h
/// \brief This header is part of Syntropy application module. It contains definitions for default console style.
///
/// \author Raffaele D. Facendola - June 2020.

#pragma once

#include "syntropy/language/foundation.h"
#include "syntropy/core/string.h"

#include "syntropy/application/console/auto_console_style.h"
#include "syntropy/application/console/console_output_sections.h"
#include "syntropy/application/console/console_line_builder.h"

namespace Syntropy
{
    /************************************************************************/
    /* DEFAULT CONSOLE STYLE                                                */
    /************************************************************************/

    /// \brief Default console style.
    /// \author Raffaele D. Facendola - June 2020.
    class DefaultConsoleStyle
    {
    public:

        /// \brief Default line style.
        static constexpr Int kDefaultLineSize = 110;

        /// \brief Default constructor.
        DefaultConsoleStyle() = default;

        /// \brief Default copy constructor.
        DefaultConsoleStyle(const DefaultConsoleStyle&) = default;

        /// \brief Default move constructor.
        DefaultConsoleStyle(DefaultConsoleStyle&&) = default;

        /// \brief Create a new default CLI style.
        DefaultConsoleStyle(Int line_size);

        /// \brief Default destructor.
        ~DefaultConsoleStyle() = default;

        /// \brief Default copy-assignment operator.
        DefaultConsoleStyle& operator=(const DefaultConsoleStyle&) = default;

        /// \brief Default move-assignment operator.
        DefaultConsoleStyle& operator=(DefaultConsoleStyle&&) = default;

        /// \brief Push a title section.
        String PushSection(ConsoleTitleSection, const StringView& text) const;

        /// \brief Push a heading section.
        String PushSection(ConsoleHeading1Section, const StringView& text) const;

        /// \brief Push a heading section.
        String PushSection(ConsoleHeading2Section, const StringView& text) const;

        /// \brief Push a heading section.
        String PushSection(ConsoleHeading3Section, const StringView& text) const;

        /// \brief Push a heading section.
        String PushSection(ConsoleHeading4Section, const StringView& text) const;

        /// \brief Pop a title section.
        String PopSection(ConsoleTitleSection) const;

        /// \brief Push a heading section.
        String PopSection(ConsoleHeading1Section) const;

        /// \brief Push a heading section.
        String PopSection(ConsoleHeading2Section) const;

        /// \brief Pop a heading section.
        String PopSection(ConsoleHeading3Section) const;

        /// \brief Push a heading section.
        String PopSection(ConsoleHeading4Section) const;

        /// \brief Print a string in any active section.
        String Print(const StringView& text) const;

        /// \brief Insert a new line in any active section.
        String LineFeed() const;

    private:

        /// \brief Size of each line, in characters.
        Int line_size_{ kDefaultLineSize };

    };

    /************************************************************************/
    /* REGISTRATION                                                         */
    /************************************************************************/

    /// \brief Define sections accepted by DefaultConsoleStyle.
    inline const auto& kAutoDefaultConsoleStyle = MakeAutoConsoleStyle<DefaultConsoleStyle>()
        .Section<ConsoleTitleSection>()
        .Section<ConsoleHeading1Section>()
        .Section<ConsoleHeading2Section>()
        .Section<ConsoleHeading3Section>()
        .Section<ConsoleHeading4Section>();

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // DefaultConsoleStyle.

    inline DefaultConsoleStyle::DefaultConsoleStyle(Int line_size)
        : line_size_(line_size)
    {

    }

    inline String DefaultConsoleStyle::PushSection(ConsoleTitleSection, const StringView& text) const
    {
        return ConsoleLineBuilder{ line_size_ }
            .Center("/\\", "_")
            .Fill("=")
            .Blank(2)
            .Center(text)
            .Blank(2)
            .Fill("=")
            .Blank()
            .Build();
    }

    inline String DefaultConsoleStyle::PushSection(ConsoleHeading1Section, const StringView& text) const
    {
        return ConsoleLineBuilder{ line_size_ }
            .Center("/\\", "_")
            .Fill("=")
            .Blank()
            .Center(text)
            .Blank()
            .Fill("=")
            .Blank()
            .Build();
    }

    inline String DefaultConsoleStyle::PushSection(ConsoleHeading2Section, const StringView& text) const
    {
        return ConsoleLineBuilder{ line_size_ }
            .Center("/\\", "_")
            .Fill("=")
            .Center(text)
            .Fill("=")
            .Blank()
            .Build();
    }

    inline String DefaultConsoleStyle::PushSection(ConsoleHeading3Section, const StringView& text) const
    {
        return ConsoleLineBuilder{ line_size_ }
            .Left(text)
            .LineSize(text.size())
            .Fill("=")
            .Blank()
            .Build();
    }

    inline String DefaultConsoleStyle::PushSection(ConsoleHeading4Section, const StringView& text) const
    {
        return ConsoleLineBuilder{ line_size_ }
            .Left(text)
            .LineSize(text.size())
            .Fill("-")
            .Blank()
            .Build();
    }

    inline String DefaultConsoleStyle::PopSection(ConsoleTitleSection) const
    {
        return ConsoleLineBuilder{ line_size_ }
            .Blank()
            .Fill("/", 6)
            .Blank()
            .Build();
    }

    inline String DefaultConsoleStyle::PopSection(ConsoleHeading1Section) const
    {
        return ConsoleLineBuilder{ line_size_ }
            .Blank()
            .Fill("/", 4)
            .Blank()
            .Build();
    }

    inline String DefaultConsoleStyle::PopSection(ConsoleHeading2Section) const
    {
        return ConsoleLineBuilder{ line_size_ }
            .Blank()
            .Fill("/", 2)
            .Blank()
            .Build();
    }

    inline String DefaultConsoleStyle::PopSection(ConsoleHeading3Section) const
    {
        return ConsoleLineBuilder{ line_size_ }
            .Blank()
            .Fill("=")
            .Blank()
            .Build();
    }

    inline String DefaultConsoleStyle::PopSection(ConsoleHeading4Section) const
    {
        return ConsoleLineBuilder{ line_size_ }
            .Blank()
            .Fill("-")
            .Blank()
            .Build();
    }

    inline String DefaultConsoleStyle::Print(const StringView& text) const
    {
        return ConsoleLineBuilder{ line_size_ }
            .Left(text)
            .Build();
    }

    inline String DefaultConsoleStyle::LineFeed() const
    {
        return ConsoleLineBuilder{ line_size_ }
            .Blank()
            .Build();
    }

}