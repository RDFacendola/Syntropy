/// \file shared.h
/// \brief Temporary file for syntropy refactoring.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <memory>

#include "syntropy/containers/map.h"
#include "syntropy/containers/vector.h"

#include "syntropy/types/string.h"

#include "nlohmann/json.hpp"

#include "syntropy/experimental/core/polymorphic_allocator.h"

namespace Syntropy::serialization
{
    // NOTE: Even if NLohmann says basic_json can use unordered_maps as object types, the fact that Hash and KeyEqual template types are not propagated correctly prohibits so. 

    // NOTE: We can use a proxy type that has the same template argument list as an ordinary std::map but aliases to a std::unordered_map, however
    //       it may trigger undefined\untested behaviors. So we use std::map rather than std::unordered_map here.

    //using JSON = nlohmann::basic_json<
    //    std::map,
    //    std::vector,
    //    String,
    //    bool,
    //    std::int64_t,
    //    std::uint64_t,
    //    double,
    //    PolymorphicAllocator,
    //    nlohmann::adl_serializer
    //>;

    // #TEMP: won't use custom containers.

    using JSON = nlohmann::json;
}