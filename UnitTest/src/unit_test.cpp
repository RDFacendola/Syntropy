// ProtoDW.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>

#include "platform/system.h"
#include "memory/memory.h"
#include "memory/page_allocator.h"
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
    syntropy::PageAllocator palloc(0x200000000,         // 8GB reserved
                                   0x10000);            // 64KB pages

    int* p = reinterpret_cast<int*>(palloc.AllocatePage());
    int* q = reinterpret_cast<int*>(palloc.AllocatePage());

    *p = 1000;
    *q = 500;

    palloc.DeallocatePage(p);
    palloc.DeallocatePage(q);

    p = reinterpret_cast<int*>(palloc.AllocatePage());
    q = reinterpret_cast<int*>(palloc.AllocatePage());

    palloc.DeallocatePage(p);
    palloc.DeallocatePage(q);

    //

    SYNTROPY_LOG((Root), "Finish!");

    log_file.close();

    system("pause");

}