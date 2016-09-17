
/// \file deserializers.h
/// \brief Contains functors used to deserialize JSON objects.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "reflection/property.h"

#include "nlohmann/json/src/json.hpp"

namespace syntropy {

    namespace serialization {

        class IJsonDeserializer;

        template <typename TType, typename = void>
        struct JsonDeserializer {

            void operator()(TType& object, const nlohmann::json& json) {

                auto& object_class = reflection::ClassOf(object);

                for (auto json_property = json.cbegin(); json_property != json.cend(); ++json_property) {

                    auto object_property = object_class.GetProperty(json_property.key());

                    if (object_property) {

                        auto serializer = object_property->GetInterface<IJsonDeserializer>();

                        if (serializer) {

                            (*serializer)(object, json_property.value());

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

    }

}