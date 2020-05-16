
/// \file pcg_random engine.h
/// \brief This header is part of the Syntropy math module. It contains the implementation of the PCG random number generator.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <cstdint>
#include <random>
#include <type_traits>

namespace syntropy
{
    /************************************************************************/
    /* PCG RANDOM ENGINE                                                    */
    /************************************************************************/

    /// \brief Wraps a PCG random number generator, using the multiple-stream version of the generator.
    /// Each stream yields a different, independent, sequence: samples from different streams are guaranteed to be uncorrelated.
    /// A 32-bit output 64-bit state generator is preferred over a 64-bit output 128-bit state generator since 128-bit numbers implementations are slower on certain platforms.
    /// \see http://www.pcg-random.org/
    /// \author Raffaele D. Facendola - November 2017
    class PCGRandomEngine
    {
    public:

        /// \brief Create a new engine with randomized state.
        PCGRandomEngine() noexcept;

        /// \brief Default copy constructor.
        PCGRandomEngine(const PCGRandomEngine& rhs);

        /// \brief Default assignment operator.
        PCGRandomEngine& operator=(const PCGRandomEngine& rhs);

        /// \brief Default destructor.
        ~PCGRandomEngine() = default;

        /// \brief Create a new random number generator with explicit values for the seed and the stream to use.
        /// \param seed Seed for the random sequence. Identifies how far in the sequence the samples are taken from.
        /// \param stream Index of the stream to use. Identifies which sequence the samples are taken from.
        PCGRandomEngine(std::int64_t seed, std::int64_t stream) noexcept;

        /// \brief Reset the status of the random number generator.
        /// \param seed Seed for the random sequence. Identifies how far in the sequence the samples are taken from.
        /// \param stream Index of the stream to use. Identifies which sequence the samples are taken from.
        void Seed(std::int64_t seed, std::int64_t stream) noexcept;

        /// \brief Randomize the status of the engine.
        void Randomize() noexcept;

        /// \brief Advances the state of the engine.
        /// \param steps Amount of states to advance.
        void Advance(std::int64_t steps) noexcept;

        /// \brief Generate a random number uniformly distributed in the range [0; +1).
        float operator()() noexcept;

    private:

        /// \brief Concrete type of a 64-bit state 32-bit output random number engine.
        struct PCG32;

        /// \brief Initialize the underlying random number engine.
        void InitializeEngine();

        /// \brief Pointer to the actual engine.
        PCG32* engine_{ nullptr };

        /// \brief Underlying uniform distribution.
        std::uniform_real_distribution<float> distribution_;

        /// \brief Engine state storage.
        std::aligned_storage_t<sizeof(std::int64_t), alignof(std::int64_t)> engine_storage_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // PCGRandomEngine.

    inline PCGRandomEngine::PCGRandomEngine() noexcept
    {
        InitializeEngine();
        Randomize();
    }

    inline PCGRandomEngine::PCGRandomEngine(std::int64_t seed, std::int64_t stream) noexcept
    {
        InitializeEngine();
        Seed(seed, stream);
    }

}