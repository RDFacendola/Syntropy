#include "syntropy/random/pcg_random_engine.h"

#include "pcg/include/pcg_random.hpp"

namespace syntropy
{
    /************************************************************************/
    /* PCG RANDOM ENIGNE :: PCG64                                           */
    /************************************************************************/

    /// \brief Concrete implementation of the PCG random engine.
    struct PCGRandomEngine::PCG64 : pcg64 {};

    /************************************************************************/
    /* RANDOM                                                               */
    /************************************************************************/

    PCGRandomEngine::PCGRandomEngine(const PCGRandomEngine& rhs)
    {
        InitializeEngine();

        *this = rhs;
    }

    PCGRandomEngine& PCGRandomEngine::operator=(const PCGRandomEngine& rhs)
    {
        *engine_ = *rhs.engine_;

        return *this;
    }

    void PCGRandomEngine::InitializeEngine()
    {
        engine_ = new (&engine_storage_) PCG64();
    }

    void PCGRandomEngine::Seed(Int seed, Int stream) noexcept
    {
        engine_->seed(seed, stream);
    }

    void PCGRandomEngine::Randomize() noexcept
    {
        auto random_device = std::random_device{};

        auto seeder = std::seed_seq{ random_device(), random_device(), random_device(), random_device(), random_device(), random_device(), random_device(), random_device() };

        engine_->seed(seeder);
    }

    void PCGRandomEngine::Advance(Int steps) noexcept
    {
        engine_->advance(steps);
    }

    Float PCGRandomEngine::operator()() noexcept
    {
        return distribution_(*engine_);
    }

}