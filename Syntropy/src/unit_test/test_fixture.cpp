#include "unit_test/test_fixture.h"

namespace syntropy
{
    /************************************************************************/
    /* TEST FIXTURE                                                         */
    /************************************************************************/

    const std::vector<TestCase>& TestFixture::GetTestCases() const
    {
        return test_cases_;
    }

    Observable<TestFixture&, const TestFixture::OnResultEventArgs&>& TestFixture::OnResult()
    {
        return on_result_;
    }

    void TestFixture::NotifyResult(TestCaseResult result)
    {
        on_result_.Notify(*this, OnResultEventArgs{ std::move(result) });
    }

    void TestFixture::Before()
    {

    }

    void TestFixture::After()
    {

    }

}