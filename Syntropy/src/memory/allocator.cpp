#include "memory/allocators/allocator.h"

#include <algorithm>

#include "contexts.h"

#include "diagnostics/log.h"
#include "diagnostics/assert.h"

namespace syntropy
{

    /************************************************************************/
    /* ALLOCATOR :: REGISTER                                                */
    /************************************************************************/

    /// \brief Class used to register allocator instances.
    class Allocator::Register
    {
    public:

        /// \brief Get the singleton instance.
        static Register& GetInstance();

        /// \brief Get an allocator by name.
        /// \param allocator_name Name of the allocator to get.
        /// \return Returns a pointer to an allocator with the provided name, if no such allocator could be found returns nullptr.
        Allocator* GetAllocatorByName(const HashedString& allocator_name);

        /// \brief Register a named allocator.
        void RegisterAllocator(Allocator& allocator);

        /// \brief Unregister an unnamed allocator.
        void UnregisterAllocator(Allocator& allocator);

        /// \brief Move the registration of an allocator that was move-constructed.
        void MoveRegisteredAllocator(Allocator& old_allocator, Allocator& new_allocator);

    private:

        std::vector<Allocator*> allocators_;        ///< \brief List of registered allocators.

    };

    Allocator::Register& Allocator::Register::GetInstance()
    {
        static Register instance;
        return instance;
    }

    Allocator* Allocator::Register::GetAllocatorByName(const HashedString& allocator_name)
    {
        if (allocator_name)
        {
            auto it = std::find_if(allocators_.begin(), allocators_.end(), [&allocator_name](const Allocator* allocator)
            {
                return allocator->GetName() == allocator_name;
            });

            if (it != allocators_.end())
            {
                return *it;
            }
        }

        return nullptr;
    }

    void Allocator::Register::RegisterAllocator(Allocator& allocator)
    {
        SYNTROPY_PRECONDITION(allocator.GetName());

        auto& name = allocator.GetName();

        if (GetAllocatorByName(name))
        {
            SYNTROPY_ERROR((MemoryCtx), "An allocator with name ", name, " already exists.");
        }

        allocators_.emplace_back(&allocator);
    }

    void Allocator::Register::UnregisterAllocator(Allocator& allocator)
    {
        SYNTROPY_PRECONDITION(allocator.GetName());

        allocator.name_ = HashedString::kEmpty;

        allocators_.erase(
            std::remove(allocators_.begin(), allocators_.end(), &allocator),
            allocators_.end());
    }

    void Allocator::Register::MoveRegisteredAllocator(Allocator& old_allocator, Allocator& new_allocator)
    {
        SYNTROPY_PRECONDITION(old_allocator.GetName());

        UnregisterAllocator(old_allocator);

        allocators_.emplace_back(&new_allocator);
    }

    /************************************************************************/
    /* ALLOCATOR                                                            */
    /************************************************************************/

    Allocator* Allocator::GetAllocatorByName(const HashedString& allocator_name)
    {
        return Register::GetInstance().GetAllocatorByName(allocator_name);
    }

    Allocator::Allocator()
        : context_(MemoryCtx)
    {

    }

    Allocator::Allocator(const HashedString& name)
        : name_(name)
        , context_(name ? MemoryCtx | name_ : MemoryCtx)
    {
        if (name_)
        {
            Register::GetInstance().RegisterAllocator(*this);
        }
    }

    Allocator::Allocator(Allocator&& other)
        : name_(std::move(other.name_))
        , context_(std::move(other.context_))
    {
        if (name_)
        {
            Register::GetInstance().MoveRegisteredAllocator(other, *this);
        }
    }

    Allocator::~Allocator()
    {
        if (name_)
        {
            Register::GetInstance().UnregisterAllocator(*this);
        }
    }

    const HashedString& Allocator::GetName() const
    {
        return name_;
    }

    Allocator::operator Context() const
    {
        return context_;
    }

}

/************************************************************************/
/* NEW \ DELETE                                                         */
/************************************************************************/

void* operator new (std::size_t size, syntropy::Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace)
{
    auto ptr = allocator.Allocate(syntropy::Bytes(size));

    SYNTROPY_LOG((allocator), "Allocating ", size, " bytes. Address: ", ptr, ". Caller: ", stack_trace);

    return ptr;
}

void operator delete (void* ptr, syntropy::Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace)
{
    allocator.Free(ptr);

    SYNTROPY_LOG((allocator), "Deallocating memory. Address: ", ptr, ". Caller: ", stack_trace);
}

