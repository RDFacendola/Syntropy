// ProtoDW.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>

#include "platform/system.h"
#include "memory/memory.h"
#include "diagnostics/log.h"

syntropy::diagnostics::Context Root;

struct FooStruct
{
    char buffer[1024];

    FooStruct(int v)
    {
        std::cout << "ctor: " << v << "\n";
    }

    ~FooStruct()
    {
        std::cout << "dtor\n";
    }
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

    auto mp = syntropy::platform::GetSystem().GetMemoryInfo();

    SYNTROPY_UNUSED(mp);

    auto& memory = syntropy::GetMemory();

    auto chunk = memory.ReserveVirtualRange(0xEC00000000);

    auto block = memory.AllocMemoryBlock(chunk, 0, 0x100000000);
    block = memory.AllocMemoryBlock(chunk, 0, 0x5000);                  // Memory stomp, yay!

    for (auto i = 0; i < block.GetCount(); i += 4096)
    {
        *(block.GetBaseAddress() + i) = 47;
    }

    std::cout << memory.FreeMemoryBlock(block);

    std::cout << memory.ReleaseVirtualRange(chunk);

    //

    SYNTROPY_LOG((Root), "Finish!");

    log_file.close();

    system("pause");

}