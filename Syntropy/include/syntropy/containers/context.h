
/// \file context.h
/// \brief This header is part of the syntropy containers. It contains classes used to handle hashed paths.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <ostream>

#include "syntropy/types/label.h"

namespace syntropy
{
    /************************************************************************/
    /* CONTEXT                                                              */
    /************************************************************************/

    /// \brief Hashed string path supporting inclusion tests.
    /// A context is a hashed string of the form "Layer1|Layer2|...|LayerN".
    /// Inclusion test can be used to check whether a given context is a subcontext of another one.
    /// \author Raffaele D. Facendola - November 2016
    class Context
    {
    public:

        /// \brief character used to separate two context levels.
        static const char kSeparator = '|';

        /// \brief Represents a root context.
        static const Context kRoot;

        /// \brief Create a root context.
        Context();

        /// \brief Copy constructor.
        Context(const Context& other) = default;

        /// \brief Create a new context from a name.
        Context(const Label& name);

        /// \brief Create a new context from a null-terminated string.
        Context(const char* name);

        /// \brief Implicit conversion to label.
        operator const Label&() const;

        /// \brief Check whether two contexts are the same.
        /// \param other Other context to  check against.
        /// \return Returns true if the two contexts are the same, returns false otherwise.
        bool operator==(const Context& other) const;

        /// \brief Check whether two contexts are not the same.
        /// \param other Other context to  check against.
        /// \return Returns true if the two contexts are not the same, returns false otherwise.
        bool operator!=(const Context& other) const;

        /// \brief Append a subcontext to this one and return the resulting context.
        /// \param subcontext Name of the subcontext to append. May contain sub-subcontexts.
        /// \return Returns a context associated with the provided subcontext whose parent is this context.
        Context operator |(const Label& subcontext) const;

        /// \brief Get the context name.
        const Label& GetName() const;

        /// \brief Check whether this context is equal to another context or is a more general context than it.
        /// \return Returns true if this context is equal to other or any of its ancestors, returns false otherwise.
        bool Contains(const Context& other) const;

    private:

        /// \brief Flyweight used to wrap a chain of contexts.
        struct InnerContext;

        /// \brief Pool of inner contexts shared among all the instances.
        class Pool;

        const InnerContext* context_;               ///< \brief Pointer to the actual context flyweight. Non-owning pointer.
    };

    /// \brief Used to sort contexts.
    bool operator<(const Context& first, const Context& second);

    /// \brief Stream insertion for Context.
    std::ostream& operator<<(std::ostream& out, const Context& context);
}
