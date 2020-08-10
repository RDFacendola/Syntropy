
/// \file console_output_section_scope.h
/// \brief This header is part of Syntropy application module. It contains definitions scope-based console output sections.
///
/// \author Raffaele D. Facendola - June 2020.

#pragma once

#include "syntropy/application/console/console_output.h"

namespace Syntropy
{
    /************************************************************************/
    /* CONSOLE OUTPUT SECTION SCOPE <SECTION>                               */
    /************************************************************************/

    /// \brief Represents a scope-based output section scope.
    /// This RAII guard is used to push sections on console output on construction
    /// and pop it upon destruction.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TSection>
    class ConsoleOutputSectionScope
    {
    public:

        /// \brief Default constructor.
        ConsoleOutputSectionScope() = delete;

        /// \brief No copy constructor.
        ConsoleOutputSectionScope(const ConsoleOutputSectionScope&) = delete;

        /// \brief No copy-assignment operator.
        ConsoleOutputSectionScope& operator=(const ConsoleOutputSectionScope&) = delete;

        /// \brief Push a new scope-based section.
        template <typename... TArguments>
        ConsoleOutputSectionScope(TArguments&&... arguments);

        /// \brief Default move constructor.
        ConsoleOutputSectionScope(ConsoleOutputSectionScope&&) = default;

        /// \brief Default move-assignment operator.
        ConsoleOutputSectionScope& operator=(ConsoleOutputSectionScope&&) = default;

        /// \brief Destroy this instance and pops the current section.
        ~ConsoleOutputSectionScope();

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new scope-based output section scope.
    template <typename TSection, typename... TArguments>
    ConsoleOutputSectionScope<TSection> MakeConsoleOutputSectionScope(TArguments&&... arguments);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ConsoleOutputSectionScope<TSection>.

    template <typename TSection>
    template <typename... TArguments>
    inline ConsoleOutputSectionScope<TSection>::ConsoleOutputSectionScope(TArguments&&... arguments)
    {
        ConsoleOutput::GetSingleton().PushSection<TSection>(std::forward<TArguments>(arguments)...);
    }

    template <typename TSection>
    inline ConsoleOutputSectionScope<TSection>::~ConsoleOutputSectionScope()
    {
        ConsoleOutput::GetSingleton().PopSection();
    }

    // ConsoleOutputSectionScope<TSection>.

    template <typename TSection, typename... TArguments>
    ConsoleOutputSectionScope<TSection> MakeConsoleOutputSectionScope(TArguments&&... arguments)
    {
        return ConsoleOutputSectionScope<TSection>(std::forward<TArguments>(arguments)...);
    }

}