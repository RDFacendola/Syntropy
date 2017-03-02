// ProtoDW.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>

#include "vm/virtual_machine.h"
#include "vm/intrinsics.h"

#include "memory/std_allocator.h"
#include "memory/segregated_allocator.h"

#include "diagnostics/log.h"

syntropy::diagnostics::Context Root;

struct FooSmall
{
    char buffer[154];
};

struct FooMedium
{
    char buffer[356];
};

struct FooLarge
{
    char buffer[623];
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
        syntropy::LinearSegregatedFitAllocator lsfa("linear", 0x100000, 256, 10);

        auto p = SYNTROPY_NEW(lsfa) FooSmall();
        auto q = SYNTROPY_NEW(lsfa) FooMedium();
        auto r = SYNTROPY_NEW(lsfa) FooLarge();

        SYNTROPY_DELETE(lsfa, p);

        p = SYNTROPY_NEW(lsfa) FooSmall();

        SYNTROPY_DELETE(lsfa, p);
        SYNTROPY_DELETE(lsfa, q);

        q = SYNTROPY_NEW(lsfa) FooMedium();

        SYNTROPY_DELETE(lsfa, q);
        SYNTROPY_DELETE(lsfa, r);

        r = SYNTROPY_NEW(lsfa) FooLarge();

        SYNTROPY_DELETE(lsfa, r);

    }

    //

    log_file.close();

    system("pause");

}