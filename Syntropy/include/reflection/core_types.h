
/// \file core_types.h
/// \brief This header is part of the syntropy reflection system. It contains the definition of all the core types of syntropy.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "class.h"

#include "containers/hashed_string.h"

namespace syntropy {

    namespace reflection {

        //////////////// SYNTROPY::HASHEDSTRING ////////////////

        template <>
        struct ClassDeclaration<HashedString> {

            static constexpr const char* GetName() noexcept
            {
                return "syntropy::HashedString";
            }

            template <typename TDefinition>
            void operator()(TDefinition definition) const
            {
                definition.DefineNameAlias("HashedString");
            }

        };

    }

}
