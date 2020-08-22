/// \file command_line_argument.h
/// \brief This header is part of the Syntropy application module. It contains classes used to represent command line arguments.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <ostream>

#include "syntropy/core/string.h"
#include "syntropy/core/label.h"
#include "syntropy/language/language.h"
#include "syntropy/core/vector.h"

namespace Syntropy
{
    /************************************************************************/
    /* COMMAND LINE ARGUMENT                                                */
    /************************************************************************/

    /// \brief Represents a command line argument along with its values.
    /// \author Raffaele D. Facendola - December 2017
    class CommandLineArgument
    {
    public:

        /// \brief Default constructor.
        CommandLineArgument() = default;

        /// \brief Default copy constructor.
        CommandLineArgument(const CommandLineArgument&) = default;

        /// \brief Default move constructor.
        CommandLineArgument(CommandLineArgument&&) = default;

        /// \brief Create a new command line argument from explicit name and value.
        /// \param name Name of the command line argument.
        /// \param value Argument value.
        CommandLineArgument(const Label& name, String value);

        /// \brief Create a new command line argument from explicit name and values.
        /// \param name Name of the command line argument.
        /// \param values Argument values.
        CommandLineArgument(const Label& name, Array<String> values);

        /// \brief Default assignment operator.
        CommandLineArgument& operator=(const CommandLineArgument&) = default;

        /// \brief Get the argument name.
        /// \return Returns the argument name.
        const Label& GetName() const;

        /// \brief Get the first argument value.
        /// Do not call if the argument is empty.
        /// \return Returns the first argument value.
        const String& GetValue() const;

        /// \brief Get the argument values.
        /// \return Returns the argument values.
        const Array<String>& GetValues() const;

        /// \brief Check whether the argument has at least one value.
        /// \return Returns true if the argument has no value, returns false otherwise.
        Bool IsEmpty() const;

        /// \brief Append new values to the argument.
        template <typename TBegin, typename TEnd>
        void AppendValues(TBegin begin, TEnd end);

    private:

        /// \brief Argument name.
        Label name_;

        /// \brief Argument values.
        Array<String> values_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

        /// \brief Stream insertion for command line arguments.
    std::ostream& operator<<(std::ostream& out, const CommandLineArgument& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // CommandLineArgument.

    inline CommandLineArgument::CommandLineArgument(const Label& name, Array<String> values)
        : name_(name)
        , values_(std::move(values))
    {

    }

    inline CommandLineArgument::CommandLineArgument(const Label& name, String value)
        : name_(name)
    {
        values_.emplace_back(std::move(value));
    }

    inline const Label& CommandLineArgument::GetName() const
    {
        return name_;
    }

    inline const String& CommandLineArgument::GetValue() const
    {
        return values_.front();
    }

    inline const Array<String>& CommandLineArgument::GetValues() const
    {
        return values_;
    }

    inline Bool CommandLineArgument::IsEmpty() const
    {
        return values_.empty();
    }

    template <typename TBegin, typename TEnd>
    inline void CommandLineArgument::AppendValues(TBegin begin, TEnd end)
    {
        values_.insert(std::end(values_), begin, end);
    }

    // Non-member functions.

    inline std::ostream& operator<<(std::ostream& out, const CommandLineArgument& rhs)
    {
        out << rhs.GetName() << " {";

        auto current_it = std::begin(rhs.GetValues());
        auto end_it = std::end(rhs.GetValues());

        if (current_it != end_it)
        {
            out << *current_it;

            for (++current_it; current_it != end_it; ++current_it)
            {
                out << ", " << *current_it;
            }
        }

        out << "}";

        return out;
    }

}