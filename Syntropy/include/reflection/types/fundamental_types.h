
/// \file fundamental_types.h
/// \brief This header is part of the syntropy reflection system. It exposes all the fundamental types supported by the C++ standard to the syntropy reflection system.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "reflection/class.h"

namespace syntropy::reflection
{
    /// \brief Append additional name aliases for fixed width integer types.
    template <typename TClass>
    void AddFixedWidthIntegerTypeDefinition(ClassT<TClass>& class_t)
    {
        if (std::is_same_v<TClass, int8_t>)               class_t.AddNameAlias("int8_t");
        if (std::is_same_v<TClass, int16_t>)              class_t.AddNameAlias("int16_t");
        if (std::is_same_v<TClass, int32_t>)              class_t.AddNameAlias("int32_t");
        if (std::is_same_v<TClass, int64_t>)              class_t.AddNameAlias("int64_t");

        if (std::is_same_v<TClass, int_fast8_t>)          class_t.AddNameAlias("int_fast8_t");
        if (std::is_same_v<TClass, int_fast16_t>)         class_t.AddNameAlias("int_fast16_t");
        if (std::is_same_v<TClass, int_fast32_t>)         class_t.AddNameAlias("int_fast32_t");
        if (std::is_same_v<TClass, int_fast64_t>)         class_t.AddNameAlias("int_fast64_t");

        if (std::is_same_v<TClass, int_least8_t>)         class_t.AddNameAlias("int_least8_t");
        if (std::is_same_v<TClass, int_least16_t>)        class_t.AddNameAlias("int_least16_t");
        if (std::is_same_v<TClass, int_least32_t>)        class_t.AddNameAlias("int_least32_t");
        if (std::is_same_v<TClass, int_least64_t>)        class_t.AddNameAlias("int_least64_t");

        if (std::is_same_v<TClass, intmax_t>)             class_t.AddNameAlias("intmax_t");
        if (std::is_same_v<TClass, intptr_t>)             class_t.AddNameAlias("intptr_t");

        if (std::is_same_v<TClass, uint8_t>)              class_t.AddNameAlias("uint8_t");
        if (std::is_same_v<TClass, uint16_t>)             class_t.AddNameAlias("uint16_t");
        if (std::is_same_v<TClass, uint32_t>)             class_t.AddNameAlias("uint32_t");
        if (std::is_same_v<TClass, uint64_t>)             class_t.AddNameAlias("uint64_t");

        if (std::is_same_v<TClass, uint_fast8_t>)         class_t.AddNameAlias("uint_fast8_t");
        if (std::is_same_v<TClass, uint_fast16_t>)        class_t.AddNameAlias("uint_fast16_t");
        if (std::is_same_v<TClass, uint_fast32_t>)        class_t.AddNameAlias("uint_fast32_t");
        if (std::is_same_v<TClass, uint_fast64_t>)        class_t.AddNameAlias("uint_fast64_t");

        if (std::is_same_v<TClass, uint_least8_t>)        class_t.AddNameAlias("uint_least8_t");
        if (std::is_same_v<TClass, uint_least16_t>)       class_t.AddNameAlias("uint_least16_t");
        if (std::is_same_v<TClass, uint_least32_t>)       class_t.AddNameAlias("uint_least32_t");
        if (std::is_same_v<TClass, uint_least64_t>)       class_t.AddNameAlias("uint_least64_t");

        if (std::is_same_v<TClass, uintmax_t>)            class_t.AddNameAlias("uintmax_t");
        if (std::is_same_v<TClass, uintptr_t>)            class_t.AddNameAlias("uintptr_t");
    }

    /************************************************************************/
    /* VOID                                                                 */
    /************************************************************************/

    template <>
    struct ClassDeclarationT<void>
    {
        static constexpr const char* name_{ "void" };
    };

    /************************************************************************/
    /* BOOLEAN TYPES                                                        */
    /************************************************************************/

    template <>
    struct ClassDeclarationT<bool>
    {
        static constexpr const char* name_{ "bool" };
    };

    /************************************************************************/
    /* CHARACTER TYPES                                                      */
    /************************************************************************/
        
    template <>
    struct ClassDeclarationT<signed char>
    {
        static constexpr const char* name_{ "signed char" };

        void operator()(ClassT<signed char>& class_t) const
        {
            AddFixedWidthIntegerTypeDefinition(class_t);
        }
    };

    template <>
    struct ClassDeclarationT<unsigned char>
    {
        static constexpr const char* name_{ "unsigned char" };

        void operator()(ClassT<unsigned char>& class_t) const
        {
            AddFixedWidthIntegerTypeDefinition(class_t);
        }
    };

    template <>
    struct ClassDeclarationT<char>
    {
        static constexpr const char* name_{ "char" };

        void operator()(ClassT<char>& class_t) const
        {
            AddFixedWidthIntegerTypeDefinition(class_t);
        }
    };

    template <>
    struct ClassDeclarationT<wchar_t>
    {
        static constexpr const char* name_{ "wchar_t" };

        void operator()(ClassT<wchar_t>& class_t) const
        {
            AddFixedWidthIntegerTypeDefinition(class_t);
        }
    };

    template <>
    struct ClassDeclarationT<char16_t>
    {
        static constexpr const char* name_{ "char16_t" };

        void operator()(ClassT<char16_t>& class_t) const
        {
            AddFixedWidthIntegerTypeDefinition(class_t);
        }
    };

    template <>
    struct ClassDeclarationT<char32_t>
    {
        static constexpr const char* name_{ "char32_t" };

        void operator()(ClassT<char32_t>& class_t) const
        {
            AddFixedWidthIntegerTypeDefinition(class_t);
        }
    };

    /************************************************************************/
    /* INTEGER TYPES                                                        */
    /************************************************************************/

    template <>
    struct ClassDeclarationT<short int>
    {
        static constexpr const char* name_{ "short int" };

        void operator()(ClassT<short int>& class_t) const
        {
            class_t.AddNameAlias("short");
            class_t.AddNameAlias("signed short");
            class_t.AddNameAlias("signed short int");

            AddFixedWidthIntegerTypeDefinition(class_t);
        }
    };

    template <>
    struct ClassDeclarationT<unsigned short int>
    {
        static constexpr const char* name_{ "unsigned short int" };

        void operator()(ClassT<unsigned short int>& class_t) const
        {
            class_t.AddNameAlias("unsigned short int");
                
            AddFixedWidthIntegerTypeDefinition(class_t);
        }
    };

    template <>
    struct ClassDeclarationT<int>
    {
        static constexpr const char* name_{ "int" };

        void operator()(ClassT<int>& class_t) const
        {
            class_t.AddNameAlias("signed");
            class_t.AddNameAlias("signed int");

            AddFixedWidthIntegerTypeDefinition(class_t);
        }
    };

    template <>
    struct ClassDeclarationT<unsigned int>
    {
        static constexpr const char* name_{ "unsigned int" };

        void operator()(ClassT<unsigned int>& class_t) const
        {
            class_t.AddNameAlias("unsigned");

            AddFixedWidthIntegerTypeDefinition(class_t);
        }
    };

    template <>
    struct ClassDeclarationT<long int>
    {
        static constexpr const char* name_{ "long int" };

        void operator()(ClassT<long int>& class_t) const
        {
            class_t.AddNameAlias("long");
            class_t.AddNameAlias("signed long");
            class_t.AddNameAlias("signed long int");

            AddFixedWidthIntegerTypeDefinition(class_t);
        }
    };

    template <>
    struct ClassDeclarationT<unsigned long int>
    {
        static constexpr const char* name_{ "unsigned long int" };

        void operator()(ClassT<unsigned long int>& class_t) const
        {
            class_t.AddNameAlias("unsigned long");

            AddFixedWidthIntegerTypeDefinition(class_t);
        }
    };

    template <>
    struct ClassDeclarationT<long long int>
    {
        static constexpr const char* name_{ "long long int" };

        void operator()(ClassT<long long int>& class_t) const
        {
            class_t.AddNameAlias("long long");
            class_t.AddNameAlias("signed long long");
            class_t.AddNameAlias("signed long long int");

            AddFixedWidthIntegerTypeDefinition(class_t);
        }
    };

    template <>
    struct ClassDeclarationT<unsigned long long int>
    {
        static constexpr const char* name_{ "unsigned long long int" };

        void operator()(ClassT<unsigned long long int>& class_t) const
        {
            class_t.AddNameAlias("unsigned long long");

            AddFixedWidthIntegerTypeDefinition(class_t);
        }
    };
        
    /************************************************************************/
    /* FLOATING POINT TYPES                                                 */
    /************************************************************************/

    template <>
    struct ClassDeclarationT<float>
    {
        static constexpr const char* name_{ "float" };
    };

    template <>
    struct ClassDeclarationT<double>
    {
        static constexpr const char* name_{ "double" };
    };

    template <>
    struct ClassDeclarationT<long double>
    {
        static constexpr const char* name_{ "long double" };
    };
}
