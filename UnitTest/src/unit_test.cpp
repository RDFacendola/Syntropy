// ProtoDW.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>

#include "platform/system.h"
#include "memory/memory.h"
#include "memory/segregated_allocator.h"
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
    syntropy::SegregatedPoolAllocator allocator(0x20000000,         /// 512MB capacity
                                                0x4000);            /// 16KB pages

    size_t count = 0x100000;

    std::vector<bool> allocate;
    allocate.reserve(count);

    while (count-- > 0)
    {
        allocate.push_back(rand() % 2 == 0);
    }

    struct Foo
    {
        int64_t a;          // 8
        char padding[1];
    };

    size_t elements;

    auto t0 = std::chrono::high_resolution_clock::now();

    {
        size_t i;
        std::vector<Foo*> v;
        v.reserve(0x100000);

        for(auto choice : allocate)
        {
            if (choice)
            {
                v.push_back(reinterpret_cast<Foo*>(allocator.Allocate(sizeof(Foo))));
                v.back()->a = v.size();
            }
            else if (v.size() > 0)
            {
                //i = rand() % v.size();
                i = v.size() - 1;

                allocator.Free(v[i]);
                v.erase(v.begin() + i);
            }
        }
        elements = v.size();
    }
    
    auto t1 = std::chrono::high_resolution_clock::now();

    {
        size_t i;
        std::vector<Foo*> v;
        v.reserve(0x100000);

        for (auto choice : allocate)
        {
            if (choice)
            {
                v.push_back(new Foo());
                v.back()->a = v.size();
            }
            else if (v.size() > 0)
            {
                //i = rand() % v.size();
                i = v.size() - 1;

                delete v[i];
                v.erase(v.begin() + i);
            }
        }
    }

    auto t2 = std::chrono::high_resolution_clock::now();

    auto t10 = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0);
    auto t21 = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

    std::cout << "Syntropy took: " << t10.count() * 1000.0 << " ms using " << allocator.GetSize() << " bytes of memory for " << elements << " elements\n";
    std::cout << "New took: " << t21.count() * 1000.0 << " ms\n";
    std::cout << "Speedup: " << t21.count() / t10.count() << "\n";

    //

    SYNTROPY_LOG((Root), "Finish!");

    log_file.close();

    system("pause");

}