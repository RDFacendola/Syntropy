
/// \file diagnostics_meta.h
/// \brief This header is part of the syntropy diagnostic system. It contains reflection and serialization definitions for the syntropy diagnostic system.
/// Definitions are provided in a separate file to avoid circular dependencies among syntropy diagnostic, reflection and serialization systems.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "syntropy/containers/context.h"

#include "syntropy/diagnostics/diagnostics.h"
#include "syntropy/diagnostics/log.h"
#include "syntropy/diagnostics/log_channels.h"

#include "syntropy/reflection/class.h"
#include "syntropy/reflection/types/fundamental_types.h"
#include "syntropy/reflection/types/stl_types.h"
#include "syntropy/reflection/types/core_types.h"

#include "syntropy/serialization/json/json.h"
#include "syntropy/serialization/json/deserializers/stl_deserializers.h"

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

        void operator()(ClassT<diagnostics::Severity>& class_t) const
        {
            using diagnostics::Severity;

            class_t << serialization::JSONClass();

            class_t << reflection::EnumerationClass<Severity>(
            {
                {"Informative", Severity::kInformative},
                {"Warning", Severity::kWarning},
                {"Error", Severity::kError},
                {"Critical", Severity::kCritical}
            });
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

        void operator()(ClassT<diagnostics::StreamLogChannel>& class_t) const
        {
            class_t.AddBaseClass<diagnostics::LogChannel>();
        }
    };

    // Template specialization for FileLogChannel.
    template <>
    struct ClassDeclarationT<diagnostics::FileLogChannel>
    {
        static constexpr const char* name_{ "syntropy::diagnostics::FileLogChannel" };

        void operator()(ClassT<diagnostics::FileLogChannel>& class_t) const
        {
            class_t << serialization::JSONClass();

            class_t.AddBaseClass<diagnostics::StreamLogChannel>();
        }
    };
}

namespace syntropy::serialization
{
    /************************************************************************/
    /* LOG_CHANNELS.H                                                       */
    /************************************************************************/

    // Template specialization for FileLogChannel
    template <>
    struct JSONDeserializerT<diagnostics::FileLogChannel>
    {
        std::optional<diagnostics::FileLogChannel> operator()(const JSON& json) const
        {
            static Vector<Context> default_contexts = { Context() };

            auto file = DeserializeObjectFromJSON<String>(json, std::nullopt, "file");
            auto format = DeserializeObjectFromJSON<String>(json, "{message}", "format");
            auto contexts = DeserializeObjectFromJSON<Vector<Context> >(json, default_contexts, "contexts");
            auto verbosity = DeserializeObjectFromJSON<diagnostics::Severity>(json, diagnostics::Severity::kInformative, "verbosity");

            if (file)
            {
                return std::make_optional<diagnostics::FileLogChannel>(std::move(*file), std::move(*format), std::move(*contexts), *verbosity);
            }

            return std::nullopt;
        }
    };
}
