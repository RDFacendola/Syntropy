
/// \file fundamental_serializers.h
/// \brief This header is part of the syntropy serialization system. It contains serializers for STL types.
///
/// \author Giuseppe Spizzico - 2018

#pragma once

#include "type_traits.h"

#include "serialization/json/json.h"

#include "nlohmann/json/src/json.hpp"

namespace syntropy::serialization
{
	/// \brief Partial specialization of JSONSerializerT for shared_ptr.
	template<typename TType>
	struct JSONSerializerT<std::shared_ptr<TType>>
	{
		template<typename TType>
		void operator()(nlohmann::json& json, std::shared_ptr<TType> instance) const
		{
			JSONSerialize(json, instance.get());
			json[kSharedPtrIdToken] = std::hash<std::shared_ptr<TType>>()(instance);
		}
	};

	/// \brief Partial specialization of JSONSerializerT for weak_ptr.
	template<typename TType>
	struct JSONSerializerT<std::weak_ptr<TType>>
	{
		template<typename TType>
		void operator()(nlohmann::json& json, std::weak_ptr<TType> instance) const
		{
			JSONSerialize(json, instance.lock());
		}
	};

	/// \brief Partial specialization of JSONSerializerT for std::map.
	template <typename TKey, typename TValue>
	struct JSONSerializerT<std::map<TKey, TValue>>
	{
		template <typename TKey, typename TValue>
		void operator()(nlohmann::json& json, const std::map<TKey, TValue>& map) const
		{			
			for (auto&& pair : map)
			{			
				if constexpr(std::is_constructible_v<nlohmann::json::object_t::key_type, TKey>)
				{
					JSONSerialize(json[std::get<0>(pair)], std::get<1>(pair));
				}
				else
				{
					nlohmann::json pair_json;
					JSONSerialize(pair_json[map::kIdToken], std::get<0>(pair));
					JSONSerialize(pair_json[map::kValueToken], std::get<1>(pair));
					json.push_back(pair_json);
				}
			}
		}
	};
}