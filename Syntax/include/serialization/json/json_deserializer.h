
/// \file deserializers.h
/// \brief Contains functors used to deserialize JSON objects.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "syntropy.h"
#include "hashed_string.h"

#include "reflection/reflection.h"
#include "nlohmann/json/src/json.hpp"

#include <string>
#include <memory>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

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

            // Pointers-to-raw-pointers are not supported since it requires to make bold assumptions about ownership of each level of indirection.

            template <typename TType>
            struct JSONDeserializer<TType*, std::enable_if_t<!std::is_pointer<TType>::value>> {

                bool operator()(TType*& object, const nlohmann::json& json) {

                    // IMPORTANT: Raw pointers are considered *owning* pointers. 
                    //            The client must ensure that the current pointer is initialized and the object is not referenced elsewhere.
                    //            Consider using smart pointers instead.

                    SafeDelete(object);

                    if (json.is_null()) {

                        return true;

                    }
                    
                    auto instance = InstantiateFromJSON(reflection::ClassOf<TType>(), json);

                    if (!instance) {

                        return false;

                    }

                    object = instance.As<TType>();          // Move instance ownership.

                    return true;

                }
                
            };

            template <typename TType>
            struct JSONDeserializer<std::unique_ptr<TType>, std::enable_if_t<!std::is_pointer<TType>::value>> {

                bool operator()(std::unique_ptr<TType>& object, const nlohmann::json& json) {

                    object.reset();                         // Release the old object.

                    if (json.is_null()) {

                        return true;

                    }

                    // Note: it won't use std::make_unique since we don't know the concrete type at compile time. This doesn't affect performances like std::make_shared though.
                    // We could use a class interface but that would require us to add it to *every* class in the project.

                    auto instance = InstantiateFromJSON(reflection::ClassOf<TType>(), json);

                    if (!instance) {

                        return false;

                    }

                    object.reset(instance.As<TType>());     // Move instance ownership.

                    return true;

                }

            };

            template <typename TType>
            struct JSONDeserializer<std::shared_ptr<TType>, std::enable_if_t<!std::is_pointer<TType>::value>> {

                bool operator()(std::shared_ptr<TType>& object, const nlohmann::json& json) {

                    object.reset();                         // Release the old object.

                    if (json.is_null()) {

                        return true;

                    }

                    // Note: it won't use std::make_shared since we don't know the concrete type at compile time. This can affect peformances since the control block won't be allocated near the object.
                    // We don't expect many shared_ptr being deserialized this way (if any at all) so the performance penalty should be negligible.
                    // We could use a class interface but that would require us to add it to *every* class in the project.

                    auto instance = InstantiateFromJSON(reflection::ClassOf<TType>(), json);

                    if (!instance) {

                        return false;

                    }

                    object.reset(instance.As<TType>());     // Move instance ownership.

                    return true;

                }

            };

            //////////////// VECTOR DESERIALIZATION ////////////////

            template <typename TType>
            struct JSONDeserializer<std::vector<TType>> {

                bool operator()(std::vector<TType>& object, const nlohmann::json& json) {

                    if (json.is_array()) {

                        TType item;

                        object.reserve(json.size());

                        for (unsigned int array_index = 0; array_index < json.size(); ++array_index) {

                            if (JSONDeserializer<TType>()(item, json[array_index])) {

                                object.push_back(std::move(item));

                            }

                        }

                        return true;

                    }

                    return false;

                }

            };

            //////////////// SETS DESERIALIZATION ////////////////

            template <typename TSet>
            struct JSONDeserializer<TSet, std::enable_if_t<is_set_v<TSet>>> {

                bool operator()(TSet& object, const nlohmann::json& json) {

                    if (json.is_array()) {

                        TSet::value_type item;

                        for (unsigned int array_index = 0; array_index < json.size(); ++array_index) {

                            if (JSONDeserializer<TSet::value_type>()(item, json[array_index])) {

                                object.insert(std::move(item));

                            }

                        }

                        return true;

                    }

                    return false;

                }

            };

            //////////////// MAPS DESERIALIZATION ////////////////
            
            template <typename TConvert>
            struct json_map_key_converter : std::false_type {};

            template <>
            struct json_map_key_converter<std::string> : std::true_type {
            
                std::string operator()(const std::string& key) const {
                    
                    return key;

                }

            };

            template <>
            struct json_map_key_converter<std::wstring> : std::true_type {

                std::wstring operator()(const std::string& key) const {

                    return to_wstring(key);

                }

            };

            template <>
            struct json_map_key_converter<HashedString> : std::true_type {

                HashedString operator()(const std::string& key) const {

                    return key;

                }

            };

            template <typename TMap>
            struct JSONDeserializer<TMap, std::enable_if_t<std::conditional_t<is_map_v<TMap>,
                                                                              json_map_key_converter<typename TMap::key_type>,
                                                                              std::false_type>::value>> {

                bool operator()(TMap& object, const nlohmann::json& json) {

                    if (json.is_object()) {

                        TMap::mapped_type item;

                        for (auto json_property = json.cbegin(); json_property != json.cend(); ++json_property) {

                            if (JSONDeserializer<TMap::mapped_type>()(item, json_property.value())) {
                                
                                object.insert(std::make_pair(json_map_key_converter<typename TMap::key_type>()(json_property.key()),
                                                             item));

                            }

                        }

                        return true;

                    }

                    return false;

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