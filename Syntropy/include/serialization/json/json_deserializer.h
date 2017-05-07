
/// \file deserializers.h
/// \brief Contains functors used to deserialize JSON objects.
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

#include "containers/hashed_string.h"

#include "reflection/reflection.h"
#include "reflection/class.h"
#include "reflection/any.h"

#include "diagnostics/log.h"

#include "nlohmann/json/src/json.hpp"

namespace syntropy
{
    namespace serialization
    {

        /// \brief Deserialize an object properties from JSON.
        /// This method enumerates the properties defined by the JSON and attempts to deserialize the corresponding object properties directly.
        /// Properties that are not defined either by the JSON or by the object are ignored and any existing state is preserved.
        /// \param object Object to deserialize. It must contain a pointer to the concrete type to deserialize.
        /// \param json Source JSON object to parse.
        void DeserializeObjectFromJSON(const reflection::Any& object, const nlohmann::json& json);

        /// \brief Deserialize an object properties from JSON.
        /// This overload only participates in overload resolution if TInstance is not reflection::Any.
        /// This method enumerates the properties defined by the JSON and attempts to deserialize the corresponding object properties directly.
        /// Properties that are not defined either by the JSON or by the object are ignored and any existing state is preserved.
        /// \param object Object to deserialize.
        /// \param json Source JSON object to parse.
        template <typename TInstance, typename = std::enable_if_t<!std::is_same_v<std::decay_t<TInstance>, reflection::Any>>>
        void DeserializeObjectFromJSON(TInstance& object, const nlohmann::json& json)
        {
            return DeserializeObjectFromJSON(std::addressof(object), json);
        }

        /************************************************************************/
        /* OBJECT DESERIALIZATION                                               */
        /************************************************************************/

        /// \brief Functor used to deserialize an object from JSON.
        /// \author Raffaele D. Facendola - September 2016
        template <typename TType, typename = void>
        struct JSONDeserializerT
        {
            std::optional<TType> operator()(const nlohmann::json& json) const
            {
                if (json.is_object())
                {
                    auto object = std::make_optional<TType>();

                    DeserializeObjectFromJSON(&(object.value()), json);

                    return object;
                }

                return std::nullopt;
            }
        };

        /// \brief Utility object for JSONDeserializerT.
        /// Usage: JSONDeserializer<TType>(json) instead of JSONDeserializerT<TType>{}(json)
        /// \author Raffaele D. Facendola - May 2017
        template <typename TType>
        constexpr JSONDeserializerT<TType> JSONDeserializer{};

        /************************************************************************/
        /* POINTERS DESERIALIZATION                                             */
        /************************************************************************/

        /// \brief Functor used to deserialize a pointer to an object from JSON.
        /// The actual concrete object instantiated by this functor depends on the class defined by the JSON itself.
        /// IMPORTANT: Providing a custom template specialization for this functor is not advised! It must take into account polymorphism.
        /// \author Raffaele D. Facendola - September 2016
        template <typename TType>
        struct JSONDeserializerT<TType*, std::enable_if_t<!std::is_pointer<TType>::value>>
        {
            /// \brief JSON property field used to determine the class type of the object being deserialized.
            static constexpr char* kClassToken = "$class";

            std::optional<TType*> operator()(const nlohmann::json& json) const
            {
                auto concrete_class = GetClassFromJSON(json);

                if (!concrete_class)
                {
                    return std::nullopt;        // The class was invalid.
                }

                // Attempts a direct construction of the object via JSON (if the construction fails fall back to recursive property deserialization)

                auto json_constructible = concrete_class->GetInterface<JSONConstructible>();

                if (json_constructible)
                {
                    auto instance = (*json_constructible)(json);

                    if (instance.HasValue())
                    {
                        return reflection::AnyCast<TType*>(instance);
                    }
                }

                // Attempts to default-construct the object and deserialize each of its properties recursively.

                auto default_constructible = concrete_class->GetInterface<reflection::Constructible<>>();

                if (default_constructible)
                {
                    auto instance = (*default_constructible)();

                    DeserializeObjectFromJSON(instance, json);

                    return reflection::AnyCast<TType*>(instance);
                }

                // The object is neither default-constructible nor JSON-constructible.
                return std::nullopt;
            }

        private:

            /// \brief Get the class associated to the provided JSON object.
            /// \return Returns the class associated to the provided JSON object. If the class is not compatible or non-existent, returns nullptr.
            const reflection::Class* GetClassFromJSON(const nlohmann::json& json) const
            {
                auto& base_class = reflection::ClassOf<TType>();

                auto it = json.find(kClassToken);

                if (it == json.end())
                {
                    return &base_class;         // No class was specified: use TType.
                }

                if (!it->is_string())
                {
                    SYNTROPY_WARNING((SerializationCtx), "Expected a string value for the property '", kClassToken, "'.");
                    return nullptr;
                }

                auto concrete_class = reflection::GetClass(it->get<std::string>());

                if (concrete_class == nullptr)
                {
                    SYNTROPY_WARNING((SerializationCtx), "Unnrecognized class '", it->get<std::string>(), "'.");
                    return nullptr;
                }

                if (*concrete_class != base_class)
                {
                    SYNTROPY_WARNING((SerializationCtx), "Cannot deserialize an object of type '", base_class, "' from type ", it->get<std::string>(), ".");
                    return nullptr;
                }

                return concrete_class;          // Use a derived class.
            }

        };

        /// \brief Functor used to deserialize an unique pointer to an object from JSON.
        /// The actual concrete object instantiated by this functor depends on the class defined by the JSON itself.
        /// \author Raffaele D. Facendola - September 2016
        template <typename TType>
        struct JSONDeserializerT<std::unique_ptr<TType>, std::enable_if_t<!std::is_pointer<TType>::value>>
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

        /// \brief Functor used to deserialize a shared pointer to an object from JSON.
        /// The actual concrete object instantiated by this functor depends on the class defined by the JSON itself.
        /// \author Raffaele D. Facendola - September 2016
        template <typename TType>
        struct JSONDeserializerT<std::shared_ptr<TType>, std::enable_if_t<!std::is_pointer<TType>::value>>
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

        /************************************************************************/
        /* VECTOR DESERIALIZATION                                               */
        /************************************************************************/

        /// \brief Functor used to deserialize a vector of objects from JSON.
        /// \author Raffaele D. Facendola - October 2016
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
                            vector->push_back(*std::move(item));
                        }
                    }

                    return vector;
                }

                return std::nullopt;
            }
        };

        /************************************************************************/
        /* SET DESERIALIZATION                                                  */
        /************************************************************************/

        /// \brief Functor used to deserialize a set of objects from JSON.
        /// \author Raffaele D. Facendola - October 2016
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
                            set->emplace(*std::move(item));
                        }
                    }

                    return set;
                }

                return std::nullopt;
            }
        };

        /************************************************************************/
        /* MAP DESERIALIZATION                                                  */
        /************************************************************************/

        /// \brief Functor used to deserialize a map of objects from JSON.
        /// This functor supports deserialization from either an array or an object:
        /// In the first case, each element is deserialized as a map entry. One of its field (kIdToken) is used as a key.
        /// In the second case, each object property is interpreted as a key-value pair, where the key is the property name and the value is the deserialized object value.
        /// \author Raffaele D. Facendola - October 2016
        template <typename TMap>
        struct JSONDeserializerT<TMap, std::enable_if_t<is_map_v<TMap>>>
        {
            /// \brief JSON property field used to determine the id of an object.
            static constexpr char* kIdToken = "id";

            std::optional<TMap> operator()(const nlohmann::json& json) const
            {
                if (json.is_array())
                {
                    return DeserializeFromArray(json);
                }
                else if (json.is_object())
                {
                    return DeserializeFromObject(json);
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

            /// \brief Specialization for pointer types
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

            /// \brief Key-value pair deserializer for keys that are not convertible from string. Does nothing.
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
            struct KeyValuePairDeserializer<TTKey, std::enable_if_t<std::is_constructible_v<TKey, nlohmann::json::string_t>>>
            {
                std::optional<TMap> operator()(const nlohmann::json& json) const
                {
                    auto map = std::make_optional<TMap>();

                    for (auto json_property = json.cbegin(); json_property != json.cend(); ++json_property)
                    {
                        auto value = JSONDeserializer<TValue>(json_property.value());

                        if (value)
                        {
                            map->insert(std::make_pair(json_property.key(), *std::move(value)));
                        }
                    }

                    return map;
                }
            };

            /// \brief Deserialize from an array of objects: the elements are the mapped objects, while one particular field is used as a key.
            std::optional<TMap> DeserializeFromArray(const nlohmann::json& json) const
            {
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
                                map->insert(std::make_pair(*std::move(key), *std::move(value)));
                            }
                            else
                            {
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
        /* STRING DESERIALIZATION                                               */
        /************************************************************************/

        /// \brief Functor used to deserialize a std::wstring from JSON.
        /// \author Raffaele D. Facendola - October 2016
        template <>
        struct JSONDeserializerT<std::wstring>
        {
            std::optional<std::wstring> operator()(const nlohmann::json& json) const
            {
                if (json.is_string())
                {
                    return to_wstring(json.get<std::string>());
                }
                return std::nullopt;
            }
        };

        /// \brief Functor used to deserialize a std::string from JSON.
        /// \author Raffaele D. Facendola - October 2016
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

        /// \brief Functor used to deserialize a HashedString from JSON.
        /// \author Raffaele D. Facendola - November 2016
        template <>
        struct JSONDeserializerT<HashedString>
        {
            std::optional<HashedString> operator()(const nlohmann::json& json) const
            {
                if (json.is_string())
                {
                   return HashedString(json.get<std::string>());
                }
                return std::nullopt;
            }
        };

        /************************************************************************/
        /* FUNDAMENTAL TYPES DESERIALIZATION                                    */
        /************************************************************************/

        /// \brief Functor used to deserialize a boolean variable from JSON.
        /// \author Raffaele D. Facendola - October 2016
        template <>
        struct JSONDeserializerT<bool>
        {
            std::optional<bool> operator()(const nlohmann::json& json) const
            {
                if (json.is_boolean())
                {
                    return json.get<bool>();
                }
                return std::nullopt;
            }
        };

        /// \brief Functor used to deserialize a numeric variable from JSON.
        /// \author Raffaele D. Facendola - September 2016
        template <typename TType>
        struct JSONDeserializerT<TType, typename std::enable_if_t<std::is_arithmetic_v<TType>>>
        {
            std::optional<TType> operator()(const nlohmann::json& json) const
            {
                if (json.is_number())
                {
                    return json.get<TType>();
                }
                return std::nullopt;
            }
        };

    }

}