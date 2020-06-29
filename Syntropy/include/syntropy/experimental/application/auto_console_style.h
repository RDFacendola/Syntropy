
/// \file auto_console_style.h
/// \brief This header is part of Syntropy application module. It contains definitions for self-registering console styles.
///
/// \author Raffaele D. Facendola - June 2020.

#pragma once

#include "syntropy/experimental/application/auto_console_output_section.h"

namespace syntropy
{
    /************************************************************************/
    /* AUTO CONSOLE STYLE                                                   */
    /************************************************************************/

    /// \brief Represents a self-registering console style.
    /// \usage auto& instance = MakeAutoConsoleStyle<MyStyle>().Section<ConsoleTitleSection>().Section<MyCustomSection>()...
    ///
    /// Expects a style to expose methods (either const or non-const) of the form:
    /// 
    /// PushSection(MySectionTag, const StringView& text);
    /// PopSection(MySectionTag);
    /// Print(MySectionTag, const StringView& text);
    /// LineFeed(MySectionTag);
    ///
    /// Any or all methods above can be omitted, the system will automatically to fallback to generic methods:
    ///
    /// PushSection(const StringView& text);
    /// PopSection();
    /// Print(const StringView& text);
    /// LineFeed();
    ///
    /// If the methods above are also omitted, the system will fallback Print(const StringView& text) if exists, or will forward the text as-is.
    ///
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TStyle>
    class AutoConsoleStyle
    {
    public:

        /// \brief Default constructor.
        AutoConsoleStyle() = default;

        /// \brief No copy-constructor.
        AutoConsoleStyle(const AutoConsoleStyle&) = delete;

        /// \brief No move-constructor.
        AutoConsoleStyle(AutoConsoleStyle&&) = delete;

        /// \brief No copy-assignment.
        AutoConsoleStyle& operator=(const AutoConsoleStyle&) = delete;

        /// \brief No move-assignment.
        AutoConsoleStyle& operator=(AutoConsoleStyle&&) = delete;

        /// \brief Default destructor.
        ~AutoConsoleStyle() = default;

        /// \brief Declare a new output section.
        template <typename TSection>
        AutoConsoleStyle& Section();

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new unique self-registering console style.
    template <typename TStyle>
    AutoConsoleStyle<TStyle>& MakeAutoConsoleStyle();

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // AutoConsoleStyle<TStyle>.

    template <typename TStyle>
    template <typename TSection>
    AutoConsoleStyle<TStyle>& AutoConsoleStyle<TStyle>::Section()
    {
        static auto inline_output_section = MakeAutoConsoleOutputSection<TStyle, TSection>();

        return *this;
    }

    // Non-member functions.

    template <typename TStyle>
    inline AutoConsoleStyle<TStyle>& MakeAutoConsoleStyle()
    {
        static auto auto_console_style = AutoConsoleStyle<TStyle>();

        return auto_console_style;
    }

}