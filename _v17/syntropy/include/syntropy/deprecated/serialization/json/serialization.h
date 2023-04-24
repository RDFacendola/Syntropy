/// \file serialization.h
/// \brief This header is part of the syntropy serialization system. It contains classes and function used to serialize objects to JSON.
///
/// \author Raffaele D. Facendola - 2018

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
    /// \brief Class token used to identify the class type in a JSON. 
    static constexpr const char* kClassToken = "$class";

    namespace map
    {
        /// \brief JSON property token used to determine the id of a map pair.
        static constexpr const char* kIdToken = "$id";

        /// \brief JSON property token used to determine the value of map pair.
        static constexpr const char* kValueToken = "$value";
    }
    
    /// \brief Token used to identify a shared_ptr object in a JSON.
    static constexpr const char* kSharedPtrIdToken = "$spid";

    /// \brief Serialize an object properties to a JSON.
    /// This method enumerates TType's properties and attempts to serialize each one.
    /// \param object Object to serialize.
    /// \return Returns a JSON object if successful.
    template <typename TType>
    std::optional<JSON> SerializeObjectToJSON(const TType& object);

    template <typename TType>
    constexpr void JSONSerialize(JSON& json, const TType& instance);

    /************************************************************************/
    /* JSON SERIALIZABLE                                                    */
    /************************************************************************/

    /// \brief Property interface used to serialize properties to a JSON object.
    /// \author Giuseppe Spizzico - January 2018
    class JSONSerializable
    {
    public:
        /// \brief Create a new interface from a member field.
        /// The field must be copy-assignable.
        template <typename TClass, typename TProperty, typename = std::enable_if_t<std::is_member_object_pointer<TProperty(TClass::*)>::value>>
        JSONSerializable(TProperty(TClass::*field))
        {
            static_assert(std::is_move_assignable_v<TProperty>, "TProperty must be copy-assignable");

            serializer_ = [field](const String& name, const Syntropy::reflection::Any& instance, JSON& json)
            {
                // #TODO Custom container conversion. Use JSON with custom containers.

                JSONSerialize(json[name.c_str()], reflection::AnyCast<TClass const *>(instance)->*field);
            };
        }

        /// \brief Create a new interface from a getter method.
        template <typename TClass, typename TProperty>
        JSONSerializable(TProperty(TClass::* getter)(void) const)
        {
            static_assert(std::is_move_constructible_v<std::remove_cvref_t<TProperty>>, "remove_reference_cv_t<TProperty> must be move-constructible");

            serializer_ = [getter](const String& name, const Syntropy::reflection::Any& instance, JSON& json)
            {
                // #TODO Custom container conversion. Use JSON with custom containers.

                JSONSerialize(json[name.c_str()], (reflection::AnyCast<TClass const*>(instance)->*getter)());
            };
        }
        /// \brief Serialize the property value.
        /// \param name Name of the property to serialize.
        /// \param instance Object that owns the property to serialize. Expects pointer to the actual object instance.
        /// \param json JSON containing the serialized property.
        void operator()(const String& name, reflection::Any& instance, JSON& json) const;

        /// \brief Serialize the property value.
        /// This overload only participates in overload resolution if TInstance is not reflection::Any.
        /// \param name Name of the property to serialize.
        /// \param instance Object that owns the property to serialize.
        /// \param json JSON containing the serialized property.
        template<typename TInstance>
        void operator()(const String& name, TInstance& instance, JSON& json) const
        {
            serializer_(name, std::addressof(instance), json);
        }
        /// \brief Serialize the property value.
        /// This overload only participates in overload resolution if TInstance is not reflection::Any.
        /// \param property reflection::Property to serialize. 
        /// \param instance Object that owns the property to serialize.
        /// \param json JSON containing the serialized property.
        template<typename TInstance>
        void operator()(const Syntropy::reflection::Property& property, TInstance& instance, JSON& json) const
        {
            (*this)(property.GetName().GetString(), Forward<TInstance>(instance), json);
        }

    private:

        std::function<void(const String& name, const reflection::Any& object, JSON& json)> serializer_; ///< \brief Functor used to serialize the property.
    };

    /************************************************************************/
    /* JSON CONVERTIBLE                                                     */
    /************************************************************************/

    /// \brief Class interface used to serialize an instance to a JSON object.
    /// \author Giuseppe Spizzico- January 2018
    class JSONConvertible
    {
    public:

        /// \brief Default constructor
        constexpr JSONConvertible() = default;

        /// \brief Serialize an instance of TClass to a JSON object.
        /// \param instance The instance that will be serialized
        /// \return JSON object containing the serialized properties.
        template<typename TClass>
        std::optional<JSON> operator()(const TClass& instance) const
        {
            auto Class = &Syntropy::reflection::ClassOf<TClass>();

            size_t serialized_properties = 0;

            JSON json;
            for (auto it = Class->GetProperties().begin(); it != Class->GetProperties().end(); ++it)
            {
                auto SerializableInterface = (*it).GetInterface<JSONSerializable>();
                if (SerializableInterface)
                {
                    (*SerializableInterface)(*it, instance, json);
                    serialized_properties++;
                }
            }

            if (serialized_properties > 0)	/// \brief Not serializing the object if there were no properties to be serialized.
            {
                json[kClassToken] = Class->GetDefaultName().GetString();
                return std::make_optional<JSON>(json);
            }

            return std::nullopt;
        }
    };

    /************************************************************************/
    /* JSON SERIALIZER                                                      */
    /************************************************************************/

    /// \brief Functor used to serialize an object to a JSON.
    /// Can be specialized for any object requiring particular JSON serialization capabilities.
    /// \author Giuseppe Spizzico - January 2018
    template<typename TType, typename = void>
    struct JSONSerializerT
    {
        /// \brief Base specialization of JSONSerializerT for object types.
        template<typename TType>
        void operator()(JSON& json, const TType& instance) const
        {
            /// \brief Compile time error if there is not a specialization of to_json in the same namespace of TType or the global namespace for TType.
            json = instance; 
        }
    };

    /// \brief Utility function for JSONSerializerT.
    ///  Usage:  JSONSerialize(json, value) instead of JSONSerializerT<TType>{}(json, value)
    /// \author Giuseppe Spizzico - January 2018
    template <typename TType>
    inline constexpr void JSONSerialize(JSON& json, const TType& instance)
    {
        JSONSerializerT<TType>()(json, instance);
    };

    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/

    template<typename TType>
    std::optional<JSON> SerializeObjectToJSON(const TType& object)
    {
        if (auto ConvertibleInterface = reflection::ClassOf<TType>().GetInterface<JSONConvertible>())
        {
            return (*ConvertibleInterface)(object);
        }

        return std::nullopt;
    }
}