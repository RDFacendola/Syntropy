
/// \file deserializers.h
/// \brief Contains functors used to deserialize JSON objects.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "reflection/reflection.h"

#include "nlohmann/json/src/json.hpp"

#include <string>
#include <memory>

namespace syntropy {

    namespace syntax {

        namespace serialization {

            class JSONDeserializable;

            //////////////// OBJECT DESERIALIZATION ////////////////

            /// \brief Functor used to deserialize JSON object into a concrete instance.
            /// This version uses the Syntropy reflection system to recursively deserialize object properties.
            /// \author Raffaele D. Facendola - September 2016
            template <typename TType, typename = void>
            struct JSONDeserializer {

                /// \brief Deserialize a JSON object inside a concrete object.
                /// Properties that are not declared by the JSON object are ignored.
                /// \param object Object to deserialize into.
                /// \param json JSON object to deserialize.
                /// \return Returns true if the JSON object contains an object, returns false otherwise.
                bool operator()(TType& object, const nlohmann::json& json) {

                    // Wraps the object inside an instance and use the reflection-based deserialization
                    return JSONDeserializer<reflection::Instance>()(reflection::MakeInstance(object), json);
                    
                }

            };

            /// \brief Functor used to deserialize JSON object into a reflection-instanced object.
            /// This version uses the Syntropy reflection system to recursively deserialize object properties.
            /// \author Raffaele D. Facendola - September 2016
            template <>
            struct JSONDeserializer<reflection::Instance> {

                /// \brief Deserialize a JSON object inside a concrete object.
                /// Properties that are not declared by the JSON object are ignored.
                /// \param object Object to deserialize into.
                /// \param json JSON object to deserialize.
                /// \return Returns true if the JSON object contains an object, returns false otherwise.
                bool operator()(reflection::Instance object, const nlohmann::json& json);

            };

            //////////////// POINTERS DESERIALIZATION ////////////////

            template <typename TType>
            struct JSONDeserializer<TType*> {

                bool operator()(TType*& object, const nlohmann::json& json) {

                    // DESIGN CHOICE: Pointers-to-pointers are not supported. It would make unclear who "owns" each level of indirectness and the concrete type of each of them. Additionally it would complicate the code below for no good reason.
                    // IMPORTANT: *Dumb* pointers are considered as owning pointers. Any previous value is deleted.

                    static_assert(std::is_default_constructible<TType>::value, "TType must be default constructible");
                    static_assert(!std::is_pointer_v<TType>, "Pointers-to-pointers cannot be deserialized");
                    
                    SafeDelete(object);

                    if (json.is_null()) {

                        return true;

                    }
                    
                    auto instance = InstantiateFromJSON(reflection::ClassOf<TType>(), json);

                    if (!instance) {

                        return false;

                    }

                    object = instance.As<TType>();

                    return true;

                }
                
            };

            template <typename TType>
            struct JSONDeserializer<std::unique_ptr<TType>> {

                bool operator()(std::unique_ptr<TType>& object, const nlohmann::json& json) {

                    static_assert(std::is_default_constructible<TType>::value, "TType must be default constructible");
                    static_assert(!std::is_pointer_v<TType>, "Pointers-to-pointers cannot be deserialized");

                    object.reset();             // Release the old object.

                    if (json.is_null()) {

                        return true;

                    }

                    auto instance = InstantiateFromJSON(reflection::ClassOf<TType>(), json);      // TODO: perform the instantiation via std::make_unique<TConcrete>

                    if (!instance) {

                        return false;

                    }

                    object = std::move(*instance.As<std::unique_ptr<TType>>());

                    return true;

                }

            };

            template <typename TType>
            struct JSONDeserializer<std::shared_ptr<TType>> {

                bool operator()(std::shared_ptr<TType>& object, const nlohmann::json& json) {

                    static_assert(std::is_default_constructible<TType>::value, "TType must be default constructible");
                    static_assert(!std::is_pointer_v<TType>, "Pointers-to-pointers cannot be deserialized");

                    object.reset();             // Release the old object.

                    if (json.is_null()) {

                        return true;

                    }

                    auto instance = InstantiateFromJSON(reflection::ClassOf<TType>(), json);      // TODO: perform the instantiation via std::make_shared<TConcrete>

                    if (!instance) {

                        return false;

                    }

                    object = *instance.As<std::shared_ptr<TType>>();

                    return true;

                }

            };

            //////////////// STRINGS DESERIALIZATION ////////////////

            /// \brief Functor used to deserialize a std::wstring from JSON.
            /// \author Raffaele D. Facendola - October 2016
            template <>
            struct JSONDeserializer<std::wstring> {

                /// \brief Deserialize a std::wstring from a JSON object property.
                /// \return Returns true if the JSON object contains a string value, returns false otherwise.
                bool operator()(std::wstring& object, const nlohmann::json& json) {

                    if (json.is_string()) {

                        object = to_wstring(json.get<std::string>());

                    }

                    return false;

                }

            };

            /// \brief Functor used to deserialize a std::string from JSON.
            /// \author Raffaele D. Facendola - October 2016
            template <>
            struct JSONDeserializer<std::string> {

                /// \brief Deserialize a std::string from a JSON object property.
                /// \return Returns true if the JSON object contains a string value, returns false otherwise.
                bool operator()(std::string& object, const nlohmann::json& json) {

                    if (json.is_string()) {

                        object = json.get<std::string>();

                    }

                    return false;

                }

            };

            //////////////// FUNDAMENTAL TYPES DESERIALIZATION ////////////////

            /// \brief Functor used to deserialize a boolean variable from JSON.
            /// \author Raffaele D. Facendola - October 2016
            template <>
            struct JSONDeserializer<bool> {

                /// \brief Deserialize a boolean variable from a JSON object property.
                /// \return Returns true if the JSON object contains a boolean value, returns false otherwise.
                bool operator()(bool& object, const nlohmann::json& json) {

                    if (json.is_boolean()) {

                        object = json.get<bool>();

                        return true;

                    }

                    return false;

                }

            };

            /// \brief Functor used to deserialize a numeric-variable from JSON.
            /// \author Raffaele D. Facendola - September 2016
            template <typename TType>
            struct JSONDeserializer<TType, typename std::enable_if_t<std::is_arithmetic_v<TType> && !std::is_const<TType>::value>> {
            
                /// \brief Deserialize a numeric variable from a JSON object property.
                /// \return Returns true if the JSON object contains a numeric value, returns false otherwise.
                bool operator()(TType& object, const nlohmann::json& json) {

                    if (json.is_number()) {

                        object = json.get<TType>();

                        return true;

                    }

                    return false;

                }

            };

            //////////////// NON-DESERIALIZABLE ////////////////

            /// \brief Functor used to handle const objects.
            /// Since const objects cannot be deserialized, this functor does nothing.
            /// \author Raffaele D. Facendola - September 2016
            template <typename TType>
            struct JSONDeserializer<const TType> {

                /// \brief Do nothing.
                /// \return Returns false.
                bool operator()(const TType&, const nlohmann::json&) {

                    return false;

                }

            };

        }

    }

}