
/// \file numbers.inl
///
/// \author Raffaele D. Facendola - April 2021

#include <limits>

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* MATH                                                                 */
    /************************************************************************/

    [[nodiscard]] constexpr Float
    Math
    ::Infinity() noexcept
    {
        return std::numeric_limits<float>::infinity();
    }

    [[nodiscard]] constexpr Float
    Math
    ::NotANumber() noexcept
    {
        return std::numeric_limits<float>::quiet_NaN();
    }

}

// ===========================================================================
