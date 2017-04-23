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

        const char* JSONDeserializable::kClassToken = "$class";
        const char* JSONDeserializable::kIdToken = "id";

        JSONDeserializable::JSONDeserializable(const JSONDeserializable& other) noexcept
        {
            reinterpret_cast<const IContent*>(&(other.content_))->Clone(content_);
        }

        JSONDeserializable::JSONDeserializable(JSONDeserializable&& other) noexcept
            : content_(std::move(other.content_))
        {

        }

        JSONDeserializable& JSONDeserializable::operator=(JSONDeserializable other) noexcept
        {
            JSONDeserializable(other).Swap(*this);

            return *this;
        }

        void JSONDeserializable::Swap(JSONDeserializable& other) noexcept
        {
            std::swap(content_, other.content_);
        }

        JSONDeserializable::~JSONDeserializable()
        {
            reinterpret_cast<IContent*>(&(content_))->~IContent();
        }

        /************************************************************************/
        /* JSON CONSTRUCTIBLE                                                   */
        /************************************************************************/

        reflection::Instance JSONConstructible::operator()(const nlohmann::json& /*json*/) const
        {
            return reflection::Instance();
        }

        /************************************************************************/
        /* METHODS                                                              */
        /************************************************************************/

        reflection::Instance InstantiateFromJSON(const reflection::Class& base_class, const nlohmann::json& json) {
 
            const reflection::Class* instance_class = &base_class;

            auto class_it = json.find(JSONDeserializable::kClassToken);
 
            // Search for the concrete class to instantiate

            if (class_it != json.end())
            {
                 // A concrete class type was defined
 
                if (!class_it->is_string())
                {
                    SYNTROPY_WARNING((SerializationCtx), "Expected a string value for the property ", JSONDeserializable::kClassToken, ".");

                    return reflection::Instance();              // Expected a class name.
                }
 
                auto concrete_class = reflection::GetClass(class_it->get<std::string>());
 
                if (concrete_class == nullptr)
                {
                    SYNTROPY_WARNING((SerializationCtx), "Unnrecognized class ", class_it->get<std::string>(), ".");

                    return reflection::Instance();              // No such class.
                }
 
                if (*concrete_class != base_class)
                {
                    SYNTROPY_WARNING((SerializationCtx), "Cannot deserialize an object of type ", base_class, " from type ", class_it->get<std::string>(), ".");

                    return reflection::Instance();              // Wrong class hierarchy.
                }
 
                instance_class = concrete_class;
            }
 
            // Instantiation and deserialization

            auto json_constructible = instance_class->GetInterface<JSONConstructible>();

            if (json_constructible)
            {
                // Construct via custom JSON-constructor.
                return (*json_constructible)(json);
            }

            auto default_constructible = instance_class->GetInterface<reflection::Constructible<>>();

            if (default_constructible)
            {
                // Default-construct a new instance and deserialize each property.
                auto instance = (*default_constructible)();

                if (instance)
                {
                    JSONDeserializer<reflection::Instance>()(instance, json);
                }

                return instance;
            }

            // Unable to deserialize the object.
            return reflection::Instance();

        }

    }

}
