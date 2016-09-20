/// \file json.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <fstream>
#include <memory>
#include <functional>

#include "reflection/reflection.h"

#include "serialization/json/json_deserializer.h"

namespace syntropy {

    namespace serialization {

        class IJsonDeserializer{

        public:

            IJsonDeserializer() {}

            template<typename TClass, typename TField>
            IJsonDeserializer(TField TClass::* field){

                deserializer_ = [field](reflection::Instance instance, const nlohmann::json& json){

                    auto concrete_instance = instance.As<TClass>();

                    if (concrete_instance){
                    
                        JsonDeserializer<TField>()(concrete_instance->*field, json);

                    }

                };

            }

            template<typename TClass, typename TProperty>
            IJsonDeserializer(TProperty(TClass::*)() const) {

                deserializer_ = [](reflection::Instance, const nlohmann::json&) {

                    // Read-only property

                };

            }

            template<typename TClass, typename TProperty, typename TReturn>
            IJsonDeserializer(TReturn(TClass::* setter)(TProperty)) {

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
            IJsonDeserializer(TProperty&(TClass::* setter)()) {

                deserializer_ = [setter](reflection::Instance instance, const nlohmann::json& json) {

                    auto concrete_instance = instance.As<TClass>();

                    if (concrete_instance) {

                        JsonDeserializer<TProperty>()((concrete_instance->*setter)(), json);

                    }

                };

            }

            template <typename TInstance>
            void operator()(TInstance&& instance, const nlohmann::json& json) const{

                deserializer_(reflection::MakeInstance(instance),
                              json);

            }

        private:

            std::function<void(reflection::Instance, const nlohmann::json&)> deserializer_;

        };

         // Property interface
         struct JsonDeserializable {
 
             IJsonDeserializer operator()() const {

                 return IJsonDeserializer();

             }

         };

        // Utilities

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

namespace syntropy {

    namespace serialization {

    }

}