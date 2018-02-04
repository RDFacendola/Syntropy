/// \file deserialization.h
/// \brief This header is part of the syntropy serialization system. It contains generic functionalities needed to handle JSON serialization process.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "serialization/serialization.h"
#include "serialization/json/serialization.h"
#include "serialization/json/deserialization.h"

#include "diagnostics/log.h"

#include "reflection/reflection.h"

#include "nlohmann/json/src/json.hpp"

namespace syntropy::serialization
{
    /************************************************************************/
    /* JSON PROPERTY                                                        */
    /************************************************************************/

    /// \brief Functor object used to give JSON capabilities to properties.
    /// \author Raffaele D. Facendola - September 2016
    struct JSONProperty
    {
        /// \brief Add a JSONDeserializable interface to the provided property.
        /// \param property Property to add the interface to.
        /// \param field Property field.
        template <typename TClass, typename TField, typename... TAccessors>
        void operator()(reflection::PropertyDefinitionT<TAccessors...>& property, TField(TClass::* field))
        {
            property.AddInterface<JSONDeserializable>(field);
            property.AddInterface<JSONSerializable>(field);
        }

        /// \brief Add a JSONDeserializable interface to the provided property.
        /// \param property Property to add the interface to.
        /// \param getter Property getter.
        /// \param setter Property setter.
        template <typename TClass, typename TPropertyGetter, typename TPropertySetter, typename... TAccessors>
        void operator()(reflection::PropertyDefinitionT<TAccessors...>& property, TPropertyGetter(TClass::* getter)() const, void (TClass::* setter)(TPropertySetter))
        {
            property.AddInterface<JSONDeserializable>(setter);
            property.AddInterface<JSONSerializable>(getter);
        }

        /// \brief Add a JSONDeserializable interface to the provided property.
        /// \param property Property to add the interface to.
        /// \param getter Property const accessor.
        /// \param setter Property non-const accessor.
        template <typename TClass, typename TProperty, typename... TAccessors>
        void operator()(reflection::PropertyDefinitionT<TAccessors...>& property, const TProperty&(TClass::* getter)() const, TProperty& (TClass::* setter)())
        {
            property.AddInterface<JSONDeserializable>(setter);
            property.AddInterface<JSONSerializable>(getter);
        }
    };

    /************************************************************************/
    /* JSON CLASS                                                           */
    /************************************************************************/

    /// \brief Functor object used to give JSON capabilities to classes.
    /// \author Raffaele D. Facendola - May 2017
    struct JSONClass
    {
        /// \brief Add a JSONConstructible interface to the provided class.
        /// \param class_definition Definition of the class the interface will be added to.
        template <typename TClass>
        void operator()(reflection::ClassT<TClass>& class_definition) const
        {
            class_definition.AddInterface<JSONConstructible>(tag_t<TClass>{});
            //class_definition.AddInterface<JSONConvertible>(tag_t<TClass>{});
        }
    };

    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/

    /// \brief Class token used to identify the class type in a JSON. 
    static constexpr const char* kClassToken = "$class";

    /// \brief Get the class associated to the provided JSON object.
    /// \param json JSON object to parse looking for the class.
    /// \param base_class Base class to match. If the found class is not in the same hierarchy of this class, the method returns nullptr. Optional.
    /// \return Returns the class associated to the provided JSON object. If the class is not compatible with the provided class or it is non-existent, returns nullptr.
    const reflection::Class* GetClassFromJSON(const nlohmann::json& json, const reflection::Class* base_class = nullptr);
}