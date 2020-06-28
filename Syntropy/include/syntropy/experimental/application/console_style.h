
/// \file console_style.h
/// \brief This header is part of Syntropy application module. It contains definitions for command line output styles.
///
/// \author Raffaele D. Facendola - June 2020.

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/core/string.h"

#include "syntropy/experimental/application/default_console_style.h"

namespace syntropy
{
    /************************************************************************/
    /* CONSOLE STYLE                                                        */
    /************************************************************************/

    /// \brief Base interface for styles used to format command line outputs.
    /// This interface contains primitives used to print lines.
    /// \author Raffaele D. Facendola - June 2020.
    class ConsoleStyle
    {
    public:

        /// \brief Default constructor.
        ConsoleStyle() = default;

        /// \brief Default virtual destructor.
        virtual ~ConsoleStyle() = default;

        /// \brief Get a title string.
        virtual String Title(const StringView& title) const = 0;

        /// \brief Get a heading string.
        virtual String Heading1(const StringView& heading) const = 0;

        /// \brief Get a heading string.
        virtual String Heading2(const StringView& heading) const = 0;

        /// \brief Get a heading string.
        virtual String Heading3(const StringView& heading) const = 0;

        /// \brief Get a heading string.
        virtual String Heading4(const StringView& heading) const = 0;

        /// \brief Get a normal line.
        virtual String Line(const StringView& message) const = 0;

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

    };

    /************************************************************************/
    /* CONSOLE STYLE <STYLE>                                                */
    /************************************************************************/

    /// \brief Wrapper for styles used to format console outputs.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TStyle>
    class ConsoleStyleT : public ConsoleStyle
    {
    public:

        /// \brief Create a new wrapper.
        ConsoleStyleT(TStyle& cli_style);

        /// \brief Copy constructor.
        ConsoleStyleT(const ConsoleStyleT&) = default;

        /// \brief Default virtual destructor.
        virtual ~ConsoleStyleT() = default;

        /// \brief Default copy-assignment operator.
        ConsoleStyleT& operator=(const ConsoleStyleT&) = default;

        virtual String Title(const StringView& title) const override;

        virtual String Heading1(const StringView& heading) const override;

        virtual String Heading2(const StringView& heading) const override;

        virtual String Heading3(const StringView& heading) const override;

        virtual String Heading4(const StringView& heading) const override;

        virtual String Line(const StringView& message) const override;

        virtual String End() const override;

        virtual String Break1() const override;

        virtual String Break2() const override;

        virtual String Break3() const override;

        virtual String Break4() const override;

        virtual String LineFeed() const override;

    private:

        /// \brief Underlying style.
        ObserverPtr<TStyle> style_{ nullptr };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a ConsoleStyleT by deducing template from arguments.
    template <typename TStyle>
    ConsoleStyleT<TStyle> MakeConsoleStyle(TStyle& style);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ConsoleStyleT<TStyle>.

    template <typename TStyle>
    inline ConsoleStyleT<TStyle>::ConsoleStyleT(TStyle& cli_style)
        : style_(&cli_style)
    {

    }

    template <typename TStyle>
    inline String ConsoleStyleT<TStyle>::Title(const StringView& title) const
    {
        return style_->Title(title);
    }

    template <typename TStyle>
    inline String ConsoleStyleT<TStyle>::Heading1(const StringView& heading) const
    {
        return style_->Heading1(heading);
    }

    template <typename TStyle> 
    inline String ConsoleStyleT<TStyle>::Heading2(const StringView& heading) const
    {
        return style_->Heading2(heading);
    }

    template <typename TStyle>
    inline String ConsoleStyleT<TStyle>::Heading3(const StringView& heading) const
    {
        return style_->Heading3(heading);
    }

    template <typename TStyle> 
    inline String ConsoleStyleT<TStyle>::Heading4(const StringView& heading) const
    {
        return style_->Heading4(heading);
    }

    template <typename TStyle>
    inline String ConsoleStyleT<TStyle>::Line(const StringView& message) const
    {
        return style_->Line(message);
    }

    template <typename TStyle>
    inline String ConsoleStyleT<TStyle>::End() const
    {
        return style_->End();
    }

    template <typename TStyle>
    inline String ConsoleStyleT<TStyle>::Break1() const
    {
        return style_->Break1();
    }

    template <typename TStyle>
    inline String ConsoleStyleT<TStyle>::Break2() const
    {
        return style_->Break2();
    }

    template <typename TStyle>
    inline String ConsoleStyleT<TStyle>::Break3() const
    {
        return style_->Break3();
    }

    template <typename TStyle>
    inline String ConsoleStyleT<TStyle>::Break4() const
    {
        return style_->Break4();
    }

    template <typename TStyle> 
    inline String ConsoleStyleT<TStyle>::LineFeed() const
    {
        return style_->LineFeed();
    }

    // Non-member functions.

    template <typename TStyle>
    inline ConsoleStyleT<TStyle> MakeConsoleStyle(TStyle& style)
    {
        return { style };
    }

}