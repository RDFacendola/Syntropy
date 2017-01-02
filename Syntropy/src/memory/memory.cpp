#include "memory/memory.h"

#include <cstdlib>
#include <cstring>
#include <algorithm>

#include "platform/os.h"

#include "syntropy.h"

void* operator new(std::size_t size)
{
    return std::malloc(size);
}

void operator delete(void* pointer) noexcept 
{
    std::free(pointer);
}

namespace syntropy
{

    //////////////// MEMORY ////////////////
    
    Memory& Memory::GetInstance()
    {
        return platform::PlatformMemory::GetInstance();
    }

    Memory& GetMemory()
    {
        return Memory::GetInstance();
    }

}