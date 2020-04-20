/// \file command_line_argument.h
/// \brief This header is part of the syntropy application system. It contains classes used to represent command line arguments.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/containers/string.h"
#include "syntropy/containers/vector.h"
#include "syntropy/containers/hashed_string.h"

namespace syntropy
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
        CommandLineArgument(const HashedString& name, String value);

        /// \brief Create a new command line argument from explicit name and values.
        /// \param name Name of the command line argument.
        /// \param values Argument values.
        CommandLineArgument(const HashedString& name, Vector<String> values);

        /// \brief Create a new command line argument from explicit name and values in a range.
        /// \param name Name of the command line argument.
        /// \param first Iterator to the first argument value.
        /// \param last Iterator past the last argument value.
        template <typename TInputIterator>
        CommandLineArgument(const HashedString& name, TInputIterator first, TInputIterator last);

        /// \brief Default assignment operator.
        CommandLineArgument& operator=(const CommandLineArgument&) = default;

        /// \brief Get the argument name.
        /// \return Returns the argument name.
        const HashedString& GetName() const;

        /// \brief Get the first argument value.
        /// Do not call if the argument is empty.
        /// \return Returns the first argument value.
        const String& GetValue() const;

        /// \brief Get the argument values.
        /// \return Returns the argument values.
        const Vector<String>& GetValues() const;

        /// \brief Check whether the argument has at least one value.
        /// \return Returns true if the argument has no value, returns false otherwise.
        bool IsEmpty() const;

    private:

        /// \brief Argument name.
        HashedString name_;

        /// \brief Argument values.
        Vector<String> values_;
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // CommandLineArgument.

    inline CommandLineArgument::CommandLineArgument(const HashedString& name, Vector<String> values)
        : name_(name)
        , values_(std::move(values))
    {

    }

    template <typename TInputIterator>
    inline CommandLineArgument::CommandLineArgument(const HashedString& name, TInputIterator first, TInputIterator last)
        : name_(name)
        , values_(first, last)
    {

    }

    inline CommandLineArgument::CommandLineArgument(const HashedString& name, String value)
        : name_(name)
    {
        values_.emplace_back(std::move(value));
    }

    inline const HashedString& CommandLineArgument::GetName() const
    {
        return name_;
    }

    inline const String& CommandLineArgument::GetValue() const
    {
        return values_.front();
    }

    inline const Vector<String>& CommandLineArgument::GetValues() const
    {
        return values_;
    }

    inline bool CommandLineArgument::IsEmpty() const
    {
        return values_.empty();
    }

}