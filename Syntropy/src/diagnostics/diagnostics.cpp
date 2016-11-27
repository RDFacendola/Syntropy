#include "diagnostics/diagnostics.h"

namespace syntropy 
{
    namespace diagnostics 
    {

        //////////////// TRACE ////////////////

        Trace::Trace(const char* file, const char* function, int line)
            : file_(file)
            , function_(function)
            , line_(line) 
        {

        }

        //////////////// CALLSTACK ////////////////

        Callstack::Callstack(const Trace& trace)
        {
            callstack_.push_back(trace);
        }

        Callstack::Callstack(Callstack&& other) noexcept
            : callstack_(std::move(other.callstack_))
        {

        }

        Callstack& Callstack::operator=(Callstack other) noexcept
        {
            other.Swap(*this);
            return *this;
        }

        void Callstack::Swap(Callstack& other) noexcept
        {
            std::swap(callstack_, other.callstack_);
        }

        Callstack::operator Trace&()
        {
            return callstack_[0];
        }

        Callstack::operator const Trace&() const
        {
            return callstack_[0];
        }

        //////////////// CONTEXT ////////////////

        std::mutex& Context::GetMutex()
        {
            static std::mutex mutex;
            return mutex;
        }

        std::unordered_map<Context::id_t, HashedString>& Context::GetContexts()
        {
            static std::unordered_map<id_t, HashedString> contexts;
            return contexts;
        }

        Context::Context(const char* name)
            : Context(HashedString(name))
        {

        }

        Context::Context(const HashedString& name)
            : id_(name.GetHash())
        {
            std::unique_lock<std::mutex> lock(GetMutex());

            auto& contexts = GetContexts();

            if (contexts.find(id_) == contexts.end())
            {
                // Add a new context to the map
                contexts.insert(std::make_pair(id_, name));
            }
        }

        const HashedString& Context::GetName() const
        {
            std::unique_lock<std::mutex> lock(GetMutex());

            return GetContexts()[id_];
        }

        bool Context::operator==(const Context& other) const
        {
            return id_ == other.id_;
        }

        bool Context::operator!=(const Context& other) const
        {
            return id_ != other.id_;
        }

        //////////////// EVENT ////////////////

        Event::Event(std::initializer_list<Context> contexts, const Callstack& callstack, Severity severity)
            : timestamp_(std::chrono::high_resolution_clock::now())
            , severity_(severity)
            , thread_id_(std::this_thread::get_id())
            , contexts_(contexts)
            , callstack_(callstack)
        {

        }

    }

}