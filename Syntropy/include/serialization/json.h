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

        class JSONDeserializable{

        public:

            template <typename TInstance>
            void operator()(TInstance&& instance, const nlohmann::json& json) const {

                Deserialize(reflection::MakeInstance(instance), json);

            }

        protected:

            virtual void Deserialize(reflection::Instance, const nlohmann::json&) const = 0;

        };

        template <typename... TAccessors>
        class JSONDeserializableT;

        template <typename TClass, typename TField>
        class JSONDeserializableT<TField TClass::*> : public JSONDeserializable{

        public:

            JSONDeserializableT(TField TClass::* field)
                : field_(field){}

        private:

            virtual void Deserialize(reflection::Instance instance, const nlohmann::json& json) const override {

                auto concrete_instance = instance.As<TClass>();

                if (concrete_instance){

                    JsonDeserializer<TField>()(concrete_instance->*field, json);

                }

            }

            TField TClass::* field_;                ///< \brief Member field.

        };

        template <typename TClass, typename TProperty>
        class JSONDeserializableT<TProperty(TClass::*)() const> : public JSONDeserializable {

        public:

            JSONDeserializableT(TProperty(TClass::*)() const) {}

        private:

            virtual void Deserialize(reflection::Instance, const nlohmann::json&) const override {

                // Read-only

            }

        };

        template <typename TClass, typename TProperty, typename TReturn>
        class JSONDeserializableT<TProperty(TClass::*)() const, TReturn(TClass::*)(TProperty)> : public JSONDeserializable {

        public:

            JSONDeserializableT(TProperty(TClass::*)() const, TReturn(TClass::* setter)(TProperty))
                : setter_(setter) {}
            
        private:

            virtual void Deserialize(reflection::Instance instance, const nlohmann::json& json) const override {

                auto concrete_instance = instance.As<TClass>();

                if (concrete_instance){

                    JsonDeserializer<TProperty>()((concrete_instance->*setter_)(), json);

                }

            }

            TReturn(TClass::* setter_)(TProperty);          ///< \brief Setter method of the property.

        };

        template <typename TClass, typename TProperty>
        class JSONDeserializableT<const TProperty&(TClass::*)() const, TProperty&(TClass::*)()> : public JSONDeserializable {

        public:

            JSONDeserializableT(const TProperty&(TClass::*)() const, TProperty&(TClass::* setter)())
                : setter_(setter) {}

        private:

            virtual void Deserialize(reflection::Instance instance, const nlohmann::json& json) const override {

                auto concrete_instance = instance.As<TClass>();

                if (concrete_instance){

                    JsonDeserializer<TProperty>()((concrete_instance->*setter_)(), json);

                }

            }

            TProperty&(TClass::* setter_)();                   ///< \brief Setter method of the property.

        };

        // Property interface

        struct JsonDeserializable {
 
            template <typename... TAccessors>
            void operator()(reflection::Property& property, TAccessors&&... accessors) const {

                property.AddVirtualInterface<JSONDeserializable, JSONDeserializableT<std::remove_reference_t<TAccessors>...>>(std::forward<TAccessors>(accessors)...);

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