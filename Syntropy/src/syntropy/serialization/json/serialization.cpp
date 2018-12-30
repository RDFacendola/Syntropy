#include "syntropy/serialization/json/serialization.h"

#include "syntropy/diagnostics/log.h"

namespace syntropy::serialization
{
	/************************************************************************/
	/* JSON SERIALIZABLE                                                  */
	/************************************************************************/

	void JSONSerializable::operator()(const std::string& name, syntropy::reflection::Any& instance, nlohmann::json& json) const
	{
		serializer_(name, instance, json);
	}
}
