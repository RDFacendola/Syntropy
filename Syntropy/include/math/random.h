
/// \file random.h
/// \brief This header is part of the syntropy math system. It contains classes and functions used to generate random numbers.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <limits>
#include <iterator>
#include <type_traits>

#include "pcg/include/pcg_random.hpp"

namespace syntropy
{

    /// \brief Wraps a PCG random number generator and exposes methods to generate random numbers.
    /// This wrapper uses the multiple-stream version of the generator.
    /// Each stream yields a different, independent, sequence: samples from different streams are guaranteed to be uncorrelated.
    /// A 32-bit output 64-bit state generator is preferred over a 64-bit output 128-bit state generator since 128-bit numbers implementations are slower on certain platforms.
    /// \see http://www.pcg-random.org/
    /// \author Raffaele D. Facendola - November 2017
    class Random
    {
    public:

        /// \brief Create a new random number generator with randomized state.
        Random();

        /// \brief Create a new random number generator with explicit values for the seed and the stream to use.
        /// \param seed Seed for the random sequence. Identifies how far in the sequence the samples are taken from.
        /// \param stream Index of the stream to use. Identifies which sequence the samples are taken from.
        Random(uint64_t seed, uint64_t stream);

        /// \brief Reset the status of the random number generator.
        /// \param seed Seed for the random sequence. Identifies how far in the sequence the samples are taken from.
        /// \param stream Index of the stream to use. Identifies which sequence the samples are taken from.
        void Seed(uint64_t seed, uint64_t stream);

        /// \brief Randomize the status of the random number generator.
        void Randomize();

        /// \brief Advances the state of the random number generator.
        /// \param steps Amount of states to advance.
        void Advance(uint64_t steps);

        /// \brief Get the underlying engine.
        /// \return Returns the underlying engine.
        pcg32& GetEngine();

        /// \brief Generate a random number uniformly distributed in [0; max].
        /// \param max Maximum value to generate (inclusive).
        /// \return Returns a random number uniformly distributed in [0; max].
        int32_t Range(int32_t max);

        /// \brief Generate a random number uniformly distributed in [min; max].
        /// \param min Minimum value to generate (inclusive).
        /// \param max Maximum value to generate (inclusive).
        int32_t Range(int32_t min, int32_t max);

        /// \brief Generate a random number uniformly distributed in [min; max).
        /// \param max Maximum value to generate (exclusive).
        /// \return Returns a random number uniformly distributed in [0; max).
        float Range(float max);

        /// \brief Generate a random number distributed in [min; max).
        /// \param min Minimum value to generate (inclusive).
        /// \param max Maximum value to generate (exclusive).
        float Range(float min, float max);

        /// \brief Generate a random number uniformly distributed in [0; 1).
        /// \return Returns a random number uniformly distributed in [0; 1).
        float Uniform();

        /// \brief Generate a random boolean value.
        /// \param probability Probability of getting a true result. Range [0; 1].
        /// \return Returns a random boolean value, where P(true) is equal to probability.
        bool Boolean(float probability = 0.5f);

        /// \brief Generate a random number distributed according to a Gaussian distribution.
        /// \param mean Expected value of the distribution.
        /// \param standard_deviation Standard deviation of the distribution.
        /// \return Returns a random number distributed according to a Gaussian distribution N(mean, std^2).
        float Gaussian(float mean, float standard_deviation);

        /// \brief Randomly shuffle the elements in the range [begin; end).
        /// \param begin Iterator to the first element to shuffle.
        /// \param end Iterator past the last element to shuffle.
        template <typename TIterator>
        void Shuffle(TIterator begin, TIterator end)
        {
            Shuffle(begin, end, end);
        }

        /// \brief Randomly shuffle elements in [begin; middle) picking random elements from [begin; end).
        /// After this call the range [begin; middle) contains random elements, while the content of [middle; end) is unspecified.
        /// \param begin Iterator to the first element in the collection.
        /// \param middle Iterator past the last element in the shuffled collection.
        /// \param end Iterator past the last element in the collection.
        template <typename TIterator>
        void Shuffle(TIterator begin, TIterator middle, TIterator end)
        {
            //Fisher-Yates shuffle.

            for (auto distance = static_cast<int32_t>(std::distance(begin, end)); begin != middle; ++begin)
            {
                std::iter_swap(begin, begin + Range(--distance));
            }
        }

        /// \brief Pick a random element in the range [begin; end).
        /// \return Return an iterator to any element in the range [begin;end). If the range is empty returns end.
        template <typename TIterator>
        TIterator Pick(const TIterator& begin, const TIterator& end)
        {
            if (begin == end)
            {
                return end;
            }

            return begin + Range(static_cast<int32_t>(std::distance(begin, end)) - 1);
        }

    private:

        pcg32 engine_;              ///< \brief Underlying random engine.

    };

}