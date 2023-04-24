#include "syntropy/serialization/json/serialization.h"

#include "syntropy/diagnostics/log.h"

namespace Syntropy::serialization
{
	/************************************************************************/
	/* JSON SERIALIZABLE                                                  */
	/************************************************************************/

	void JSONSerializable::operator()(const String& name, Syntropy::reflection::Any& instance, JSON& json) const
	{
		serializer_(name, instance, json);
	}
}
