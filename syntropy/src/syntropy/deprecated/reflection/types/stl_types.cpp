#include "syntropy/reflection/types/stl_types.h"
#include "syntropy/reflection/types/core_types.h"
#include "syntropy/reflection/types/fundamental_types.h"

#include "syntropy/reflection/reflection.h"

namespace Syntropy::reflection
{
    // nullptr

    AutoRegisterClass<std::nullptr_t> nullptr_class;

    // string

    AutoRegisterClass<String> string_class;
    AutoRegisterClass<std::wstring> wstring_class;
}
