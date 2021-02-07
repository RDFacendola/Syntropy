
/// \file math.inl
///
/// \author Raffaele D. Facendola - 2017

#include <cmath>

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* MATH                                                                 */
    /************************************************************************/

    template <typename TValue, typename... TValues>
    [[nodiscard]] constexpr
    Templates::CommonType<TValue, TValues...> Math
    ::Min(Immutable<TValue> first, Immutable<TValues>... rest) noexcept
    {
        auto min = Templates::CommonType<TValue, TValues...>{ first };

        auto fold = [&min]<typename UValue>(Immutable<UValue> value)
        {
            min = (min < value) ? min : value;
        };

        (fold(rest), ...);

        return min;
    }

    template <typename TValue, typename... TValues>
    [[nodiscard]] constexpr
    Templates::CommonType<TValue, TValues...> Math
    ::Max(Immutable<TValue> first, Immutable<TValues>... rest) noexcept
    {
        auto max = Templates::CommonType<TValue, TValues...>{ first };

        auto fold = [&max]<typename UValue>(Immutable<UValue> value)
        {
            max = (max > value) ? max : value;
        };

        (fold(rest), ...);

        return max;
    }

    template <typename TNumber>
    [[nodiscard]] constexpr
    TNumber Math
    ::Ceil(TNumber rhs)
    {
        return std::ceil(rhs);
    }

    template <typename TNumber>
    [[nodiscard]] constexpr
    TNumber Math
    ::Floor(TNumber rhs)
    {
        return std::floor(rhs);
    }

    template <typename TNumber>
    [[nodiscard]] constexpr
    TNumber Math
    ::Ceil(TNumber rhs, TNumber multiple)
    {
        return DivCeil(rhs, multiple) * multiple;
    }

    template <typename TNumber>
    [[nodiscard]] constexpr
    TNumber Math
    ::Floor(TNumber rhs, TNumber multiple)
    {
        return DivFloor(rhs, multiple) * multiple;
    }

    template <typename TNumber>
    [[nodiscard]] constexpr
    auto Math
    ::DivCeil(TNumber lhs, TNumber rhs)
    {
        return DivFloor(lhs + rhs - TNumber{ 1 }, rhs);
    }

    template <typename TNumber>
    [[nodiscard]] constexpr
    auto Math
    ::DivFloor(TNumber lhs, TNumber rhs)
    {
        auto quotient = lhs / rhs;
        auto remainder = lhs % rhs;

        if (   (remainder != decltype(remainder){ 0 })
            && ((lhs < TNumber{ 0 }) ^ (rhs < TNumber{ 0 })))
        {
            quotient -= decltype(quotient){ 1 };
        }

        return quotient;
    }
}

// ===========================================================================
