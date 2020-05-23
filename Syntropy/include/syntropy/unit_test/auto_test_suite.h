
/// \file auto_test_suiye.h
/// \brief This header is part of the Syntropy unit test module. It contains classes used to define self-registering test suites.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <tuple>

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

        /// \brief Get the test suite name
        const Label& GetName() const;

        /// \brief Create a new test suite.
        virtual UniquePtr<TestSuite> CreateTestSuite() const;

    protected:

        /// \brief Create a new self-registering test suite.
        /// Protected to prevent direct instantiation.
        AutoTestSuite(const Label& name);

    private:

        /// \brief Get the first element in a linked list to which every other self-registering test-suite is linked to.
        static AutoTestSuite& GetLinkedList();

        /// \brief Create an empty test suite.
        AutoTestSuite() = default;

        /// \brief Link this test-suite to the others and return the next test-suite after this one.
        ObserverPtr<const AutoTestSuite> LinkBefore();

        /// \brief Test suite name.
        Label name_;

        /// \brief Next auto test suite.
        ObserverPtr<const AutoTestSuite> next_test_suite_{ nullptr };

    };

    /************************************************************************/
    /* AUTO TEST SUITE <TTEST FIXTURE, TFIXTURE ARGUMENTS>                  */
    /************************************************************************/

    /// \brief Represents a self-registering test suite for a test fixture.
    /// \tparam TFixtureArguments Type of the arguments used to construct the test fixture.
    /// \author Raffaele D. Facendola - May 2020.
    template <typename TTestFixture, typename... TFixtureArguments>
    class AutoTestSuiteT : public AutoTestSuite
    {
    public:

        /// \brief Create a new self-registering test suite.
        template <typename... UFixtureArguments>
        AutoTestSuiteT(const Label& name, UFixtureArguments&&... fixture_arguments);

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

        /// \brief Create a new test suite.
        virtual UniquePtr<TestSuite> CreateTestSuite() const override;

        /// \brief Construction arguments for the test suite.
        std::tuple<TFixtureArguments...> fixture_arguments_;
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create an self-registering test suite by deducing the fixture type from arguments.
    /// \usage const auto auto_test_suite = MakeAutoTestSuite(name, arg0, arg1, ...).
    template <typename TTestFixture, typename... TFixtureArguments>
    AutoTestSuiteT<TTestFixture, TFixtureArguments...> MakeAutoTestSuite(const Label& name, TFixtureArguments&&... arguments);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // AutoTestSuite.

    template <typename TFunction>
    inline void AutoTestSuite::ForEach(TFunction&& function)
    {
        // Skip the very first test suite as it is the sentinel to which every other test suite is linked to.

        for (auto auto_test_suite = GetLinkedList().next_test_suite_; auto_test_suite; auto_test_suite = auto_test_suite->next_test_suite_)
        {
            function(*auto_test_suite);
        }
    }

    inline AutoTestSuite::AutoTestSuite(const Label& name)
        : name_(name)
        , next_test_suite_(LinkBefore())
    {

    }

    const Label& AutoTestSuite::GetName() const
    {
        return name_;
    }

    inline AutoTestSuite& AutoTestSuite::GetLinkedList()
    {
        static auto linked_list = AutoTestSuite{};

        return linked_list;
    }

    inline ObserverPtr<const AutoTestSuite> AutoTestSuite::LinkBefore()
    {
        auto& linked_list = GetLinkedList();

        auto next = linked_list.next_test_suite_;

        linked_list.next_test_suite_ = this;

        return next;
    }

    inline UniquePtr<TestSuite> AutoTestSuite::CreateTestSuite() const
    {
        return nullptr;
    }

    // AutoTestSuiteT<TTestFixture>.

    template <typename TTestFixture, typename... TFixtureArguments>
    template <typename... UFixtureArguments>
    inline AutoTestSuiteT<TTestFixture, TFixtureArguments...>::AutoTestSuiteT(const Label& name, UFixtureArguments&&... fixture_arguments)
        : AutoTestSuite(name)
        , fixture_arguments_(fixture_arguments...)
    {

    }

    template <typename TTestFixture, typename... TFixtureArguments>
    inline UniquePtr<TestSuite> AutoTestSuiteT<TTestFixture, TFixtureArguments...>::CreateTestSuite() const
    {
        auto make_unique_test_suite = [this](const TFixtureArguments&... arguments)
        {
            return MakeUnique<TestSuiteT<TTestFixture, TFixtureArguments...>>(GetName(), arguments...);
        };

        return std::apply(make_unique_test_suite, fixture_arguments_);
    }

    // Non-member functions.

    template <typename TTestFixture, typename... TFixtureArguments>
    inline AutoTestSuiteT<TTestFixture, TFixtureArguments...> MakeAutoTestSuite(const Label& name, TFixtureArguments&&... arguments)
    {
        return AutoTestSuiteT<TTestFixture, TFixtureArguments...>(name, std::forward<TFixtureArguments>(arguments)...);
    }

}
