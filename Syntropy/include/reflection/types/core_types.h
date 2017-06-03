
/// \file core_types.h
/// \brief This header is part of the syntropy reflection system. It exposes all the types supported by syntropy to the syntropy reflection system.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "containers/hashed_string.h"

#include "reflection/class.h"

namespace syntropy
{

    namespace reflection
    {

        /************************************************************************/
        /* HASHED STRING                                                        */
        /************************************************************************/

        template <>
        struct ClassDeclaration<HashedString>
        {
            static constexpr const char* GetName() noexcept
            {
                return "syntropy::HashedString";
            }

            void operator()(ClassDefinitionT<HashedString> definition) const
            {
                definition.DefineNameAlias("HashedString");
            }
        };

    }

}
