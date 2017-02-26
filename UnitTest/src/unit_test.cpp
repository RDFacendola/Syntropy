// ProtoDW.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>

#include "vm/virtual_machine.h"
#include "vm/intrinsics.h"

#include "memory/std_allocator.h"

#include "diagnostics/log.h"

syntropy::diagnostics::Context Root;

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

    auto p = SYNTROPY_NEW(syntropy::g_std_allocator) int64_t;

    SYNTROPY_DELETE(syntropy::g_std_allocator, p);

    //
    {
        syntropy::syntax::VirtualMachine vm(4096, syntropy::g_std_allocator);

        while (vm.IsRunning())
        {
            vm.ExecuteNext();
        }
    }

    //

    log_file.close();

    system("pause");

}