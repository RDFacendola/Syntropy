/// \file command_parser.h
/// \brief This header is part of the syntropy application system. It contains classes used to parse and call commands by string.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <functional>
#include <unordered_map>
#include <sstream>

#include "syntropy/type_traits.h"
#include "syntropy/patterns/tuple.h"
#include "syntropy/containers/hashed_string.h"

namespace syntropy
{
    /************************************************************************/
    /* COMMAND PARSER T                                                     */
    /************************************************************************/

    /// \brief Class used to parse and call commands by string.
    /// \author Raffaele D. Facendola - November 2019
    template <typename TStream>
    class CommandParserT
    {
    public:

        /// \brief Bind a new command to the parser.
        /// If another command is bound with the same name, the behavior of this method is undefined.
        /// \return Returns a reference to the parser.
        template <typename TCommand>
        CommandParserT& Bind(const HashedString& name, TCommand&& command);

        /// \brief Parse a command by string and call the corresponding callable object.
        /// \param command Command string to parse.
        /// \return Returns true if a command could be parsed and called, returns false otherwise.
        bool Parse(const std::string& command);

    private:

        /// \brief Callable object.
        using Command = std::function<bool(TStream& stream)>;

        /// \brief Commands bound to the parser.
        std::unordered_map<syntropy::HashedString, Command> commands_;

    };

    /// \brief Default command parser.
    using CommandParser = CommandParserT<std::stringstream>;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    template <typename TStream>
    template <typename TCommand>
    CommandParserT<TStream>& CommandParserT<TStream>::Bind(const HashedString& name, TCommand&& command)
    {
        commands_[name] = [this, command = std::move(command)](TStream& stream)
        {
            auto arguments = syntropy::function_arguments_t<TCommand>{};

            auto deserialize_argument = [&stream](auto&& argument)
            {
                if (!stream.fail())
                {
                    stream >> argument;
                }
            };

            syntropy::LockstepApply(deserialize_argument, arguments);

            if (!stream.fail() && stream.eof())
            {
                std::apply(command, std::move(arguments));
                return true;
            }

            return false;
        };

        return *this;
    }

    template <typename TStream>
    bool CommandParserT<TStream>::Parse(const std::string& command)
    {
        auto stream = TStream{ command };

        auto command_name = std::string{};

        stream >> command_name;

        if (auto command_it = commands_.find(command_name); command_it != end(commands_))
        {
            return command_it->second(stream);
        }

        return false;
    }

}