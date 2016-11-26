#include "diagnostics/log.h"

namespace syntropy {

    namespace diagnostics {

        //////////////// LOG ENTRY ////////////////

        LogEntry::LogEntry(const SourceCodeLocation& where)
            : where_(where)
        {}

        //////////////// LOG MANAGER ////////////////

        LogManager& LogManager::GetInstance() {

            static LogManager instance;

            return instance;

        }

    }

}