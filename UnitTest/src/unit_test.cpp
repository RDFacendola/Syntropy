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
        syntropy::TwoLevelSegregatedFitAllocator tlsfa("linear", 0x800000, 2);

        auto p = SYNTROPY_NEW(tlsfa) FooSmall();
        auto q = SYNTROPY_NEW(tlsfa) FooMedium();
        auto r = SYNTROPY_NEW(tlsfa) FooLarge();

        SYNTROPY_DELETE(tlsfa, p);

        p = SYNTROPY_NEW(tlsfa) FooSmall();

        SYNTROPY_DELETE(tlsfa, p);
        SYNTROPY_DELETE(tlsfa, q);

        q = SYNTROPY_NEW(tlsfa) FooMedium();

        SYNTROPY_DELETE(tlsfa, q);
        SYNTROPY_DELETE(tlsfa, r);

        r = SYNTROPY_NEW(tlsfa) FooLarge();

        SYNTROPY_DELETE(tlsfa, r);

    }

    //

    log_file.close();

    system("pause");

}