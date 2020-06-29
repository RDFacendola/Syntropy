
/// \file console_output.h
/// \brief This header is part of the Syntropy application module. It contains definitions for command line output streams.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <iostream>

#include "syntropy/core/string.h"
#include "syntropy/core/string_stream.h"
#include "syntropy/core/smart_pointers.h"

#include "syntropy/experimental/application/default_console_style.h"
#include "syntropy/experimental/application/console_style.h"

namespace syntropy
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
        void SetStyle(TArguments&&... arguments);

        /// \brief Push a new section with provided arguments.
        template <typename TSection, typename... TArguments>
        void PushSection(TArguments&&... arguments);

        /// \brief Pop the current active section.
        void PopSection();

        /// \brief Print one or more lines according to the current section and style.
        template <typename... TArguments>
        void Print(TArguments&&... arguments);

        /// \brief Insert a new line.
        void LineFeed();

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
    inline void ConsoleOutput::SetStyle(TArguments&&... arguments)
    {
        style_ = NewConsoleStyle<TStyle>(std::forward<TArguments>(arguments)...);
    }

    template <typename TSection, typename... TArguments>
    inline void ConsoleOutput::PushSection(TArguments&&... arguments)
    {
        auto text = Strings::Build(std::forward<TArguments>(arguments)...);

        std::cout << style_->PushSection(typeid(TSection), text);
    }

    inline void ConsoleOutput::PopSection()
    {
        std::cout << style_->PopSection();
    }

    template <typename... TArguments>
    inline void ConsoleOutput::Print(TArguments&&... arguments)
    {
        auto text = Strings::Build(std::forward<TArguments>(arguments)...);

        std::cout << style_->Print(text);
    }

    inline void ConsoleOutput::LineFeed()
    {
        std::cout << style_->LineFeed();
    }

}
