
/// \file context.h
/// \brief This header is part of the Syntropy core module. It contains classes used to handle hierarchical labels (contexts).
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <ostream>

#include "syntropy/language/fundamentals.h"
#include "syntropy/core/label.h"
#include "syntropy/core/string.h"

namespace Syntropy
{
    /************************************************************************/
    /* CONTEXT                                                              */
    /************************************************************************/

    /// \brief Hierarchical label types supporting inclusion tests.
    /// A context is a case-sensitive label of the form "LayerN.LayerN-1.???.Layer1.Layer0". Similar to URL, innermost layers are written before outer ones.
    /// Inclusion test can be used to check whether a given context is a sub-context of another one.
    /// A root context is either created from default constructor or from nullptr; empty contexts are considered regular (non-root) contexts.
    /// All non-root contexts are implicitly sub-context of a root-context, therefore a root context contains every other context.
    /// \author Raffaele D. Facendola - November 2016
    class Context
    {
    public:

        /// \brief character used to separate two context levels.
        static constexpr char kSeparator = '.';

        /// \brief Type of a string view that can be used to initialize the context.
        using TStringView = Label::TStringView;

        /// \brief Create a root context.
        Context() = default;

        /// \brief Create a root context.
        Context(std::nullptr_t);

        /// \brief Default copy constructor.
        Context(const Context& other) = default;

        /// \brief Create a new context from a string view.
        Context(const TStringView& name);

        /// \brief Create a new context from a string.
        Context(const Label::TString& name);

        /// \brief Create a new context from a label.
        Context(const Label& name);

        /// \brief Create a new context from a null-terminated string.
        Context(const Label::TChar* name);

        /// \brief Implicit conversion to label.
        operator const Label&() const;

        /// \brief Get the full context name.
        const Label& GetName() const;

        /// \brief Check whether this context contains another one.
        Bool Contains(const Context& other) const;

    private:

        class Registry;

        /// \brief Full context name.
        Label name_;

        /// \brief Outer context.
        const Context* outer_{ nullptr };

    };

    /************************************************************************/
    /* NON MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Equality comparison for Context.
    Bool operator==(const Context& lhs, const Context& rhs);

    /// \brief Inequality comparison for Context.
    Bool operator!=(const Context& lhs, const Context& rhs);

    /// \brief Append an outer context rhs to lhs.
    Context operator+(const Context& lhs, const Context& rhs);

    /// \brief Get the non-cryptographic 64-bit hash associated to a context.
    Int Hash64(const Context& lhs);

    /// \brief Stream insertion for Context.
    std::ostream& operator<<(std::ostream& out, const Context& context);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Context.

    inline Context::Context(std::nullptr_t)
    {

    }

    inline Context::Context(const Label::TString& name)
        : Context(TStringView(name))
    {

    }

    inline Context::Context(const Label& name)
        : Context(name.GetCharacters())
    {

    }

    inline Context::Context(const Label::TChar* name)
        : Context(TStringView(name))
    {

    }

    inline Context::operator const Label&() const
    {
        return GetName();
    }


    inline const Label& Context::GetName() const
    {
        return name_;
    }

    inline Bool Context::Contains(const Context& other) const
    {
        // Checking context outers provide consistent behaviors when matching empty contexts against root contexts and vice-versa.

        return ((name_ == other.name_) && (outer_ == other.outer_)) || (other.outer_ && Contains(*other.outer_));
    }

    // Non-member functions.

    inline Bool operator==(const Context& lhs, const Context& rhs)
    {
        return lhs.GetName() == rhs.GetName();
    }

    inline Bool operator!=(const Context& lhs, const Context& rhs)
    {
        return !(lhs == rhs);
    }

    inline Context operator+(const Context& lhs, const Context& rhs)
    {
        // #TODO Add an inline memory context to avoid temporary allocations.

        auto context = String(lhs.GetName().GetCharacters()) + Context::kSeparator + rhs.GetName().GetCharacters();

        return Context(context);
    }

    inline Int Hash64(const Context& lhs)
    {
        return Hash64(lhs.GetName());
    }

    inline std::ostream& operator<<(std::ostream& out, const Context& context)
    {
        out << context.GetName();
        return out;
    }
}
