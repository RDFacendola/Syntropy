/// \file json.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <fstream>
#include <memory>
#include <functional>

#include "reflection/reflection.h"

#include "nlohmann/json/src/json.hpp"

namespace syntropy {

    namespace serialization {

        template <typename TType, typename>
        struct JsonDeserializer;

        class JsonPropertySerializer{

        public:

            template<typename TClass, typename TField>
            JsonPropertySerializer(TField TClass::* field){

                deserializer_ = [field](reflection::Instance instance, const nlohmann::json& json){

                    auto concrete_instance = instance.As<TClass>();

                    if (concrete_instance){
                    
                        JsonDeserializer<TField>()(concrete_instance->*field, json);

                    }

                };

            }

            template<typename TClass, typename TGetter>
            JsonPropertySerializer(TGetter(TClass::* /*getter*/)() const) {

                //serializer_ =  lambda(getter)

            }

            template<typename TClass, typename TProperty>
            JsonPropertySerializer(TProperty(TClass::* /*getter*/)() const, void(TClass::* setter)(TProperty)) {

                deserializer_ = [setter](reflection::Instance instance, const nlohmann::json& json) {

                    auto concrete_instance = instance.As<TClass>();

                    if (concrete_instance) {

                        using TTemp = std::remove_cv_t<std::remove_reference_t<TProperty>>;

                        TTemp temp_value;

                        JsonDeserializer<TTemp>()(temp_value, json);

                        (concrete_instance->*setter)(temp_value);
                        
                    }

                };

            }

            template<typename TClass, typename TProperty>
            JsonPropertySerializer(const TProperty&(TClass::* /*getter*/)() const, TProperty&(TClass::* /*setter*/)()) {

                //serializer_ =  lambda(getter)

            }

            template <typename TInstance>
            void Deserialize(TInstance&& instance, const nlohmann::json& json) const{

                deserializer_(reflection::MakeInstance(instance),
                              json);

            }

            template <typename TInstance>
            void Serialize(const TInstance& instance, nlohmann::json& json) const{

                serializer_(reflection::MakeConstInstance(instance),
                            json);

            }

        private:

            std::function<void(reflection::Instance, const nlohmann::json&)> deserializer_;

            std::function<void(reflection::Instance, nlohmann::json&)> serializer_;

        };

        // Serializers

        template <typename TType, typename = void>
        struct JsonDeserializer {
        
            void operator()(TType& object, const nlohmann::json& json) {

                auto& object_class = reflection::ClassOf(object);

                for (auto json_property = json.cbegin(); json_property != json.cend(); ++json_property) {

                    auto object_property = object_class.GetProperty(json_property.key());

                    if (object_property) {

                        auto serializer = object_property->GetInterface<JsonPropertySerializer>();

                        if (serializer){

                            serializer->Deserialize(object, json_property.value());

                        }

                    }

                }

            }

        };

        template <typename TType>
        struct JsonDeserializer<TType*> {

            void operator()(TType*& /*object*/, const nlohmann::json& /*json*/) {



            }

        };

        template <typename TType>
        struct JsonDeserializer<TType, typename std::enable_if_t<std::is_arithmetic_v<TType>>> {

            void operator()(TType& object, const nlohmann::json& json) {

                conditional_assign(object, json.get<TType>());

            }

        };

        inline nlohmann::json ParseJSON(const char* path) {

            std::fstream file_stream(path);

            nlohmann::json json;

            if (file_stream.good()) {

                file_stream >> json;

            }

            return json;

        }

        template <typename TClass>
        bool DeserializeJSON(TClass& object, const nlohmann::json& json){

            JsonDeserializer<TClass>()(object, json);

            return true;

        }

        template <typename TClass>
        std::unique_ptr<TClass> DeserializeJSON(const nlohmann::json& json){

            TClass* object;

            JsonDeserializer<TClass>()(object, json);

            return object;

        }

        template <typename TClass>
        bool DeserializeJSON(TClass& object, const char* path) {

            return DeserializeJSON(object, ParseJSON(path));
            
        }

        template <typename TClass>
        std::unique_ptr<TClass> DeserializeJSON(const char* path) {

            return DeserializeJSON(ParseJSON(path));

        }
        


    }

}