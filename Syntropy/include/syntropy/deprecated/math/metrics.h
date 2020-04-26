
/// \file metrics.h
/// \brief This header is part of the syntropy math system. It contains metrics used to compare strings and numbers.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <cstdint>
#include <cstdlib>
#include <type_traits>

namespace syntropy
{
	/// \brief Count the number of bits equal to 1 in the provided number.
	/// \param rhs Number to count the bits of.
	/// \return Returns the number of bits equal to 1 in the provided number.
	template <typename TNumber>
	constexpr TNumber CountSetBits(TNumber rhs);

	/// \brief Get the Hamming distance between two numbers.
	/// \param lhs First operand.
	/// \param rhs Second operand.
	/// \return Returns the Hamming distance between lhs and rhs, that is the number of different bits in both numbers.
	template <typename TNumber>
	constexpr TNumber HammingDistance(TNumber lhs, TNumber rhs);

	/************************************************************************/
	/* IMPLEMENTATION                                                       */
	/************************************************************************/

	template <typename TNumber>
	constexpr TNumber CountSetBits(TNumber rhs)
	{
		static_assert(std::is_integral_v<TNumber>, "TNumber must be integral.");

		// Keringhan's algorithm, runs in O(log(bits)).

		auto count = TNumber{ 0 };

		for (; rhs != 0; rhs &= (rhs - 1))
		{
			++count;
		}

		return count;
	}

	template <typename TNumber>
	constexpr TNumber HammingDistance(TNumber lhs, TNumber rhs)
	{
		return CountSetBits(lhs ^ rhs);
	}
}
