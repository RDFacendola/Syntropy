#include "serialization/json.h"

#include <fstream>

namespace syntropy {

    namespace syntax {

        namespace serialization {

            //////////////// JSON DESERIALIZABLE ////////////////

            const char* JSONDeserializable::kClassToken = "$class";

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

            nlohmann::json ParseJSONFile(const char* path) {

                std::fstream file_stream(path);

                nlohmann::json json;

                if (file_stream.good()) {

                    file_stream >> json;

                }

                return json;

            }

            reflection::Instance InstantiateFromJSON(const reflection::Class& base_class, const nlohmann::json& json, bool* success) {

                 if (success) {
                     
                     *success = false;
 
                 }
 
                 reflection::Instance instance;
 
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
 
                         return reflection::Instance();              // The specified concrete class doesn't derive from the base class.
 
                     }
 
                     instance = concrete_class->Instantiate();       // Attempts to instantiate a concrete class
 
                 }
                 else {
 
                     instance = base_class.Instantiate();            // Attempts to instantiate the base class
 
                 }
 
                 // Instance deserialization
 
                 if (JSONDeserializer<reflection::Instance>()(instance, json) && success) {
 
                     *success = true;
 
                 }
 
                 return instance;

            }

        }

    }

}
