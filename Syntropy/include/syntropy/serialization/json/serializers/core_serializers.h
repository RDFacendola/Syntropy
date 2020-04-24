
/// \file core_serializers.h
/// \brief This header is part of the syntropy serialization system. It contains serializers for syntropy's core types.
///
/// \author Giuseppe Spizzico - February 2018

#pragma once

#include "syntropy/types/label.h"
#include "syntropy/containers/context.h"

#include "syntropy/serialization/json/json.h"

namespace syntropy::serialization
{
	/************************************************************************/
	/* HASHED STRING                                                        */
	/************************************************************************/

	template <>
	struct JSONSerializerT<Label>
	{
		void operator()(nlohmann::json& json, const Label& hashed_string) const
		{
			JSONSerialize(json, hashed_string.GetString());
		}
	};

	/************************************************************************/
	/* CONTEXT                                                              */
	/************************************************************************/

	template <>
	struct JSONSerializerT<Context>
	{
	void operator()(nlohmann::json& json, const Context& context) const
		{
			JSONSerialize(json, context.GetName());
		}
	};
}
