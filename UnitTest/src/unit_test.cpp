// ProtoDW.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>

#include "vm/virtual_machine.h"
#include "vm/intrinsics.h"

#include "diagnostics/log.h"

syntropy::diagnostics::Context Root;

class TestAllocator : public syntropy::Allocator
{
public:

    TestAllocator()
        : syntropy::Allocator("TestAllocator")
    {

    }

    virtual void* Allocate(size_t size) override
    {
        return std::malloc(size);
    }

    virtual void* Allocate(size_t size, size_t alignment) override
    {
        return std::malloc(size + alignment);
    }

    virtual void Free(void* block) override
    {
        std::free(block);
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
    TestAllocator ta;

    syntropy::syntax::VirtualMachine vm(4096, ta);

    while (vm.IsRunning())
    {
        vm.ExecuteNext();
    }

    //

    log_file.close();

    system("pause");

}