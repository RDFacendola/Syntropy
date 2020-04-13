
/// \file memory_meta.h
/// \brief This header is part of the syntropy memory system. It contains reflection and serialization definitions for the syntropy memory system.
/// Definitions are provided in a separate file to avoid circular dependencies among systems.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <algorithm>

#include "syntropy/memory/bytes.h"

#include "syntropy/memory/allocators/allocator.h"
//#include "syntropy/memory/allocators/segregated_allocator.h"

#include "syntropy/reflection/reflection.h"
#include "syntropy/reflection/types/fundamental_types.h"
#include "syntropy/reflection/types/stl_types.h"
#include "syntropy/reflection/types/core_types.h"

#include "syntropy/serialization/json/json.h"
#include "syntropy/serialization/json/deserializers/fundamental_deserializers.h"
#include "syntropy/serialization/json/deserializers/core_deserializers.h"
#include "syntropy/serialization/json/deserializers/stl_deserializers.h"

namespace syntropy::reflection
{
    
    /************************************************************************/
    /* MEMORY UNITS.H                                                       */
    /************************************************************************/

    // Template specialization for Bytes
    template<>
    struct ClassDeclarationT<Bytes>
    {
        static constexpr const char* name_{ "syntropy::Bytes" };
    };

    /************************************************************************/
    /* ALLOCATOR.H                                                          */
    /************************************************************************/

    // Template specialization for Allocator
    template<>
    struct ClassDeclarationT<Allocator>
    {
        static constexpr const char* name_{ "syntropy::Allocator" };

        void operator()(ClassT<Allocator>& class_t) const
        {
            class_t.AddProperty("name", &Allocator::GetName);
            class_t.AddProperty("max_allocation_size", &Allocator::GetMaxAllocationSize);
        }
    };

    /************************************************************************/
    /* SEGREGATED ALLOCATOR.H                                               */
    /************************************************************************/

    // Template specialization for LinearSegregatedFitAllocator
//     template<>
//     struct ClassDeclarationT<LinearSegregatedFitAllocator>
//     {
//         static constexpr const char* name_{ "syntropy::LinearSegregatedFitAllocator" };
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
//         static constexpr const char* name_{ "syntropy::ExponentialSegregatedFitAllocator" };
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
//         static constexpr const char* name_{ "syntropy::TwoLevelSegregatedFitAllocator" };
// 
//         void operator()(ClassT<TwoLevelSegregatedFitAllocator>& class_t) const
//         {
//             class_t << serialization::JSONClass();
// 
//             class_t.AddBaseClass<Allocator>();
//         }
//     };

}

namespace syntropy::serialization
{

    /************************************************************************/
    /* MEMORY UNITS.H                                                       */
    /************************************************************************/

    // Template specialization for Bytes
    template <>
    struct JSONDeserializerT<Bytes>
    {
        std::optional<Bytes> operator()(const nlohmann::json& json) const
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
//         std::optional<LinearSegregatedFitAllocator> operator()(const nlohmann::json& json) const
//         {
//             auto name = DeserializeObjectFromJSON<std::string>(json, std::nullopt, "name");
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
//         std::optional<ExponentialSegregatedFitAllocator> operator()(const nlohmann::json& json) const
//         {
//             auto name = DeserializeObjectFromJSON<std::string>(json, std::nullopt, "name");
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
//         std::optional<TwoLevelSegregatedFitAllocator> operator()(const nlohmann::json& json) const
//         {
//             auto name = DeserializeObjectFromJSON<std::string>(json, std::nullopt, "name");
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