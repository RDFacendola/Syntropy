
/// \file core_types.h
/// \brief This header is part of the syntropy reflection system. It exposes all the types supported by syntropy to the syntropy reflection system.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "containers/hashed_string.h"
#include "math/hash.h"

#include "reflection/class.h"

namespace syntropy::reflection
{
    /************************************************************************/
    /* HASHED STRING                                                        */
    /************************************************************************/

    template <>
    struct ClassDeclarationT<HashedString>
    {
        static constexpr const char* name_{ "syntropy::HashedString" };
    };

    /************************************************************************/
    /* HASHES                                                               */
    /************************************************************************/

    template <typename TString>
    struct ClassDeclarationT<StringHasher64<TString>>
    {
        static constexpr const char* name_{ "syntropy::StringHasher64" };
    };

    template <typename TString>
    struct ClassDeclarationT<StringHasher32<TString>>
    {
        static constexpr const char* name_{ "syntropy::StringHasher32" };
    };
}
