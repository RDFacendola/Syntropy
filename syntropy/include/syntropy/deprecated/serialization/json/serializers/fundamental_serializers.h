
/// \file fundamental_serializers.h
/// \brief This header is part of the syntropy serialization system. It contains serializers for fundamental types.
///
/// \author Giuseppe Spizzico - February 2018

#pragma once

#include "syntropy/type_traits.h"
#include "syntropy/types/label.h"

#include "syntropy/reflection/reflection.h"
#include "syntropy/reflection/interfaces/class_interfaces.h"

#include "syntropy/serialization/json/json.h"

namespace Syntropy::serialization
{
    /************************************************************************/
    /* POINTERS                                                             */
    /************************************************************************/

    /// \brief Partial specialization of JSONSerializerT for pointer types.
    template <typename TType>
    struct JSONSerializerT<TType*>
    {
        void operator()(JSON& json, const TType* value) const
        {
            auto value_json = Syntropy::serialization::SerializeObjectToJSON(*value);

            if (value_json.has_value())
            {
                json = *value_json;
            }
        }
    };

	/************************************************************************/
	/* ENUMERATIONS                                                         */
	/************************************************************************/

	/// \brief Partial specialization of JSONSerializerT for enumeration types.
	template <typename TType>
	struct JSONSerializerT<TType, typename std::enable_if_t<std::is_enum_v<TType>> >
	{
		void operator()(JSON& json, const TType& value) const
		{
			if (auto enum_interface = reflection::ClassOf<TType>().GetInterface<reflection::Enumeration>())
			{				
				if(auto name = enum_interface->GetNameByValue(value); name.has_value())
				{
					using TNameType = decltype(name.value());
					JSONSerialize(json, name.value());
				}
			}		
		}
	};
}