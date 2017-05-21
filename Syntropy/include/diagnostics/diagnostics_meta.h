
/// \file diagnostics.h
/// \brief This header is part of the syntropy diagnostic system. It contains meta definitions for the file diagnostics.h.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "diagnostics/diagnostics.h"

#include "reflection/class.h"
#include "reflection/core_types.h"

#include "serialization/json.h"
#include "serialization/json/json_deserializer.h"

namespace syntropy
{
    namespace reflection
    {
        extern const Class& ClassOf_Severity;
        extern const Class& ClassOf_Context;

        // Reflection specialization for Context
        template<>
        struct ClassDeclaration<diagnostics::Severity>
        {
            static constexpr const char* GetName() noexcept
            {
                return "syntropy::diagnostics::Severity";
            }

            void operator()(ClassDefinitionT<diagnostics::Context>& definition) const
            {
                definition << serialization::JSONConstruct();
            }
        };

        // Reflection specialization for Context
        template<>
        struct ClassDeclaration<diagnostics::Context>
        {
            static constexpr const char* GetName() noexcept
            {
                return "syntropy::diagnostics::Context";
            }

            void operator()(ClassDefinitionT<diagnostics::Context>& definition) const
            {
                definition << serialization::JSONConstruct();
            }
        };
    }

    namespace serialization
    {
        // Used to deserialize a Severity from a JSON object.
        template <>
        struct JSONDeserializerT<diagnostics::Severity>
        {
            std::optional<diagnostics::Severity> operator()(const nlohmann::json& json) const
            {
                if (json.is_string())
                {
                    auto str = json.get<std::string>();

                    if (str == "Informative")       return diagnostics::Severity::kInformative;
                    if (str == "Warning")           return diagnostics::Severity::kWarning;
                    if (str == "Error")             return diagnostics::Severity::kError;
                    if (str == "Critical")          return diagnostics::Severity::kCritical;

                }
                return std::nullopt;
            }
        };

        // Used to deserialize a Context from a JSON object.
        template <>
        struct JSONDeserializerT<diagnostics::Context>
        {
            std::optional<diagnostics::Context> operator()(const nlohmann::json& json) const
            {
                if (json.is_string())
                {
                    return diagnostics::Context(json.get<std::string>());
                }
                return std::nullopt;
            }
        };
    }
}
