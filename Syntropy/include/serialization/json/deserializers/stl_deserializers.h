
/// \file stl_deserializers.h
/// \brief This header is part of the syntropy serialization system. It contains deserializers for STL types.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <string>
#include <memory>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <optional>

#include "syntropy.h"
#include "type_traits.h"

#include "serialization/json/json.h"

namespace syntropy
{
    namespace serialization
    {

        /************************************************************************/
        /* VECTOR                                                               */
        /************************************************************************/

        template <typename TType>
        struct JSONDeserializerT<std::vector<TType>>
        {
            std::optional<std::vector<TType>> operator()(const nlohmann::json& json) const
            {
                if (json.is_array())
                {
                    auto vector = std::make_optional<std::vector<TType>>();

                    vector->reserve(json.size());

                    for (unsigned int array_index = 0; array_index < json.size(); ++array_index)
                    {
                        auto item = JSONDeserializer<TType>(json[array_index]);

                        if (item)
                        {
                            vector->push_back(std::move(*item));
                        }
                    }

                    return vector;
                }

                return std::nullopt;
            }
        };

        /************************************************************************/
        /* SETS                                                                 */
        /************************************************************************/

        template <typename TSet>
        struct JSONDeserializerT<TSet, std::enable_if_t<is_set_v<TSet>>>
        {
            std::optional<TSet> operator()(const nlohmann::json& json) const
            {
                if (json.is_array())
                {
                    auto set = std::make_optional<TSet>();

                    // Each array element is deserialized as a set item.
                    for (unsigned int array_index = 0; array_index < json.size(); ++array_index)
                    {
                        auto item = JSONDeserializer<TSet::value_type>(json[array_index]);

                        if (item)
                        {
                            set->emplace(std::move(*item));
                        }
                    }

                    return set;
                }

                return std::nullopt;
            }
        };

        /************************************************************************/
        /* MAPS                                                                 */
        /************************************************************************/

        template <typename TMap>
        struct JSONDeserializerT<TMap, std::enable_if_t<is_map_v<TMap>>>
        {


            std::optional<TMap> operator()(const nlohmann::json& json) const
            {
                if (json.is_array())
                {
                    return DeserializeFromArray(json);      // Store each element as a map entry. The first field is used as key.
                }
                else if (json.is_object())
                {
                    return DeserializeFromObject(json);     // Store each property as a map entry. The key is the property name, the value is the deserialized property value.
                }

                return std::nullopt;
            }

        private:

            using TKey = typename TMap::key_type;
            using TValue = typename TMap::mapped_type;

            /// \brief Deleter for deserialized values.
            /// A map requires that both the key and the value are correctly deserialized. If one of the two fails, both must be destroyed to prevent a memory leak.
            template <typename TType>
            struct Deleter
            {
                static void Delete(std::optional<TType>&) {}
            };

            /// \brief Specialization for pointer types.
            template <typename TType>
            struct Deleter<TType*>
            {
                static void Delete(std::optional<TType*>& pointer)
                {
                    if (pointer)
                    {
                        delete *pointer;
                    }
                }
            };

            /// \brief Key-value pair deserializer for keys that are not constructible from string. Does nothing.
            template <typename TTKey, typename = void>
            struct KeyValuePairDeserializer
            {
                std::optional<TMap> operator()(const nlohmann::json&) const
                {
                    return std::nullopt;
                }
            };

            /// \brief Key-value pair deserializer for keys that can be converted from strings.
            template <typename TTKey>
            struct KeyValuePairDeserializer<TTKey, std::enable_if_t<std::is_constructible_v<TKey, nlohmann::json::object_t::key_type>>>
            {
                std::optional<TMap> operator()(const nlohmann::json& json) const
                {
                    auto map = std::make_optional<TMap>();

                    for (auto json_property = json.cbegin(); json_property != json.cend(); ++json_property)
                    {
                        auto value = JSONDeserializer<TValue>(json_property.value());

                        if (value)
                        {
                            map->insert(std::make_pair(json_property.key(), std::move(*value)));
                        }
                    }

                    return map;
                }
            };

            /// \brief Deserialize from an array of objects: the elements are the mapped objects, while one particular field is used as a key.
            std::optional<TMap> DeserializeFromArray(const nlohmann::json& json) const
            {
                /// \brief JSON property field used to determine the id of an object.
                static const char kIdToken[] = "id";

                auto map = std::make_optional<TMap>();

                for (unsigned int array_index = 0; array_index < json.size(); ++array_index)
                {
                    auto& json_item = json[array_index];

                    if (json_item.is_object())
                    {
                        auto key_it = json_item.find(kIdToken);

                        if (key_it != json_item.cend())
                        {
                            auto key = JSONDeserializer<TKey>(*key_it);
                            auto value = JSONDeserializer<TValue>(json_item);

                            if (key && value)
                            {
                                map->insert(std::make_pair(std::move(*key), std::move(*value)));
                            }
                            else
                            {
                                // If either the key or the value couldn't be deserialized, the entire pair is discarded and deleted.
                                Deleter<TKey>::Delete(key);
                                Deleter<TValue>::Delete(value);
                            }
                        }
                    }
                }

                return map;
            }

            /// \brief Deserialize from an object: each property is interpreted as a key-value pair, where the key is the property name and the value the property value.
            /// TKey must be constructible from string.
            std::optional<TMap> DeserializeFromObject(const nlohmann::json& json) const
            {
                return KeyValuePairDeserializer<TKey>()(json);
            }

        };

        /************************************************************************/
        /* STRING                                                               */
        /************************************************************************/

        template <>
        struct JSONDeserializerT<std::wstring>
        {
            std::optional<std::wstring> operator()(const nlohmann::json& json) const
            {
                if (json.is_string())
                {
                    //return to_wstring(json.get<std::string>());
                }
                return std::nullopt;
            }
        };

        template <>
        struct JSONDeserializerT<std::string>
        {
            std::optional<std::string> operator()(const nlohmann::json& json) const
            {
                if (json.is_string())
                {
                    return json.get<std::string>();
                }
                return std::nullopt;
            }
        };

        /************************************************************************/
        /* SMART POINTERS                                                       */
        /************************************************************************/

        template <typename TType>
        struct JSONDeserializerT<std::unique_ptr<TType> >
        {
            std::optional<std::unique_ptr<TType>> operator()(const nlohmann::json& json) const
            {
                auto ptr = JSONDeserializer<TType*>(json);      // Deserialize as raw pointer

                if (ptr)
                {
                    return std::unique_ptr<TType>(*ptr);        // Wrap inside an unique_ptr.
                }

                return std::nullopt;
            }

        };

        template <typename TType>
        struct JSONDeserializerT<std::shared_ptr<TType> >
        {
            std::optional<std::shared_ptr<TType>> operator()(const nlohmann::json& json) const
            {
                auto ptr = JSONDeserializer<TType*>(json);      // Deserialize as raw pointer

                if (ptr)
                {
                    return std::shared_ptr<TType>(*ptr);        // Wrap inside a shared_ptr. Note: the control block won't be allocated near the object (we don't know the concrete type at compile time).
                }

                return std::nullopt;
            }
        };

    }

}