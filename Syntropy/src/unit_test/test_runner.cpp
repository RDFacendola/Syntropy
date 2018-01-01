#include "unit_test/test_runner.h"

namespace syntropy
{
    /************************************************************************/
    /* TEST RUNNER                                                          */
    /************************************************************************/

    TestRunner& TestRunner::GetInstance()
    {
        static TestRunner instance;
        return instance;
    }

    const std::vector<TestSuite>& TestRunner::GetTestSuites() const
    {
        return test_suites_;
    }

    void TestRunner::Run(const Context& context)
    {
        for (auto&& test_suite : test_suites_)
        {
            test_suite.Run(context);
        }
    }

}