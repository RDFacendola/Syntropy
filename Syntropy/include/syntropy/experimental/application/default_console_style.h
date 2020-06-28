
/// \file default_console_style.h
/// \brief This header is part of Syntropy application module. It contains definitions for default console interface style.
///
/// \author Raffaele D. Facendola - June 2020.

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/core/string.h"

#include "syntropy/experimental/application/console_line_builder.h"

namespace syntropy
{
    /************************************************************************/
    /* DEFAULT CONSOLE STYLE                                                */
    /************************************************************************/

    /// \brief Default style for command line applications.
    /// \author Raffaele D. Facendola - June 2020.
    class DefaultConsoleStyle
    {
    public:

        /// \brief Default constructor.
        DefaultConsoleStyle() = default;

        /// \brief Default copy constructor.
        DefaultConsoleStyle(const DefaultConsoleStyle&) = default;

        /// \brief Create a new default CLI style.
        DefaultConsoleStyle(Int line_size);

        /// \brief Default destructor.
        ~DefaultConsoleStyle() = default;

        /// \brief Default assignment operator.
        DefaultConsoleStyle& operator=(const DefaultConsoleStyle&) = default;

        /// \brief Get a title string.
        String Title(const StringView& title) const;

        /// \brief Get a heading string.
        String Heading1(const StringView& heading) const;

        /// \brief Get a heading string.
        String Heading2(const StringView& heading) const;

        /// \brief Get a heading string.
        String Heading3(const StringView& heading) const;

        /// \brief Get a heading string.
        String Heading4(const StringView& heading) const;

        /// \brief Get a normal line.
        String Line(const StringView& message) const;

        /// \brief Get an ending string.
        String End() const;

        /// \brief Get a line break string.
        String Break1() const;

        /// \brief Get a line break string.
        String Break2() const;

        /// \brief Get a line break string.
        String Break3() const;

        /// \brief Get a line break string.
        String Break4() const;

        /// \brief Get a new empty line.
        String LineFeed() const;

    private:

        /// \brief Size of each line, in characters.
        Int line_size_{ 80 };

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // DefaultConsoleStyle.

    inline DefaultConsoleStyle::DefaultConsoleStyle(Int line_size)
        : line_size_(line_size)
    {

    }

    inline String DefaultConsoleStyle::Title(const StringView& text) const
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

    inline String DefaultConsoleStyle::Heading1(const StringView& text) const
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

    inline String DefaultConsoleStyle::Heading2(const StringView& text) const
    {
        return ConsoleLineBuilder{ line_size_ }
            .Center("/\\", "_")
            .Fill("=")
            .Center(text)
            .Fill("=")
            .Blank()
            .Build();
    }

    inline String DefaultConsoleStyle::Heading3(const StringView& text) const
    {
        return ConsoleLineBuilder{ line_size_ }
            .Left(text)
            .LineSize(text.size())
            .Fill("=")
            .Blank()
            .Build();
    }

    inline String DefaultConsoleStyle::Heading4(const StringView& text) const
    {
        return ConsoleLineBuilder{ line_size_ }
            .Left(text)
            .LineSize(text.size())
            .Fill("-")
            .Blank()
            .Build();
    }

    inline String DefaultConsoleStyle::Line(const StringView& message) const
    {
        return ConsoleLineBuilder{ line_size_ }
            .Left(message)
            .Build();
    }

    inline String DefaultConsoleStyle::End() const
    {
        return ConsoleLineBuilder{ line_size_ }
            .Blank()
            .Fill("/", 6)
            .Blank()
            .Build();
    }

    inline String DefaultConsoleStyle::Break1() const
    {
        return ConsoleLineBuilder{ line_size_ }
            .Blank()
            .Fill("/", 4)
            .Blank()
            .Build();
    }

    inline String DefaultConsoleStyle::Break2() const
    {
        return ConsoleLineBuilder{ line_size_ }
            .Blank()
            .Fill("/", 2)
            .Blank()
            .Build();
    }

    inline String DefaultConsoleStyle::Break3() const
    {
        return ConsoleLineBuilder{ line_size_ }
            .Blank()
            .Fill("=")
            .Blank()
            .Build();
    }

    inline String DefaultConsoleStyle::Break4() const
    {
        return ConsoleLineBuilder{ line_size_ }
            .Blank()
            .Fill("-")
            .Blank()
            .Build();
    }

    inline String DefaultConsoleStyle::LineFeed() const
    {
        return ConsoleLineBuilder{ line_size_ }
            .Blank()
            .Build();
    }

}