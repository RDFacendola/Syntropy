#include "memory/allocator.h"

#include <algorithm>

#include "diagnostics/log.h"

namespace syntropy
{

    /************************************************************************/
    /* ALLOCATOR                                                            */
    /************************************************************************/

    std::vector<Allocator*> Allocator::allocators_;

    Allocator* Allocator::GetAllocatorByName(const HashedString& name)
    {
        auto it = std::find_if(
            std::begin(allocators_),
            std::end(allocators_),
            [&name](const Allocator* allocator)
            {
                return allocator->GetName() == name;
            });

        return it != std::end(allocators_) ?
            *it :
            nullptr;
    }

    Allocator::Allocator()
        : context_(MemoryCtx)
    {

    }

    Allocator::Allocator(const HashedString& name)
        : name_(name)
        , context_(MemoryCtx | name_)
    {
        SYNTROPY_ASSERT(GetAllocatorByName(name) == nullptr);       // Ensures the uniqueness of the allocator name.

        allocators_.push_back(this);
    }

    Allocator::~Allocator()
    {
        // Remove the allocator from the allocator list.
        allocators_.erase(
            std::remove(
                std::begin(allocators_),
                std::end(allocators_),
                this),
            std::end(allocators_));
    }

    const HashedString& Allocator::GetName() const
    {
        return name_;
    }

    Allocator::operator diagnostics::Context() const
    {
        return context_;
    }
}

/************************************************************************/
/* NEW \ DELETE                                                         */
/************************************************************************/

void* operator new (std::size_t size, syntropy::Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace)
{
    auto ptr = allocator.Allocate(size);

    SYNTROPY_LOG((allocator), "Allocating ", size, " bytes. Address: ", ptr, ". Caller: ", stack_trace);

    return ptr;
}

void operator delete (void* ptr, syntropy::Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace)
{
    allocator.Free(ptr);

    SYNTROPY_LOG((allocator), "Deallocating memory. Address: ", ptr, ". Caller: ", stack_trace);
}

