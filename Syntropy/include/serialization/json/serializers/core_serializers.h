
/// \file core_serializers.h
/// \brief This header is part of the syntropy serialization system. It contains serializers for syntropy's core types.
///
/// \author Giuseppe Spizzico - February 2018

#pragma once

#include "containers/hashed_string.h"
#include "containers/context.h"

#include "serialization/json/json.h"

namespace syntropy::serialization
{
	/************************************************************************/
	/* HASHED STRING                                                        */
	/************************************************************************/

	template <>
	struct JSONSerializerT<HashedString>
	{
		void operator()(nlohmann::json& json, const HashedString& hashed_string) const
		{
			using TStringType = decltype(hashed_string.GetString());
			JSONSerializer<TStringType>(json, hashed_string.GetString());
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
			using TNameType = decltype(context.GetName());
			JSONSerializer<TNameType>(json, context.GetName());
		}
	};
}
