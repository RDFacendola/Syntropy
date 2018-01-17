
/// \file vector.h
///
/// \author Raffaele D. Facendola - 2018

#include "unit_test/test_fixture.h"
#include "unit_test/test_case.h"

#include <vector>

#include "math/vector.h"

/************************************************************************/
/* TEST SYNTROPY MATH VECTOR                                            */
/************************************************************************/

/// \brief Test suite used to test syntropy::VectorN
class TestSyntropyMathVector : public syntropy::TestFixture
{
public:

    static std::vector<syntropy::TestCase> GetTestCases();

    /// \brief Test vector constructors.
    void TestConstructors();

    /// \brief Test vector comparison.
    void TestComparison();

    /// \brief Test vector operators.
    void TestOperators();

};

