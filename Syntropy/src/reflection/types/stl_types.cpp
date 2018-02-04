#include "reflection/types/stl_types.h"

#include "reflection/types/fundamental_types.h"

#include "reflection/reflection.h"

namespace syntropy::reflection
{
    // nullptr

    AutoRegisterClass<std::nullptr_t> nullptr_class;

    // string

    AutoRegisterClass<std::string> string_class;
    AutoRegisterClass<std::wstring> wstring_class;
}
