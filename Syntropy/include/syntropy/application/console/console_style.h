
/// \file console_style.h
/// \brief This header is part of Syntropy application module. It contains definitions for command line output styles.
///
/// \author Raffaele D. Facendola - June 2020.

#pragma once

#include <typeinfo>

#include "syntropy/language/fundamentals.h"
#include "syntropy/core/string.h"

#include "syntropy/application/console/console_output_section.h"
#include "syntropy/application/console/auto_console_output_section.h"

namespace Syntropy
{
    /************************************************************************/
    /* CONSOLE STYLE                                                        */
    /************************************************************************/

    /// \brief Base interface for stateful styles used to format command line outputs.
    /// \author Raffaele D. Facendola - June 2020.
    class ConsoleStyle
    {
    public:

        /// \brief Default constructor.
        ConsoleStyle() = default;

        /// \brief No copy constructor.
        ConsoleStyle(const ConsoleStyle&) = delete;

        /// \brief No move constructor.
        ConsoleStyle(ConsoleStyle&&) = delete;

        /// \brief Default virtual destructor.
        virtual ~ConsoleStyle() = default;

        /// \brief No copy-assignment operator.
        ConsoleStyle& operator=(const ConsoleStyle&) = delete;

        /// \brief No move-assignment operator.
        ConsoleStyle& operator=(ConsoleStyle&&) = delete;

        /// \brief Push a new section, making it the active one.
        virtual String PushSection(const std::type_info& section_type, const StringView& text) = 0;

        /// \brief Pop the current section, activating the previous one.
        virtual String PopSection() = 0;

        /// \brief Print a text.
        virtual String Print(const StringView& text) = 0;

        /// \brief Insert a new line.
        virtual String LineFeed() = 0;

    };

    /************************************************************************/
    /* CONSOLE STYLE <STYLE>                                                */
    /************************************************************************/

    /// \brief Polymorphic class used to type-erase instances of ConsoleStyle.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TStyle>
    class ConsoleStyleT : public ConsoleStyle
    {
    public:

        /// \brief Create a new console style.
        template <typename... TArguments>
        ConsoleStyleT(TArguments&&... arguments);

        /// \brief No copy constructor.
        ConsoleStyleT(const ConsoleStyleT&) = delete;

        /// \brief No move constructor.
        ConsoleStyleT(ConsoleStyleT&&) = delete;

        /// \brief Default virtual destructor.
        virtual ~ConsoleStyleT() = default;

        /// \brief No copy-assignment operator.
        ConsoleStyleT& operator=(const ConsoleStyleT&) = delete;

        /// \brief No move-assignment operator.
        ConsoleStyleT& operator=(ConsoleStyleT&&) = delete;

        virtual String PushSection(const std::type_info& section_type, const StringView& text) override;

        virtual String PopSection() override;

        virtual String Print(const StringView& text) override;

        virtual String LineFeed() override;

    private:

        struct NestedSection;

        /// \brief Underlying style.
        TStyle style_;

        /// \brief Current active section.
        UniquePtr<NestedSection> active_section_;

    };

    /************************************************************************/
    /* CONSOLE STYLE <STYLE> :: NESTED SECTION                              */
    /************************************************************************/

    /// \brief Represents a console output section nested within another section.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TStyle>
    struct ConsoleStyleT<TStyle>::NestedSection
    {
        /// \brief Outer section, this section is nested into.
        UniquePtr<NestedSection> outer_section_;

        /// \brief Concrete section.
        Pointer<ConsoleOutputSection<TStyle>> section_;
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a ConsoleStyleT<TStyle> unique pointer by deducing template from arguments.
    template <typename TStyle, typename... TArguments>
    UniquePtr<ConsoleStyleT<TStyle>> NewConsoleStyle(TArguments&&... arguments);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ConsoleStyleT<TStyle>.

    template <typename TStyle>
    template <typename... TArguments>
    inline ConsoleStyleT<TStyle>::ConsoleStyleT(TArguments&&... arguments)
        : style_(Forward<TArguments>(arguments)...)
    {

    }

    template <typename TStyle>
    inline String ConsoleStyleT<TStyle>::PushSection(const std::type_info& section_type, const StringView& text)
    {
        auto active_section = MakeUnique<NestedSection>();

        active_section->outer_section_ = Move(active_section_);

        active_section->section_ = AutoConsoleOutputSection<TStyle>::FindSection(section_type);

        active_section_ = Move(active_section);

        return active_section_->section_->Push(style_, text);
    }

    template <typename TStyle>
    inline String ConsoleStyleT<TStyle>::PopSection()
    {
        auto outer_section = Move(active_section_->outer_section_);

        auto result = active_section_->section_->Pop(style_);

        active_section_ = Move(outer_section);

        return result;
    }

    template <typename TStyle>
    inline String ConsoleStyleT<TStyle>::Print(const StringView& text)
    {
        if (active_section_)
        {
            return active_section_->section_->Print(style_, text);
        }
        else
        {
            return AutoConsoleOutputSection<TStyle>::GetFallbackSection()->Print(style_, text);
        }
    }

    template <typename TStyle>
    inline String ConsoleStyleT<TStyle>::LineFeed()
    {
        if (active_section_)
        {
            return active_section_->section_->LineFeed(style_);
        }
        else
        {
            return AutoConsoleOutputSection<TStyle>::GetFallbackSection()->LineFeed(style_);
        }
    }

    // Non-member functions.

    template <typename TStyle, typename... TArguments>
    inline UniquePtr<ConsoleStyleT<TStyle>> NewConsoleStyle(TArguments&&... arguments)
    {
        return MakeUnique<ConsoleStyleT<TStyle>>(Forward<TArguments>(arguments)...);
    }

}