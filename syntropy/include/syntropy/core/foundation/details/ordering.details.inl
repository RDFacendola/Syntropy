
/// \file ordering.details.inl
///
/// \author Raffaele D. Facendola - March 2021

#pragma once

// ===========================================================================

namespace Syntropy::Details
{
    /************************************************************************/
    /* CONVERSIONS                                                          */
    /************************************************************************/

    [[nodiscard]] constexpr ComparisonResult
    ToComparisonResult(Immutable<std::strong_ordering> rhs) noexcept
    {
        if (rhs == std::strong_ordering::equivalent)
        {
            return Details::ComparisonResult::kEquivalent;
        }

        if (rhs == std::strong_ordering::greater)
        {
            return Details::ComparisonResult::kGreater;
        }

        /*if (rhs == std::strong_ordering::less)*/
        {
            return  Details::ComparisonResult::kLess;
        }
    }

}

// ===========================================================================
