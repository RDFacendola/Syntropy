
/// \file diagnostics.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

/// \brief Expands to an object representing the location of the current line of code.
#define SYNTROPY_SOURCECODE syntropy::diagnostics::SourceCodeLocation(__FILE__, __FUNCTION__, __LINE__)

namespace syntropy {

    namespace diagnostics {

        /// \brief Represents the location of a line of code
        struct SourceCodeLocation {
            
            SourceCodeLocation(const char* file, const char* function, int line);

            const char* file_;                                              ///< \brief Name of the file where the log was defined.

            const char* function_;                                          ///< \brief Name of the function where the log was defined.

            int line_;                                                      ///< \brief Line inside the file where the log was defined.

        };

    }

}
