// ProtoDW.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>

#include "diagnostics/log.h"

syntropy::diagnostics::Context Engine("Engine");
syntropy::diagnostics::Context Graphics("Graphics");

void ConcurrentTest()
{
    auto race = []() {
        for (int i = 0; i < 10000000; ++i)
        {
            SYNTROPY_LOG((Engine, Graphics), "It ", "Works");
            SYNTROPY_LOG((Engine), "This is the log #", 2);
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

    SYNTROPY_ERROR(("MyContext"), "Yis");
    ConcurrentTest();

    system("pause");

}