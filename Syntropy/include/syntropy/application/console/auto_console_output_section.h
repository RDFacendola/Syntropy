
/// \file auto_console_output_section.h
/// \brief This header is part of the Syntropy application module. It contains classes used to define self-registering console output sections.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/smart_pointers.h"

#include "syntropy/application/console/console_output_section.h"

namespace syntropy
{
    /************************************************************************/
    /* AUTO CONSOLE OUTPUT SECTION <STYLE>                                  */
    /************************************************************************/

    /// \brief Represents a self-registering console output section for a given console style.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TStyle>
    class AutoConsoleOutputSection
    {
    public:

        /// \brief Find a console output section matching the provided section type.
        /// If more than one section matches the provided type, which returned element among those is unspecified.
        /// If no section matches the provided type, returns a fallback output section.
        /// \see GetFallbackSection().
        static Pointer<const ConsoleOutputSection<TStyle>> FindSection(const std::type_info& section_type);

        /// \brief Get a generic, fallback, output section.
        static Pointer<const ConsoleOutputSection<TStyle>> GetFallbackSection();

        /// \brief Create a new self-registering console output.
        AutoConsoleOutputSection();

        /// \brief No copy-constructor.
        AutoConsoleOutputSection(const AutoConsoleOutputSection&) = delete;

        /// \brief No move-constructor.
        AutoConsoleOutputSection(AutoConsoleOutputSection&&) = delete;

        /// \brief No copy-assignment.
        AutoConsoleOutputSection& operator=(const AutoConsoleOutputSection&) = delete;

        /// \brief No move-assignment.
        AutoConsoleOutputSection& operator=(AutoConsoleOutputSection&&) = delete;

        /// \brief Default virtual destructor.
        virtual ~AutoConsoleOutputSection() = default;

    private:

        /// \brief Access the underlying console output section.
        virtual const ConsoleOutputSection<TStyle>& GetConsoleOutputSection() const = 0;

        /// \brief Get the first element in a linked list to which every other self-registering console output section is linked to.
        static Pointer<const AutoConsoleOutputSection>& GetLinkedList();

        /// \brief Link this console output section to the others and return the next after this one.
        Pointer<const AutoConsoleOutputSection> LinkBefore();

        /// \brief Next auto console output section relative to the style.
        Pointer<const AutoConsoleOutputSection> next_console_output_section_{ nullptr };

    };

    /************************************************************************/
    /* AUTO CONSOLE OUTPUT SECTION <STYLE, SECTION>                         */
    /************************************************************************/

    /// \brief Represents a concrete self-registering console output section for a given console style.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TStyle, typename TSection>
    class AutoConsoleOutputSectionT : public AutoConsoleOutputSection<TStyle>
    {
    public:

        /// \brief Create a new self-registering console output section.
        AutoConsoleOutputSectionT();

        /// \brief No copy-constructor.
        AutoConsoleOutputSectionT(const AutoConsoleOutputSectionT&) = delete;

        /// \brief No move-constructor.
        AutoConsoleOutputSectionT(AutoConsoleOutputSectionT&&) = delete;

        /// \brief No copy-assignment.
        AutoConsoleOutputSectionT& operator=(const AutoConsoleOutputSectionT&) = delete;

        /// \brief No move-assignment.
        AutoConsoleOutputSectionT& operator=(AutoConsoleOutputSectionT&&) = delete;

        /// \brief Default destructor.
        virtual ~AutoConsoleOutputSectionT() = default;

    private:

        virtual const ConsoleOutputSection<TStyle>& GetConsoleOutputSection() const override;

        /// \brief Underlying console output section.
        ConsoleOutputSectionT<TStyle, TSection> console_output_section_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a self-registering console output section.
    template <typename TStyle, typename TSection>
    AutoConsoleOutputSectionT<TStyle, TSection> MakeAutoConsoleOutputSection();

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // AutoConsoleOutputSection<TStyle>.

    template <typename TStyle>
    inline Pointer<const ConsoleOutputSection<TStyle>> AutoConsoleOutputSection<TStyle>::FindSection(const std::type_info& section_type)
    {
        for (auto auto_console_output_section = GetLinkedList(); auto_console_output_section; auto_console_output_section = auto_console_output_section->next_console_output_section_)
        {
            auto& console_output_section = auto_console_output_section->GetConsoleOutputSection();

            if (console_output_section.IsA(section_type))
            {
                return &console_output_section;
            }
        }

        return GetFallbackSection();
    }

    template <typename TStyle>
    inline Pointer<const ConsoleOutputSection<TStyle>> AutoConsoleOutputSection<TStyle>::GetFallbackSection()
    {
        static auto fallback_output_section = ConsoleOutputSectionT<TStyle, void>{};

        return &fallback_output_section;
    }

    template <typename TStyle>
    inline AutoConsoleOutputSection<TStyle>::AutoConsoleOutputSection()
        : next_console_output_section_(LinkBefore())
    {

    }

    template <typename TStyle>
    inline Pointer<const AutoConsoleOutputSection<TStyle>>& AutoConsoleOutputSection<TStyle>::GetLinkedList()
    {
        static auto linked_list = Pointer<const AutoConsoleOutputSection<TStyle>>{ nullptr };

        return linked_list;
    }

    template <typename TStyle>
    inline Pointer<const AutoConsoleOutputSection<TStyle>> AutoConsoleOutputSection<TStyle>::LinkBefore()
    {
        auto& linked_list = GetLinkedList();

        auto next_console_output_section = linked_list;

        linked_list = this;

        return next_console_output_section;
    }

    // AutoConsoleOutputSectionT<TStyle, TSection>

    template <typename TStyle, typename TSection>
    inline AutoConsoleOutputSectionT<TStyle, TSection>::AutoConsoleOutputSectionT()
        : console_output_section_(MakeConsoleOutputSection<TStyle, TSection>())
    {

    }

    template <typename TStyle, typename TSection>
    inline const ConsoleOutputSection<TStyle>& AutoConsoleOutputSectionT<TStyle, TSection>::GetConsoleOutputSection() const
    {
        return console_output_section_;
    }

    // Non-member functions.

    template <typename TStyle, typename TSection>
    inline AutoConsoleOutputSectionT<TStyle, TSection> MakeAutoConsoleOutputSection()
    {
        return {};
    }

}
