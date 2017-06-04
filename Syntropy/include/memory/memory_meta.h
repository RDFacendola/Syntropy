
/// \file memory_meta.h
/// \brief This header is part of the syntropy memory system. It contains reflection and serialization definitions for the syntropy memory system.
/// Definitions are provided in a separate file to avoid circular dependencies among systems.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "memory/memory_units.h"

#include "memory/allocators/allocator.h"
#include "memory/allocators/segregated_allocator.h"
#include "memory/allocators/master_allocator.h"

#include "reflection/reflection.h"
#include "reflection/types/stl_types.h"
#include "reflection/types/core_types.h"

#include "serialization/json/json.h"
#include "serialization/json/deserializers/stl_deserializers.h"

namespace syntropy
{

    namespace reflection
    {

        /************************************************************************/
        /* ALLOCATOR.H                                                          */
        /************************************************************************/

        // Template specialization for Allocator
        template<>
        struct ClassDeclaration<Allocator>
        {
            static constexpr const char* GetName() noexcept
            {
                return "syntropy::Allocator";
            }

            void operator()(ClassDefinitionT<Allocator>& definition) const
            {
                definition.DefineProperty("name", &Allocator::GetName);
                definition.DefineProperty("max_allocation_size", &Allocator::GetMaxAllocationSize);
            }
        };

        /************************************************************************/
        /* SEGREGATED ALLOCATOR.H                                               */
        /************************************************************************/

        // Template specialization for LinearSegregatedFitAllocator
        template<>
        struct ClassDeclaration<LinearSegregatedFitAllocator>
        {
            static constexpr const char* GetName() noexcept
            {
                return "syntropy::LinearSegregatedFitAllocator";
            }

            void operator()(ClassDefinitionT<LinearSegregatedFitAllocator>& definition) const
            {
                definition << serialization::JSONClass();

                definition.DefineBaseClass<Allocator>();

                definition.DefineProperty("order", &LinearSegregatedFitAllocator::GetOrder);
                definition.DefineProperty("page_size", &LinearSegregatedFitAllocator::GetPageSize);
            }
        };

        // Template specialization for ExponentialSegregatedFitAllocator
        template<>
        struct ClassDeclaration<ExponentialSegregatedFitAllocator>
        {
            static constexpr const char* GetName() noexcept
            {
                return "syntropy::ExponentialSegregatedFitAllocator";
            }

            void operator()(ClassDefinitionT<ExponentialSegregatedFitAllocator>& definition) const
            {
                definition << serialization::JSONClass();

                definition.DefineBaseClass<Allocator>();

                definition.DefineProperty("order", &ExponentialSegregatedFitAllocator::GetOrder);
                definition.DefineProperty("class_size", &ExponentialSegregatedFitAllocator::GetClassSize);
            }
        };

        // Template specialization for TwoLevelSegregatedFitAllocator
        template<>
        struct ClassDeclaration<TwoLevelSegregatedFitAllocator>
        {
            static constexpr const char* GetName() noexcept
            {
                return "syntropy::TwoLevelSegregatedFitAllocator";
            }

            void operator()(ClassDefinitionT<TwoLevelSegregatedFitAllocator>& definition) const
            {
                definition << serialization::JSONClass();

                definition.DefineBaseClass<Allocator>();
            }
        };

        /************************************************************************/
        /* MASTER ALLOCATOR.H                                                   */
        /************************************************************************/

        // Template specialization for MasterAllocator
        template<>
        struct ClassDeclaration<MasterAllocator>
        {
            static constexpr const char* GetName() noexcept
            {
                return "syntropy::MasterAllocator";
            }

            void operator()(ClassDefinitionT<MasterAllocator>& definition) const
            {
                //definition << serialization::JSONClass();

                definition.DefineBaseClass<Allocator>();
            }
        };

    }

    namespace serialization
    {

        /************************************************************************/
        /* SEGREGATED ALLOCATOR.H                                               */
        /************************************************************************/

        // Template specialization for LinearSegregatedFitAllocator
        template <>
        struct JSONDeserializerT<LinearSegregatedFitAllocator>
        {
            std::optional<LinearSegregatedFitAllocator> operator()(const nlohmann::json& json) const
            {
                auto name = DeserializeObjectFromJSON<std::string>(json, std::nullopt, "name");
                auto capacity = DeserializeObjectFromJSON<size_t>(json, std::nullopt, "capacity");
                auto class_size = DeserializeObjectFromJSON<size_t>(json, 8_Bytes, "class_size");
                auto order = DeserializeObjectFromJSON<size_t>(json, 32, "order");
                auto page_size = DeserializeObjectFromJSON<size_t>(json, 16_KiBytes, "page_size");

                if (name && capacity)
                {
                    return LinearSegregatedFitAllocator(std::move(*name), *capacity, *class_size, *order, *page_size);
                }

                return std::nullopt;
            }
        };

        // Template specialization for ExponentialSegregatedFitAllocator
        template <>
        struct JSONDeserializerT<ExponentialSegregatedFitAllocator>
        {
            std::optional<ExponentialSegregatedFitAllocator> operator()(const nlohmann::json& json) const
            {
                auto name = DeserializeObjectFromJSON<std::string>(json, std::nullopt, "name");
                auto capacity = DeserializeObjectFromJSON<size_t>(json, std::nullopt, "capacity");
                auto class_size = DeserializeObjectFromJSON<size_t>(json, 64_KiBytes, "class_size");
                auto order = DeserializeObjectFromJSON<size_t>(json, 10, "order");

                if (name && capacity)
                {
                    return ExponentialSegregatedFitAllocator(std::move(*name), *capacity, *class_size, *order);
                }

                return std::nullopt;
            }
        };

        // Template specialization for TwoLevelSegregatedFitAllocator
        template <>
        struct JSONDeserializerT<TwoLevelSegregatedFitAllocator>
        {
            std::optional<TwoLevelSegregatedFitAllocator> operator()(const nlohmann::json& json) const
            {
                auto name = DeserializeObjectFromJSON<std::string>(json, std::nullopt, "name");
                auto capacity = DeserializeObjectFromJSON<size_t>(json, std::nullopt, "capacity");
                auto second_level_index = DeserializeObjectFromJSON<size_t>(json, 5, "second_level_index");

                if (name && capacity)
                {
                    return TwoLevelSegregatedFitAllocator(std::move(*name), *capacity, *second_level_index);
                }

                return std::nullopt;
            }
        };

    }
}