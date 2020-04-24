
/// \file test_case.h
/// \brief This header is part of the syntropy unit test system. It contains classes used to handle test cases.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <string>
#include <functional>

#include "syntropy/unit_test/test.h"
#include "syntropy/unit_test/test_fixture.h"

#include "syntropy/types/label.h"

#include "syntropy/diagnostics/diagnostics.h"
#include "syntropy/diagnostics/assert.h"

#include "syntropy/patterns/observable.h"

namespace syntropy
{
    /************************************************************************/
    /* TEST CASE                                                            */
    /************************************************************************/

    /// \brief Represents a single test case.
    /// \author Raffaele D. Facendola - December 2017
    class TestCase
    {
    public:

        /// \brief Arguments of the event called whenever a test case starts.
        struct OnStartedEventArgs
        {

        };

        /// \brief Arguments of the event called whenever a running test case finishes.
        struct OnFinishedEventArgs
        {
            TestResult result_;                                 ///< \brief Overall test result.
        };

        /// \brief Arguments of the event called whenever a test case result is notified.
        struct OnResultNotifiedEventArgs
        {
            TestResult result_;                                 ///< \brief Result.

            std::string message_;                               ///< \brief Result message.

            diagnostics::StackTraceElement location_;           ///< \brief Code that issued the result.
        };

        /// \brief Arguments of the event called whenever a test case notifies a message.
        struct OnMessageNotifiedEventArgs
        {
            std::string message_;                               ///< \brief Notified message.
        };

        /// \brief Create a new test case.
        /// \param name Name of the test case.
        /// \param test_case Function bound to this test case.
        template <typename TTestFixture>
        TestCase(Label name, void (TTestFixture::* test_case)())
            : name_(std::move(name))
        {
            test_case_ = [this, test_case](TestFixture& fixture)
            {
                auto test_fixture = dynamic_cast<TTestFixture*>(std::addressof(fixture));

                SYNTROPY_ASSERT(test_fixture);

                (test_fixture->*test_case)();
            };
        }

        /// \brief Get the test case name.
        /// \return Return the test case name.
        const Label& GetName() const;

        /// \brief Run the test case.
        /// \param fixture Fixture the test case will be run with.
        TestResult Run(TestFixture& fixture) const;

        /// \brief Observable event called whenever this instance starts running.
        const Observable<const TestCase&, const OnStartedEventArgs&>& OnStarted() const;

        /// \brief Observable event called whenever a this instance finished running.
        const Observable<const TestCase&, const OnFinishedEventArgs&>& OnFinished() const;

        /// \brief Observable event called whenever a new test case result is notified.
        const Observable<const TestCase&, const OnResultNotifiedEventArgs&>& OnResultNotified() const;

        /// \brief Observable event called whenever a message is notified.
        const Observable<const TestCase&, const OnMessageNotifiedEventArgs&>& OnMessageNotified() const;

    private:

        Label name_;                                                                 ///< \brief Name of the test case.

        std::function<void(TestFixture& fixture)> test_case_;                               ///< \brief Thunk used to run the actual test case function on a concrete fixture.

        Event<const TestCase&, const OnStartedEventArgs&> on_started_;                      ///< \brief Event raised whenever this instance starts running.

        Event<const TestCase&, const OnFinishedEventArgs&> on_finished_;                    ///< \brief Event raised whenever this instance finished running.

        Event<const TestCase&, const OnResultNotifiedEventArgs&> on_result_notified_;       ///< \brief Event raised whenever a new result is notified.

        Event<const TestCase&, const OnMessageNotifiedEventArgs&> on_message_notified_;     ///< \brief Event triggered whenever a message is notified.
    };
}
