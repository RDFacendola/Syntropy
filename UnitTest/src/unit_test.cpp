// ProtoDW.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>

#include "diagnostics/log.h"

syntropy::diagnostics::Context Root;
syntropy::diagnostics::Context Engine("Engine|CtxA");
syntropy::diagnostics::Context Graphics("Engine|CtxB");

namespace test
{
    void MyFunction()
    {
        SYNTROPY_WARNING((Engine), "This is the log #", 2);
        SYNTROPY_LOG((Graphics, Engine), "It ", "Works");
    }
}

void ConcurrentTest()
{
    auto race = []() {
        for (int i = 0; i < 100; ++i)
        {
            test::MyFunction();
        }
    };

    auto t1 = std::chrono::high_resolution_clock::now();

    std::thread th1(race);
    std::thread th2(race);
    std::thread th3(race);
    std::thread th4(race);

    th1.join();
    th2.join();
    th3.join();
    th4.join();

    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

    std::cout << "It took me " << time_span.count() << " seconds.";
    std::cout << std::endl;

}

int main()
{

    std::ofstream log_file;

    log_file.open("log.txt");

    auto& log_manager = syntropy::diagnostics::LogManager::GetInstance();

    //auto format = "{date} {time} [thread: {thread}][{context}][{severity}]: {message}\nAt {trace}";
    auto format = "[{context}]: {message}";

    auto stream = log_manager.CreateStream<syntropy::diagnostics::StreamLogger>(std::cout , format);
    
    stream->BindContext({ Graphics });
    stream->SetVerbosity(syntropy::diagnostics::Severity::kInformative);

    ConcurrentTest();

    log_file.close();

    system("pause");

}