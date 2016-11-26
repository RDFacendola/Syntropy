#include "diagnostics/diagnostics.h"

namespace syntropy {

    namespace diagnostics {

        //////////////// SOURCE CODE LOCATION ////////////////

        SourceCodeLocation::SourceCodeLocation(const char* file, const char* function, int line)
            : file_(file)
            , function_(function)
            , line_(line) {

        }

    }

}