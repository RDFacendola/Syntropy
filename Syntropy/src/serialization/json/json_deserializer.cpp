#include "serialization/json/json_deserializer.h"

#include "serialization/json.h"

namespace syntropy
{

    namespace serialization
    {

        /************************************************************************/
        /* REFLECTION-BASED DESERIALIZATION                                     */
        /************************************************************************/

        bool JSONDeserializer<reflection::Instance>::operator()(reflection::Instance object, const nlohmann::json& json)
        {
            // Reflection-based deserialization: cycles through all the properties defined by the JSON object and fill the matching object properties
            if (json.is_object())
            {
                const reflection::Property* object_property;
                const JSONDeserializable* deserializable;

                auto& object_class = object.GetType().GetClass();

                for (auto json_property = json.cbegin(); json_property != json.cend(); ++json_property)
                {
                    object_property = object_class.GetProperty(json_property.key());                // Matching object property

                    if (object_property)
                    {
                        deserializable = object_property->GetInterface<JSONDeserializable>();

                        if (deserializable)
                        {
                            (*deserializable)(object, json_property.value());                       // Recursive deserialization
                        }
                    }

                }

                return true;
            }

            return false;

        }

    }

}



