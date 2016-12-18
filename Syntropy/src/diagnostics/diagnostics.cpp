#include "diagnostics/diagnostics.h"

#include <mutex>
#include <unordered_map>

namespace syntropy 
{
    namespace diagnostics 
    {
        //////////////// SEVERITY ////////////////

        static const std::unordered_map<Severity, const char*> severity_map = { { Severity::kInformative, "Info" },
                                                                                { Severity::kWarning, "Warning" },
                                                                                { Severity::kError, "Error" },
                                                                                { Severity::kCritical, "Critical" } };

        std::ostream& operator<<(std::ostream& out, Severity severity)
        {
            auto it = severity_map.find(severity);

            if (it != severity_map.end())
            {
                out << it->second;
            }
            else
            {
                out << "<Unknown severity>";
            }

            return out;
        }
        
        //////////////// CONTEXT :: INNER CONTEXT ////////////////

        /// \brief Represents the actual context.
        /// \author Raffaele D. Facendola - December 2016
        struct Context::InnerContext
        {

            /// \brief Create a root context.
            InnerContext() = default;

            /// \brief Create a context by name.
            /// \param Name of the context.
            /// \param Parent context.
            InnerContext(const HashedString& name, std::shared_ptr<InnerContext> parent);

            /// \brief Check whether this context contains another one.
            /// \param other Context to check the inclusion of.
            /// \return Returns true if other is contained inside this context, returns false otherwise.
            bool Contains(const InnerContext& other) const;

            HashedString name_;                                 ///< \brief Full name of the context.

            std::shared_ptr<InnerContext> parent_;              ///< \brief Parent context. Nullptr if root context.

        };

        Context::InnerContext::InnerContext(const HashedString& name, std::shared_ptr<InnerContext> parent)
            : name_(name)
            , parent_(parent)
        {

        }

        bool Context::InnerContext::Contains(const InnerContext& other) const
        {
            auto other_context = &other;

            while (other_context)
            {
                if (name_ == other_context->name_)
                {
                    return true;
                }
                other_context = other_context->parent_.get();       // Walk up the hierarchy
            }

            return false;
        }

        //////////////// CONTEXT :: POOL ////////////////

        /// \brief Pool of contexts allocated so far.
        /// \author Raffaele D. Facendola - December 2016
        class Context::Pool
        {

        public:

            /// \brief Get the singleton instance.
            /// \return Returns the singleton instance.
            static Pool& GetInstance();

            /// \brief Get a context by name.
            /// The method will create a new context if none is found.
            std::shared_ptr<InnerContext> GetContextByName(const HashedString& name);

            /// \brief Get the root context.
            /// The root is the context that contains every other one.
            std::shared_ptr<InnerContext> GetRootContext();

        private:

            /// \brief Create a new pool of contexts.
            Pool();

            std::shared_ptr<Context::InnerContext> root_;                                                           ///< \brief Root context

            std::recursive_mutex mutex_;                                                                            ///< \brief Used for synchronization

            std::unordered_map<HashedString::hash_t, std::shared_ptr<InnerContext>> contexts_;                      ///< \brief List of contexts registered so far

        };

        Context::Pool::Pool()
            : root_(std::make_shared<Context::InnerContext>())
        {
            contexts_.insert(std::make_pair(root_->name_.GetHash(), root_));        // Add the root to the context list
        }

        Context::Pool& Context::Pool::GetInstance()
        {
            static Pool instance;
            return instance;
        }

        std::shared_ptr<Context::InnerContext> Context::Pool::GetContextByName(const HashedString& name)
        {

            std::unique_lock<std::recursive_mutex> lock(mutex_);

            auto it = contexts_.find(name.GetHash());

            if (it != contexts_.end())
            {
                return it->second;      // Found
            }

            // Create a new context
            
            auto index = name.GetString().find_last_of(kSeparator);

            std::string parent_name = index != std::string::npos ?
                                      name.GetString().substr(0, index) :
                                      "";

            auto context = std::make_shared<Context::InnerContext>(name, 
                                                                   GetContextByName(parent_name));      // Get the parent context (recursive)

            contexts_.insert(std::make_pair(name.GetHash(),
                                            context));

            return context;
        }

        std::shared_ptr<Context::InnerContext> Context::Pool::GetRootContext()
        {
            return root_;
        }

        //////////////// CONTEXT ////////////////

        Context::Context()
            : context_(Pool::GetInstance().GetRootContext())
        {

        }

        Context::Context(const HashedString& name)
            : context_(Pool::GetInstance().GetContextByName(name))
        {

        }

        bool Context::operator==(const Context& other) const
        {
            return context_ == other.context_;
        }

        bool Context::operator!=(const Context& other) const
        {
            return context_ != other.context_;
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

        std::ostream& operator<<(std::ostream& out, const std::set<Context>& contexts)
        {
            if (contexts.size() > 0)
            {
                auto&& it = contexts.begin();

                out << *it;                      // First context

                for (++it; it != contexts.end(); ++it)
                {
                    out << ", " << *it;          // Remaining contexts
                }
            }
            return out;
        }

    }
}