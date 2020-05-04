#include "syntropy/application/command_line.h"

namespace syntropy
{
    /************************************************************************/
    /* COMMAND LINE                                                         */
    /************************************************************************/

    CommandLine::CommandLine(Vector<String> command_line)
    {
        arguments_.reserve(command_line.size());

        auto current_argument = FindNextArgument(std::begin(command_line), std::end(command_line));

        auto end_argument = std::end(command_line);

        while (current_argument != end_argument)
        {
            current_argument = EmplaceNextArgument(current_argument, end_argument);
        }
    }

    template <typename TIterator>
    TIterator CommandLine::EmplaceNextArgument(TIterator begin_argument, TIterator end_argument)
    {
        auto argument_name = Label{ begin(*begin_argument) + 1, end(*begin_argument) };

        auto next_argument = FindNextArgument(++begin_argument, end_argument);

        // Everything between this argument and the next one is considered to be this argument's values.

        if (auto argument = GetArgumentForUpdate(argument_name))
        {
            // The argument already exists: append the new values.

            argument->AppendValues(std::make_move_iterator(begin_argument), std::make_move_iterator(next_argument));
        }
        else
        {
            // The argument doesn't exist yet: add a new one.

            arguments_.emplace_back(argument_name, Vector<String>{ std::make_move_iterator(begin_argument), std::make_move_iterator(next_argument) });
        }

        return next_argument;
    }
}
