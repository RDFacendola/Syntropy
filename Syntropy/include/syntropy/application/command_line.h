/// \file command_line.h
/// \brief This header is part of the syntropy application system. It contains classes used to parse and query command line arguments.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <string>
#include <vector>

#include "syntropy/types/label.h"

#include "syntropy/application/command_line_argument.h"

namespace syntropy
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
        static const char kSigil;

        /// \brief Create a new command line.
        /// \param argc Number of arguments in the command line.
        /// \param argv List of arguments in the command line. Each argument is a null-terminated string.
        CommandLine(int argc, char** argv);

        /// \brief Create a new command line.
        /// \param argument_list List of arguments passed to the command line.
        CommandLine(std::vector<std::string> command_line);

        /// \brief Get an argument by name.
        /// \param argument_name Name of the argument to get.
        /// \return Returns the requested argument. If no such argument exists returns nullptr.
        const CommandLineArgument* GetArgument(const Label& argument_name) const;

        /// \brief Check whether the command line defines an argument.
        /// \param argument_name Name of the argument to test.
        /// \return Returns true if the requested argument was defined, returns false otherwise.
        bool HasArgument(const Label& argument_name) const;

        /// \brief Get the arguments in this command line.
        /// \return Returns the arguments in this command line, as key-value pairs.
        const std::vector<CommandLineArgument>& GetArguments() const;

    private:

        /// \brief Emplace a new argument from the provided range of strings.
        /// \param first Iterator to the first string in the range. Must be a valid argument name.
        /// \param last Iterator past the last string in the range.
        /// \return Returns an iterator to the next argument name in the range.
        template <typename TInputIterator>
        TInputIterator EmplaceNextArgument(TInputIterator first, TInputIterator last);

        /// \brief Find the next argument in the provided range of strings.
        /// \param first Iterator to the first string in the range.
        /// \param last Iterator past the last string in the range.
        /// \return Returns an iterator to the first argument found in the provided range.
        template <typename TInputIterator>
        TInputIterator FindNextArgument(TInputIterator first, TInputIterator last);

        /// \brief Convert a string to an argument name, removing any extra token from it.
        /// The string must be a valid argument name, otherwise the behavior of this method is undefined.
        /// \param string String to convert.
        /// \return Returns an argument name by stripping any extra token from the provided string.
        Label ToArgumentName(std::string string);

        std::vector<CommandLineArgument> arguments_;            ///< \brief Arguments in the command line.
    };

}