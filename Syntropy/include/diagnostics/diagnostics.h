
/// \file diagnostics.h
/// \brief This header is part of the syntropy diagnostic system. It contains base definitions for diagnostics functionalities.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <ostream>
#include <string>
#include <vector>
#include <type_traits>
#include <iomanip>
#include <set>

#include "containers/hashed_string.h"
#include "debug.h"
#include "date.h"

namespace syntropy 
{
    namespace diagnostics 
    {

        /// \brief Severity of an event.
        /// \author Raffaele D. Facendola - November 2016
        enum class Severity : uint8_t
        {
            kInformative = 0u,                                              /// \brief Severity used for informative and tracing events that cause no side-effects on the application.
            kWarning = 1u,                                                  /// \brief Severity used for uncommon events that may cause undesired, yet acceptable, side-effects on the application.
            kError = 2u,                                                    /// \brief Severity used for rare events that have substantial impact on the application. Those events should not occur on a shipping version of the application.
            kCritical = 3u                                                  /// \brief Severity used for exceptional events that will cause the application to crash. Those events should never occur!
        };

        /// \brief Stream insertion for Severity.
        std::ostream& operator<<(std::ostream& out, Severity severity);

        /// \brief Context used to categorize events.
        /// \author Raffaele D. Facendola - November 2016
        class Context
        {

        public:

            /// \brief character used to separate two context levels.
            static const char kSeparator = '|';

            /// \brief Create a root context.
            Context();

            /// \brief Create a new context from a name.
            Context(const HashedString& name);

            /// \brief Check whether two contexts are the same.
            /// \param other Other context to  check against.
            /// \return Returns true if the two contexts are the same, returns false otherwise.
            bool operator==(const Context& other) const;

            /// \brief Check whether two contexts are not the same.
            /// \param other Other context to  check against.
            /// \return Returns true if the two contexts are not the same, returns false otherwise.
            bool operator!=(const Context& other) const;

            /// \brief Append a subcontext to this one.
            /// \param subcontext Name of the subcontext to append. May contain sub-subcontexts.
            /// \return Returns a context associated with the provided subcontext whose parent is this context.
            Context operator |(const HashedString& subcontext) const;

            /// \brief Get the context name.
            const HashedString& GetName() const;

            /// \brief Check whether this context is equal to another context or is a more general context than it.
            /// \return Returns true if this context is equal to other or any of its ancestors, returns false otherwise.
            bool Contains(const Context& other) const;

        private:

            class Pool;

            struct InnerContext;

            const InnerContext* context_;               ///< \brief Pointer to the actual context flyweight. Non-owning pointer.
        };

        /// \brief Used to sort contexts.
        bool operator<(const Context& first, const Context& second);

        /// \brief Stream insertion for Context.
        std::ostream& operator<<(std::ostream& out, const Context& context);

        /// \brief Stream insertion for set of Contexts.
        std::ostream& operator<<(std::ostream& out, const std::set<Context>& contexts);

    }
}