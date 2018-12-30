
/// \file utility
/// \brief Exposes additional utility features.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <utility>

namespace syntropy
{

	/// \brief Don't care functor.
	/// This type can be constructed or called with any number of arguments and does nothing in either case.
	/// \author Raffaele D. Facendola - September 2016
	struct _
	{
		template <typename... TArgs>
		_(TArgs&&...) {}

		template <typename... TArgs>
		void operator()(TArgs&&...) {}
	};

	/// \brief Tag type used to declare a type when automatic type deduction cannot be used.
	template <typename TType>
	struct tag_t
	{
		explicit tag_t() = default;
	};

	template <typename TType>
	inline constexpr tag_t<TType> tag{};

}