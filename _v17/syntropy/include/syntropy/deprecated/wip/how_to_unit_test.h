
class MyUnitTest
{
public:

    // Optional method called before each test case.
    void Before();

    // Optional method called after each test cases.
    void After();

    // Test cases.
    void TestCase0()
    {
        SYNTROPY_UNIT_ASSERT((1 + 1) == 2);
    }

    ...

    void TestCaseN()
    {
        SYNTROPY_UNIT_ASSERT((1 + 1) == 2);
    }
};

// Declare the test suite for MyUnitTest.
auto MyUnitTestSuite = Syntropy::MakeAutoTestSuite<MyUnitTest>("MyUnitTest.suite");

// Declare individual test cases for MyUnitTest.
const auto MyUnitTestSuiteTestCase0 = Syntropy::MakeAutoTestCase("TestCase0", &MyUnitTest::TestCase0);
const auto MyUnitTestSuiteTestCaseN = Syntropy::MakeAutoTestCase("TestCaseN", &MyUnitTest::TestCaseN);
