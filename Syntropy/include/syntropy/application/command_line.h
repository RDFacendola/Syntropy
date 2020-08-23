/// \file command_line.h
/// \brief This header is part of the Syntropy application module. It contains classes used to parse and query command line arguments.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <algorithm>

#include "syntropy/core/string.h"
#include "syntropy/core/label.h"
#include "syntropy/language/fundamentals.h"
#include "syntropy/core/vector.h"
#include "syntropy/application/command_line_argument.h"

namespace Syntropy
{
    /************************************************************************/
    /* COMMAND LINE                                                         */
    /************************************************************************/

    /// Class used to parse and query command line arguments.
    /// 
    /// The command line has the form:
    /// -{argument0} [{value0} ... {valueI}] ... -{argumentN} [{value0} ... {valueJ}]
    /// 
    /// \author Raffaele D. Facendola - December 2017
    class CommandLine
    {
    public:

        /// \brief Token character used to identify argument names on the command line.
        static constexpr char kSigil = '-';

        /// \brief Create a new command line.
        /// \param argc Number of arguments in the command line.
        /// \param argv List of arguments in the command line. Each argument is a null-terminated string.
        CommandLine(int argc, char** argv);

        /// \brief Create a new command line.
        /// \param argument_list List of arguments passed to the command line.
        CommandLine(Array<String> command_line);

        /// \brief Get an argument by name.
        /// \param argument_name Name of the argument to get.
        /// \return Returns the requested argument. If no such argument exists returns nullptr.
        const CommandLineArgument* GetArgument(const Label& argument_name) const;

        /// \brief Check whether the command line defines an argument.
        /// \param argument_name Name of the argument to test.
        /// \return Returns true if the requested argument was defined, returns false otherwise.
        Bool HasArgument(const Label& argument_name) const;

        /// \brief Get the arguments in this command line.
        /// \return Returns the arguments in this command line, as key-value pairs.
        const Array<CommandLineArgument>& GetArguments() const;

    private:

        /// \brief Get an argument with the intent of updating it.
        CommandLineArgument* GetArgumentForUpdate(const Label& argument_name);

        /// \brief Emplace a new argument from the provided range of strings.
        /// \return Returns an iterator to the next argument name in the range.
        template <typename TIterator>
        TIterator EmplaceNextArgument(TIterator begin_argument, TIterator end_argument);

        /// \brief Find the next argument in the provided range of strings.
        /// \return Returns an iterator to the first argument found in the provided range.
        template <typename TIterator>
        TIterator FindNextArgument(TIterator begin_argument, TIterator end_argument);

        /// \brief Arguments in the command line.
        Array<CommandLineArgument> arguments_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Stream insertion for command lines.
    std::ostream& operator<<(std::ostream& out, const CommandLine& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // CommandLine.

    inline CommandLine::CommandLine(int argc, char** argv)
        : CommandLine(Array<String>{ argv, argv + argc })
    {

    }

    inline const CommandLineArgument* CommandLine::GetArgument(const Label& argument_name) const
    {
        auto it = std::find_if(std::begin(arguments_), std::end(arguments_), [&argument_name](auto& argument)
        {
            return argument.GetName() == argument_name;
        });

        return it != std::end(arguments_) ? &(*it) : nullptr;
    }

    inline Bool CommandLine::HasArgument(const Label& argument_name) const
    {
        return GetArgument(argument_name) != nullptr;
    }

    inline const Array<CommandLineArgument>& CommandLine::GetArguments() const
    {
        return arguments_;
    }

    inline CommandLineArgument* CommandLine::GetArgumentForUpdate(const Label& argument_name)
    {
        return const_cast<CommandLineArgument*>(std::as_const(*this).GetArgument(argument_name));
    }

    template <typename TIterator>
    inline TIterator CommandLine::FindNextArgument(TIterator begin_argument, TIterator end_argument)
    {
        return std::find_if(begin_argument, end_argument, [](auto& argument)
        {
            return argument.size() > 0 && argument[0] == kSigil;
        });
    }

    inline std::ostream& operator<<(std::ostream& out, const CommandLine& rhs)
    {
        auto current_it = std::begin(rhs.GetArguments());
        auto end_it = std::end(rhs.GetArguments());
        
        if (current_it != end_it)
        {
            out << *current_it;

            for (++current_it; current_it != end_it; ++current_it)
            {
                out << ", " << *current_it;
            }
        }

        return out;
    }

}