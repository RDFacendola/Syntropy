
// ProtoDW.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <random>
#include <thread>
#include <exception>
#include <chrono>

#include "unit1.h"

#include "vm/virtual_machine.h"
#include "vm/intrinsics.h"

#include "memory/memory.h"
#include "memory/memory_units.h"
#include "memory/memory_manager.h"

#include "memory/allocators/std_allocator.h"
#include "memory/allocators/segregated_allocator.h"
#include "memory/allocators/stack_allocator.h"
#include "memory/allocators/layered_allocator.h"

#include "memory/memory_meta.h"

#include "diagnostics/log.h"
#include "diagnostics/log_channels.h"
#include "diagnostics/diagnostics_meta.h"

#include "serialization/json/json.h"

#include "platform/system.h"

#include "patterns/observable.h"

#include "synergy.h"
#include "task/task.h"

syntropy::diagnostics::Context Root;

struct FooSmall
{
    char buffer[256];
};

struct FooMedium
{
    char buffer[257];
};

struct FooLarge
{
    char buffer[258];
};

void Initialize()
{
    // Initialize log

    using syntropy::diagnostics::FileLogChannel;

    SYNTROPY_UNUSED(syntropy::reflection::ClassOf<syntropy::diagnostics::FileLogChannel>());

    syntropy::diagnostics::ImportLogConfigurationFromJSON("log.cfg");

    // Initialization of the memory manager

    SYNTROPY_UNUSED(syntropy::reflection::ClassOf<syntropy::LinearSegregatedFitAllocator>());
    SYNTROPY_UNUSED(syntropy::reflection::ClassOf<syntropy::ExponentialSegregatedFitAllocator>());
    SYNTROPY_UNUSED(syntropy::reflection::ClassOf<syntropy::TwoLevelSegregatedFitAllocator>());
    SYNTROPY_UNUSED(syntropy::reflection::ClassOf<syntropy::LayeredAllocator>());

    syntropy::ImportMemoryConfigurationFromJSON("memory.cfg");
}

void ReflectionAndSerializationTest()
{
    Tester t;
    t.Do();
}

void AllocTest()
{
    void* p;
    void* q;
    void* r;

    {
        syntropy::MemoryContext ctx1("MasterAllocator2");

        p = SYNTROPY_MM_ALLOC(23_Bytes);

        {
            syntropy::MemoryContext ctx2("MasterAllocator1");

            q = SYNTROPY_MM_ALLOC(24_KiBytes);
            r = SYNTROPY_MM_ALLOC(2_MiBytes);
        }

        SYNTROPY_MM_FREE(p);        // TODO: quirk! p was allocated by "small", the actual allocator who handled the call was "master2". Incidentally also "master1" references "small", so it will take care of its deallocation. (that's correct but a little bit obscure)
        SYNTROPY_MM_FREE(q);
        SYNTROPY_MM_FREE(r);
    }
}

void MultithreadTest()
{
    using syntropy::synergy::LambdaTask;
    using namespace std::literals::chrono_literals;

    auto& s = syntropy::synergy::GetScheduler();

    s.CreateTask<LambdaTask>(
    {},
    []()
    {
        std::cout << "What's up? " << std::this_thread::get_id() << "\n";

        std::this_thread::sleep_for(2s);

        auto& s = syntropy::synergy::GetScheduler();

        s.CreateTask<LambdaTask>(
        {},
        []()
        {
            std::cout << "Dude! " << std::this_thread::get_id() << "\n";
            return nullptr;
        });

        s.CreateTask<LambdaTask>(
        {},
        []()
        {
            std::cout << "Hey! " << std::this_thread::get_id() << "\n";
            return nullptr;
        });

        return nullptr;
    });

    s.Join();

}

int main()
{

    Initialize();

    //

    ReflectionAndSerializationTest();

    AllocTest();

    MultithreadTest();

    //

    system("pause");

}

