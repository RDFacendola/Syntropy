
/// \file core_types.h
/// \brief This header is part of the syntropy reflection system. It exposes all the types supported by syntropy to the syntropy reflection system.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "syntropy/types/label.h"
#include "syntropy/containers/context.h"
#include "syntropy/math/hash.h"

#include "syntropy/reflection/class.h"

#include "syntropy/serialization/json/json.h"

namespace syntropy::reflection
{
    /************************************************************************/
    /* HASHED STRING                                                        */
    /************************************************************************/

    template <>
    struct ClassDeclarationT<Label>
    {
        static constexpr const char* name_{ "syntropy::Label" };
    };

    /************************************************************************/
    /* CONTEXT                                                              */
    /************************************************************************/

    template<>
    struct ClassDeclarationT<Context>
    {
        static constexpr const char* name_{ "syntropy::diagnostics::Context" };

        void operator()(ClassT<Context>& class_t) const
        {
            class_t << serialization::JSONClass();
        }
    };

}
