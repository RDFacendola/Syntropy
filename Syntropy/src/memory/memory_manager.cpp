#include "memory/memory_manager.h"

#include <algorithm>
#include <fstream>

#include "diagnostics/log.h"

#include "serialization/json/json.h"

#include "memory/memory_meta.h"

namespace syntropy
{

    /************************************************************************/
    /* MEMORY MANAGER                                                       */
    /************************************************************************/

    thread_local std::vector<Allocator*> MemoryManager::allocator_stack_;

    MemoryManager& MemoryManager::GetInstance()
    {
        static MemoryManager instance;
        return instance;
    }

    MemoryManager::MemoryManager()
    {
        allocator_stack_.reserve(16);
    }

    void MemoryManager::PushContext(const HashedString& allocator_name)
    {
        auto allocator = Allocator::GetAllocatorByName(allocator_name);

        if (allocator)
        {
            allocator_stack_.push_back(allocator);
        }
        else
        {
            // If this call fails the client would use the allocator on top of the stack: we don't really want to allocate on a random allocator in that case.
            allocator_stack_.push_back(allocators_.front().get());

            SYNTROPY_ERROR((MemoryCtx), "No allocator '", allocator_name, "' found. Using default allocator.");
        }
    }

    void MemoryManager::PopContext()
    {
        allocator_stack_.pop_back();
    }

    bool MemoryManager::SetDefaultAllocator(const HashedString& allocator_name)
    {
        auto it = std::find_if
        (
            allocators_.begin(),
            allocators_.end(),
            [&allocator_name](const std::unique_ptr<Allocator>& allocator)
            {
                return allocator->GetName() == allocator_name;
            }
        );

        if (it != allocators_.end())
        {
            std::iter_swap(allocators_.begin(), it);        // Swap the allocator with the element on the head (which is the position for the default allocator).
            return true;
        }
        else
        {
            return false;
        }
    }

    Allocator& MemoryManager::GetDefaultAllocator()
    {
        return *allocators_.front();        // First registered allocator.
    }

    Allocator& MemoryManager::GetAllocator()
    {
        return !allocator_stack_.empty() ?
            *allocator_stack_.back() :          // Current allocator on the stack.
            GetDefaultAllocator();              // Default allocator if the stack's empty.
    }

    Allocator* MemoryManager::GetAllocator(void* block)
    {
        auto it = std::find_if
        (
            allocators_.begin(),
            allocators_.end(),
            [block](const std::unique_ptr<Allocator>& allocator)
            {
                return allocator->Owns(block);
            }
        );

        return it != allocators_.end() ?
            it->get() :
            nullptr;
    }

    MemoryManager& GetMemoryManager()
    {
        return MemoryManager::GetInstance();
    }

    bool ImportMemoryConfigurationFromJSON(const std::string& path)
    {
        // Read the file inside the JSON object.

        std::ifstream file(path);

        nlohmann::json json;

        file >> json;

        // Deserialize the JSON object.

        auto default_allocator_name = serialization::DeserializeObjectFromJSON<std::string>(json, std::nullopt, "default_allocator");
        auto allocators = syntropy::serialization::DeserializeObjectFromJSON<std::vector<std::unique_ptr<syntropy::Allocator>>>(json, std::nullopt, "allocators");

        auto& memory_manager = MemoryManager::GetInstance();

        // Add deserialized allocators to the memory manager

        if (allocators)
        {
            for (auto&& allocator : *allocators)
            {
                memory_manager.AcquireAllocator(std::move(allocator));
            }
        }

        // Set a default allocator

        if (default_allocator_name)
        {
            memory_manager.SetDefaultAllocator(*default_allocator_name);
        }

        return (allocators && !allocators->empty()) || default_allocator_name;
    }

    /************************************************************************/
    /* MEMORY CONTEXT                                                       */
    /************************************************************************/

    MemoryContext::MemoryContext(const HashedString& context_name)
    {
        MemoryManager::GetInstance().PushContext(context_name);
    }

    MemoryContext::~MemoryContext()
    {
        MemoryManager::GetInstance().PopContext();
    }

}
