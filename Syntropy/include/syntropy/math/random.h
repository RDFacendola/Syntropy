
/// \file random.h
/// \brief This header is part of the Syntropy math module. It contains methods to generate random samples.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <cstdint>
#include <cmath>

#include "syntropy/core/types.h"
#include "syntropy/math/vector.h"
#include "syntropy/math/random_engine.h"
#include "syntropy/math/numeric.h"
#include "syntropy/math/math.h"

namespace syntropy
{
    /************************************************************************/
    /* RANDOM                                                               */
    /************************************************************************/

    /// \brief Exposes random number generation functionalities.
    /// \author Raffaele D. Facendola - 2017
    namespace Random
    {
        /// \brief Generate a random number uniformly distributed in [0; 1).
        /// \return Returns a random number uniformly distributed in [0; 1).
        Float Uniform();

        /// \brief Generate a random number uniformly distributed in [min; max).
        /// \param max Maximum value to generate (exclusive).
        /// \return Returns a random number uniformly distributed in [0; max).
        Float Uniform(Float max);

        /// \brief Generate a random number distributed in [min; max).
        /// \param min Minimum value to generate (inclusive).
        /// \param max Maximum value to generate (exclusive).
        Float Uniform(Float min, Float max);

        /// \brief Generate a random number uniformly distributed in [0; max).
        /// \param max Maximum value to generate (exclusive).
        /// \return Returns a random number uniformly distributed in [0; max).
        Int Uniform(std::int32_t max);

        /// \brief Generate a random number uniformly distributed in [min; max).
        /// \param min Minimum value to generate (inclusive).
        /// \param max Maximum value to generate (exclusive).
        Int Uniform(std::int32_t min, std::int32_t max);

        /// \brief Generate a random boolean value.
        /// \param probability Probability of getting 'true' as result. Range [0; 1].
        /// \return Returns a random boolean value, where P(true) is equal to probability.
        bool Bernoulli(Float probability);

        /// \brief Generate a random number distributed according to a Normal distribution.
        /// \return Returns a random number distributed according to a Normal distribution N(0, 1).
        Float Normal();

        /// \brief Generate a random number distributed according to a Gaussian distribution.
        /// \param mean Expected value of the distribution.
        /// \param standard_deviation Standard deviation of the distribution.
        /// \return Returns a random number distributed according to a Gaussian distribution N(mean, std^2).
        Float Gaussian(Float mean, Float standard_deviation);

        /// \brief Generate a random number distributed according to an Exponential distribution.
        /// \param lambda Expected occurrences per unit of time.
        /// \return Returns the time at which the next event happens according to the exponential distribution.
        Float Exponential(Float lambda);

        /// \brief Generate a random point in a unit box (-1; +1) x (-1; +1).
        /// \return Returns a random point in a unit box (-1; +1) x (-1; +1).
        Math::Float2 UniformBox();

        /// \brief Generate a random point in a unit circle with radius [0; 1).
        /// \returns a random point in a unit circle with radius [0; 1).
        Math::Float2 UniformCircle();

    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Random.

    inline Float Random::Uniform()
    {
        return GetRandomEngine()();
    }

    inline Float Random::Uniform(Float max)
    {
        return Uniform() * max;
    }

    inline Float Random::Uniform(Float min, Float max)
    {
        return Uniform() * (max - min) + min;
    }

    inline Int Random::Uniform(std::int32_t max)
    {
        return static_cast<std::int32_t>(Math::Floor(Uniform() * max));
    }

    inline Int Random::Uniform(std::int32_t min, std::int32_t max)
    {
        return Uniform(max - min) + min;
    }

    inline bool Random::Bernoulli(Float probability)
    {
        return Uniform() < probability;
    }

    inline Float Random::Normal()
    {
        // Marsaglia polar method.

        auto point = UniformCircle();

        for (; point == Math::Float2{}; point = UniformCircle());                           // Reject corner case x = y = 0.

        auto length_sqr = LengthSqr(point);

        return point.x_ * Math::FastSqrt((-2.0f * Math::Ln(length_sqr)) / length_sqr);      // point.y_ is a valid sample as well.
    }

    inline Float Random::Gaussian(Float mean, Float standard_deviation)
    {
        return Normal() * standard_deviation + mean;
    }

    inline Float Random::Exponential(Float lambda)
    {
        return -Math::Ln(Uniform()) / lambda;
    }

    inline Math::Float2 Random::UniformBox()
    {
        auto min = Math::Next(-1.0f);
        auto max = +1.0f;

        return Math::Float2{ Uniform(min, max), Uniform(min, max) };
    }

    inline Math::Float2 Random::UniformCircle()
    {
        auto point = UniformBox();

        for (; LengthSqr(point) >= 1.0f; point = UniformBox());                             // Reject outside circumference x^2 + y^2 = 1

        return point;
    }

}