#include "syntropy/math/random.h"

#include <random>

namespace syntropy
{
    /************************************************************************/
    /* RANDOM                                                               */
    /************************************************************************/

    Random::Random()
    {
        Randomize();
    }

    Random::Random(uint64_t seed, uint64_t stream)
    {
        Seed(seed, stream);
    }

    void Random::Seed(uint64_t seed, uint64_t stream)
    {
        engine_.seed(seed, stream);
    }

    void Random::Randomize()
    {
        std::random_device random_device;

        std::seed_seq seeder{ random_device(), random_device(), random_device(), random_device() };     // > 128-bit state.

        engine_.seed(seeder);
    }

    void Random::Advance(uint64_t steps)
    {
        engine_.advance(steps);
    }

    pcg32& Random::GetEngine()
    {
        return engine_;
    }

    int32_t Random::Range(int32_t max)
    {
        return Range(0, max);
    }

    int32_t Random::Range(int32_t min, int32_t max)
    {
        std::uniform_int_distribution<int32_t> distribution(min, max);

        return distribution(engine_);
    }

    float Random::Range(float max)
    {
        return Range(0.0f, max);
    }

    float Random::Range(float min, float max)
    {
        std::uniform_real_distribution<float> distribution(min, max);

        return distribution(engine_);
    }

    float Random::Uniform()
    {
        return Range(1.0f);
    }

    bool Random::Boolean(float probability)
    {
        return Uniform() < probability;
    }

    float Random::Gaussian(float mean, float standard_deviation)
    {
        std::normal_distribution<float> distribution(mean, standard_deviation);

        return distribution(engine_);
    }

}