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

    virtual const syntropy::HashedString& GetName() const override 
    {
        static const syntropy::HashedString kName("TestAllocator");
        return kName;
    }

};

struct TestStruct
{

    static void __Public(syntropy::syntax::VMExecutionContext& context)
    {
        auto instance = context.GetNextArgument<TestStruct*>();

        (*instance)->Public();
    }

    static void __Protected(syntropy::syntax::VMExecutionContext& context)
    {
        auto instance = context.GetNextArgument<TestStruct*>();

        (*instance)->Protected();
    }

public:

    void Public() {}

protected:

    void Protected() {}

private:

    void Private() {}

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

    {
        syntropy::MemoryBuffer buff1(10, ta);
        syntropy::MemoryBuffer buff2(2, ta);

        auto value = *reinterpret_cast<int64_t*>(*buff1);

        *reinterpret_cast<int64_t*>(*buff1) = 56;

        buff2 = buff1;

        value = *reinterpret_cast<int64_t*>(*buff2);

        *reinterpret_cast<int64_t*>(*buff2) = 654;

        buff1 = std::move(buff2);

        value = *reinterpret_cast<int64_t*>(*buff1);

    }

    syntropy::syntax::VirtualMachine vm(4096, ta);

    while (vm.IsRunning())
    {
        vm.ExecuteNext();
    }

    //

    log_file.close();

    system("pause");

}