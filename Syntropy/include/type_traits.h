
/// \file type_traits
/// \brief Exposes additional type traits.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <type_traits>

namespace syntropy {

	template <typename TStream, typename TType>
	class is_stream_insertable {

		template<typename S, typename T>
		static auto test(int) -> decltype(std::declval<S&>() << std::declval<T>(), std::true_type());

		template<typename, typename>
		static auto test(...)->std::false_type;

	public:

		static const bool value = decltype(test<TStream, TType>(0))::value;

	};

	template <typename TStream, typename TType>
	class is_stream_extractable {

		template<typename S, typename T>
		static auto test(int) -> decltype(std::declval<S&>() >> std::declval<T&>(), std::true_type());

		template<typename, typename>
		static auto test(...)->std::false_type;

	public:

		static const bool value = decltype(test<TStream, TType>(0))::value;

	};

}