
/// \file console_output_section.h
/// \brief This header is part of the Syntropy application module. It contains definitions for command line output sections.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <typeinfo>

#include "syntropy/language/traits.h"

#include "syntropy/language/foundation.h"
#include "syntropy/core/strings/string.h"

namespace Syntropy
{
    /************************************************************************/
    /* CONSOLE OUTPUT SECTION <STYLE>                                       */
    /************************************************************************/

    /// \brief Base interface for console output sections.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TStyle>
    class ConsoleOutputSection
    {
    public:

        /// \brief Create a console output section
        ConsoleOutputSection() = default;

        /// \brief Default copy-constructor.
        ConsoleOutputSection(const ConsoleOutputSection&) = default;

        /// \brief Default move-constructor.
        ConsoleOutputSection(ConsoleOutputSection&&) = default;

        /// \brief Default copy-assignment.
        ConsoleOutputSection& operator=(const ConsoleOutputSection&) = default;

        /// \brief Default move-assignment.
        ConsoleOutputSection& operator=(ConsoleOutputSection&&) = default;

        /// \brief Default virtual destructor.
        virtual ~ConsoleOutputSection() = default;

        /// \brief Check whether the underlying section type matches the provided type.
        /// Note that this method is not required to support polymorphism.
        virtual Bool IsA(const std::type_info& section_type) const = 0;

        /// \brief Called when entering the active section.
        /// This call attempts to find the most specific "Push" method in TStyle, falling back to a simple print if no other method was found.
        virtual String Push(TStyle& style, const StringView& text) const = 0;

        /// \brief Called when leaving the active section.
        /// This call attempts to find the most specific "Pop" method in TStyle, falling back to a simple new-line character if no other method was found.
        virtual String Pop(TStyle& style) const = 0;

        /// \brief Print a text.
        /// This call attempts to find the most specific "Print" method in TStyle, falling back to a simple print if no other method was found.
        virtual String Print(TStyle& style, const StringView& text) const = 0;

        /// \brief Insert a new line.
        /// This call attempts to find the most specific "LineFeed" method in TStyle, falling back to a simple new-line character if no other method was found.
        virtual String LineFeed(TStyle& style) const = 0;

    };

    /************************************************************************/
    /* CONSOLE OUTPUT SECTION <STYLE, SECTION>                              */
    /************************************************************************/

    /// \brief Wraps a concrete console output section.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TStyle, typename TSection>
    class ConsoleOutputSectionT : public ConsoleOutputSection<TStyle>
    {
    public:

        /// \brief Create a concrete console output section implementation
        ConsoleOutputSectionT() = default;

        /// \brief Default copy-constructor.
        ConsoleOutputSectionT(const ConsoleOutputSectionT&) = default;

        /// \brief Default move-constructor.
        ConsoleOutputSectionT(ConsoleOutputSectionT&&) = default;

        /// \brief Default copy-assignment.
        ConsoleOutputSectionT& operator=(const ConsoleOutputSectionT&) = default;

        /// \brief Default move-assignment.
        ConsoleOutputSectionT& operator=(ConsoleOutputSectionT&&) = default;

        /// \brief Default virtual destructor.
        virtual ~ConsoleOutputSectionT() = default;

        virtual Bool IsA(const std::type_info& section_type) const override;

        virtual String Push(TStyle& style, const StringView& text) const override;

        virtual String Pop(TStyle& style) const override;

        virtual String Print(TStyle& style, const StringView& text) const override;

        virtual String LineFeed(TStyle& style) const override;

    private:

        /// \brief Check if the style defines a method PushSection(USection{}, StringView{}) (either const or non-const).
        /// This method, if any, has precedence over any other Push alternative.
        template <typename UStyle, typename USection>
        using HasPushSection = decltype(Declval<UStyle>().PushSection(Declval<USection>(), StringView{}));

        /// \brief Check if the style defines a method PushSection(StringView{}) (either const or non-const).
        /// This method, if any, has precedence only if no section-specific Push method is provided by the underlying style.
        template <typename UStyle>
        using HasGenericPushSection = decltype(Declval<UStyle>().PushSection(StringView{}));

        /// \brief Check if the style defines a method PopSection(USection{}) (either const or non-const).
        /// This method, if any, has precedence over any other Pop alternative.
        template <typename UStyle, typename USection>
        using HasPopSection = decltype(Declval<UStyle>().PopSection(Declval<USection>()));

        /// \brief Check if the style defines a method PopSection() (either const or non-const).
        /// This method, if any, has precedence only if no section-specific Pop method is provided by the underlying style.
        template <typename UStyle>
        using HasGenericPopSection = decltype(Declval<UStyle>().PopSection());

        /// \brief Check if the style defines a method Print(USection{}, StringView{}) (either const or non-const).
        template <typename UStyle, typename USection>
        using HasPrint = decltype(Declval<UStyle>().Print(Declval<USection>(), StringView{}));

        /// \brief Check if the style defines a method Print(StringView{}) (either const or non-const).
        template <typename UStyle>
        using HasGenericPrint = decltype(Declval<UStyle>().Print(StringView{}));

        /// \brief Check if the style defines a method LineFeed() (either const or non-const).
        template <typename UStyle, typename USection>
        using HasLineFeed = decltype(Declval<UStyle>().LineFeed(Declval<USection>()));

        /// \brief Check if the style defines a method LineFeed() (either const or non-const).
        template <typename UStyle>
        using HasGenericLineFeed = decltype(Declval<UStyle>().LineFeed());

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new console output section by deducing templates from arguments.
    template <typename TStyle, typename TSection>
    ConsoleOutputSectionT<TStyle, TSection> MakeConsoleOutputSection();

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ConsoleOutputSectionT<TStyle, TSection>.

    template <typename TStyle, typename TSection>
    inline Bool ConsoleOutputSectionT<TStyle, TSection>::IsA(const std::type_info& section_type) const
    {
        return typeid(TSection) == section_type;
    }

    template <typename TStyle, typename TSection>
    inline String ConsoleOutputSectionT<TStyle, TSection>::Push(TStyle& style, const StringView& text) const
    {
        if constexpr (Traits::IsValidExpression<HasPushSection, TStyle, TSection>)
        {
            return style.PushSection(TSection{}, text);
        }
        else if constexpr (Traits::IsValidExpression<HasGenericPushSection, TStyle>)
        {
            return style.PushSection(text);
        }
        else
        {
            return Print(style, text);
        }
    }

    template <typename TStyle, typename TSection>
    inline String ConsoleOutputSectionT<TStyle, TSection>::Pop(TStyle& style) const
    {
        if constexpr (Traits::IsValidExpression<HasPopSection, TStyle, TSection>)
        {
            return style.PopSection(TSection{});
        }
        else if constexpr (Traits::IsValidExpression<HasGenericPopSection, TStyle>)
        {
            return style.PopSection();
        }
        else
        {
            return LineFeed(style);
        }
    }

    template <typename TStyle, typename TSection>
    inline String ConsoleOutputSectionT<TStyle, TSection>::Print(TStyle& style, const StringView& text) const
    {
        if constexpr (Traits::IsValidExpression<HasPrint, TStyle, TSection>)
        {
            return style.Print(TSection{}, text);
        }
        else if constexpr (Traits::IsValidExpression<HasGenericPrint, TStyle>)
        {
            return style.Print(text);
        }
        else
        {
            return String{ text } + "\n";
        }
    }

    template <typename TStyle, typename TSection>
    inline String ConsoleOutputSectionT<TStyle, TSection>::LineFeed(TStyle& style) const
    {
        if constexpr (Traits::IsValidExpression<HasLineFeed, TStyle, TSection>)
        {
            return style.LineFeed(TSection{});
        }
        else if constexpr (Traits::IsValidExpression<HasGenericLineFeed, TStyle>)
        {
            return style.LineFeed();
        }
        else
        {
            return "\n";
        }
    }

    // Non-member functions.

    template <typename TStyle, typename TSection>
    inline ConsoleOutputSectionT<TStyle, TSection> MakeConsoleOutputSection()
    {
        return {};
    }

}