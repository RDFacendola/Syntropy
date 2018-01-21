
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
    void AddFixedWidthIntegerTypeDefinition(ClassDefinitionT<TClass>& definition)
    {
        if (std::is_same_v<TClass, int8_t>)               definition.DefineNameAlias("int8_t");
        if (std::is_same_v<TClass, int16_t>)              definition.DefineNameAlias("int16_t");
        if (std::is_same_v<TClass, int32_t>)              definition.DefineNameAlias("int32_t");
        if (std::is_same_v<TClass, int64_t>)              definition.DefineNameAlias("int64_t");

        if (std::is_same_v<TClass, int_fast8_t>)          definition.DefineNameAlias("int_fast8_t");
        if (std::is_same_v<TClass, int_fast16_t>)         definition.DefineNameAlias("int_fast16_t");
        if (std::is_same_v<TClass, int_fast32_t>)         definition.DefineNameAlias("int_fast32_t");
        if (std::is_same_v<TClass, int_fast64_t>)         definition.DefineNameAlias("int_fast64_t");

        if (std::is_same_v<TClass, int_least8_t>)         definition.DefineNameAlias("int_least8_t");
        if (std::is_same_v<TClass, int_least16_t>)        definition.DefineNameAlias("int_least16_t");
        if (std::is_same_v<TClass, int_least32_t>)        definition.DefineNameAlias("int_least32_t");
        if (std::is_same_v<TClass, int_least64_t>)        definition.DefineNameAlias("int_least64_t");

        if (std::is_same_v<TClass, intmax_t>)             definition.DefineNameAlias("intmax_t");
        if (std::is_same_v<TClass, intptr_t>)             definition.DefineNameAlias("intptr_t");

        if (std::is_same_v<TClass, uint8_t>)              definition.DefineNameAlias("uint8_t");
        if (std::is_same_v<TClass, uint16_t>)             definition.DefineNameAlias("uint16_t");
        if (std::is_same_v<TClass, uint32_t>)             definition.DefineNameAlias("uint32_t");
        if (std::is_same_v<TClass, uint64_t>)             definition.DefineNameAlias("uint64_t");

        if (std::is_same_v<TClass, uint_fast8_t>)         definition.DefineNameAlias("uint_fast8_t");
        if (std::is_same_v<TClass, uint_fast16_t>)        definition.DefineNameAlias("uint_fast16_t");
        if (std::is_same_v<TClass, uint_fast32_t>)        definition.DefineNameAlias("uint_fast32_t");
        if (std::is_same_v<TClass, uint_fast64_t>)        definition.DefineNameAlias("uint_fast64_t");

        if (std::is_same_v<TClass, uint_least8_t>)        definition.DefineNameAlias("uint_least8_t");
        if (std::is_same_v<TClass, uint_least16_t>)       definition.DefineNameAlias("uint_least16_t");
        if (std::is_same_v<TClass, uint_least32_t>)       definition.DefineNameAlias("uint_least32_t");
        if (std::is_same_v<TClass, uint_least64_t>)       definition.DefineNameAlias("uint_least64_t");

        if (std::is_same_v<TClass, uintmax_t>)            definition.DefineNameAlias("uintmax_t");
        if (std::is_same_v<TClass, uintptr_t>)            definition.DefineNameAlias("uintptr_t");
    }

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

        void operator()(ClassDefinitionT<signed char>& definition) const
        {
            AddFixedWidthIntegerTypeDefinition(definition);
        }
    };

    template <>
    struct ClassDeclarationT<unsigned char>
    {
        static constexpr const char* name_{ "unsigned char" };

        void operator()(ClassDefinitionT<unsigned char>& definition) const
        {
            AddFixedWidthIntegerTypeDefinition(definition);
        }
    };

    template <>
    struct ClassDeclarationT<char>
    {
        static constexpr const char* name_{ "char" };

        void operator()(ClassDefinitionT<char>& definition) const
        {
            AddFixedWidthIntegerTypeDefinition(definition);
        }
    };

    template <>
    struct ClassDeclarationT<wchar_t>
    {
        static constexpr const char* name_{ "wchar_t" };

        void operator()(ClassDefinitionT<wchar_t>& definition) const
        {
            AddFixedWidthIntegerTypeDefinition(definition);
        }
    };

    template <>
    struct ClassDeclarationT<char16_t>
    {
        static constexpr const char* name_{ "char16_t" };

        void operator()(ClassDefinitionT<char16_t>& definition) const
        {
            AddFixedWidthIntegerTypeDefinition(definition);
        }
    };

    template <>
    struct ClassDeclarationT<char32_t>
    {
        static constexpr const char* name_{ "char32_t" };

        void operator()(ClassDefinitionT<char32_t>& definition) const
        {
            AddFixedWidthIntegerTypeDefinition(definition);
        }
    };

    /************************************************************************/
    /* INTEGER TYPES                                                        */
    /************************************************************************/

    template <>
    struct ClassDeclarationT<short int>
    {
        static constexpr const char* name_{ "short int" };

        void operator()(ClassDefinitionT<short int>& definition) const
        {
            definition.DefineNameAlias("short");
            definition.DefineNameAlias("signed short");
            definition.DefineNameAlias("signed short int");

            AddFixedWidthIntegerTypeDefinition(definition);
        }
    };

    template <>
    struct ClassDeclarationT<unsigned short int>
    {
        static constexpr const char* name_{ "unsigned short int" };

        void operator()(ClassDefinitionT<unsigned short int>& definition) const
        {
            definition.DefineNameAlias("unsigned short int");
                
            AddFixedWidthIntegerTypeDefinition(definition);
        }
    };

    template <>
    struct ClassDeclarationT<int>
    {
        static constexpr const char* name_{ "int" };

        void operator()(ClassDefinitionT<int>& definition) const
        {
            definition.DefineNameAlias("signed");
            definition.DefineNameAlias("signed int");

            AddFixedWidthIntegerTypeDefinition(definition);
        }
    };

    template <>
    struct ClassDeclarationT<unsigned int>
    {
        static constexpr const char* name_{ "unsigned int" };

        void operator()(ClassDefinitionT<unsigned int>& definition) const
        {
            definition.DefineNameAlias("unsigned");

            AddFixedWidthIntegerTypeDefinition(definition);
        }
    };

    template <>
    struct ClassDeclarationT<long int>
    {
        static constexpr const char* name_{ "long int" };

        void operator()(ClassDefinitionT<long int>& definition) const
        {
            definition.DefineNameAlias("long");
            definition.DefineNameAlias("signed long");
            definition.DefineNameAlias("signed long int");

            AddFixedWidthIntegerTypeDefinition(definition);
        }
    };

    template <>
    struct ClassDeclarationT<unsigned long int>
    {
        static constexpr const char* name_{ "unsigned long int" };

        void operator()(ClassDefinitionT<unsigned long int>& definition) const
        {
            definition.DefineNameAlias("unsigned long");

            AddFixedWidthIntegerTypeDefinition(definition);
        }
    };

    template <>
    struct ClassDeclarationT<long long int>
    {
        static constexpr const char* name_{ "long long int" };

        void operator()(ClassDefinitionT<long long int>& definition) const
        {
            definition.DefineNameAlias("long long");
            definition.DefineNameAlias("signed long long");
            definition.DefineNameAlias("signed long long int");

            AddFixedWidthIntegerTypeDefinition(definition);
        }
    };

    template <>
    struct ClassDeclarationT<unsigned long long int>
    {
        static constexpr const char* name_{ "unsigned long long int" };

        void operator()(ClassDefinitionT<unsigned long long int>& definition) const
        {
            definition.DefineNameAlias("unsigned long long");

            AddFixedWidthIntegerTypeDefinition(definition);
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
