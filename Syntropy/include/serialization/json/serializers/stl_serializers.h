
/// \file fundamental_serializers.h
/// \brief This header is part of the syntropy serialization system. It contains serializers for STL types.
///
/// \author Giuseppe Spizzico - 2018
#pragma once
#include "type_traits.h"

#include "serialization/json/json.h"
#include "serialization/serialization.h"

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
			JSONSerializer<TType>(json, instance.get());
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
			JSONSerializer<std::shared_ptr<TType>>(json, instance.lock());
		}
	};
}