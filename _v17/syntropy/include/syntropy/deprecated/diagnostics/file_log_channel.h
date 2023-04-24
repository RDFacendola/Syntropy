
/// \file file_log_channel.h
/// \brief This header is part of the Syntropy diagnostic module. It contains definition for log channels that output on a file.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <filesystem>
#include <ostream>
#include <fstream>

#include "syntropy/diagnostics/log_event.h"
#include "syntropy/diagnostics/log_event_formatter.h"
#include "syntropy/diagnostics/log_event_filter.h"
#include "syntropy/diagnostics/auto_log_channel.h"

namespace Syntropy
{
    /************************************************************************/
    /* FILE LOG CHANNEL                                                     */
    /************************************************************************/

    /// \brief A log channel that outputs log events to a file.
    /// Using the same file from two or more log channels results in undefined behavior.
    /// \author Raffaele D. Facendola - May 2020.
    class FileLogChannel
    {
    public:

        /// \brief Create a new file log channel.
        FileLogChannel(const std::filesystem::path& file_path, const LogEventFilter& filter = LogEventFilter{}, const String& format = "[%time][%context][%severity]: %message");

        /// \brief No copy-constructor.
        /// \remarks Opening the same file more than once results in undefined behavior.
        FileLogChannel(const FileLogChannel&) = delete;

        /// \brief Default move-constructor.
        FileLogChannel(FileLogChannel&&) = default;

        /// \brief No copy-assignment.
        /// \remarks Opening the same file more than once results in undefined behavior.
        FileLogChannel& operator=(const FileLogChannel&) = delete;

        /// \brief Default move-assignment.
        FileLogChannel& operator=(FileLogChannel&&) = default;

        /// \brief Destructor.
        ~FileLogChannel();

        /// \brief Send a log event to the channel.
        void Send(const LogEvent& log_event);

        /// \brief Flush cached states.
        void Flush();

    private:

        /// \brief Underlying file stream.
        std::ofstream file_stream_;

        /// \brief Filter to match against incoming log messages.
        LogEventFilter filter_;

        /// \brief Formatter used when writing a log event.
        LogEventFormatter formatter_;

    };

    /************************************************************************/
    /* TYPE ALIAS                                                           */
    /************************************************************************/

    /// \brief Type alias for a self-registering log file channel.
    using AutoLogFile = AutoLogChannelT<FileLogChannel>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a self-registering file log channel.
    template <typename... TArguments>
    AutoLogFile MakeAutoLogFile(TArguments&&... arguments);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // LogChannelT<TLogChannel>.

    inline FileLogChannel::FileLogChannel(const std::filesystem::path& file_path, const LogEventFilter& filter, const String& format)
        : file_stream_(file_path)
        , filter_(filter)
        , formatter_(format)
    {

    }

    inline FileLogChannel::~FileLogChannel()
    {
        file_stream_.close();
    }

    inline void FileLogChannel::Send(const LogEvent& log_event)
    {
        if (filter_(log_event))
        {
            file_stream_ << formatter_(log_event) << '\n';
        }
    }

    inline void FileLogChannel::Flush()
    {
        file_stream_.flush();
    }

    // Non-member functions.

    template <typename... TArguments>
    inline AutoLogFile MakeAutoLogFile(TArguments&&... arguments)
    {
        return { Forward<TArguments>(arguments)... };
    }

}
