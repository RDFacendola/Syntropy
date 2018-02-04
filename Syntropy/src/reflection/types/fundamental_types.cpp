#include "reflection/types/fundamental_types.h"

#include "reflection/reflection.h"

namespace syntropy::reflection
{
    // void

    AutoRegisterClass<void> void_class;

    // boolean

    AutoRegisterClass<bool> bool_class;

    // char

    AutoRegisterClass<signed char> signed_char_class;
    AutoRegisterClass<unsigned char> unsigned_char_class;
    AutoRegisterClass<char> char_class;
    AutoRegisterClass<wchar_t> wchar_t_class;
    AutoRegisterClass<char16_t> char16_t_class;
    AutoRegisterClass<char32_t> char32_t_class;

    // integers

    AutoRegisterClass<short int> short_int_class;
    AutoRegisterClass<unsigned short int> usnigned_short_int_class;
    AutoRegisterClass<int> int_class;
    AutoRegisterClass<unsigned int> unsigned_int_class;
    AutoRegisterClass<long int> long_int_class;
    AutoRegisterClass<unsigned long int> unsigned_long_int_class;
    AutoRegisterClass<long long int> long_long_int_class;
    AutoRegisterClass<unsigned long long int> unsigned_long_long_int_class;

    // real

    AutoRegisterClass<float> float_class;
    AutoRegisterClass<double> double_class;
    AutoRegisterClass<long double> long_double_class;
}
