
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

namespace syntropy::serialization
{
    /************************************************************************/
    /* VECTOR                                                               */
    /************************************************************************/

    /// \brief Partial specialization of JSONDeserializerT for std::vector.
    template <typename TType>
    struct JSONDeserializerT<std::vector<TType>>
    {
        std::optional<std::vector<TType>> operator()(const nlohmann::json& json) const
        {
            if (json.is_array())
            {
                auto vector = std::make_optional<std::vector<TType>>();

                vector->reserve(json.size());

                for (auto array_index = 0u; array_index < json.size(); ++array_index)
                {
                    if (auto item = JSONDeserializer<TType>(json[array_index]))
                    {
                        vector->emplace_back(std::move(*item));
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

    /// \brief Partial specialization of JSONDeserializerT for std::set and std::unordered_set.
    template <typename TSet>
    struct JSONDeserializerT<TSet, std::enable_if_t<is_set_v<TSet>>>
    {
        std::optional<TSet> operator()(const nlohmann::json& json) const
        {
            if (json.is_array())
            {
                auto set = std::make_optional<TSet>();

                for (auto array_index = 0u; array_index < json.size(); ++array_index)
                {
                    if (auto item = JSONDeserializer<TSet::value_type>(json[array_index]))
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

    /// \brief Partial specialization of JSONDeserializerT for std::map and std::unordered_map.
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

        /// \brief Deserialize from an array of objects: the elements are the mapped objects, while one particular field is used as a key.
        std::optional<TMap> DeserializeFromArray(const nlohmann::json& json) const
        {
            /// \brief JSON property field used to determine the id of an object.
            static const char kIdToken[] = "id";

            auto map = std::make_optional<TMap>();

            for (unsigned int array_index = 0; array_index < json.size(); ++array_index)
            {
                if (auto& json_item = json[array_index]; json_item.is_object())
                {
                    if (auto key_it = json_item.find(kIdToken); key_it != json_item.cend())
                    {
                        if (auto key = JSONDeserializer<TKey>(*key_it))
                        {
                            if (auto value = JSONDeserializer<TValue>(json_item))
                            {
                                map->insert(std::make_pair(std::move(*key), std::move(*value)));
                            }
                            else if constexpr(std::is_pointer_v<TKey>)
                            {
                                delete *key;        // Avoid leaking the key when a the value could not be deserialized and the key represents a raw pointer.
                            }
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
            if constexpr(std::is_constructible_v<TKey, nlohmann::json::object_t::key_type>)
            {
                auto map = std::make_optional<TMap>();

                for (auto json_property = json.cbegin(); json_property != json.cend(); ++json_property)
                {
                    if (auto value = JSONDeserializer<TValue>(json_property.value()))
                    {
                        map->insert(std::make_pair(json_property.key(), std::move(*value)));
                    }
                }

                return map;
			}
			else
			{
				return std::nullopt;
			}
        }

    };

    /************************************************************************/
    /* STRING                                                               */
    /************************************************************************/

    /// \brief Partial specialization of JSONDeserializerT for std::string.
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

    /// \brief Partial specialization of JSONDeserializerT for std::unique_ptr.
    template <typename TType>
    struct JSONDeserializerT<std::unique_ptr<TType> >
    {
        std::optional<std::unique_ptr<TType>> operator()(const nlohmann::json& json) const
        {
            if (auto ptr = JSONDeserializer<TType*>(json))      // Deserialize as raw pointer.
            {
                return std::unique_ptr<TType>(*ptr);            // Wrap inside an unique_ptr.
            }

            return std::nullopt;
        }

    };

    /// \brief Partial specialization of JSONDeserializerT for std::shared_ptr.
    template <typename TType>
    struct JSONDeserializerT<std::shared_ptr<TType> >
    {
        std::optional<std::shared_ptr<TType>> operator()(const nlohmann::json& json) const
        {
            if (auto ptr = JSONDeserializer<TType*>(json))      // Deserialize as raw pointer.
            {
                return std::shared_ptr<TType>(*ptr);            // Wrap inside a shared_ptr. Note: the control block won't be allocated near the object (we don't know the concrete type at compile time).
            }

            return std::nullopt;
        }
    };
}