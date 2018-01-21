
/// \file diagnostics_meta.h
/// \brief This header is part of the syntropy diagnostic system. It contains reflection and serialization definitions for the syntropy diagnostic system.
/// Definitions are provided in a separate file to avoid circular dependencies among syntropy diagnostic, reflection and serialization systems.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "containers/context.h"

#include "diagnostics/diagnostics.h"
#include "diagnostics/log.h"
#include "diagnostics/log_channels.h"

#include "reflection/reflection.h"
#include "reflection/types/stl_types.h"

#include "serialization/json/json.h"
#include "serialization/json/deserializers/stl_deserializers.h"

namespace syntropy::reflection
{
    /************************************************************************/
    /* DIAGNOSTICS.H                                                        */
    /************************************************************************/

    // Template specialization for Severity
    template<>
    struct ClassDeclarationT<diagnostics::Severity>
    {
        static constexpr const char* name_{ "syntropy::diagnostics::Severity" };

        void operator()(ClassDefinitionT<diagnostics::Severity>& definition) const
        {
            using diagnostics::Severity;

            definition << serialization::JSONClass();

            definition << reflection::EnumerationClass<Severity>(
            {
                {"Informative", Severity::kInformative},
                {"Warning", Severity::kWarning},
                {"Error", Severity::kError},
                {"Critical", Severity::kCritical}
            });
        }
    };

    // Template specialization for Context
    template<>
    struct ClassDeclarationT<Context>
    {
        static constexpr const char* name_{ "syntropy::diagnostics::Context" };

        void operator()(ClassDefinitionT<Context>& definition) const
        {
            definition << serialization::JSONClass();
        }
    };

    /************************************************************************/
    /* LOG.H                                                                */
    /************************************************************************/

    // Template specialization for LogChannel.
    template <>
    struct ClassDeclarationT<diagnostics::LogChannel>
    {
        static constexpr const char* name_{ "syntropy::diagnostics::LogChannel" };
    };

    /************************************************************************/
    /* LOG_CHANNELS.H                                                       */
    /************************************************************************/

    // Template specialization for StreamLogChannel.
    template <>
    struct ClassDeclarationT<diagnostics::StreamLogChannel>
    {
        static constexpr const char* name_{ "syntropy::diagnostics::StreamLogChannel" };

        void operator()(ClassDefinitionT<diagnostics::StreamLogChannel>& definition) const
        {
            definition.DefineBaseClass<diagnostics::LogChannel>();
        }
    };

    // Template specialization for FileLogChannel.
    template <>
    struct ClassDeclarationT<diagnostics::FileLogChannel>
    {
        static constexpr const char* name_{ "syntropy::diagnostics::FileLogChannel" };

        void operator()(ClassDefinitionT<diagnostics::FileLogChannel>& definition) const
        {
            definition << serialization::JSONClass();

            definition.DefineBaseClass<diagnostics::StreamLogChannel>();
        }
    };
}

namespace syntropy::serialization
{
    /************************************************************************/
    /* DIAGNOSTICS.H                                                        */
    /************************************************************************/

    // Template specialization for Context
    template <>
    struct JSONDeserializerT<Context>
    {
        std::optional<Context> operator()(const nlohmann::json& json) const
        {
            if (json.is_string())
            {
                return Context(json.get<std::string>());
            }
            return std::nullopt;
        }
    };

    /************************************************************************/
    /* LOG_CHANNELS.H                                                       */
    /************************************************************************/

    // Template specialization for FileLogChannel
    template <>
    struct JSONDeserializerT<diagnostics::FileLogChannel>
    {
        std::optional<diagnostics::FileLogChannel> operator()(const nlohmann::json& json) const
        {
            static std::vector<Context> default_contexts = { Context() };

            auto file = DeserializeObjectFromJSON<std::string>(json, std::nullopt, "file");
            auto format = DeserializeObjectFromJSON<std::string>(json, "{message}", "format");
            auto contexts = DeserializeObjectFromJSON<std::vector<Context> >(json, default_contexts, "contexts");
            auto verbosity = DeserializeObjectFromJSON<diagnostics::Severity>(json, diagnostics::Severity::kInformative, "verbosity");

            if (file)
            {
                return std::make_optional<diagnostics::FileLogChannel>(std::move(*file), std::move(*format), std::move(*contexts), *verbosity);
            }

            return std::nullopt;
        }
    };
}
