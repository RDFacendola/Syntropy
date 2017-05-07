/// \file json.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <functional>

#include "type_traits.h"

#include "serialization/serialization.h"
#include "serialization/json/json_deserializer.h"

#include "reflection/any.h"

namespace syntropy 
{
    namespace serialization
    {

        /// \brief Property interface used to deserialize properties from JSON object.
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

        /// \brief Functor used to instantiate an object from JSON.
        /// \author Raffaele D. Facendola - May 2017
        template <typename TClass>
        struct JSONConstructorT 
        {
            //reflection::Any operator()(const nlohmann::json& json) const;
        };

        template <typename TClass>
        constexpr JSONConstructorT<TClass> JSONConstructor{};

        /// \brief Class interface used to grant JSON construction capabilities.
        /// A class defining this interface can be directly constructed via JSON object.
        /// \author Raffaele D. Facendola - September 2016
        struct JSONConstructible
        {
            /// \brief Create a new interface.
            template <typename TClass>
            JSONConstructible(tag_t<TClass>);

            /// \brief Construct a new instance via JSON object.
            /// \param json JSON object the object will be constructed form.
            /// \return Returns an instance of an object constructed via a JSON object. If the object could not be constructed with the provided JSON object, returns an empty instance.
            reflection::Any operator()(const nlohmann::json& json) const;

        private:

            /// \brief Instantiate a new object.
            /// \param json JSON object the object will be constructed form.
            template <typename TClass>
            static reflection::Any Instantiate(const nlohmann::json& json);

            reflection::Any(*instancer_)(const nlohmann::json&);                    /// \brief Functor used to instantiate the class.

        };
        
        /// \brief Functor object used to assign the interface JSONDeserializable to properties.
        /// \author Raffaele D. Facendola - September 2016
        struct JSONRead 
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

        struct JSONConstruct
        {
            /// \brief Add a JSONConstructible interface to the provided class.
            /// \param class_definition Definition of the class the interface will be added to.
            template <typename TClass>
            void operator()(reflection::ClassDefinitionT<TClass>& class_definition) const;
        };

    }

}

namespace syntropy
{
    namespace serialization 
    {

        /************************************************************************/
        /* JSON DESERIALIZABLE                                                  */
        /************************************************************************/

        template <typename TClass, typename TProperty, typename>
        JSONDeserializable::JSONDeserializable(TProperty(TClass::* field))
        {
            static_assert(std::is_move_assignable_v<TProperty>, "TProperty must be move-assignable");

            deserializer_ = [field](const reflection::Any& object, const nlohmann::json& json)
            {
                auto value = JSONDeserializer<TProperty>(json);

                if (value)
                {
                    reflection::AnyCast<TClass*>(object)->*field = *std::move(value);
                }

                return value.has_value();
            };
        }

        template <typename TClass, typename TProperty>
        JSONDeserializable::JSONDeserializable(void(TClass::* setter)(TProperty))
        {
            static_assert(std::is_move_constructible_v<remove_reference_cv_t<TProperty>>, "remove_reference_cv_t<TProperty> must be move-constructible");

            deserializer_ = [setter](const reflection::Any& object, const nlohmann::json& json)
            {
                auto value = JSONDeserializer<remove_reference_cv_t<TProperty>>(json);

                if (value)
                {
                    (reflection::AnyCast<TClass*>(object)->*setter)(*std::move(value));
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
                auto value = JSONDeserializer<TProperty>(json);

                if (value)
                {
                    (reflection::AnyCast<TClass*>(object)->*setter)() = *std::move(value);
                }

                return value.has_value();
            };
        }

        template <typename TInstance, typename>
        bool JSONDeserializable::operator()(TInstance& instance, const nlohmann::json& json) const
        {
            return (*this)(std::addressof(object), json);
        }

        /************************************************************************/
        /* JSON CONSTRUCTIBLE                                                   */
        /************************************************************************/

        template <typename TClass>
        JSONConstructible::JSONConstructible(tag_t<TClass>)
            : instancer_(&Instantiate<TClass>)
        {

        }

        template <typename TClass>
        reflection::Any JSONConstructible::Instantiate(const nlohmann::json& json)
        {
            return JSONConstructor<TClass>(json);
        }

        /************************************************************************/
        /* JSON READ                                                            */
        /************************************************************************/

        template <typename TClass, typename TField, typename... TAccessors>
        void JSONRead::operator()(reflection::PropertyDefinitionT<TAccessors...>& property, TField(TClass::* field))
        {
            property.AddInterface<JSONDeserializable>(field);
        }

        template <typename TClass, typename TPropertyGetter, typename TPropertySetter, typename... TAccessors>
        void JSONRead::operator()(reflection::PropertyDefinitionT<TAccessors...>& property, TPropertyGetter(TClass::*)() const, void (TClass::* setter)(TPropertySetter))
        {
            property.AddInterface<JSONDeserializable>(setter);
        }

        template <typename TClass, typename TProperty, typename... TAccessors>
        void JSONRead::operator()(reflection::PropertyDefinitionT<TAccessors...>& property, const TProperty&(TClass::*)() const, TProperty& (TClass::* setter)())
        {
            property.AddInterface<JSONDeserializable>(setter);
        }

        /************************************************************************/
        /* JSON CONSTRUCT                                                       */
        /************************************************************************/

        template <typename TClass>
        void JSONConstruct::operator()(reflection::ClassDefinitionT<TClass>& class_definition) const
        {
            class_definition.AddInterface<JSONConstructible>(tag_t<TClass>{});
        }

    }

}
