// ProtoDW.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>

#include "vm/virtual_machine.h"
#include "vm/intrinsics.h"

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

    //

    syntropy::syntax::VirtualMachine vm(4096, 4096);

    while (vm.IsRunning())
    {
        vm.ExecuteNext();
    }

    //

    log_file.close();

    system("pause");

}