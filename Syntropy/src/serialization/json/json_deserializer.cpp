#include "serialization/json/json_deserializer.h"

#include "serialization/json.h"

#include "diagnostics/debug.h"

namespace syntropy
{
    namespace serialization
    {

        /************************************************************************/
        /* DESERIALIZE OBJECT FROM JSON                                         */
        /************************************************************************/

        void DeserializeObjectFromJSON(const reflection::Any& object, const nlohmann::json& json)
        {
            SYNTROPY_ASSERT(object.HasValue());

            if (json.is_object())
            {
                auto& object_class = object.GetType().GetClass();

                for (auto json_property = json.cbegin(); json_property != json.cend(); ++json_property)
                {
                    auto object_property = object_class.GetProperty(json_property.key());           // Matching object property

                    if (object_property)
                    {
                        auto deserializable = object_property->GetInterface<JSONDeserializable>();

                        if (deserializable)
                        {
                            (*deserializable)(object, json_property.value());                       // Recursive property deserialization.
                        }
                    }
                }
            }
        }

    }

}

