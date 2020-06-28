
/// \file console_output_section.h
/// \brief This header is part of the Syntropy application module. It contains definitions for command line output sections.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <typeinfo>

#include "syntropy/core/types.h"
#include "syntropy/core/string.h"

namespace syntropy
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
        /// Note that this method doesn't support polymorphism.
        template <typename TSection>
        Bool IsA() const;

        /// \brief Called when entering an active section.
        virtual String Push(TStyle& style, const StringView& text) const = 0;


        /// \brief Called when leaving an active section.
        virtual String Pop(TStyle& style) const = 0;

    protected:

        /// \brief Check whether the underlying section type matches the provided type.
        /// Note that this method is not required to support polymorphism.
        virtual Bool IsA(const std::type_info& type) const = 0;

    };

    /************************************************************************/
    /* CONSOLE OUTPUT SECTION <STYLE, SECTION, PUSH OP, POP OP>             */
    /************************************************************************/

    /// \brief Wraps a concrete console output section.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TStyle, typename TSection, typename TPushOp, typename TPopOp>
    class ConsoleOutputSectionT : public ConsoleOutputSection<TStyle>
    {
    public:

        /// \brief Create a concrete console output section implementation
        template <typename UPushOp, typename UPopOp>
        ConsoleOutputSectionT(UPushOp&& push_implementation, UPopOp&& pop_implementation);

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

        virtual String Push(TStyle& style, const StringView& text) const override;

        virtual String Pop(TStyle& style) const override;

    private:

        virtual Bool IsA(const std::type_info& type) const override;

        /// \brief Executed when entering the section.
        TPushOp push_implementation_;

        /// \brief Executed when leaving the section.
        TPopOp pop_implementation_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new console output section by deducing templates from arguments.
    template <typename TStyle, typename TSection, typename TPushOp, typename TPopOp>
    ConsoleOutputSectionT<TStyle, TSection, TPushOp, TPopOp> MakeConsoleOutputSection(TPushOp&& push_implementation, TPopOp&& pop_implementation);

    /************************************************************************/
    /* TITLE SECTION                                                        */
    /************************************************************************/

    /// \brief Represents a title section type tag.
    /// \author Raffaele D. Facendola - June 2020.
    struct ConsoleTitleSection {};

    /************************************************************************/
    /* HEADING 1 SECTION                                                    */
    /************************************************************************/

    /// \brief Represents a heading section.
    /// \author Raffaele D. Facendola - June 2020.
    class ConsoleHeading1Section {};

    /************************************************************************/
    /* HEADING 2 SECTION                                                    */
    /************************************************************************/

    /// \brief Represents a heading section.
    /// \author Raffaele D. Facendola - June 2020.
    class ConsoleHeading2Section {};

    /************************************************************************/
    /* HEADING 3 SECTION                                                    */
    /************************************************************************/

    /// \brief Represents a heading section.
    /// \author Raffaele D. Facendola - June 2020.
    class ConsoleHeading3Section {};

    /************************************************************************/
    /* HEADING 4 SECTION                                                    */
    /************************************************************************/

    /// \brief Represents a heading section.
    /// \author Raffaele D. Facendola - June 2020.
    class ConsoleHeading4Section {};

    /************************************************************************/
    /* LINE SECTION                                                         */
    /************************************************************************/

    /// \brief Represents a line section.
    /// \author Raffaele D. Facendola - June 2020.
    class ConsoleLineSection {};

    /************************************************************************/
    /* LINE FEED SECTION                                                    */
    /************************************************************************/

    /// \brief Represents a line feed section.
    /// \author Raffaele D. Facendola - June 2020.
    class ConsoleLineFeedSection {};

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ConsoleOutputSection<TStyle>.

    template <typename TStyle>
    template <typename TSection>
    inline Bool ConsoleOutputSection<TStyle>::IsA() const
    {
        return IsA(typeid(TSection));
    }

    // ConsoleOutputSectionT<TStyle, TSection, TPushOp, TPopOp>.

    template <typename TStyle, typename TSection, typename TPushOp, typename TPopOp>
    template <typename UPushOp, typename UPopOp>
    inline ConsoleOutputSectionT<TStyle, TSection, TPushOp, TPopOp>::ConsoleOutputSectionT(UPushOp&& push_implementation, UPopOp&& pop_implementation)
        : push_implementation_(std::forward<UPushOp>(push_implementation))
        , pop_implementation_(std::forward<UPopOp>(pop_implementation))
    {

    }

    template <typename TStyle, typename TSection, typename TPushOp, typename TPopOp>
    inline String ConsoleOutputSectionT<TStyle, TSection, TPushOp, TPopOp>::Push(TStyle& style, const StringView& text) const
    {
        return push_implementation_(style, text);
    }

    template <typename TStyle, typename TSection, typename TPushOp, typename TPopOp>
    inline String ConsoleOutputSectionT<TStyle, TSection, TPushOp, TPopOp>::Pop(TStyle& style) const
    {
        return pop_implementation_(style);
    }

    template <typename TStyle, typename TSection, typename TPushOp, typename TPopOp>
    inline Bool ConsoleOutputSectionT<TStyle, TSection, TPushOp, TPopOp>::IsA(const std::type_info& type) const
    {
        return typeid(TSection) == type;
    }

    // Non-member functions.

    template <typename TStyle, typename TSection, typename TPushOp, typename TPopOp>
    inline ConsoleOutputSectionT<TStyle, TSection, TPushOp, TPopOp> MakeConsoleOutputSection(TPushOp&& push_implementation, TPopOp&& pop_implementation)
    {
        return { std::forward<TPushOp>(push_implementation), std::forward<TPopOp>(pop_implementation) };
    }

}