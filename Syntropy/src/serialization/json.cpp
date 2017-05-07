#include "serialization/json.h"

#include <fstream>

#include "reflection/class_interfaces.h"

#include "diagnostics/log.h"

namespace syntropy 
{
    namespace serialization 
    {

        /************************************************************************/
        /* JSON DESERIALIZABLE                                                  */
        /************************************************************************/

        bool JSONDeserializable::operator()(reflection::Any& instance, const nlohmann::json& json) const
        {
            return deserializer_(instance, json);
        }

        bool JSONDeserializable::operator()(reflection::Any&& instance, const nlohmann::json& json) const
        {
            return deserializer_(instance, json);
        }

        /************************************************************************/
        /* JSON CONSTRUCTIBLE                                                   */
        /************************************************************************/

        reflection::Any JSONConstructible::operator()(const nlohmann::json& /*json*/) const
        {
            return reflection::Any();
        }

    }

}
