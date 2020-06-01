
/// \file random_engine.h
/// \brief This header is part of the Syntropy math module. It contains random number engine interfaces and classes.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/math/pcg_random_engine.h"

namespace syntropy
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    class RandomEngine;

/// \brief Get the thread-local default random number generator.
    RandomEngine& GetDefaultRandomEngine() noexcept;

    /// \brief Get the thread-local random number generator.
    RandomEngine& GetRandomEngine() noexcept;

    /// \brief Set the thread-local random number generator.
    /// \return Returns the previous value of the thread-local random number generator.
    RandomEngine& SetRandomEngine(RandomEngine& random_engine) noexcept;

    /************************************************************************/
    /* RANDOM ENGINE                                                        */
    /************************************************************************/

    /// \brief Represents a class that can generate unpredictable numbers.
    /// \author Raffaele D. Facendola - May 2020.
    class RandomEngine
    {
        friend RandomEngine& GetDefaultRandomEngine() noexcept;
        friend RandomEngine& GetRandomEngine() noexcept;
        friend RandomEngine& SetRandomEngine(RandomEngine& random_engine) noexcept;

    public:

        /// \brief Default virtual destructor.
        virtual ~RandomEngine() = default;

        /// \brief Generate a random number uniformly distributed in the range [0; +1).
        virtual Float operator()() noexcept = 0;

    private:

        /// \brief Thread-local current random number generator.
        static inline thread_local RandomEngine* random_engine_ = &GetDefaultRandomEngine();

    };

    /************************************************************************/
    /* RANDOM ENGINE T <TRANDOM ENGINE>                                     */
    /************************************************************************/

    /// \brief Wraps a random number engine, adapting it to RandomNumberEngine interface.
    /// \author Raffaele D. Facendola - May 2020.
    template <typename TRandomEngine>
    class RandomEngineT : public RandomEngine
    {
    public:

        /// \brief Wrap a random engine.
        RandomEngineT(TRandomEngine& random_engine);

        /// \brief The underlying random engine should outlive the wrapper.
        RandomEngineT(const TRandomEngine&& random_engine) = delete;

        /// \brief Default virtual destructor.
        virtual ~RandomEngineT() = default;

        /// \brief Generate a random number uniformly distributed in the range [0; +1).
        virtual Float operator()() noexcept override;

        /// \brief Access the underlying random engine.
        TRandomEngine& GetRandomEngine() const noexcept;

    private:

        /// \brief Underlying random engine.
        TRandomEngine* random_engine_{ nullptr };

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.

    inline RandomEngine& GetDefaultRandomEngine() noexcept
    {
        static auto pcg_random_engine = PCGRandomEngine{};

        static auto random_engine = RandomEngineT<PCGRandomEngine>{ pcg_random_engine };

        return random_engine;
    }

    inline RandomEngine& GetRandomEngine() noexcept
    {
        return *RandomEngine::random_engine_;
    }

    inline RandomEngine& SetRandomEngine(RandomEngine& random_engine) noexcept
    {
        auto& previous_random_engine = GetRandomEngine();

        RandomEngine::random_engine_ = &random_engine;

        return previous_random_engine;
    }

    // RandomEngineT<TRandomEngine>.

    template <typename TRandomEngine>
    inline RandomEngineT<TRandomEngine>::RandomEngineT(TRandomEngine& random_engine)
        : random_engine_(&random_engine)
    {

    }

    template <typename TRandomEngine>
    inline Float RandomEngineT<TRandomEngine>::operator()() noexcept
    {
        return (*random_engine_)();
    }

    template <typename TRandomEngine>
    inline TRandomEngine& RandomEngineT<TRandomEngine>::GetRandomEngine() const noexcept
    {
        return random_engine_;
    }

}