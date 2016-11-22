#include "serialization/json.h"

#include <fstream>

namespace syntropy {

    namespace syntax {

        namespace serialization {

            //////////////// JSON DESERIALIZABLE ////////////////

            const char* JSONDeserializable::kClassToken = "$class";
            const char* JSONDeserializable::kIdToken = "id";

            JSONDeserializable::JSONDeserializable(const JSONDeserializable& other) noexcept {

                reinterpret_cast<const IContent*>(&(other.content_))->Clone(content_);

            }
        
            JSONDeserializable::JSONDeserializable(JSONDeserializable&& other) noexcept
                : content_(std::move(other.content_)) {}

            JSONDeserializable& JSONDeserializable::operator=(JSONDeserializable other) noexcept {

                JSONDeserializable(other).Swap(*this);

                return *this;

            }

            void JSONDeserializable::Swap(JSONDeserializable& other) noexcept {

                std::swap(content_, other.content_);

            }

            JSONDeserializable::~JSONDeserializable(){

                reinterpret_cast<IContent*>(&(content_))->~IContent();
            
            }

            //////////////// METHODS ////////////////

            reflection::Instance InstantiateFromJSON(const reflection::Class& base_class, const nlohmann::json& json) {
 
                 const reflection::Class* instance_class = &base_class;

                 auto class_it = json.find(JSONDeserializable::kClassToken);
 
                 if (class_it != json.end()) {
 
                     // A concrete class type was defined
 
                     if (!class_it->is_string()) {
 
                         return reflection::Instance();              // Expected a class name.
 
                     }
 
                     auto concrete_class = reflection::GetClass(class_it->get<std::string>());
 
                     if (concrete_class == nullptr) {
 
                         return reflection::Instance();              // No such class.
 
                     }
 
                     if (*concrete_class != base_class) {
 
                         return reflection::Instance();              // Wrong class hierarchy.
 
                     }
 
                     instance_class = concrete_class;

                 }
 
                 // Instantiation and deserialization

                 auto instance = instance_class->Instantiate();
 
                 JSONDeserializer<reflection::Instance>()(instance, json);

                 return instance;

            }

        }

    }

}
