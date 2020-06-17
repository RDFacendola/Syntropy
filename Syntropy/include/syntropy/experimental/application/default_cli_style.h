
/// \file default_cli_style.h
/// \brief This header is part of Syntropy application module. It contains definition for a default CLI output style.
///
/// \author Raffaele D. Facendola - June 2020.

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/core/string.h"
#include "syntropy/core/string_stream.h"
#include "syntropy/core/smart_pointers.h"

#include "syntropy/experimental/application/cli_line_builder.h"

namespace syntropy
{
    /************************************************************************/
    /* DEFAULT CLI STYLE                                                    */
    /************************************************************************/

    /// \brief Default style for CLI applications.
    /// \author Raffaele D. Facendola - June 2020.
    class DefaultCLIStyle
    {
    public:

        /// \brief Default constructor.
        DefaultCLIStyle() = default;

        /// \brief Create a new default CLI style.
        DefaultCLIStyle(Int line_size);

        /// \brief Default destructor.
        ~DefaultCLIStyle() = default;

        /// \brief Get a title string.
        String Title(const String& title) const;

        /// \brief Get a heading string.
        String Heading1(const String& heading) const;

        /// \brief Get a heading string.
        String Heading2(const String& heading) const;

        /// \brief Get a heading string.
        String Heading3(const String& heading) const;

        /// \brief Get a heading string.
        String Heading4(const String& heading) const;

        /// \brief Get a normal line.
        String Line(const String& message) const;

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

        /// \brief Size of each line.
        Int line_size_{ 80 };

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // DefaultCLIStyle.

    inline DefaultCLIStyle::DefaultCLIStyle(Int line_size)
        : line_size_(line_size)
    {

    }

    inline String DefaultCLIStyle::Title(const String& text) const
    {
        return CLILineBuilder{}.LineSize(line_size_)
            .Center("/\\", "_")
            .Fill("=")
            .Blank(2)
            .Center(text)
            .Blank(2)
            .Fill("=")
            .Blank()
            .Build();
    }

    inline String DefaultCLIStyle::Heading1(const String& text) const
    {
        return CLILineBuilder{}.LineSize(line_size_)
            .Center("/\\", "_")
            .Fill("=")
            .Blank()
            .Center(text)
            .Blank()
            .Fill("=")
            .Build();
    }

    inline String DefaultCLIStyle::Heading2(const String& text) const
    {
        return CLILineBuilder{}.LineSize(line_size_)
            .Center("/\\", "_")
            .Fill("=")
            .Center(text)
            .Fill("=")
            .Blank()
            .Build();
    }

    inline String DefaultCLIStyle::Heading3(const String& text) const
    {
        return CLILineBuilder{}.LineSize(line_size_)
            .Left(text)
            .LineSize(text.size())
            .Fill("=")
            .Build();
    }

    inline String DefaultCLIStyle::Heading4(const String& text) const
    {
        return CLILineBuilder{}.LineSize(line_size_)
            .Left(text)
            .LineSize(text.size())
            .Fill("-")
            .Blank()
            .Build();
    }

    inline String DefaultCLIStyle::Line(const String& message) const
    {
        return CLILineBuilder{}.LineSize(line_size_)
            .Left(message)
            .Build();
    }

    inline String DefaultCLIStyle::End() const
    {
        return CLILineBuilder{}.LineSize(line_size_)
            .Blank()
            .Fill("/", 6)
            .Blank()
            .Build();
    }

    inline String DefaultCLIStyle::Break1() const
    {
        return CLILineBuilder{}.LineSize(line_size_)
            .Blank()
            .Fill("/", 4)
            .Blank()
            .Build();
    }

    inline String DefaultCLIStyle::Break2() const
    {
        return CLILineBuilder{}.LineSize(line_size_)
            .Blank()
            .Fill("/", 2)
            .Blank()
            .Build();
    }

    inline String DefaultCLIStyle::Break3() const
    {
        return CLILineBuilder{}.LineSize(line_size_)
            .Blank()
            .Blank()
            .Fill("=")
            .Blank()
            .Build();
    }

    inline String DefaultCLIStyle::Break4() const
    {
        return CLILineBuilder{}.LineSize(line_size_)
            .Fill("-")
            .Build();
    }

    inline String DefaultCLIStyle::LineFeed() const
    {
        return CLILineBuilder{}.LineSize(line_size_)
            .Blank()
            .Build();
    }

}