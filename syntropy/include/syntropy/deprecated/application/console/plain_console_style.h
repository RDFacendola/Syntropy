
/// \file plain_console_style.h
/// \brief This header is part of Syntropy application module. It contains definitions for minimal console styles.
///
/// \author Raffaele D. Facendola - June 2020.

#pragma once

#include "syntropy/core/strings/string.h"

namespace Syntropy
{
    /************************************************************************/
    /* DEFAULT CONSOLE STYLE                                                */
    /************************************************************************/

    /// \brief Minimal console style.
    /// \author Raffaele D. Facendola - June 2020.
    class PlainOfConsoleStyle
    {
    public:

        /// \brief Default constructor.
        PlainOfConsoleStyle() = default;

        /// \brief Default copy constructor.
        PlainOfConsoleStyle(const PlainOfConsoleStyle&) = default;

        /// \brief Default move constructor.
        PlainOfConsoleStyle(PlainOfConsoleStyle&&) = default;

        /// \brief Default destructor.
        ~PlainOfConsoleStyle() = default;

        /// \brief Default copy-assignment operator.
        PlainOfConsoleStyle& operator=(const PlainOfConsoleStyle&) = default;

        /// \brief Default move-assignment operator.
        PlainOfConsoleStyle& operator=(PlainOfConsoleStyle&&) = default;

        /// \brief Push a section.
        String PushSection(const StringView& text) const;

        /// \brief Pop a section.
        String PopSection() const;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // PlainOfConsoleStyle.

    inline String PlainOfConsoleStyle::PushSection(const StringView& text) const
    {
        return String{ text } + "\n\n";
    }

    inline String PlainOfConsoleStyle::PopSection() const
    {
        return "\n\n";
    }


}