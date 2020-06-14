
/// \file random_context.h
/// \brief This header is part of the Syntropy random numbers generation module. It contains definitions for scope-based random number generators.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/random/random_engine.h"

namespace syntropy
{
    /************************************************************************/
    /* RANDOM CONTEXT                                                       */
    /************************************************************************/

    /// \brief Represents a RAII guard to change the random engine in the current scope and restore the previous one upon destruction.
    /// Random contexts can be nested but overlapping them results in undefined behavior.
    /// \author Raffaele D. Facendola - May 2020.
    template <typename TRandomEngine>
    class RandomContext
    {
    public:

        /// \brief Set a new random engine for the current scope.
        RandomContext(TRandomEngine& random_engine);

        /// \brief Random engines should outlive the context.
        RandomContext(const TRandomEngine&& random_engine) = delete;

        /// \brief Restore the previous random engine.
        ~RandomContext();

        /// \brief No copy constructor.
        RandomContext(const RandomContext&) = delete;

        /// \brief No assignment operator.
        RandomContext& operator=(const RandomContext&) = delete;

    public:

        /// \brief Current random engine.
        RandomEngineT<TRandomEngine> random_engine_;

        /// \brief Previous random engine.
        RandomEngine* previous_random_engine_= nullptr;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a random context deducing the type from the arguments.
    template <typename TRandomEngine>
    RandomContext<TRandomEngine> MakeRandomContext(TRandomEngine& random_engine);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // RandomContext<TRandomEngine>.

    template <typename TRandomEngine>
    inline RandomContext<TRandomEngine>::RandomContext(TRandomEngine& random_engine)
        : random_engine_(random_engine)
        , previous_random_engine_(&SetRandomEngine(random_engine_))
    {

    }

    template <typename TRandomEngine>
    inline RandomContext<TRandomEngine>::~RandomContext()
    {
        SetRandomEngine(*previous_random_engine_);
    }

    // Non-member functions.

    template <typename TRandomEngine>
    inline RandomContext<TRandomEngine> MakeRandomContext(TRandomEngine& random_engine)
    {
        return RandomContext<TRandomEngine>(random_engine);
    }

}
