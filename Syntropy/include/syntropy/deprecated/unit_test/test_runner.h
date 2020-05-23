
/// \file test_runner.h
/// \brief This header is part of the syntropy unit test system. It contains classes used to run test suites.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <memory>
#include "syntropy/containers/vector.h"
#include <type_traits>

#include "syntropy/unit_test/test_suite.h"

#include "syntropy/patterns/observable.h"

namespace syntropy
{
    /************************************************************************/
    /* TEST RUNNER                                                          */
    /************************************************************************/

    /// \brief Singleton class used to run multiple test suites.
    /// \author Raffaele D. Facendola - December 2017
    class TestRunner
    {
    public:



        /// \brief Get the singleton instance.
        static TestRunner& GetInstance();

        /// \brief Run registered test suites.
        /// \param context Context used to filter test suites. Provide an empty context to run all the test suites available.
        /// \return Returns the result of the tests. Won't return TestResult::kSkipped under any circumstance.
        TestResult Run(const Context& context = "") const;


    private:
 };

}
