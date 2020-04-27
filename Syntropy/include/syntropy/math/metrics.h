
/// \file metrics.h
/// \brief This header is part of the Syntropy math module. It contains metrics used to compare strings and numbers.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include "syntropy/syntropy.h"

#include "syntropy/language/type_traits.h"

namespace syntropy
{
    /************************************************************************/
    /* METRICS                                                              */
    /************************************************************************/

	/// \brief Count the number of bits equal to 1 in the provided number.
	/// \return Returns the number of bits equal to 1 in the provided number.
	template <typename TIntegral>
	constexpr TIntegral CountSetBits(TIntegral rhs);

	/// \brief Get the Hamming distance between two numbers.
	/// \return Returns the Hamming distance between lhs and rhs, that is the number of different bits in both numbers.
	template <typename TIntegral>
	constexpr TIntegral HammingDistance(TIntegral lhs, TIntegral rhs);

	/************************************************************************/
	/* IMPLEMENTATION                                                       */
	/************************************************************************/

    // Metrics.

	template <typename TIntegral>
	constexpr TIntegral CountSetBits(TIntegral rhs)
	{
		static_assert(std::is_integral_v<TIntegral>, "TIntegral must be integral.");

		// Keringhan's algorithm, runs in O(log(bits)).

		auto count = TIntegral{ 0 };

		for (; rhs != 0; rhs &= (rhs - 1))
		{
			++count;
		}

		return count;
	}

	template <typename TIntegral>
	constexpr TIntegral HammingDistance(TIntegral lhs, TIntegral rhs)
	{
		return CountSetBits(lhs ^ rhs);
	}
}
