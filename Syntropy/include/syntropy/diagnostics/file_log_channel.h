
/// \file file_log_channel.h
/// \brief This header is part of the Syntropy diagnostic module. It contains definition for log channels that output on a file.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <filesystem>
#include <ostream>
#include <fstream>

#include "syntropy/diagnostics/log_event.h"

namespace syntropy
{
    /************************************************************************/
    /* FILE LOG CHANNEL                                                     */
    /************************************************************************/

    /// \brief A log channel that outputs log events to a file.
    /// Using the same file from two or more log channels results in undefined behavior.
    /// \author Raffaele D. Facendola - MAY 2020.
    class FileLogChannel
    {
    public:

        /// \brief Create a new file log channel.
        FileLogChannel(const std::filesystem::path& file_path);

        /// \brief Send a log event to the channel.
        void Send(const LogEvent& log_event);

        /// \brief Flush cached states.
        void Flush();

    private:

        /// \brief Underlying file stream.
        std::ofstream file_stream_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // LogChannelT<TLogChannel>.

    inline FileLogChannel::FileLogChannel(const std::filesystem::path& file_path)
        : file_stream_(file_path)
    {

    }

    inline void FileLogChannel::Send(const LogEvent& log_event)
    {
        file_stream_ << log_event;
    }

    inline void FileLogChannel::Flush()
    {
        file_stream_.flush();
    }

}
