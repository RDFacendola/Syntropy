#include "reflection/types/core_types.h"

#include "reflection/types/fundamental_types.h"
#include "reflection/types/stl_types.h"

#include "reflection/reflection.h"

namespace syntropy::reflection
{
    // Hashed string and context

    AutoRegisterClass<HashedString> hashed_string_class;
    AutoRegisterClass<Context> context_class;

}
