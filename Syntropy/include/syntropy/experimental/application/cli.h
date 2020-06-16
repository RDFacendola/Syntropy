
/// \file cli_style.h
/// \brief This header is part of Syntropy application module. This file contains definition for CLI I\O visual style.
///
/// \author Raffaele D. Facendola - June 2020.

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/core/string.h"
#include "syntropy/core/string_stream.h"
#include "syntropy/core/smart_pointers.h"

#include "syntropy/experimental/application/default_cli_style.h"

namespace syntropy
{
    /************************************************************************/
    /* CLI                                                                  */
    /************************************************************************/

    class CLIStyle;

    /// \brief Exposes formatting methods to write text on a command-line-interface.
    namespace CLI
    {
        /// \brief Get the thread-local default command line interface style.
        CLIStyle& GetDefaultStyle() noexcept;

        /// \brief Get the thread-local command line interface style.
        CLIStyle& GetStyle() noexcept;

        /// \brief Set the thread-local command line interface style.
        /// \return Returns the previous value of the thread-local command line interface style.
        CLIStyle& SetStyle(CLIStyle& cli_style) noexcept;
    };

    /************************************************************************/
    /* CLI STYLE                                                            */
    /************************************************************************/

    /// \brief Base interface for objects used to format CLI text lines.
    /// \author Raffaele D. Facendola - June 2020.
    class CLIStyle
    {
        friend CLIStyle& CLI::GetDefaultStyle() noexcept;
        friend CLIStyle& CLI::GetStyle() noexcept;
        friend CLIStyle& CLI::SetStyle(CLIStyle& cli_style) noexcept;

    public:

        /// \brief Default virtual destructor.
        virtual ~CLIStyle() = default;

        /// \brief Get a title string.
        virtual String Title(const String& title) const = 0;

        /// \brief Get a heading string.
        virtual String Heading1(const String& heading) const = 0;

        /// \brief Get a heading string.
        virtual String Heading2(const String& heading) const = 0;

        /// \brief Get a heading string.
        virtual String Heading3(const String& heading) const = 0;

        /// \brief Get a heading string.
        virtual String Heading4(const String& heading) const = 0;

        /// \brief Get a normal line.
        virtual String Line(const String& message) const = 0;

        /// \brief Get an ending string.
        virtual String End() const = 0;

        /// \brief Get a line break string.
        virtual String Break1() const = 0;

        /// \brief Get a line break string.
        virtual String Break2() const = 0;

        /// \brief Get a line break string.
        virtual String Break3() const = 0;

        /// \brief Get a line break string.
        virtual String Break4() const = 0;

        /// \brief Get a new empty line.
        virtual String LineFeed() const = 0;

    private:

        /// \brief Thread-local current command line interface style.
        static inline thread_local ObserverPtr<CLIStyle> cli_style_ = &CLI::GetDefaultStyle();

    };

    /************************************************************************/
    /* CLI STYLE T < T CLI STYLE >                                          */
    /************************************************************************/

    /// \brief Wrapper for an object used to format CLI text lines.
        /// \author Raffaele D. Facendola - June 2020.
    template <typename TCLIStyle>
    class CLIStyleT : public CLIStyle
    {
    public:

        /// \brief Wraps a CLI style.
        CLIStyleT(TCLIStyle& cli_style);

        /// \brief Default virtual destructor.
        virtual ~CLIStyleT() = default;

        virtual String Title(const String& title) const override;

        virtual String Heading1(const String& heading) const override;

        virtual String Heading2(const String& heading) const override;

        virtual String Heading3(const String& heading) const override;

        virtual String Heading4(const String& heading) const override;

        virtual String Line(const String& message) const override;

        virtual String End() const override;

        virtual String Break1() const override;

        virtual String Break2() const override;

        virtual String Break3() const override;

        virtual String Break4() const override;

        virtual String LineFeed() const override;

    private:

        /// \brief Underlying CLI style.
        ObserverPtr<TCLIStyle> cli_style_{ nullptr };

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // CLI.

    inline CLIStyle& CLI::GetDefaultStyle() noexcept
    {
        static auto default_cli_style = DefaultCLIStyle{};

        static auto cli_style = CLIStyleT<DefaultCLIStyle>{ default_cli_style };

        return cli_style;
    }

    inline CLIStyle& CLI::GetStyle() noexcept
    {
        return *CLIStyle::cli_style_;
    }

    inline CLIStyle& CLI::SetStyle(CLIStyle& cli_style) noexcept
    {
        auto& previous_cli_style = GetStyle();

        CLIStyle::cli_style_ = &cli_style;

        return previous_cli_style;
    }

    // CLIStyle.

    template <typename TCLIStyle>
    inline CLIStyleT<TCLIStyle>::CLIStyleT(TCLIStyle& cli_style)
        : cli_style_(&cli_style)
    {

    }

    template <typename TCLIStyle>
    inline String CLIStyleT<TCLIStyle>::Title(const String& title) const
    {
        return cli_style_->Title(title);
    }

    template <typename TCLIStyle>
    inline String CLIStyleT<TCLIStyle>::Heading1(const String& heading) const
    {
        return cli_style_->Heading1(heading);
    }

    template <typename TCLIStyle> 
    inline String CLIStyleT<TCLIStyle>::Heading2(const String& heading) const
    {
        return cli_style_->Heading2(heading);
    }

    template <typename TCLIStyle>
    inline String CLIStyleT<TCLIStyle>::Heading3(const String& heading) const
    {
        return cli_style_->Heading3(heading);
    }

    template <typename TCLIStyle> 
    inline String CLIStyleT<TCLIStyle>::Heading4(const String& heading) const
    {
        return cli_style_->Heading4(heading);
    }

    template <typename TCLIStyle>
    inline String CLIStyleT<TCLIStyle>::Line(const String& message) const
    {
        return cli_style_->Line(message);
    }

    template <typename TCLIStyle>
    inline String CLIStyleT<TCLIStyle>::End() const
    {
        return cli_style_->End();
    }

    template <typename TCLIStyle>
    inline String CLIStyleT<TCLIStyle>::Break1() const
    {
        return cli_style_->Break1();
    }

    template <typename TCLIStyle>
    inline String CLIStyleT<TCLIStyle>::Break2() const
    {
        return cli_style_->Break2();
    }

    template <typename TCLIStyle>
    inline String CLIStyleT<TCLIStyle>::Break3() const
    {
        return cli_style_->Break3();
    }

    template <typename TCLIStyle>
    inline String CLIStyleT<TCLIStyle>::Break4() const
    {
        return cli_style_->Break4();
    }

    template <typename TCLIStyle> 
    inline String CLIStyleT<TCLIStyle>::LineFeed() const
    {
        return cli_style_->LineFeed();
    }

}