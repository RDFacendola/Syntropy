
/// \file fundamental_types.h
/// \brief This header is part of the syntropy reflection system. It contains the definition of all the fundamental types supported by the C++ standard.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "class.h"

namespace syntropy {

    namespace reflection {

        //////////////// FIXED WIDTH INTEGER TYPES ////////////////

        /// \brief Append additional name aliases for fixed width integer types.
        template <typename TClass, typename TDefinition>
        void AddFixedWidthIntegerTypeDefinition(TDefinition& definition)
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

        //////////////// BOOLEAN TYPES ////////////////

        template <>
        struct ClassDeclaration<bool>
        {
            static constexpr const char* GetName() noexcept
            {
                return "bool";
            }

        };

        //////////////// CHARACTER TYPES ////////////////
        
        template <>
        struct ClassDeclaration<signed char> {

            static constexpr const char* GetName() noexcept
            {
                return "signed char";
            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                AddFixedWidthIntegerTypeDefinition<signed char>(definition);

            }

        };

        template <>
        struct ClassDeclaration<unsigned char> {

            static constexpr const char* GetName() noexcept
            {
                return "unsigned char";
            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                AddFixedWidthIntegerTypeDefinition<unsigned char>(definition);

            }

        };

        template <>
        struct ClassDeclaration<char> {

            static constexpr const char* GetName() noexcept
            {
                return "char";
            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                AddFixedWidthIntegerTypeDefinition<char>(definition);

            }

        };

        template <>
        struct ClassDeclaration<wchar_t> {

            static constexpr const char* GetName() noexcept
            {
                return "wchar_t";
            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                AddFixedWidthIntegerTypeDefinition<wchar_t>(definition);

            }

        };

        template <>
        struct ClassDeclaration<char16_t> {

            static constexpr const char* GetName() noexcept
            {
                return "char16_t";
            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                AddFixedWidthIntegerTypeDefinition<char16_t>(definition);

            }

        };

        template <>
        struct ClassDeclaration<char32_t> {

            static constexpr const char* GetName() noexcept
            {
                return "char32_t";
            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                AddFixedWidthIntegerTypeDefinition<char32_t>(definition);

            }

        };

        //////////////// INTEGER TYPES ////////////////

        template <>
        struct ClassDeclaration<short int> {

            static constexpr const char* GetName() noexcept
            {
                return "short int";
            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                definition.DefineNameAlias("short");
                definition.DefineNameAlias("signed short");
                definition.DefineNameAlias("signed short int");

                AddFixedWidthIntegerTypeDefinition<short int>(definition);

            }

        };

        template <>
        struct ClassDeclaration<unsigned short int> {

            static constexpr const char* GetName() noexcept
            {
                return "unsigned short int";
            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                definition.DefineNameAlias("unsigned short int");
                
                AddFixedWidthIntegerTypeDefinition<unsigned short int>(definition);

            }

        };

        template <>
        struct ClassDeclaration<int> {

            static constexpr const char* GetName() noexcept
            {
                return "int";
            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                definition.DefineNameAlias("signed");
                definition.DefineNameAlias("signed int");

                AddFixedWidthIntegerTypeDefinition<int>(definition);

            }

        };

        template <>
        struct ClassDeclaration<unsigned int> {

            static constexpr const char* GetName() noexcept
            {
                return "unsigned int";
            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                definition.DefineNameAlias("unsigned");

                AddFixedWidthIntegerTypeDefinition<unsigned int>(definition);

            }

        };

        template <>
        struct ClassDeclaration<long int> {

            static constexpr const char* GetName() noexcept
            {
                return "long int";
            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {
                                
                definition.DefineNameAlias("long");
                definition.DefineNameAlias("signed long");
                definition.DefineNameAlias("signed long int");

                AddFixedWidthIntegerTypeDefinition<long int>(definition);

            }

        };

        template <>
        struct ClassDeclaration<unsigned long int> {

            static constexpr const char* GetName() noexcept
            {
                return "unsigned long int";
            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                definition.DefineNameAlias("unsigned long");

                AddFixedWidthIntegerTypeDefinition<unsigned long int>(definition);

            }

        };

        template <>
        struct ClassDeclaration<long long int> {

            static constexpr const char* GetName() noexcept
            {
                return "long long int";
            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                definition.DefineNameAlias("long long");
                definition.DefineNameAlias("signed long long");
                definition.DefineNameAlias("signed long long int");

                AddFixedWidthIntegerTypeDefinition<long long int>(definition);

            }

        };

        template <>
        struct ClassDeclaration<unsigned long long int> {

            static constexpr const char* GetName() noexcept
            {
                return "unsigned long long int";
            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                definition.DefineNameAlias("unsigned long long");

                AddFixedWidthIntegerTypeDefinition<unsigned long long int>(definition);

            }

        };
        
        //////////////// FLOATING POINT TYPES ////////////////

        template <>
        struct ClassDeclaration<float> {

            static constexpr const char* GetName() noexcept
            {
                return "float";
            }

        };

        template <>
        struct ClassDeclaration<double> {

            static constexpr const char* GetName() noexcept
            {
                return "double";
            }

        };

        template <>
        struct ClassDeclaration<long double> {

            static constexpr const char* GetName() noexcept
            {
                return "long double";
            }

        };

    }

}
