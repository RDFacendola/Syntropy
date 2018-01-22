/// \file json.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <functional>
#include <optional>

#include "type_traits.h"

#include "serialization/serialization.h"

#include "diagnostics/log.h"

#include "reflection/reflection.h"
#include "reflection/any.h"

#include "nlohmann/json/src/json.hpp"

namespace syntropy::serialization
{
    /// \brief Get the class associated to the provided JSON object.
    /// \param json JSON object to parse looking for the class.
    /// \param base_class Base class to match. If the found class is not in the same hierarchy of this class, the method returns nullptr. Optional.
    /// \return Returns the class associated to the provided JSON object. If the class is not compatible with the provided class or it is non-existent, returns nullptr.
    const reflection::Class* GetClassFromJSON(const nlohmann::json& json, const reflection::Class* base_class = nullptr);

    /// \brief Deserialize an object from JSON.
    /// If TType is a pointer this method guarantees polymorphism (only if the base and the concrete classes were exposed to the syntropy reflection system).
    /// \param json JSON object to deserialize.
    /// \param default_value Default value to return if the deserialization process failed.
    /// \param property_name Name of the JSON property to deserialize. If nullptr is specified, the entire object is deserialized.
    /// \return If property_name was not specified returns the deserialized version of json, otherwise returns the deserialized version of the specified json property. If the object could not be deserialized returns default_value.
    template <typename TType>
    std::optional<TType> DeserializeObjectFromJSON(const nlohmann::json& json, std::optional<TType> default_value = std::nullopt, const char* property_name = nullptr);

    /// \brief Deserialize an object properties from JSON.
    /// This method enumerates JSON properties and attempts to deserialize the corresponding object properties.
    /// \param object Object to deserialize the properties of.
    /// \param json JSON object to read.
    /// \return Returns the number of successfully deserialized properties.
    template <typename TType>
    size_t DeserializeObjectFromJSON(TType& object, const nlohmann::json& json);

    /// \brief Property interface used to deserialize properties from a JSON object.
    /// \author Raffaele D. Facendola - September 2016
    class JSONDeserializable
    {
    public:

        /// \brief Create a new interface from a member field.
        /// The field must be copy-assignable.
        template <typename TClass, typename TProperty, typename = std::enable_if_t<std::is_member_object_pointer<TProperty(TClass::*)>::value>>
        JSONDeserializable(TProperty(TClass::* field));

        /// \brief Create a new interface from a setter method.
        template <typename TClass, typename TProperty>
        JSONDeserializable(void(TClass::* setter)(TProperty));

        /// \brief Create a new interface from a non-const accessor.
        template <typename TClass, typename TProperty>
        JSONDeserializable(TProperty&(TClass::* setter)());

        /// \brief Deserialize the property value.
        /// \param instance Object to deserialize the property of. Expects pointer to the actual object instance.
        /// \return Returns true if the property could be deserialized, returns false otherwise.
        bool operator()(const reflection::Any& instance, const nlohmann::json& json) const;

        /// \brief Deserialize the property value.
        /// This overload only participates in overload resolution if TInstance is not reflection::Any.
        /// \param instance Object to deserialize the property of.
        /// \return Returns true if the property could be deserialized, returns false otherwise.
        template <typename TInstance, typename = std::enable_if_t<!std::is_same_v<std::decay_t<TInstance>, reflection::Any>>>
        bool operator()(TInstance& instance, const nlohmann::json& json) const;

    private:

        std::function<bool(const reflection::Any& instance, const nlohmann::json& json)> deserializer_;         ///< \brief Functor used to deserialize the property.

    };

    /// \brief Class interface used to construct an instance from a JSON object.
    /// \author Raffaele D. Facendola - May 2017
    class JSONConstructible
    {
    public:

        /// \brief Create a new interface.
        template <typename TClass>
        constexpr JSONConstructible(tag_t<TClass>);

        /// \brief Construct a new instance via JSON object.
        /// \param json JSON object the object will be constructed form.
        /// \return Returns an pointer to the constructed object. If the object could not be constructed with the provided JSON object, returns an empty instance.
        reflection::Any operator()(const nlohmann::json& json) const;

    private:

        /// \brief Instantiate a new object.
        /// \param json JSON object the object will be constructed form.
        template <typename TClass>
        static reflection::Any Instantiate(const nlohmann::json& json);

        reflection::Any(*instancer_)(const nlohmann::json&);                    /// \brief Functor used to instantiate the class.

    };

    /// \brief Functor object used to give JSON capabilities to properties.
    /// \author Raffaele D. Facendola - September 2016
    struct JSONProperty 
    {
 
        /// \brief Add a JSONDeserializable interface to the provided property.
        /// \param property Property to add the interface to.
        /// \param field Property field.
        template <typename TClass, typename TField, typename... TAccessors>
        void operator()(reflection::PropertyDefinitionT<TAccessors...>& property, TField(TClass::* field));

        /// \brief Add a JSONDeserializable interface to the provided property.
        /// \param property Property to add the interface to.
        /// \param getter Property getter.
        /// \param setter Property setter.
        template <typename TClass, typename TPropertyGetter, typename TPropertySetter, typename... TAccessors>
        void operator()(reflection::PropertyDefinitionT<TAccessors...>& property, TPropertyGetter(TClass::* getter)() const, void (TClass::* setter)(TPropertySetter));

        /// \brief Add a JSONDeserializable interface to the provided property.
        /// \param property Property to add the interface to.
        /// \param getter Property const accessor.
        /// \param setter Property non-const accessor.
        template <typename TClass, typename TProperty, typename... TAccessors>
        void operator()(reflection::PropertyDefinitionT<TAccessors...>& property, const TProperty&(TClass::* getter)() const, TProperty& (TClass::* setter)());

    };

    /// \brief Functor object used to give JSON capabilities to classes.
    /// \author Raffaele D. Facendola - May 2017
    struct JSONClass
    {
        /// \brief Add a JSONConstructible interface to the provided class.
        /// \param class_definition Definition of the class the interface will be added to.
        template <typename TClass>
        void operator()(reflection::ClassDefinitionT<TClass>& class_definition) const;
    };

    /// \brief Functor used to deserialize an object from JSON.
    /// Can be specialized for any object requiring particular JSON deserialization capabilities.
    /// \author Raffaele D. Facendola - September 2016
    template <typename TType, typename = void>
    struct JSONDeserializerT
    {
        static_assert(!std::is_abstract_v<TType>, "TType must not be abstract.");
        static_assert(std::is_default_constructible_v<TType>, "TType must be default constructible, otherwise a specialization of JSONDeserializerT<TType> is required!");

        std::optional<TType> operator()(const nlohmann::json& json) const
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
    constexpr JSONDeserializerT<TType> JSONDeserializer{};

}

// Deserializers for fundamental types

namespace syntropy::serialization
{
    /************************************************************************/
    /* POINTERS                                                             */
    /************************************************************************/

    template <typename TType>
    struct JSONDeserializerT<TType*, std::enable_if_t<!std::is_pointer<TType>::value> >
    {
        std::optional<TType*> operator()(const nlohmann::json& json) const
        {
            // Find the concrete class type

            auto concrete_class = GetClassFromJSON(json, &reflection::ClassOf<TType>());

            if (concrete_class)
            {
                // Use double dispatch to ensure that the concrete type is instantiated and deserialized.

                auto json_constructible = concrete_class->GetInterface<JSONConstructible>();

                if (json_constructible)
                {
                    auto instance = (*json_constructible)(json);

                    if (instance.HasValue())
                    {
                        return reflection::AnyCast<TType*>(instance);
                    }
                }
            }

            return std::nullopt;
        }
    };

    /************************************************************************/
    /* BOOLEAN TYPES                                                        */
    /************************************************************************/

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

    /************************************************************************/
    /* ARITHMETIC TYPES                                                     */
    /************************************************************************/

    template <typename TType>
    struct JSONDeserializerT<TType, typename std::enable_if_t<std::is_arithmetic_v<TType> > >
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

    /************************************************************************/
    /* ENUMERATION TYPES                                                    */
    /************************************************************************/

    template <typename TType>
    struct JSONDeserializerT<TType, typename std::enable_if_t<std::is_enum_v<TType> > >
    {
        std::optional<TType> operator()(const nlohmann::json& json) const
        {
            if (json.is_string())
            {
                auto enum_interface = reflection::ClassOf<TType>().GetInterface<reflection::Enumeration>();

                if (enum_interface)
                {
                    return enum_interface->GetValueByName<TType>(json.get<std::string>());
                }
            }
            return std::nullopt;
        }
    };
}

// Implementation

namespace syntropy::serialization
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    template <typename TType>
    std::optional<TType> DeserializeObjectFromJSON(const nlohmann::json& json, std::optional<TType> default_value, const char* property_name)
    {
        static_assert(std::is_move_constructible_v<TType> || std::is_copy_constructible_v<TType>, "TType must either be copy-constructible or move-constructible.");

        if (property_name != nullptr)
        {
            // Deserialize a JSON object property
            auto property_it = json.find(property_name);

            if (property_it != json.end())
            {
                auto object = JSONDeserializer<TType>(*property_it);

                if (object)
                {
                    return std::move(object);
                }
            }
        }
        else
        {
            // Deserialize the entire JSON object.
            auto object = std::move(JSONDeserializer<TType>(json));

            if (object)
            {
                return std::move(object);
            }
        }

        return std::move(default_value);
    }

    template <typename TType>
    size_t DeserializeObjectFromJSON(TType& object, const nlohmann::json& json)
    {
        size_t deserialized_properties = 0;

        for (auto json_property = json.cbegin(); json_property != json.cend(); ++json_property)
        {
            auto object_property = reflection::ClassOf<TType>().GetProperty(json_property.key());           // Find a target property.

            if (object_property)
            {
                auto deserializable = object_property->GetInterface<JSONDeserializable>();

                if (deserializable && (*deserializable)(std::addressof(object), json_property.value()))     // Recursive deserialization.
                {
                    ++deserialized_properties;
                }
            }
        }

        return deserialized_properties;
    }

    /************************************************************************/
    /* JSON DESERIALIZABLE                                                  */
    /************************************************************************/

    template <typename TClass, typename TProperty, typename>
    JSONDeserializable::JSONDeserializable(TProperty(TClass::* field))
    {
        static_assert(std::is_move_assignable_v<TProperty>, "TProperty must be move-assignable");

        deserializer_ = [field](const reflection::Any& object, const nlohmann::json& json)
        {
            // TODO: Can we deserialize directly inside the field?

            auto value = JSONDeserializer<TProperty>(json);

            if (value)
            {
                reflection::AnyCast<TClass*>(object)->*field = std::move(*value);           // TODO: if TProperty is a pointer, the previous value will be leaked!
            }

            return value.has_value();
        };
    }

    template <typename TClass, typename TProperty>
    JSONDeserializable::JSONDeserializable(void(TClass::* setter)(TProperty))
    {
        static_assert(std::is_move_constructible_v<std::remove_cvref<TProperty>>, "remove_reference_cv_t<TProperty> must be move-constructible");

        deserializer_ = [setter](const reflection::Any& object, const nlohmann::json& json)
        {
            auto value = JSONDeserializer<std::remove_cvref<TProperty>>(json);

            if (value)
            {
                (reflection::AnyCast<TClass*>(object)->*setter)(std::move(*value));
            }

            return value.has_value();
        };
    }

    template <typename TClass, typename TProperty>
    JSONDeserializable::JSONDeserializable(TProperty&(TClass::* setter)())
    {
        static_assert(std::is_move_assignable_v<TProperty>, "TProperty must be copy-assignable");

        deserializer_ = [setter](const reflection::Any& object, const nlohmann::json& json)
        {
            // TODO: Can we deserialize directly inside the property?

            auto value = JSONDeserializer<TProperty>(json);

            if (value)
            {
                (reflection::AnyCast<TClass*>(object)->*setter)() = std::move(*value);      // TODO: if TProperty is a pointer, the previous value will be leaked!
            }

            return value.has_value();
        };
    }

    template <typename TInstance, typename>
    bool JSONDeserializable::operator()(TInstance& instance, const nlohmann::json& json) const
    {
        return (*this)(std::addressof(instance), json);
    }

    /************************************************************************/
    /* JSON CONSTRUCTIBLE                                                   */
    /************************************************************************/

    template <typename TClass>
    constexpr JSONConstructible::JSONConstructible(tag_t<TClass>)
        : instancer_(&Instantiate<TClass>)
    {

    }

    template <typename TClass>
    reflection::Any JSONConstructible::Instantiate(const nlohmann::json& json)
    {
        // TODO: Can we avoid the move?

        auto deserialized_object = JSONDeserializer<TClass>(json);              // Deserialize on the stack.

        if (deserialized_object)
        {
            return new TClass(std::move(*deserialized_object));                 // Move-construct on the heap.
        }

        return reflection::Any();
    }

    /************************************************************************/
    /* JSON PROPERTY                                                        */
    /************************************************************************/

    template <typename TClass, typename TField, typename... TAccessors>
    void JSONProperty::operator()(reflection::PropertyDefinitionT<TAccessors...>& property, TField(TClass::* field))
    {
        property.AddInterface<JSONDeserializable>(field);
    }

    template <typename TClass, typename TPropertyGetter, typename TPropertySetter, typename... TAccessors>
    void JSONProperty::operator()(reflection::PropertyDefinitionT<TAccessors...>& property, TPropertyGetter(TClass::*)() const, void (TClass::* setter)(TPropertySetter))
    {
        property.AddInterface<JSONDeserializable>(setter);
    }

    template <typename TClass, typename TProperty, typename... TAccessors>
    void JSONProperty::operator()(reflection::PropertyDefinitionT<TAccessors...>& property, const TProperty&(TClass::*)() const, TProperty& (TClass::* setter)())
    {
        property.AddInterface<JSONDeserializable>(setter);
    }

    /************************************************************************/
    /* JSON CLASS                                                           */
    /************************************************************************/

    template <typename TClass>
    void JSONClass::operator()(reflection::ClassDefinitionT<TClass>& class_definition) const
    {
        class_definition.AddInterface<JSONConstructible>(tag_t<TClass>{});
    }
}
