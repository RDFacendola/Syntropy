
/// \file unit_test_application.inl
///
/// \author Raffaele D. Facendola - April 2021.

// ===========================================================================

#include <iostream>

#include "syntropy/memory/foundation/byte_span.h"
#include "syntropy/memory/foundation/memory.h"

// ===========================================================================

namespace UnitTest
{
    /************************************************************************/
    /* UNIT TEST APPLICATION                                                */
    /************************************************************************/

    std::ostream& operator<<(std::ostream& stream, Syntropy::Immutable<Syntropy::String> string) noexcept
    {
        // #TODO @rfacendola This is horrible and assumes stream encoding to
        //                   be UTF8.

        char buffer[256];

        auto buffer_span = Syntropy::MakeByteSpan(buffer);

        Syntropy::Memory::Copy(buffer_span, string.GetCodeUnits());

        stream << buffer;

        return stream;
    }

    inline
    UnitTestApplication
    ::UnitTestApplication() noexcept
    {
        // Header.

        std::cout << "\n";
        std::cout << "Syntropy Unit Test Application\n";
        std::cout << "==============================\n\n";
        std::cout << "(version 0.0.0)\n";

        // Test runner events.

        auto BindMemberFunction = [this](auto function)
        {
            return std::bind(function, this, std::placeholders::_1, std::placeholders::_2);
        };

        test_listener_ += test_runner_.OnSuiteStarted(BindMemberFunction(&UnitTestApplication::OnSuiteStarted));
        test_listener_ += test_runner_.OnCaseStarted(BindMemberFunction(&UnitTestApplication::OnCaseStarted));
        test_listener_ += test_runner_.OnCaseFailure(BindMemberFunction(&UnitTestApplication::OnCaseFailure));
        test_listener_ += test_runner_.OnCaseFinished(BindMemberFunction(&UnitTestApplication::OnCaseFinished));
        test_listener_ += test_runner_.OnSuiteFinished(BindMemberFunction(&UnitTestApplication::OnSuiteFinished));
    }

    inline Syntropy::Int
    UnitTestApplication
    ::Run() noexcept
    {
        test_runner_.Run();

        std::cout << "\n";
        std::cout << "Final report\n";
        std::cout << "============\n";

        std::cout << "\n";
        std::cout << "Test suites tested: " << total_test_suites_ << "\n";
        std::cout << "\n";
        std::cout << "    Success:        " << passed_test_suites_ << "\n";
        std::cout << "    Failed:         " << failed_test_suites_ << "\n";

        std::cout << "\n";
        std::cout << "Test cases tested:  " << total_test_cases_ << "\n";
        std::cout << "\n";
        std::cout << "    Success:        " << passed_test_cases_ << "\n";
        std::cout << "    Failed:         " << failed_test_cases_ << "\n";

        std::cout << "\n";
        return ((failed_test_cases_ == 0) ? 0 : 1);
    }

    inline void
    UnitTestApplication
    ::OnSuiteStarted(Syntropy::Immutable<Syntropy::TestRunner> sender,
                     Syntropy::Immutable<Syntropy::OnTestRunnerSuiteStartedEventArgs> event_args)
    noexcept
    {
        test_suite_failed_ = false;

        ++total_test_suites_;
    }

    inline void
    UnitTestApplication
    ::OnCaseStarted(Syntropy::Immutable<Syntropy::TestRunner> sender,
                    Syntropy::Immutable<Syntropy::OnTestRunnerCaseStartedEventArgs> event_args)
    noexcept
    {
        test_case_failed_ = false;

        ++total_test_cases_;
    }

    inline void
    UnitTestApplication
    ::OnCaseFailure(Syntropy::Immutable<Syntropy::TestRunner> sender,
                    Syntropy::Immutable<Syntropy::OnTestRunnerCaseFailureEventArgs> event_args)
    noexcept
    {
        if (!test_suite_failed_)
        {
            test_suite_failed_ = true;

            std::cout << "\n";
            std::cout << "Test suite: " << event_args.test_suite_ << "\n";
            std::cout << "===========\n";
        }

        if (!test_case_failed_)
        {
            test_case_failed_ = true;

            std::cout << "\n";
            std::cout << " Test case: " << event_args.test_case_ << "\n";

            auto& location = event_args.location_.GetFunctionName();

            std::cout << "  >"
                      << event_args.location_.GetFileName()
                      << "\n\n";
        }

        std::cout << "  @" << event_args.location_.GetLine() << " "
                  << event_args.expression_
                  << " returned '"
                  << event_args.result_
                  << "' but '"
                  << event_args.expected_
                  << "' was expected.\n";
    }

    inline void
    UnitTestApplication
    ::OnCaseFinished(Syntropy::Immutable<Syntropy::TestRunner> sender,
                     Syntropy::Immutable<Syntropy::OnTestRunnerCaseFinishedEventArgs> event_args)
    noexcept
    {
        if (test_case_failed_)
        {
            ++failed_test_cases_;
        }
        else
        {
            ++passed_test_cases_;
        }
    }

    inline void
    UnitTestApplication
    ::OnSuiteFinished(Syntropy::Immutable<Syntropy::TestRunner> sender,
                      Syntropy::Immutable<Syntropy::OnTestRunnerSuiteFinishedEventArgs> event_args)
    noexcept
    {
        if (test_suite_failed_)
        {
            ++failed_test_suites_;
        }
        else
        {
            ++passed_test_suites_;
        }
    }

}
