#include "syntropy/reflection/types/core_types.h"

#include "syntropy/reflection/types/fundamental_types.h"
#include "syntropy/reflection/types/stl_types.h"

#include "syntropy/reflection/reflection.h"

namespace syntropy::reflection
{
    // Hashed string and context

    AutoRegisterClass<Label> hashed_string_class;
    AutoRegisterClass<Context> context_class;

}
