
/// \file diagnostics_meta.h
/// \brief This header is part of the syntropy diagnostic system. It contains reflection and serialization definitions for the syntropy diagnostic system.
/// Definitions are provided in a separate file to avoid circular dependencies among syntropy diagnostic, reflection and serialization systems.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "diagnostics/diagnostics.h"
#include "diagnostics/log.h"
#include "diagnostics/log_channels.h"

#include "reflection/reflection.h"
#include "reflection/types/stl_types.h"

#include "serialization/json/json.h"
#include "serialization/json/deserializers/stl_deserializers.h"

namespace syntropy
{
    namespace reflection
    {

        /************************************************************************/
        /* DIAGNOSTICS.H                                                        */
        /************************************************************************/

        // Template specialization for Severity
        template<>
        struct ClassDeclaration<diagnostics::Severity>
        {
            static constexpr const char* GetName() noexcept
            {
                return "syntropy::diagnostics::Severity";
            }

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
        struct ClassDeclaration<diagnostics::Context>
        {
            static constexpr const char* GetName() noexcept
            {
                return "syntropy::diagnostics::Context";
            }

            void operator()(ClassDefinitionT<diagnostics::Context>& definition) const
            {
                definition << serialization::JSONClass();
            }
        };

        /************************************************************************/
        /* LOG.H                                                                */
        /************************************************************************/

        // Template specialization for LogChannel.
        template <>
        struct ClassDeclaration<diagnostics::LogChannel>
        {
            static constexpr const char* GetName() noexcept
            {
                return "diagnostics::LogChannel";
            }
        };

        /************************************************************************/
        /* LOG_CHANNELS.H                                                       */
        /************************************************************************/

        // Template specialization for StreamLogChannel.
        template <>
        struct ClassDeclaration<diagnostics::StreamLogChannel>
        {
            static constexpr const char* GetName() noexcept
            {
                return "syntropy::diagnostics::StreamLogChannel";
            }

            void operator()(ClassDefinitionT<diagnostics::StreamLogChannel>& definition) const
            {
                definition.DefineNameAlias("StreamLogChannel");

                definition.DefineBaseClass<diagnostics::LogChannel>();
            }
        };

        // Template specialization for FileLogChannel.
        template <>
        struct ClassDeclaration<diagnostics::FileLogChannel>
        {
            static constexpr const char* GetName() noexcept
            {
                return "syntropy::diagnostics::FileLogChannel";
            }

            void operator()(ClassDefinitionT<diagnostics::FileLogChannel>& definition) const
            {
                definition << serialization::JSONClass();

                definition.DefineNameAlias("FileLogChannel");

                definition.DefineBaseClass<diagnostics::StreamLogChannel>();
            }
        };

    }

    namespace serialization
    {

        /************************************************************************/
        /* DIAGNOSTICS.H                                                        */
        /************************************************************************/

        // Template specialization for Context
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

        /************************************************************************/
        /* LOG_CHANNELS.H                                                       */
        /************************************************************************/

        // Template specialization for FileLogChannel
        template <>
        struct JSONDeserializerT<diagnostics::FileLogChannel>
        {
            std::optional<diagnostics::FileLogChannel> operator()(const nlohmann::json& json) const
            {
                static std::vector<diagnostics::Context> default_contexts = { diagnostics::Context() };

                auto file = DeserializeObjectFromJSON<std::string>(json, std::nullopt, "file");
                auto format = DeserializeObjectFromJSON<std::string>(json, "{message}", "format");
                auto contexts = DeserializeObjectFromJSON<std::vector<diagnostics::Context> >(json, default_contexts, "contexts");
                auto verbosity = DeserializeObjectFromJSON<diagnostics::Severity>(json, diagnostics::Severity::kInformative, "verbosity");

                if (file)
                {
                    return std::make_optional<diagnostics::FileLogChannel>(std::move(*file), std::move(*format), std::move(*contexts), *verbosity);
                }

                return std::nullopt;
            }
        };

    }

}
