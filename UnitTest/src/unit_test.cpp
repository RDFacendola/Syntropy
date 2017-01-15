// ProtoDW.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>

#include "platform/system.h"
#include "memory/memory.h"
#include "memory/segregated_allocator.h"
#include "memory/linear_allocator.h"
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

    struct Foo
    {
        //int64_t a;          // 8
        char padding[240];
    };

    auto t0 = std::chrono::high_resolution_clock::now();

    {
        size_t c = 100000;

        std::vector<Foo*> v;
        v.reserve(0x10000);

        //syntropy::ClusteredPoolAllocator pall(0x2800000000,     // 160 GB capacity
        //                                      0x10000,          // 64 KB pages
        //                                      10);              // 10th order: up to 32 MB allocations


        syntropy::SegregatedPoolAllocator pall(0x200000000,         // 512 MB capacity
                                               0x4000);              // 16 KB pages

        while (c-- > 0)
        {
            if (rand() % 2 == 0 || v.size() == 0)
            {
                v.push_back(reinterpret_cast<Foo*>(pall.Allocate(sizeof(Foo), 24)));
            }
            else
            {
                auto i = rand() % v.size();
                pall.Free(v[i]);
                v.erase(v.begin() + i);
            }
        }

        std::cout << "Elements: " << v.size() << " (" << pall.GetSize() << " bytes)\n";
    }

    auto t1 = std::chrono::high_resolution_clock::now();

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