/// \file json.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <fstream>
#include <memory>

#include "reflection/reflection.h"

#include "nlohmann/json/src/json.hpp"

namespace syntropy {

    namespace serialization {

        class JsonProperty;

        class JsonClass {

        public:

            template <typename TClass>
            class Definition;

            /// \brief Get the JSON class associated to TClass.
            /// \return Returns a reference to the singleton describing the JSON class TClass.
            template <typename TClass>
            static const JsonClass& GetClass();

            /// \brief No copy constructor.
            JsonClass(const JsonClass&) = delete;

            /// \brief No assignment operator.
            JsonClass& operator=(const JsonClass&) = delete;

            /// \brief Virtual destructor.
            virtual ~JsonClass() = default;

            /// \brief Get the class associated to this JSON class object.
            const reflection::Class& GetClass() const;

            /// \brief Get a json class property by name.
            /// \param property_name Name of the property to get.
            /// \return Returns a pointer to the requested property, if any. Returns nullptr otherwise.
            virtual const JsonProperty* GetProperty(const HashedString& property_name) const noexcept = 0;

        private:

            template <typename TClass>
            class ClassT;

            /// \brief Default constructor.
            JsonClass() = default;

        };

        class JsonProperty {

        public:


        };

        template <typename TType>
        struct JsonDeserializer {
        
            void operator()(TType& object, const nlohmann::json& json) {

                auto& object_class = reflection::ClassOf(object);

                const reflection::Property* object_property;

                for (auto json_property = json.cbegin(); json_property != json.cend(); ++json_property) {

                    object_property = object_class.GetProperty(json_property.key());

                    if (object_property) {

                        

                    }

                }

            }

        };

        template <typename TType>
        struct JsonDeserializer<TType*> {

            void operator()(TType*& /*object*/, const nlohmann::json& /*json*/) {



            }

        };

        template <>
        struct JsonDeserializer<int> {

            void operator()(int& /*object*/, const nlohmann::json& /*json*/) {



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
        bool DeserializeJSON(TClass& object, const char* path) {

            JsonDeserializer<TClass>()(object, ParseJSON(path));

            return true;

        }

        template <typename TClass>
        std::unique_ptr<TClass> DeserializeJSON(const char* path) {

            TClass* object;

            JsonDeserializer<TClass>()(object, ParseJSON(path));

            return object;

        }
        
    }

}