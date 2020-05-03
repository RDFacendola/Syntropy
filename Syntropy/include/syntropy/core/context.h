
/// \file context.h
/// \brief This header is part of the Syntropy core module. It contains classes used to handle hierarchical labels (contexts).
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <ostream>

#include "syntropy/core/label.h"
#include "syntropy/core/string.h"

namespace syntropy
{
    /************************************************************************/
    /* CONTEXT                                                              */
    /************************************************************************/

    /// \brief Hierarchical level supporting inclusion tests.
    /// A context is a label of the form "Layer1/Layer2/.../LayerN".
    /// Inclusion test can be used to check whether a given context is a subcontext of another one.
    /// \remarks Contexts are case-sensitive and consider empty contexts as regular ones (i.e. "a/b/c" won't match "a/b/" as the latter ends with an empty context).
    /// \author Raffaele D. Facendola - November 2016
    class Context
    {
    public:

        /// \brief character used to separate two context levels.
        static constexpr char kSeparator = '/';

        /// \brief Type of a string view that can be used to initialize the context.
        using TStringView = Label::TStringView;

        /// \brief Create a root context.
        Context() = default;

        /// \brief Default copy constructor.
        Context(const Context& other) = default;

        /// \brief Create a new context from a null-terminated string.
        Context(const TStringView& name);

        /// \brief Implicit conversion to label.
        operator const Label&() const;

        /// \brief Append a subcontext to this one and return the resulting context.
        Context operator /(const Context& other) const;

        /// \brief Get the full context name.
        const Label& GetName() const;

        /// \brief Check whether this context matches another context.
        /// A context is said to "match" another context when the former is equal or contained inside the latter.
        bool Matches(const Context& other) const;

    private:

        class Registry;

        /// \brief Full context name.
        Label name_;

        /// \brief Parent context.
        const Context* parent_{ nullptr };

    };

    /************************************************************************/
    /* NON MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Equality comparison for Context.
    bool operator==(const Context& lhs, const Context& rhs);

    /// \brief Inequality comparison for Context.
    bool operator!=(const Context& lhs, const Context& rhs);

    /// \brief Get the non-cryptographic 64-bit hash associated to a context.
    std::int64_t Hash64(const Context& lhs);

    /// \brief Stream insertion for Context.
    std::ostream& operator<<(std::ostream& out, const Context& context);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Context.

    inline Context::operator const Label&() const
    {
        return GetName();
    }

    inline Context Context::operator /(const Context& other) const
    {
        // #TODO Add an inline memory context to avoid temporary allocations.

        auto subcontext = String(name_.GetCharacters()) + kSeparator + other.GetName().GetCharacters();

        return TStringView(subcontext);
    }

    inline const Label& Context::GetName() const
    {
        return name_;
    }

    inline bool Context::Matches(const Context& other) const
    {
        return (name_ == other.name_) || (parent_ && parent_->Matches(other));
    }

    // Non-member functions.

    inline bool operator==(const Context& lhs, const Context& rhs)
    {
        return lhs.GetName() == rhs.GetName();
    }

    inline bool operator!=(const Context& lhs, const Context& rhs)
    {
        return !(lhs == rhs);
    }

    inline std::int64_t Hash64(const Context& lhs)
    {
        return Hash64(lhs.GetName());
    }

    inline std::ostream& operator<<(std::ostream& out, const Context& context)
    {
        out << context.GetName();
        return out;
    }
}
