#include "containers/context.h"

#include <mutex>
#include <unordered_map>
#include <memory>

namespace syntropy
{
    /************************************************************************/
    /* CONTEXT :: INNER CONTEXT                                             */
    /************************************************************************/

    /// \brief Flyweight used to wrap a chain of contexts.
    /// An inner context contains the full name of the context and a pointer to its parent.
    ///
    /// FullName |CtxA|CtxB|CtxC    Parent |CtxA|CtxB
    /// FullName |CtxA|CtxB|        Parent |CtxA
    /// FullName |CtxA              Parent %root
    ///
    /// \author Raffaele D. Facendola - December 2016
    struct Context::InnerContext
    {
        /// \brief Create a root context.
        InnerContext() = default;

        /// \brief Create a context by name.
        /// \param Full name of the context.
        /// \param Parent context.
        InnerContext(const HashedString& name, const InnerContext* parent)
            : name_(name)
            , parent_(parent)
        {

        }

        /// \brief Check whether this context contains another one.
        /// \param other Context to check the inclusion of.
        /// \return Returns true if other is contained inside this context, returns false otherwise.
        bool Contains(const InnerContext& other) const
        {
            auto other_context = &other;

            // Consume the inner contexts of "other" until the resulting context is empty or equal to this context.

            while (other_context && name_ != other_context->name_)
            {
                other_context = other_context->parent_;
            }

            return !!other_context;
        }

        HashedString name_;                         ///< \brief Full name of the context.

        const InnerContext* parent_;                ///< \brief Parent context. nullptr if root context. Non-owning pointer.
    };

    /************************************************************************/
    /* CONTEXT :: POOL                                                      */
    /************************************************************************/

    /// \brief Pool of contexts flyweight allocated so far.
    /// \author Raffaele D. Facendola - December 2016
    class Context::Pool
    {
    public:

        /// \brief Get the singleton instance.
        /// \return Returns the singleton instance.
        static Pool& GetInstance()
        {
            static Pool instance;
            return instance;
        }

        /// \brief Get a context by name.
        /// The method will create a new context if none is found.
        const InnerContext& GetContextByName(const HashedString& name) const
        {
            if (!name)
            {
                return GetRootContext();
            }

            std::unique_lock<std::recursive_mutex> lock(mutex_);

            auto it = contexts_.find(name);

            if (it != contexts_.end())
            {
                return *(it->second);      // Found
            }

            // Create a new context

            auto index = name.GetString().find_last_of(kSeparator);

            std::string parent_name = index != std::string::npos ?
                name.GetString().substr(0, index) :
                "";

            auto context = std::make_unique<Context::InnerContext>(name, std::addressof(GetContextByName(parent_name)));        // Parent contexts are added recursively if needed.

            return *contexts_.emplace(std::make_pair(name, std::move(context))).first->second;
        }

        /// \brief Get the root context.
        /// The root is the context that contains every other one.
        const InnerContext& GetRootContext() const
        {
            return *root_;
        }

    private:

        using TContextMap = std::unordered_map<HashedString, std::unique_ptr<InnerContext>>;

        /// \brief Create a new pool of contexts.
        Pool()
        {
            auto root = std::make_unique<InnerContext>();

            root_ = root.get();

            contexts_.emplace(std::make_pair(root_->name_, std::move(root)));      // Add the root to the context list.
        }

        mutable std::recursive_mutex mutex_;        ///< \brief Used for synchronization

        InnerContext* root_;                        /// \brief Root context. Non-owning pointer.

        mutable TContextMap contexts_;              ///< \brief List of contexts registered so far.

    };

    /************************************************************************/
    /* CONTEXT                                                              */
    /************************************************************************/

    Context::Context()
        : context_(std::addressof(Pool::GetInstance().GetRootContext()))
    {

    }

    Context::Context(const HashedString& name)
        : context_(std::addressof(Pool::GetInstance().GetContextByName(name)))
    {

    }

    Context::Context(const char* name)
        : Context(HashedString(name))
    {

    }

    Context::operator const HashedString&() const
    {
        return GetName();
    }

    bool Context::operator==(const Context& other) const
    {
        return context_ == other.context_;      // Flyweight, check by address.
    }

    bool Context::operator!=(const Context& other) const
    {
        return context_ != other.context_;      // Flyweight, check by address.
    }

    Context Context::operator |(const HashedString& subcontext) const
    {
        return Context(context_->name_.GetString() + kSeparator + subcontext.GetString());
    }

    const HashedString& Context::GetName() const
    {
        return context_->name_;
    }

    bool Context::Contains(const Context& other) const
    {
        return context_->Contains(*other.context_);
    }

    bool operator<(const Context& first, const Context& second)
    {
        return first.GetName() < second.GetName();
    }

    std::ostream& operator<<(std::ostream& out, const Context& context)
    {
        out << context.GetName();
        return out;
    }
}
