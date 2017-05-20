/// \file log.cpp
/// The point of this file is to break the dependency of the log macros from the log manager.
/// Most of the code may want to use the logging macros without ever knowing about the manager.

#include "diagnostics/log.h"

#include "diagnostics/log_manager.h"

namespace syntropy 
{
    namespace diagnostics
    {

        void SendLogMessage(StackTrace stacktrace, std::vector<Context> contexts, Severity severity, std::string message)
        {
            // Create a log message and send it along.

            LogManager::GetInstance().Send
            (
                LogMessage
                (
                    std::move(stacktrace), 
                    std::move(contexts), 
                    severity, 
                    std::move(message)
                )
            );
        }

    }
}