
/// \file core_deserializers.h
/// \brief This header is part of the syntropy serialization system. It contains deserializers for syntropy's core types.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/types/label.h"
#include "syntropy/containers/context.h"

#include "syntropy/serialization/json/json.h"

namespace Syntropy::serialization
{
    /************************************************************************/
    /* HASHED STRING                                                        */
    /************************************************************************/

    template <>
    struct JSONDeserializerT<Label>
    {
        std::optional<Label> operator()(const JSON& json) const
        {
            if (json.is_string())
            {
                return Label(json.get<String>());
            }
            return std::nullopt;
        }
    };

    /************************************************************************/
    /* CONTEXT                                                              */
    /************************************************************************/

    template <>
    struct JSONDeserializerT<Context>
    {
        std::optional<Context> operator()(const JSON& json) const
        {
            if (json.is_string())
            {
                return Context(json.get<String>());
            }
            return std::nullopt;
        }
    };
}
