
/// \file fundamental_serializers.h
/// \brief This header is part of the syntropy serialization system. It contains serializers for fundamental types.
///
/// \author Giuseppe Spizzico - February 2018

#pragma once

#include "type_traits.h"
#include "containers/hashed_string.h"

#include "reflection/reflection.h"
#include "reflection/interfaces/class_interfaces.h"

#include "serialization/json/json.h"

namespace syntropy::serialization
{
	/************************************************************************/
	/* ENUMERATIONS                                                         */
	/************************************************************************/

	/// \brief Partial specialization of JSONSerializerT for enumeration types.
	template <typename TType>
	struct JSONSerializerT<TType, typename std::enable_if_t<std::is_enum_v<TType>> >
	{
		void operator()(nlohmann::json& json, const TType& value) const
		{
			if (auto enum_interface = reflection::ClassOf<TType>().GetInterface<reflection::Enumeration>())
			{				
				if(auto name = enum_interface->GetNameByValue(value); name.has_value())
				{
					using TNameType = decltype(name.value());
					JSONSerializer<TNameType>(json, name.value());
				}
			}		
		}
	};
}