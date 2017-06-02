
/// \file fundamental_types.h
/// \brief This header is part of the syntropy reflection system. It exposes all the fundamental types supported by the C++ standard to the syntropy reflection system.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "reflection/class.h"

namespace syntropy
{
    namespace reflection
    {

        /// \brief Append additional name aliases for fixed width integer types.
        template <typename TClass>
        void AddFixedWidthIntegerTypeDefinition(ClassDefinitionT<TClass>& definition)
        {
            if (std::is_same<TClass, int8_t>::value)               definition.DefineNameAlias("int8_t");
            if (std::is_same<TClass, int16_t>::value)              definition.DefineNameAlias("int16_t");
            if (std::is_same<TClass, int32_t>::value)              definition.DefineNameAlias("int32_t");
            if (std::is_same<TClass, int64_t>::value)              definition.DefineNameAlias("int64_t");

            if (std::is_same<TClass, int_fast8_t>::value)          definition.DefineNameAlias("int_fast8_t");
            if (std::is_same<TClass, int_fast16_t>::value)         definition.DefineNameAlias("int_fast16_t");
            if (std::is_same<TClass, int_fast32_t>::value)         definition.DefineNameAlias("int_fast32_t");
            if (std::is_same<TClass, int_fast64_t>::value)         definition.DefineNameAlias("int_fast64_t");

            if (std::is_same<TClass, int_least8_t>::value)         definition.DefineNameAlias("int_least8_t");
            if (std::is_same<TClass, int_least16_t>::value)        definition.DefineNameAlias("int_least16_t");
            if (std::is_same<TClass, int_least32_t>::value)        definition.DefineNameAlias("int_least32_t");
            if (std::is_same<TClass, int_least64_t>::value)        definition.DefineNameAlias("int_least64_t");

            if (std::is_same<TClass, intmax_t>::value)             definition.DefineNameAlias("intmax_t");
            if (std::is_same<TClass, intptr_t>::value)             definition.DefineNameAlias("intptr_t");

            if (std::is_same<TClass, uint8_t>::value)              definition.DefineNameAlias("uint8_t");
            if (std::is_same<TClass, uint16_t>::value)             definition.DefineNameAlias("uint16_t");
            if (std::is_same<TClass, uint32_t>::value)             definition.DefineNameAlias("uint32_t");
            if (std::is_same<TClass, uint64_t>::value)             definition.DefineNameAlias("uint64_t");

            if (std::is_same<TClass, uint_fast8_t>::value)         definition.DefineNameAlias("uint_fast8_t");
            if (std::is_same<TClass, uint_fast16_t>::value)        definition.DefineNameAlias("uint_fast16_t");
            if (std::is_same<TClass, uint_fast32_t>::value)        definition.DefineNameAlias("uint_fast32_t");
            if (std::is_same<TClass, uint_fast64_t>::value)        definition.DefineNameAlias("uint_fast64_t");

            if (std::is_same<TClass, uint_least8_t>::value)        definition.DefineNameAlias("uint_least8_t");
            if (std::is_same<TClass, uint_least16_t>::value)       definition.DefineNameAlias("uint_least16_t");
            if (std::is_same<TClass, uint_least32_t>::value)       definition.DefineNameAlias("uint_least32_t");
            if (std::is_same<TClass, uint_least64_t>::value)       definition.DefineNameAlias("uint_least64_t");

            if (std::is_same<TClass, uintmax_t>::value)            definition.DefineNameAlias("uintmax_t");
            if (std::is_same<TClass, uintptr_t>::value)            definition.DefineNameAlias("uintptr_t");
        }

        /************************************************************************/
        /* BOOLEAN TYPES                                                        */
        /************************************************************************/

        template <>
        struct ClassDeclaration<bool>
        {
            static constexpr const char* GetName() noexcept
            {
                return "bool";
            }
        };

        /************************************************************************/
        /* CHARACTER TYPES                                                      */
        /************************************************************************/
        
        template <>
        struct ClassDeclaration<signed char>
        {
            static constexpr const char* GetName() noexcept
            {
                return "signed char";
            }

            void operator()(ClassDefinitionT<signed char>& definition) const
            {
                AddFixedWidthIntegerTypeDefinition(definition);
            }
        };

        template <>
        struct ClassDeclaration<unsigned char>
        {
            static constexpr const char* GetName() noexcept
            {
                return "unsigned char";
            }

            void operator()(ClassDefinitionT<unsigned char>& definition) const
            {
                AddFixedWidthIntegerTypeDefinition(definition);
            }
        };

        template <>
        struct ClassDeclaration<char>
        {
            static constexpr const char* GetName() noexcept
            {
                return "char";
            }

            void operator()(ClassDefinitionT<char>& definition) const
            {
                AddFixedWidthIntegerTypeDefinition(definition);
            }
        };

        template <>
        struct ClassDeclaration<wchar_t>
        {
            static constexpr const char* GetName() noexcept
            {
                return "wchar_t";
            }

            void operator()(ClassDefinitionT<wchar_t>& definition) const
            {
                AddFixedWidthIntegerTypeDefinition(definition);
            }
        };

        template <>
        struct ClassDeclaration<char16_t>
        {
            static constexpr const char* GetName() noexcept
            {
                return "char16_t";
            }

            void operator()(ClassDefinitionT<char16_t>& definition) const
            {
                AddFixedWidthIntegerTypeDefinition(definition);
            }
        };

        template <>
        struct ClassDeclaration<char32_t>
        {
            static constexpr const char* GetName() noexcept
            {
                return "char32_t";
            }

            void operator()(ClassDefinitionT<char32_t>& definition) const
            {
                AddFixedWidthIntegerTypeDefinition(definition);
            }
        };

        /************************************************************************/
        /* INTEGER TYPES                                                        */
        /************************************************************************/

        template <>
        struct ClassDeclaration<short int>
        {
            static constexpr const char* GetName() noexcept
            {
                return "short int";
            }

            void operator()(ClassDefinitionT<short int>& definition) const
            {
                definition.DefineNameAlias("short");
                definition.DefineNameAlias("signed short");
                definition.DefineNameAlias("signed short int");

                AddFixedWidthIntegerTypeDefinition(definition);
            }
        };

        template <>
        struct ClassDeclaration<unsigned short int>
        {
            static constexpr const char* GetName() noexcept
            {
                return "unsigned short int";
            }

            void operator()(ClassDefinitionT<unsigned short int>& definition) const
            {
                definition.DefineNameAlias("unsigned short int");
                
                AddFixedWidthIntegerTypeDefinition(definition);
            }
        };

        template <>
        struct ClassDeclaration<int>
        {
            static constexpr const char* GetName() noexcept
            {
                return "int";
            }

            void operator()(ClassDefinitionT<int>& definition) const
            {
                definition.DefineNameAlias("signed");
                definition.DefineNameAlias("signed int");

                AddFixedWidthIntegerTypeDefinition(definition);
            }
        };

        template <>
        struct ClassDeclaration<unsigned int>
        {
            static constexpr const char* GetName() noexcept
            {
                return "unsigned int";
            }

            void operator()(ClassDefinitionT<unsigned int>& definition) const
            {
                definition.DefineNameAlias("unsigned");

                AddFixedWidthIntegerTypeDefinition(definition);
            }
        };

        template <>
        struct ClassDeclaration<long int>
        {
            static constexpr const char* GetName() noexcept
            {
                return "long int";
            }

            void operator()(ClassDefinitionT<long int>& definition) const
            {
                definition.DefineNameAlias("long");
                definition.DefineNameAlias("signed long");
                definition.DefineNameAlias("signed long int");

                AddFixedWidthIntegerTypeDefinition(definition);
            }
        };

        template <>
        struct ClassDeclaration<unsigned long int>
        {
            static constexpr const char* GetName() noexcept
            {
                return "unsigned long int";
            }

            void operator()(ClassDefinitionT<unsigned long int>& definition) const
            {
                definition.DefineNameAlias("unsigned long");

                AddFixedWidthIntegerTypeDefinition(definition);
            }
        };

        template <>
        struct ClassDeclaration<long long int>
        {
            static constexpr const char* GetName() noexcept
            {
                return "long long int";
            }

            void operator()(ClassDefinitionT<long long int>& definition) const
            {
                definition.DefineNameAlias("long long");
                definition.DefineNameAlias("signed long long");
                definition.DefineNameAlias("signed long long int");

                AddFixedWidthIntegerTypeDefinition(definition);
            }
        };

        template <>
        struct ClassDeclaration<unsigned long long int>
        {
            static constexpr const char* GetName() noexcept
            {
                return "unsigned long long int";
            }

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
        struct ClassDeclaration<float>
        {
            static constexpr const char* GetName() noexcept
            {
                return "float";
            }
        };

        template <>
        struct ClassDeclaration<double>
        {
            static constexpr const char* GetName() noexcept
            {
                return "double";
            }
        };

        template <>
        struct ClassDeclaration<long double>
        {
            static constexpr const char* GetName() noexcept
            {
                return "long double";
            }
        };

    }

}
