
/// \file auto_test_suiye.h
/// \brief This header is part of the Syntropy unit test module. It contains classes used to define self-registering test suites.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <tuple>

#include "syntropy/core/types.h"
#include "syntropy/core/label.h"
#include "syntropy/core/smart_pointers.h"

#include "syntropy/unit_test/test_suite.h"

namespace syntropy
{
    /************************************************************************/
    /* AUTO TEST SUITE                                                      */
    /************************************************************************/

    /// \brief Represents the interface for self-registering test suites for a test fixture.
    /// \author Raffaele D. Facendola - May 2020.
    class AutoTestSuite
    {
    public:

        /// \ brief Apply a function to each self-registering test suite.
        template <typename TFunction>
        static void ForEach(TFunction&& function);

        /// \brief Create a new self-registering test suite.
        AutoTestSuite();

        /// \brief No copy-constructor.
        AutoTestSuite(const AutoTestSuite&) = delete;

        /// \brief No move-constructor.
        AutoTestSuite(AutoTestSuite&&) = delete;

        /// \brief No copy-assignment.
        AutoTestSuite& operator=(const AutoTestSuite&) = delete;

        /// \brief No move-assignment.
        AutoTestSuite& operator=(AutoTestSuite&&) = delete;

        /// \brief Default destructor.
        virtual ~AutoTestSuite() = default;

        /// \brief Run the test suite.
        virtual const TestSuite& GetTestSuite() const = 0;

    private:

        /// \brief Get the first element in a linked list to which every other self-registering test-suite is linked to.
        static ObserverPtr<const AutoTestSuite>& GetLinkedList();

        /// \brief Link this test-suite to the others and return the next test-suite after this one.
        ObserverPtr<const AutoTestSuite> LinkBefore();

        /// \brief Next auto test suite.
        ObserverPtr<const AutoTestSuite> next_test_suite_{ nullptr };

    };

    /************************************************************************/
    /* AUTO TEST SUITE <TTEST FIXTURE, TFIXTURE ARGUMENTS>                  */
    /************************************************************************/

    /// \brief Represents a self-registering test suite for a test fixture.
    /// \tparam TFixtureArguments Type of the arguments used to construct the test fixture.
    /// \author Raffaele D. Facendola - May 2020.
    template <typename TTestFixture>
    class AutoTestSuiteT : public AutoTestSuite
    {
    public:

        /// \brief Create a new self-registering test suite.
        template <typename... TFixtureArguments>
        AutoTestSuiteT(const Context& name, TFixtureArguments&&... fixture_arguments);

        /// \brief No copy-constructor.
        AutoTestSuiteT(const AutoTestSuiteT&) = delete;

        /// \brief No move-constructor.
        AutoTestSuiteT(AutoTestSuiteT&&) = delete;

        /// \brief No copy-assignment.
        AutoTestSuiteT& operator=(const AutoTestSuiteT&) = delete;

        /// \brief No move-assignment.
        AutoTestSuiteT& operator=(AutoTestSuiteT&&) = delete;

        /// \brief Default virtual destructor.
        virtual ~AutoTestSuiteT() = default;

    private:

        virtual const TestSuite& GetTestSuite() const override;

        /// \brief Underlying test_suite.
        TestSuiteT<TTestFixture> test_suite_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a self-registering test suite by deducing the fixture type from arguments.
    /// \usage const auto auto_test_suite = MakeAutoTestSuite(name, arg0, arg1, ...).
    template <typename TTestFixture, typename... TFixtureArguments>
    AutoTestSuiteT<TTestFixture> MakeAutoTestSuite(const Context& name, TFixtureArguments&&... arguments);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // AutoTestSuite.

    template <typename TFunction>
    inline void AutoTestSuite::ForEach(TFunction&& function)
    {
        // Skip the very first test suite as it is the sentinel to which every other test suite is linked to.

        for (auto auto_test_suite = GetLinkedList(); auto_test_suite; auto_test_suite = auto_test_suite->next_test_suite_)
        {
            function(AsConst(*auto_test_suite));
        }
    }

    inline AutoTestSuite::AutoTestSuite()
        : next_test_suite_(LinkBefore())
    {

    }

    inline ObserverPtr<const AutoTestSuite>& AutoTestSuite::GetLinkedList()
    {
        static auto linked_list = ObserverPtr<const AutoTestSuite>{ nullptr };

        return linked_list;
    }

    inline ObserverPtr<const AutoTestSuite> AutoTestSuite::LinkBefore()
    {
        auto& linked_list = GetLinkedList();

        auto next_test_suite = linked_list;

        linked_list = this;

        return next_test_suite;
    }

    // AutoTestSuiteT<TTestFixture>.

    template <typename TTestFixture>
    template <typename... TFixtureArguments>
    inline AutoTestSuiteT<TTestFixture>::AutoTestSuiteT(const Context& name, TFixtureArguments&&... fixture_arguments)
        : test_suite_(name, std::forward<TFixtureArguments>(fixture_arguments)...)
    {

    }

    template <typename TTestFixture>
    inline const TestSuite& AutoTestSuiteT<TTestFixture>::GetTestSuite() const
    {
        return test_suite_;
    }

    // Non-member functions.

    template <typename TTestFixture, typename... TFixtureArguments>
    inline AutoTestSuiteT<TTestFixture> MakeAutoTestSuite(const Context& name, TFixtureArguments&&... arguments)
    {
        return { name, std::forward<TFixtureArguments>(arguments)... };
    }

}
