#include "serialization/json/serialization.h"

#include "diagnostics/log.h"

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
