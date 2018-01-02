#include "unit_test/test_fixture.h"

namespace syntropy
{
    /************************************************************************/
    /* TEST FIXTURE                                                         */
    /************************************************************************/

    TestFixture::TestFixture(const Context& name)
        : name_(name)
    {

    }

    const Context& TestFixture::GetName() const
    {
        return name_;
    }

    const std::vector<TestCase>& TestFixture::GetTestCases() const
    {
        return test_cases_;
    }

    std::vector<TestCase>& TestFixture::GetTestCases()
    {
        return test_cases_;
    }

    TestCaseResult TestFixture::GetLastResult() const
    {
        return test_result_;
    }

    void TestFixture::SetLastResult(TestCaseResult result)
    {
        std::swap(test_result_, result);
    }

    void TestFixture::ClearLastResult()
    {
        test_result_ = TestCaseResult();
    }

    void TestFixture::BeforeAll()
    {

    }

    void TestFixture::AfterAll()
    {

    }

    void TestFixture::Before()
    {

    }

    void TestFixture::After()
    {

    }
}