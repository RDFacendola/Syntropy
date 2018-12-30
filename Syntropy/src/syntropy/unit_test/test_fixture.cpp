#include "syntropy/unit_test/test_fixture.h"

namespace syntropy
{
    /************************************************************************/
    /* TEST FIXTURE                                                         */
    /************************************************************************/

    Observable<TestFixture&, const TestFixture::OnResultNotifiedEventArgs&>& TestFixture::OnResultNotified()
    {
        return on_result_notified_;
    }

    Observable<TestFixture&, const TestFixture::OnMessageNotifiedEventArgs&>& TestFixture::OnMessageNotified()
    {
        return on_message_notified_;
    }

    void TestFixture::NotifyResult(const OnResultNotifiedEventArgs& result)
    {
        on_result_notified_.Notify(*this, result);
    }

    void TestFixture::Before()
    {

    }

    void TestFixture::After()
    {

    }

}