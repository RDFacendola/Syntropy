#include "unit_test/test_case.h"

#include <exception>

namespace syntropy
{
    /************************************************************************/
    /* TEST CASE                                                            */
    /************************************************************************/

    const HashedString& TestCase::GetName() const
    {
        return name_;
    }

    void TestCase::Run() const
    {
        test_case_();
    }
}