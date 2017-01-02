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
    syntropy::SegregatedAllocator allocator(0x20000000,         /// 512MB capacity
                                            0x4000);            /// 16KB pages

    auto t0 = std::chrono::high_resolution_clock::now();

    struct Foo
    {
        int64_t a;          // 8
        char padding[248];
    };

    for (size_t i = 0; i < 0x10000; ++i)
    {
        auto b = reinterpret_cast<Foo*>(allocator.Allocate(sizeof(Foo), 0));
         b->a = i;
    }

    auto t1 = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < 0x10000; ++i)
    {
        auto b = new Foo;
         b->a = i;
    }

    auto t2 = std::chrono::high_resolution_clock::now();

    auto t10 = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0);
    auto t21 = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

    std::cout << "Syntropy took: " << t10.count() * 1000.0 << " ms\n";
    std::cout << "New took: " << t21.count() * 1000.0 << " ms\n";
    std::cout << "Speedup: " << t21.count() / t10.count() << "\n";

    //

    SYNTROPY_LOG((Root), "Finish!");

    log_file.close();

    system("pause");

}