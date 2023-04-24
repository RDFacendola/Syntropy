/// \file deserialization.h
/// \brief This header is part of the syntropy serialization system. It contains classes and function used to deserialize objects from JSON.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <functional>
#include <optional>
#include <utility>

#include "syntropy/type_traits.h"

#include "syntropy/diagnostics/log.h"

#include "syntropy/reflection/reflection.h"
#include "syntropy/reflection/any.h"

#include "syntropy/serialization/json/shared.h"

namespace Syntropy::serialization
{
    /// \brief Deserialize an object properties from JSON.
    /// This method enumerates JSON properties and attempts to deserialize the corresponding object properties.
    /// \param object Object to deserialize the properties of.
    /// \param json JSON object to read.
    /// \return Returns the number of successfully deserialized properties.
    template <typename TType>
    size_t DeserializeObjectFromJSON(TType& object, const JSON& json);

    /************************************************************************/
    /* JSON DESERIALIZER                                                    */
    /************************************************************************/

    /// \brief Functor used to deserialize an object from JSON.
    /// Can be specialized for any object requiring particular JSON deserialization capabilities.
    /// \author Raffaele D. Facendola - September 2016
    template <typename TType, typename = void>
    struct JSONDeserializerT
    {
        static_assert(!std::is_abstract_v<TType>, "TType must not be abstract.");
        static_assert(std::is_default_constructible_v<TType>, "TType must be default constructible, otherwise a specialization of JSONDeserializerT<TType> is required!");

        std::optional<TType> operator()(const JSON& json) const
        {
            if (json.is_object())
            {
                auto object = std::make_optional<TType>();

                if (DeserializeObjectFromJSON(*object, json) > 0)
                {
                    return object;
                }
            }

            return std::nullopt;
        }
    };

    /// \brief Utility object for JSONDeserializerT.
    /// Usage: JSONDeserializer<TType>(json) instead of JSONDeserializerT<TType>{}(json)
    /// \author Raffaele D. Facendola - May 2017
    template <typename TType>
    inline constexpr JSONDeserializerT<TType> JSONDeserializer{};

    /************************************************************************/
    /* JSON DESERIALIZABLE                                                  */
    /************************************************************************/

    /// \brief Property interface used to deserialize properties from a JSON object.
    /// \author Raffaele D. Facendola - September 2016
    class JSONDeserializable
    {
    public:

        /// \brief Create a new interface from a member field.
        /// The field must be move-assignable.
        /// \remarks Raw pointers are not supported since it is not possible to determine whether to delete the old value (owning pointer) or just ignore it (observer pointer).
        template <typename TClass, typename TProperty, typename = std::enable_if_t<std::is_member_object_pointer<TProperty(TClass::*)>::value>>
        JSONDeserializable(TProperty(TClass::* field))
        {
            static_assert(std::is_move_assignable_v<TProperty>, "TProperty must be move-assignable");
            static_assert(!std::is_pointer_v<TProperty>, "Raw pointers are not supported since they lack ownership semantic. Please consider using smart pointers or a property setter.");

            deserializer_ = [field](const reflection::Any& object, const JSON& json)
            {
                auto value = JSONDeserializer<TProperty>(json);

                if (value)
                {
                    reflection::AnyCast<TClass*>(object)->*field = Move(*value);       // #TODO Can we deserialize inside the field directly?
                }

                return value.has_value();
            };
        }

        /// \brief Create a new interface from a setter method.
        template <typename TClass, typename TProperty>
        JSONDeserializable(void(TClass::* setter)(TProperty))
        {
            static_assert(std::is_move_constructible_v<std::remove_cvref_t<TProperty>>, "remove_reference_cv_t<TProperty> must be move-constructible");

            deserializer_ = [setter](const reflection::Any& object, const JSON& json)
            {
                auto value = JSONDeserializer<std::remove_cvref_t<TProperty>>(json);

                if (value)
                {
                    (reflection::AnyCast<TClass*>(object)->*setter)(Move(*value));
                }

                return value.has_value();
            };
        }

        /// \brief Create a new interface from a non-const accessor.
        /// \remarks Raw pointers are not supported since it is not possible to determine whether to delete the old value (owning pointer) or just ignore it (observer pointer).
        template <typename TClass, typename TProperty>
        JSONDeserializable(TProperty&(TClass::* setter)())
        {
            static_assert(std::is_move_assignable_v<TProperty>, "TProperty must be move-assignable");
            static_assert(!std::is_pointer_v<TProperty>, "Raw pointers are not supported since they lack ownership semantic. Please consider using smart pointers or a property setter.");

            deserializer_ = [setter](const reflection::Any& object, const JSON& json)
            {
                auto value = JSONDeserializer<TProperty>(json);

                if (value)
                {
                    (reflection::AnyCast<TClass*>(object)->*setter)() = Move(*value);  // #TODO Can we deserialize inside the field directly?
                }

                return value.has_value();
            };
        }

        /// \brief Deserialize the property value.
        /// \param instance Object to deserialize the property of. Expects pointer to the actual object instance.
        /// \return Returns true if the property could be deserialized, returns false otherwise.
        bool operator()(const reflection::Any& instance, const JSON& json) const;

        /// \brief Deserialize the property value.
        /// This overload only participates in overload resolution if TInstance is not reflection::Any.
        /// \param instance Object to deserialize the property of.
        /// \return Returns true if the property could be deserialized, returns false otherwise.
        template <typename TInstance, typename = std::enable_if_t<!std::is_same_v<std::decay_t<TInstance>, reflection::Any>>>
        bool operator()(TInstance& instance, const JSON& json) const
        {
            return (*this)(std::addressof(instance), json);
        }

    private:

        std::function<bool(const reflection::Any& instance, const JSON& json)> deserializer_;         ///< \brief Functor used to deserialize the property.

    };

    /************************************************************************/
    /* JSON CONSTRUCTIBLE                                                   */
    /************************************************************************/

    /// \brief Class interface used to construct an instance from a JSON object.
    /// \author Raffaele D. Facendola - May 2017
    class JSONConstructible
    {
    public:

        /// \brief Create a new interface.
        template <typename TClass>
        constexpr JSONConstructible(tag_t<TClass>)
            : instancer_(&Instantiate<TClass>)
        {

        }

        /// \brief Construct a new instance via JSON object.
        /// \param json JSON object the object will be constructed form.
        /// \return Returns an pointer to the constructed object. If the object could not be constructed with the provided JSON object, returns an empty instance.
        reflection::Any operator()(const JSON& json) const;

    private:

        /// \brief Instantiate a new object.
        /// \param json JSON object the object will be constructed form.
        template <typename TClass>
        static reflection::Any Instantiate(const JSON& json)
        {
            if (auto deserialized_object = JSONDeserializer<TClass>(json))
            {
                return new TClass(Move(*deserialized_object));             // #TODO What if TClass is neither copy-constructible nor move-constructible?
            }

            return reflection::Any();
        }

        reflection::Any(*instancer_)(const JSON&);                    /// \brief Functor used to instantiate the class.

    };


    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/

    /// \brief Deserialize an object from JSON.
    /// If TType is a pointer this method guarantees polymorphism (only if the base and the concrete classes were exposed to the syntropy reflection system).
    /// \param json JSON object to deserialize.
    /// \param default_value Default value to return if the deserialization process failed.
    /// \param property_name Name of the JSON property to deserialize. If nullptr is specified, the entire object is deserialized.
    /// \return If property_name was not specified returns the deserialized version of json, otherwise returns the deserialized version of the specified json property. If the object could not be deserialized returns default_value.
    template <typename TType>
    std::optional<TType> DeserializeObjectFromJSON(const JSON& json, std::optional<TType> default_value = std::nullopt, const char* property_name = nullptr)
    {
        static_assert(std::is_move_constructible_v<TType> || std::is_copy_constructible_v<TType>, "TType must either be copy-constructible or move-constructible.");

        if (property_name != nullptr)
        {
            // Deserialize a JSON object property
            
            if (auto property_it = json.find(property_name); property_it != json.end())
            {
                auto object = JSONDeserializer<TType>(*property_it);

                if (object)
                {
                    return Move(object);
                }
            }
        }
        else
        {
            // Deserialize the entire JSON object.

            if (auto object = Move(JSONDeserializer<TType>(json)))
            {
                return Move(object);
            }
        }

        return Move(default_value);
    }

    /// \brief Deserialize an object properties from JSON.
    /// This method enumerates JSON properties and attempts to deserialize the corresponding object properties.
    /// \param object Object to deserialize the properties of.
    /// \param json JSON object to read.
    /// \return Returns the number of successfully deserialized properties.
    template <typename TType>
    size_t DeserializeObjectFromJSON(TType& object, const JSON& json)
    {
        size_t deserialized_properties = 0;

        for (auto json_property = json.cbegin(); json_property != json.cend(); ++json_property)
        {
            if (auto object_property = reflection::ClassOf<TType>().GetProperty(String{ json_property.key() }))         // Find a property by name.
            {
                auto deserializable = object_property->GetInterface<JSONDeserializable>();

                if (deserializable && (*deserializable)(std::addressof(object), json_property.value()))                 // Recursive deserialization.
                {
                    ++deserialized_properties;
                }
            }
        }

        return deserialized_properties;
    }
}