// ProtoDW.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>

#include "platform/system.h"
#include "memory/memory.h"
#include "memory/allocation.h"
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

    int a = 100;
    //int b = 10;

    int* p;

    syntropy::MemoryAddressPool pool(0x1000000);

    for (auto i = 0; i < 200000; ++i)
    {
        pool.Push(&a);
    }

    for (auto i = 0; i < 200000; ++i)
    {
        p = reinterpret_cast<int*>(pool.Pop());
        if (p != &a)
        {
            SYNTROPY_BREAK;
        }
        p = nullptr;
    }

    //

    SYNTROPY_LOG((Root), "Finish!");

    log_file.close();

    system("pause");

}