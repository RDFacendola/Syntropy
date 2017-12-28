#include "application/command_line.h"

#include <algorithm>

#include "containers/range.h"

#include "diagnostics/assert.h"
#include "diagnostics/log.h"

#include "application/application.h"

namespace syntropy
{
    /************************************************************************/
    /* COMMAND LINE ARGUMENT                                                */
    /************************************************************************/

    CommandLineArgument::CommandLineArgument(const HashedString& name, std::vector<std::string> values)
        : name_(name)
        , values_(std::move(values))
    {

    }

    CommandLineArgument::CommandLineArgument(const HashedString& name, std::string value)
        : name_(name)
    {
        values_.emplace_back(std::move(value));
    }

    const HashedString& CommandLineArgument::GetName() const
    {
        return name_;
    }

    CommandLineArgument::operator const std::string&() const
    {
        return GetValue();
    }

    const std::string& CommandLineArgument::GetValue() const
    {
        return values_.front();
    }

    const std::vector<std::string>& CommandLineArgument::GetValues() const
    {
        return values_;
    }

    bool CommandLineArgument::IsEmpty() const
    {
        return values_.empty();
    }

    /************************************************************************/
    /* COMMAND LINE                                                         */
    /************************************************************************/

    const char CommandLine::kSigil = '-';

    CommandLine::CommandLine(int argc, char** argv)
        : CommandLine(std::vector<std::string>(argv, argv + argc))
    {

    }

    CommandLine::CommandLine(std::vector<std::string> command_line)
    {
        arguments_.reserve(command_line.size());

        auto first = FindNextArgument(std::begin(command_line), std::end(command_line));                                // Advance to the next valid argument.

        auto last = std::end(command_line);

        while (first != last)
        {
            first = EmplaceNextArgument(first, last);
        }
    }

    const CommandLineArgument* CommandLine::GetArgument(const HashedString& argument_name) const
    {
        auto it = std::find_if(std::begin(arguments_), std::end(arguments_), [&argument_name](auto& argument)
        {
            return argument.GetName() == argument_name;
        });

        return it != std::end(arguments_) ? &(*it) : nullptr;
    }

    bool CommandLine::HasArgument(const HashedString& argument_name) const
    {
        return GetArgument(argument_name) != nullptr;
    }

    const std::vector<CommandLineArgument>& CommandLine::GetArguments() const
    {
        return arguments_;
    }

    template <typename TInputIterator>
    TInputIterator CommandLine::EmplaceNextArgument(TInputIterator first, TInputIterator last)
    {
        auto argument_name = ToArgumentName(std::move(*first));

        auto next = FindNextArgument(++first, last);

        if (GetArgument(argument_name) == nullptr)
        {
            // Everything between this argument and the next one is considered to be this argument's values.

            arguments_.emplace_back(argument_name, std::make_move_iterator(first), std::make_move_iterator(next));
        }
        else
        {
            // Discard duplicate arguments to avoid unexpected behaviors.

            SYNTROPY_WARNING((ApplicationCtx), "Duplicated command line argument '", argument_name, "' having value(s) {", MakeRange(first, last), "} has been ignored.");
        }

        return next;
    }

    template <typename TInputIterator>
    TInputIterator CommandLine::FindNextArgument(TInputIterator first, TInputIterator last)
    {
        return std::find_if(first, last, [](auto& argument)
        {
            return argument.size() > 0 && argument[0] == kSigil;
        });
    }

    HashedString CommandLine::ToArgumentName(std::string string)
    {
        string.erase(std::begin(string));               // Drop the first character (sigil). Should not reallocate (the standard doesn't guarantee that, though).

        return HashedString(std::move(string));
    }
}
