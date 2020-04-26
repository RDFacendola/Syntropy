#include "syntropy/serialization/json/deserialization.h"

#include "syntropy/diagnostics/log.h"

namespace syntropy::serialization
{
    /************************************************************************/
    /* JSON DESERIALIZABLE                                                  */
    /************************************************************************/

    bool JSONDeserializable::operator()(const reflection::Any& instance, const JSON& json) const
    {
        return deserializer_(instance, json);
    }

    /************************************************************************/
    /* JSON CONSTRUCTIBLE                                                   */
    /************************************************************************/

    reflection::Any JSONConstructible::operator()(const JSON& json) const
    {
        return instancer_(json);
    }
}
