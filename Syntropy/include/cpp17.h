
/// \file cpp17.h
/// \brief Contains temporary utilities that are defined as C++17 standard but not yet fully supported by some compilers.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <utility>
#include <type_traits>

namespace std
{

	/// \brief is_callable has been renamed to "is_invocable" in the new standard.
	template <class Fn, class... ArgTypes>
	using is_invocable = typename is_callable<Fn(ArgTypes...)>;

	template <class Fn, class... ArgTypes>
	/*inline*/ constexpr bool is_invocable_v = std::is_invocable<Fn, ArgTypes...>::value;

}
