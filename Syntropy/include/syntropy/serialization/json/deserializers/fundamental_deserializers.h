
/// \file fundamental_deserializers.h
/// \brief This header is part of the syntropy serialization system. It contains deserializers for fundamental types.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "syntropy/type_traits.h"

#include "syntropy/reflection/reflection.h"
#include "syntropy/reflection/interfaces/class_interfaces.h"

#include "syntropy/serialization/json/json.h"

namespace syntropy::serialization
{
    /************************************************************************/
    /* POINTERS                                                             */
    /************************************************************************/

    /// \brief Partial specialization of JSONDeserializerT for pointer types.
    template <typename TType>
    struct JSONDeserializerT<TType*, std::enable_if_t<!std::is_pointer<TType>::value> >
    {
        std::optional<TType*> operator()(const nlohmann::json& json) const
        {
            if (auto concrete_class = GetClassFromJSON(json, &reflection::ClassOf<TType>()))            // Concrete class type.
            {
                if (auto json_constructible = concrete_class->GetInterface<JSONConstructible>())        // Double dispatch to ensure the concrete type is instantiated and deserialized.
                {
                    if (auto instance = (*json_constructible)(json); instance.HasValue())
                    {
                        return reflection::AnyCast<TType*>(instance);
                    }
                }
            }

            return std::nullopt;
        }
    };

    /************************************************************************/
    /* BOOLEAN                                                              */
    /************************************************************************/

    /// \brief Specialization of JSONDeserializerT for boolean types.
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
    /* INTEGERS, FLOATING POINTS                                            */
    /************************************************************************/

    /// \brief Partial specialization of JSONDeserializerT for arithmetic types.
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
    /* ENUMERATIONS                                                         */
    /************************************************************************/

    /// \brief Partial specialization of JSONDeserializerT for enumeration types.
    template <typename TType>
    struct JSONDeserializerT<TType, typename std::enable_if_t<std::is_enum_v<TType> > >
    {
        std::optional<TType> operator()(const nlohmann::json& json) const
        {
            if (json.is_string())
            {
                if (auto enum_interface = reflection::ClassOf<TType>().GetInterface<reflection::Enumeration>())
                {
                    return enum_interface->GetValueByName<TType>(json.get<std::string>());
                }
            }
            return std::nullopt;
        }
    };
}