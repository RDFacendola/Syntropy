#include "tests/syntropy/math/vector.h"

#include "unit_test/test_runner.h"

/************************************************************************/
/* TEST SYNTROPY MATH VECTOR                                            */
/************************************************************************/

syntropy::AutoTestSuite<TestSyntropyMathVector> suite("syntropy.math.vector");

std::vector<syntropy::TestCase> TestSyntropyMathVector::GetTestCases()
{
    return
    {
        { "constructors", &TestSyntropyMathVector::TestConstructors },
        //{ "comparisons", &Vector2TestSuite::TestComparison },
        //{ "operators", &Vector2TestSuite::TestOperators }
    };
}

void TestSyntropyMathVector::TestConstructors()
{
    {
        SYNTROPY_UNIT_TRACE(syntropy::Float3 v3);

        SYNTROPY_UNIT_ASSERT(v3.x_ == 0.0f);
        SYNTROPY_UNIT_ASSERT(v3.y_ == 0.0f);
        SYNTROPY_UNIT_ASSERT(v3.z_ == 0.0f);
    }

    {
        SYNTROPY_UNIT_TRACE(syntropy::Int3 v3(1));

        SYNTROPY_UNIT_ASSERT(v3.x_ == 1);
        SYNTROPY_UNIT_ASSERT(v3.y_ == 1);
        SYNTROPY_UNIT_ASSERT(v3.z_ == 1);
    }

    {
        SYNTROPY_UNIT_TRACE(syntropy::Float3 v3(1.0f, -2.0f, 3.0f));

        SYNTROPY_UNIT_ASSERT(v3[0] == 1.0f);
        SYNTROPY_UNIT_ASSERT(v3[1] == -2.0f);
        SYNTROPY_UNIT_ASSERT(v3[2] == 3.0f);
    }

    {
        SYNTROPY_UNIT_TRACE(syntropy::Int3 v3);
        SYNTROPY_UNIT_TRACE(v3 = 5);

        SYNTROPY_UNIT_ASSERT(v3[0] == 5);
        SYNTROPY_UNIT_ASSERT(v3[1] == 5);
        SYNTROPY_UNIT_ASSERT(v3[2] == 5);
    }

    {
        auto AcceptFloat3 = [](syntropy::Float3) {};
        auto AcceptInt3 = [](syntropy::Int3) {};

        syntropy::Float3 f3;
        syntropy::Int3 i3;

        f3 = syntropy::Float3(i3);              // Copy-assign.
        i3 = syntropy::Int3(f3);                // Copy-assign.

        //f3 = i3;                              // Error. Implicit conversion.
        //i3 = f3;                              // Error. Implicit conversion.

        AcceptFloat3(f3);                       // Copy.
        AcceptInt3(i3);                         // Copy.

        //AcceptFloat3(i3);                     // Error. Implicit conversion.
        //AcceptInt3(f3);                       // Error. Implicit conversion.

        AcceptFloat3(syntropy::Float3(i3));     // Copy. Explicit conversion.
        AcceptInt3(syntropy::Int3(f3));         // Copy. Explicit conversion.
    }

}

void TestSyntropyMathVector::TestComparison()
{
    //SYNTROPY_UNIT_ASSERT(Float3() == Float3(0.0f, 0.0f, 0.0f));
    //SYNTROPY_UNIT_ASSERT(Float2(1.0f) == Float2(1.001f, 0.999f));
    //SYNTROPY_UNIT_ASSERT(Int2(9, 10) == Int2(9, 10));
    //SYNTROPY_UNIT_ASSERT(UInt2(1, 2) != UInt2(4, 2));

    //SYNTROPY_UNIT_ASSERT(42 == Int2(42));
    //SYNTROPY_UNIT_ASSERT(Int2(42) == 42);
    //SYNTROPY_UNIT_ASSERT(42 != Int2(43));
    //SYNTROPY_UNIT_ASSERT(Int2(43) != 42);
}

void TestSyntropyMathVector::TestOperators()
{
    //{
    //    SYNTROPY_UNIT_ASSERT(-syntropy::Float2(1.0f, -7.0f) == syntropy::Float2(-1.0f, 7.0f));

    //    SYNTROPY_UNIT_ASSERT(syntropy::Float2(2.0f, 4.0f) + syntropy::Float2(7.0f, 5.0f) == syntropy::Float2(9.0f));

    //    SYNTROPY_UNIT_ASSERT(syntropy::Float2(2.0f, 4.0f) - syntropy::Float2(2.0f, 4.0f) == syntropy::Float2());

    //    //SYNTROPY_UNIT_ASSERT(syntropy::Float2(2.0f, 4.0f) * syntropy::Int2(2, 3) == syntropy::Float2(4.0f, 12.0f));

    //    SYNTROPY_UNIT_ASSERT(syntropy::Float2(2.0f, 4.0f) / syntropy::Float2(2.0f, 0.5f) == syntropy::Float2(1.0f, 8.0f));

    //    SYNTROPY_UNIT_ASSERT(syntropy::Int2(5, -7) % syntropy::Int2(2, 3) == syntropy::Int2(1, -1));
    //}

    //{
    //    SYNTROPY_UNIT_TRACE(syntropy::Float2 float2(4.0f, -2.0f));

    //    SYNTROPY_UNIT_TRACE(float2 += syntropy::Float2(4.0f));

    //    SYNTROPY_UNIT_ASSERT(float2 == syntropy::Float2(8.0f, 2.0f));

    //    SYNTROPY_UNIT_TRACE(float2 *= syntropy::Float2(-1.0f));

    //    SYNTROPY_UNIT_ASSERT(float2 == syntropy::Float2(-8.0f, -2.0f));

    //    SYNTROPY_UNIT_TRACE(float2 %= syntropy::Float2(3.0f, 0.6f));

    //    SYNTROPY_UNIT_ASSERT(float2 == syntropy::Float2(-2.0f, -0.2f));
    //}

    //{
    //    SYNTROPY_UNIT_TRACE(syntropy::Int2 int2(4, -2));

    //    SYNTROPY_UNIT_TRACE(int2 -= syntropy::Int2(20, -20));

    //    SYNTROPY_UNIT_ASSERT(int2 == syntropy::Int2(-16, 18));

    //    SYNTROPY_UNIT_TRACE(int2 /= syntropy::Int2(2));

    //    SYNTROPY_UNIT_ASSERT(int2 == syntropy::Int2(-8, 9));

    //    SYNTROPY_UNIT_TRACE(int2 %= syntropy::Int2(3, 5));

    //    SYNTROPY_UNIT_ASSERT(int2 == syntropy::Int2(-2, 4));
    //}

    //SYNTROPY_UNIT_ASSERT(2.0f + syntropy::Float2(2.0f, 4.0f) == syntropy::Float2(4.0f, 6.0f));

    //SYNTROPY_UNIT_ASSERT(syntropy::Float2(2.0f, 4.0f) - 2.0f == syntropy::Float2(7.0f, 9.0f));

    //SYNTROPY_UNIT_ASSERT(syntropy::Float2(6.0f, 4.2f) * -1.0f == -syntropy::Float2(6.0f, 4.2f));

    //SYNTROPY_UNIT_ASSERT(1.0f / syntropy::Float2(4.0) == syntropy::Float2(0.25f, 0.25f));
}
