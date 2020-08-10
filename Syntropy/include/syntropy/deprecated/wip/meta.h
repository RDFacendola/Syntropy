
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

namespace Syntropy::reflection
{
    /************************************************************************/
    /* DIAGNOSTICS.H                                                        */
    /************************************************************************/

    // Template specialization for Severity
    template<>
    struct ClassDeclarationT<diagnostics::Severity>
    {
        static constexpr const char* name_{ "Syntropy::diagnostics::Severity" };

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
        static constexpr const char* name_{ "Syntropy::diagnostics::LogChannel" };
    };

    /************************************************************************/
    /* LOG_CHANNELS.H                                                       */
    /************************************************************************/

    // Template specialization for StreamLogChannel.
    template <>
    struct ClassDeclarationT<diagnostics::StreamLogChannel>
    {
        static constexpr const char* name_{ "Syntropy::diagnostics::StreamLogChannel" };

        void operator()(ClassT<diagnostics::StreamLogChannel>& class_t) const
        {
            class_t.AddBaseClass<diagnostics::LogChannel>();
        }
    };

    // Template specialization for FileLogChannel.
    template <>
    struct ClassDeclarationT<diagnostics::FileLogChannel>
    {
        static constexpr const char* name_{ "Syntropy::diagnostics::FileLogChannel" };

        void operator()(ClassT<diagnostics::FileLogChannel>& class_t) const
        {
            class_t << serialization::JSONClass();

            class_t.AddBaseClass<diagnostics::StreamLogChannel>();
        }
    };
}

namespace Syntropy::serialization
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


/************************************************************************/
/* MEMORY UNITS.H                                                       */
/************************************************************************/

// Template specialization for Bytes
template<>
struct ClassDeclarationT<Bytes>
{
    static constexpr const char* name_{ "Syntropy::Bytes" };
};

/************************************************************************/
/* SEGREGATED ALLOCATOR.H                                               */
/************************************************************************/

// Template specialization for LinearSegregatedFitAllocator
//     template<>
//     struct ClassDeclarationT<LinearSegregatedFitAllocator>
//     {
//         static constexpr const char* name_{ "Syntropy::LinearSegregatedFitAllocator" };
// 
//         void operator()(ClassT<LinearSegregatedFitAllocator>& class_t) const
//         {
//             class_t << serialization::JSONClass();
// 
//             class_t.AddBaseClass<Allocator>();
// 
//             class_t.AddProperty("order", &LinearSegregatedFitAllocator::GetOrder);
//             class_t.AddProperty("page_size", &LinearSegregatedFitAllocator::GetPageSize);
//         }
//     };

    // Template specialization for ExponentialSegregatedFitAllocator
//     template<>
//     struct ClassDeclarationT<ExponentialSegregatedFitAllocator>
//     {
//         static constexpr const char* name_{ "Syntropy::ExponentialSegregatedFitAllocator" };
// 
//         void operator()(ClassT<ExponentialSegregatedFitAllocator>& class_t) const
//         {
//             class_t << serialization::JSONClass();
// 
//             class_t.AddBaseClass<Allocator>();
// 
//             class_t.AddProperty("order", &ExponentialSegregatedFitAllocator::GetOrder);
//             class_t.AddProperty("class_size", &ExponentialSegregatedFitAllocator::GetClassSize);
//         }
//     };

    // Template specialization for TwoLevelSegregatedFitAllocator
//     template<>
//     struct ClassDeclarationT<TwoLevelSegregatedFitAllocator>
//     {
//         static constexpr const char* name_{ "Syntropy::TwoLevelSegregatedFitAllocator" };
// 
//         void operator()(ClassT<TwoLevelSegregatedFitAllocator>& class_t) const
//         {
//             class_t << serialization::JSONClass();
// 
//             class_t.AddBaseClass<Allocator>();
//         }
//     };

}

namespace Syntropy::serialization
{

    /************************************************************************/
    /* MEMORY UNITS.H                                                       */
    /************************************************************************/

    // Template specialization for Bytes
    template <>
    struct JSONDeserializerT<Bytes>
    {
        std::optional<Bytes> operator()(const JSON& json) const
        {
            if (json.is_number())
            {
                return Bytes(json.get<size_t>());
            }
            return std::nullopt;
        }
    };

    /************************************************************************/
    /* SEGREGATED ALLOCATOR.H                                               */
    /************************************************************************/

    // Template specialization for LinearSegregatedFitAllocator
//     template <>
//     struct JSONDeserializerT<LinearSegregatedFitAllocator>
//     {
//         std::optional<LinearSegregatedFitAllocator> operator()(const JSON& json) const
//         {
//             auto name = DeserializeObjectFromJSON<String>(json, std::nullopt, "name");
//             auto capacity = DeserializeObjectFromJSON<Bytes>(json, std::nullopt, "capacity");
//             auto class_size = DeserializeObjectFromJSON<Bytes>(json, 8_Bytes, "class_size");
//             auto order = DeserializeObjectFromJSON<size_t>(json, 32, "order");
//             auto page_size = DeserializeObjectFromJSON<Bytes>(json, 16_KiBytes, "page_size");
// 
//             if (name && capacity)
//             {
//                 return LinearSegregatedFitAllocator(std::move(*name), *capacity, *class_size, *order, *page_size);
//             }
// 
//             return std::nullopt;
//         }
//     };

    // Template specialization for ExponentialSegregatedFitAllocator
//     template <>
//     struct JSONDeserializerT<ExponentialSegregatedFitAllocator>
//     {
//         std::optional<ExponentialSegregatedFitAllocator> operator()(const JSON& json) const
//         {
//             auto name = DeserializeObjectFromJSON<String>(json, std::nullopt, "name");
//             auto capacity = DeserializeObjectFromJSON<Bytes>(json, std::nullopt, "capacity");
//             auto class_size = DeserializeObjectFromJSON<Bytes>(json, 64_KiBytes, "class_size");
//             auto order = DeserializeObjectFromJSON<size_t>(json, 10, "order");
// 
//             if (name && capacity)
//             {
//                 return ExponentialSegregatedFitAllocator(std::move(*name), *capacity, *class_size, *order);
//             }
// 
//             return std::nullopt;
//         }
//     };

    // Template specialization for TwoLevelSegregatedFitAllocator
//     template <>
//     struct JSONDeserializerT<TwoLevelSegregatedFitAllocator>
//     {
//         std::optional<TwoLevelSegregatedFitAllocator> operator()(const JSON& json) const
//         {
//             auto name = DeserializeObjectFromJSON<String>(json, std::nullopt, "name");
//             auto capacity = DeserializeObjectFromJSON<Bytes>(json, std::nullopt, "capacity");
//             auto second_level_index = DeserializeObjectFromJSON<size_t>(json, 5, "second_level_index");
// 
//             if (name && capacity)
//             {
//                 return TwoLevelSegregatedFitAllocator(std::move(*name), *capacity, *second_level_index);
//             }
// 
//             return std::nullopt;
//         }
//     };

}