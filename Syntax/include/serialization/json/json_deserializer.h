
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

            /// \brief Deserializer for maps.
            /// Maps can either be deserialized from an array of object or from a single object.
            /// In the first case one field of each object in the array is used as a key for the entry, the object itself is deserialized as the value.
            /// In the second case the map expects to deserialize an object in which each field-value pair is interpreted as a key-value to add to the map.
            /// In this case keys must be string-constructible since JSON fields are identified by strings. Deserializing an array of objects has no such limitation.
            template <typename TMap>
            struct JSONDeserializer<TMap, std::enable_if_t<is_map_v<TMap>>> {

                using TKey = typename TMap::key_type;
                using TValue = typename TMap::mapped_type;

                bool operator()(TMap& object, const nlohmann::json& json) {

                    if (json.is_array()) {

                        // Array of objects
                        DeserializeFromArray(object, json);
                        return true;

                    }
                    else if (json.is_object()) {

                        // Object fields are the key-value pairs of the map. 
                        // TKey must be constructible from strings.
                        return KeyValuePairDeserializer<TKey>()(object, json);

                    }

                    return false;

                }

            private:

                /// \brief Deleter for deserialized values.
                /// A map requires that both the key and the value are correctly deserialized. If one of the two fails, the other value must
                /// be destroyed if it was deserialized by this object, otherwise it would leak.
                template <typename TType>
                struct Deleter {

                    static void Delete(TType) {}

                };

                /// \brief Specialization for pointer types
                template <typename TType>
                struct Deleter<TType*> {

                    static void Delete(TType*& pointer) {

                        SafeDelete(pointer);

                    }

                };

                /// \brief Key-value pair deserializer for keys that are not convertible from string. Does nothing.
                template <typename TTKey, typename = void>
                struct KeyValuePairDeserializer {

                    bool operator()(TMap&, const nlohmann::json&) const {

                        return false;

                    }

                };

                /// \brief Key-value pair deserializer for keys that can be converted from strings.
                template <typename TTKey>
                struct KeyValuePairDeserializer<TTKey, std::enable_if_t<is_convertible_v<nlohmann::json::string_t, TKey>>> {

                    bool operator()(TMap& object, const nlohmann::json& json) const {

                        TKey key;
                        TValue value;

                        for (auto json_property = json.cbegin(); json_property != json.cend(); ++json_property) {

                            key = convert<nlohmann::json::string_t, TKey>()(json_property.key());               // Key from string

                            if (JSONDeserializer<TValue>()(value, json_property.value())) {                     // Value from anything

                                // Update the map
                                object.insert(std::make_pair(std::move(key), 
                                                             std::move(value)));

                            }

                        }

                        return true;

                    }

                };

                /// \brief Deserialize from an array of objects: the elements are the mapped objects, while one particular field is used as a key.
                void DeserializeFromArray(TMap& object, const nlohmann::json& json) {

                    TKey key;
                    TValue value;

                    for (unsigned int array_index = 0; array_index < json.size(); ++array_index) {

                        auto& json_item = json[array_index];

                        if (json_item.is_object()) {

                            key = TKey{};
                            value = TValue{};

                            auto it = json_item.find(JSONDeserializable::kIdToken);

                            if (it != json_item.end() &&
                                JSONDeserializer<TKey>()(key, *it) &&                                           // Key
                                JSONDeserializer<TValue>()(value, json_item)) {                                 // Value

                                // Update the map
                                object.insert(std::make_pair(std::move(key), 
                                                             std::move(value)));

                            }
                            else {

                                // One among the key and value failed to deserialize: destroy any allocated object.
                                Deleter<TKey>::Delete(key);
                                Deleter<TValue>::Delete(value);

                            }

                        }

                    }

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

                        return true;

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

                        return true;

                    }

                    return false;

                }

            };

            /// \brief Functor used to deserialize a HashedString from JSON.
            /// \author Raffaele D. Facendola - November 2016
            template <>
            struct JSONDeserializer<HashedString> {

                /// \brief Deserialize a std::string from a JSON object property.
                /// \return Returns true if the JSON object contains a string value, returns false otherwise.
                bool operator()(HashedString& object, const nlohmann::json& json) {

                    if (json.is_string()) {

                        object = json.get<std::string>();

                        return true;

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