
/// \file test_context.inl
///
/// \author Raffaele D. Facendola - 2018.

// ===========================================================================

#pragma once

#include "syntropy/core/strings/string.h"
#include "syntropy/core/strings/to_string.h"

#include "syntropy/language/templates/concepts.h"
#include "syntropy/language/preprocessor/macro.h"

// ===========================================================================

namespace Syntropy::UnitTest
{
    /************************************************************************/
    /* UNIT TEST MACROS                                                     */
    /************************************************************************/

    #undef SYNTROPY_UNIT_EQUAL
    #define SYNTROPY_UNIT_EQUAL(expression, expected)                         \
        if (auto&& result = (expression); result != expected)                 \
        {                                                                     \
            Syntropy::UnitTest::ReportFailure(SYNTROPY_HERE,                  \
                                              SYNTROPY_CAT(u8, #expression),  \
                                              result,                         \
                                              expected );                     \
        }                                                                     \
        else                                                                  \
        {                                                                     \
            Syntropy::UnitTest::ReportSuccess(SYNTROPY_HERE,                  \
                                              SYNTROPY_CAT(u8, #expression)); \
        }

    #undef SYNTROPY_UNIT_SAME
    #define SYNTROPY_UNIT_SAME(type, expected)                                \
        if(!Syntropy::Templates::IsSame<SYNTROPY_TYPENAME(type),              \
                                       SYNTROPY_TYPENAME(expected)>)          \
        {                                                                     \
            Syntropy::UnitTest::ReportFailure(SYNTROPY_HERE,                  \
                                              SYNTROPY_CAT(u8, #type),        \
                                              SYNTROPY_CAT(u8, #type),        \
                                              SYNTROPY_CAT(u8, #expected));   \
        }                                                                     \
        else                                                                  \
        {                                                                     \
            Syntropy::UnitTest::ReportSuccess(SYNTROPY_HERE,                  \
                                              SYNTROPY_CAT(u8, #type) );      \
        }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    template <typename TExpression>
    void
    ReportSuccess(Immutable<SourceLocation> location,
                  Forwarding<TExpression> expression) noexcept
    {
        TestContext::context_->ReportSuccess(location,
                                             Strings::ToString(expression));
    }

    template <typename TExpression, typename TResult, typename TExpected>
    void
    ReportFailure(Immutable<SourceLocation> location,
                  Forwarding<TExpression> expression,
                  Forwarding<TResult> result,
                  Forwarding<TExpected> expected) noexcept
    {
        TestContext::context_->ReportFailure(location,
                                             Strings::ToString(expression),
                                             Strings::ToString(result),
                                             Strings::ToString(expected));
    }

    /************************************************************************/
    /* TEST CONTEXT                                                         */
    /************************************************************************/

    inline
    TestContext
    ::TestContext() noexcept
        : previous_context_(context_)
    {
        context_ = this;
    }

    inline Mutable<TestContext>
    TestContext
    ::operator=(Movable<TestContext> rhs) noexcept
    {
        context_ = Move(rhs.context_);
        previous_context_ = Move(rhs.previous_context_);
        success_event_ = Move(rhs.success_event_);
        failure_event_ = Move(rhs.failure_event_);

        return *this;
    }

    inline
    TestContext
    ::~TestContext() noexcept
    {
        context_ = previous_context_;
    }

    template <typename TDelegate>
    inline Listener
    TestContext
    ::OnSuccess(Forwarding<TDelegate> delegate) const noexcept
    {
        return success_event_.Subscribe(Forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener
    TestContext
    ::OnFailure(Forwarding<TDelegate> delegate) const noexcept
    {
        return failure_event_.Subscribe(Forward<TDelegate>(delegate));
    }

    inline void
    TestContext
    ::ReportSuccess(Immutable<SourceLocation> location,
                    Immutable<String> expression)
    const noexcept
    {
        success_event_.Notify(*this, { location, expression });
    }

    inline void
    TestContext
    ::ReportFailure(Immutable<SourceLocation> location,
                    Immutable<String> expression,
                    Immutable<String> result,
                    Immutable<String> expected)
    const noexcept
    {
        failure_event_.Notify(*this, { location,
                                       expression,
                                       result,
                                       expected });
    }

}
