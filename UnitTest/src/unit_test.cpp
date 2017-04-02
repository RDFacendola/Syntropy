
// ProtoDW.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <random>

#include "vm/virtual_machine.h"
#include "vm/intrinsics.h"

#include "memory/memory.h"
#include "memory/memory_units.h"

#include "memory/std_allocator.h"
#include "memory/segregated_allocator.h"
#include "memory/stack_allocator.h"
#include "memory/master_allocator.h"

#include "diagnostics/log.h"

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

int main()
{

    // Initialize log

    std::ofstream log_file;

    log_file.open("log.txt");

    auto& log_manager = syntropy::diagnostics::LogManager::GetInstance();

    auto format = "[{date} {time}] [{severity}] [thread: {thread}] [{context}]: {message}";
    auto stream = log_manager.CreateStream<syntropy::diagnostics::StreamLog>(log_file, format);
    
    stream->BindContext({ Root });
    stream->SetVerbosity(syntropy::diagnostics::Severity::kInformative);

    //

    {
        
        syntropy::LinearSegregatedFitAllocator small("small", 512_MiBytes, 8_Bytes, 32, 16_KiBytes);
        syntropy::ExponentialSegregatedFitAllocator large("large", 160_GiBytes, 64_KiBytes, 10);
        syntropy::MasterAllocator::Configuration cfg{ 256_Bytes, 64_KiBytes, 32_MiBytes, small, large, 8_GiBytes, 5 };

        syntropy::MasterAllocator master1("master1", cfg);
        syntropy::MasterAllocator master2("master2", cfg);

        // allocate stuffs
    }

    //

    log_file.close();

    system("pause");

}