
/// \file plain_console_style.h
/// \brief This header is part of Syntropy application module. It contains definitions for minimal console styles.
///
/// \author Raffaele D. Facendola - June 2020.

#pragma once

#include "syntropy/core/string.h"

namespace syntropy
{
    /************************************************************************/
    /* DEFAULT CONSOLE STYLE                                                */
    /************************************************************************/

    /// \brief Minimal console style.
    /// \author Raffaele D. Facendola - June 2020.
    class PlainConsoleStyle
    {
    public:

        /// \brief Default constructor.
        PlainConsoleStyle() = default;

        /// \brief Default copy constructor.
        PlainConsoleStyle(const PlainConsoleStyle&) = default;

        /// \brief Default move constructor.
        PlainConsoleStyle(PlainConsoleStyle&&) = default;

        /// \brief Default destructor.
        ~PlainConsoleStyle() = default;

        /// \brief Default copy-assignment operator.
        PlainConsoleStyle& operator=(const PlainConsoleStyle&) = default;

        /// \brief Default move-assignment operator.
        PlainConsoleStyle& operator=(PlainConsoleStyle&&) = default;

        /// \brief Push a section.
        String PushSection(const StringView& text) const;

        /// \brief Pop a section.
        String PopSection() const;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // PlainConsoleStyle.

    inline String PlainConsoleStyle::PushSection(const StringView& text) const
    {
        return String{ text } + "\n\n";
    }

    inline String PlainConsoleStyle::PopSection() const
    {
        return "\n\n";
    }


}