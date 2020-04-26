#include "syntropy/serialization/json/serialization.h"

#include "syntropy/diagnostics/log.h"

namespace syntropy::serialization
{
	/************************************************************************/
	/* JSON SERIALIZABLE                                                  */
	/************************************************************************/

	void JSONSerializable::operator()(const String& name, syntropy::reflection::Any& instance, JSON& json) const
	{
		serializer_(name, instance, json);
	}
}
