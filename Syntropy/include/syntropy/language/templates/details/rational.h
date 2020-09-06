
/// \file rational.h
/// \brief This header is part of Syntropy language module. It contains implementation details about rational numbers template machinery.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/constants.h"
#include "syntropy/language/templates/math.h"

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* RATIONAL SUM                                                         */
    /************************************************************************/

    /// \brief Exposes a public member alias Type equal to the unreduced sum of two rational numbers.
    template <typename T0Rational, typename T1Rational>
    struct RationalSum
    {
        static_assert(AlwaysFalse<T0Rational, T1Rational>, "T0Rational and T1Rational are both expected to have Rational<N, D> type.");
    };

    /// \brief Specialization for rationals.
    template <template<Int, Int> typename TRational, Int VLeftNumerator, Int VLeftDenominator, Int VRightNumerator, Int VRightDenominator>
    struct RationalSum<TRational<VLeftNumerator, VLeftDenominator>, TRational<VRightNumerator, VRightDenominator>>
    {
    private:

        /// \brief Unreduced denominator.
        static constexpr Int kDenominator = Details::LCM<VLeftDenominator, VRightDenominator>::kValue;

        /// \brief Unreduced numerator.
        static constexpr Int kNumerator = VLeftNumerator * (kDenominator / VLeftDenominator) + VRightNumerator * (kDenominator / VRightDenominator);

    public:

        using Type = typename TRational<kNumerator, kDenominator>;

    };

    /************************************************************************/
    /* RATIONAL DIFFERENCE                                                  */
    /************************************************************************/

    /// \brief Exposes a public member alias Type equal to the unreduced difference of two rational numbers.
    template <typename T0Rational, typename T1Rational>
    struct RationalDifference
    {
        static_assert(AlwaysFalse<T0Rational, T1Rational>, "T0Rational and T1Rational are both expected to have Rational<N, D> type.");
    };

    /// \brief Specialization for rationals.
    template <template<Int, Int> typename TRational, Int VLeftNumerator, Int VLeftDenominator, Int VRightNumerator, Int VRightDenominator>
    struct RationalDifference<TRational<VLeftNumerator, VLeftDenominator>, TRational<VRightNumerator, VRightDenominator>>
    {
    private:

        /// \brief Unreduced denominator.
        static constexpr Int kDenominator = Details::LCM<VLeftDenominator, VRightDenominator>::kValue;

        /// \brief Unreduced numerator.
        static constexpr Int kNumerator = VLeftNumerator * (kDenominator / VLeftDenominator) - VRightNumerator * (kDenominator / VRightDenominator);

    public:

        using Type = typename TRational<kNumerator, kDenominator>;

    };

    /************************************************************************/
    /* RATIONAL PRODUCT                                                     */
    /************************************************************************/

    /// \brief Exposes a public member alias Type equal to the unreduced product of two rational numbers.
    template <typename T0Rational, typename T1Rational>
    struct RationalProduct
    {
        static_assert(AlwaysFalse<T0Rational, T1Rational>, "T0Rational and T1Rational are both expected to have Rational<N, D> type.");
    };

    /// \brief Specialization for rationals.
    template <template<Int, Int> typename TRational, Int VLeftNumerator, Int VLeftDenominator, Int VRightNumerator, Int VRightDenominator>
    struct RationalProduct<TRational<VLeftNumerator, VLeftDenominator>, TRational<VRightNumerator, VRightDenominator>>
    {
    private:

        /// \brief Unreduced denominator.
        static constexpr Int kDenominator = VLeftDenominator * VRightDenominator;

        /// \brief Unreduced numerator.
        static constexpr Int kNumerator = VLeftNumerator * VRightNumerator;

    public:

        using Type = typename TRational<kNumerator, kDenominator>;

    };

    /************************************************************************/
    /* RATIONAL QUOTIENT                                                    */
    /************************************************************************/

    /// \brief Exposes a public member alias Type equal to the unreduced product of two rational numbers.
    template <typename T0Rational, typename T1Rational>
    struct RationalQuotient
    {
        static_assert(AlwaysFalse<T0Rational, T1Rational>, "T0Rational and T1Rational are both expected to have Rational<N, D> type.");
    };

    /// \brief Specialization for rationals.
    template <template<Int, Int> typename TRational, Int VLeftNumerator, Int VLeftDenominator, Int VRightNumerator, Int VRightDenominator>
    struct RationalQuotient<TRational<VLeftNumerator, VLeftDenominator>, TRational<VRightNumerator, VRightDenominator>>
    {
    private:

        /// \brief Unreduced denominator.
        static constexpr Int kDenominator = VLeftDenominator * VRightNumerator;

        /// \brief Unreduced numerator.
        static constexpr Int kNumerator = VLeftNumerator * VRightDenominator;

    public:

        using Type = typename TRational<kNumerator, kDenominator>;

    };

    /************************************************************************/
    /* RATIONAL REDUCE                                                      */
    /************************************************************************/

    /// \brief Exposes a public member alias Type equal to the reduced value of a rational number.
    template <typename TRational>
    struct RationalReduce
    {
        static_assert(AlwaysFalse<TRational>, "TRational is expected to have Rational<N, D> type.");
    };

    /// \brief Specialization for rationals.
    template <template<Int, Int> typename TRational, Int VNumerator, Int VDenominator>
    struct RationalReduce<TRational<VNumerator, VDenominator>>
    {
    private:

        /// \brief Reduced denominator.
        static constexpr Int kDenominator = VDenominator / Details::GCD<VNumerator, VDenominator>::kValue;

        /// \brief Reduced numerator.
        static constexpr Int kNumerator = VNumerator / Details::GCD<VNumerator, VDenominator>::kValue;

    public:

        using Type = typename TRational<kNumerator, kDenominator>;

    };


}