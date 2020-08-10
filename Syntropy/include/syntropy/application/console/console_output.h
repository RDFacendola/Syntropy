
/// \file console_output.h
/// \brief This header is part of the Syntropy application module. It contains definitions for command line output streams.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <iostream>

#include "syntropy/core/string.h"
#include "syntropy/core/string_stream.h"
#include "syntropy/core/smart_pointers.h"

#include "syntropy/application/console/default_console_style.h"
#include "syntropy/application/console/console_style.h"

namespace Syntropy
{
    /************************************************************************/
    /* CONSOLE OUTPUT                                                       */
    /************************************************************************/

    /// \brief Singleton representing an output stream for console applications.
    /// \author Raffaele D. Facendola - June 2020.
    class ConsoleOutput
    {
    public:

        /// \brief Get the singleton instance.
        static ConsoleOutput& GetSingleton();

        /// \brief Set the output style.
        template <typename TStyle, typename... TArguments>
        ConsoleOutput& SetStyle(TArguments&&... arguments);

        /// \brief Push a new section with provided arguments.
        template <typename TSection, typename... TArguments>
        ConsoleOutput& PushSection(TArguments&&... arguments);

        /// \brief Pop the current active section.
        ConsoleOutput& PopSection();

        /// \brief Print one or more lines according to the current section and style.
        template <typename... TArguments>
        ConsoleOutput& Print(TArguments&&... arguments);

        /// \brief Insert a new line.
        ConsoleOutput& LineFeed();

    private:

        /// \brief Private constructor to avoid direct instantiation of the class.
        ConsoleOutput() = default;

        /// \brief Style used to format the output strings.
        UniquePtr<ConsoleStyle> style_ = NewConsoleStyle<DefaultConsoleStyle>();

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ConsoleOutput.

    inline ConsoleOutput& ConsoleOutput::GetSingleton()
    {
        static auto singleton = ConsoleOutput{};

        return singleton;
    }

    template <typename TStyle, typename... TArguments>
    inline ConsoleOutput& ConsoleOutput::SetStyle(TArguments&&... arguments)
    {
        style_ = NewConsoleStyle<TStyle>(std::forward<TArguments>(arguments)...);

        return *this;
    }

    template <typename TSection, typename... TArguments>
    inline ConsoleOutput& ConsoleOutput::PushSection(TArguments&&... arguments)
    {
        auto text = Strings::Build(std::forward<TArguments>(arguments)...);

        std::cout << style_->PushSection(typeid(TSection), text);

        return *this;
    }

    inline ConsoleOutput& ConsoleOutput::PopSection()
    {
        std::cout << style_->PopSection();

        return *this;
    }

    template <typename... TArguments>
    inline ConsoleOutput& ConsoleOutput::Print(TArguments&&... arguments)
    {
        auto text = Strings::Build(std::forward<TArguments>(arguments)...);

        std::cout << style_->Print(text);

        return *this;
    }

    inline ConsoleOutput& ConsoleOutput::LineFeed()
    {
        std::cout << style_->LineFeed();

        return *this;
    }

}
