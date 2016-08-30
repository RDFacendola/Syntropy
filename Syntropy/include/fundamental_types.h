
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

        /// \brief Functor used to define additional name aliases for fixed width integer types.
        template <typename TClass>
        struct FixedWidthIntegerTypeDefinition {

            Class::Definition<TClass>& operator()(Class::Definition<TClass>& definition) {

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

                return definition;

            }

        };

        /// \brief Helper function for FixedWidthIntegerTypeDefinition() functor.
        template <typename TClass>
        Class::Definition<TClass>& AddFixedWidthIntegerTypeDefinition(Class::Definition<TClass>& definition) {

            return FixedWidthIntegerTypeDefinition<TClass>()(definition);

        }

        //////////////// VOID TYPE ////////////////

        template <>
        struct ClassDeclaration<void> {
         
            auto operator()() const {

                return Class::Definition<void>("void");

            }

        };

        //////////////// STD::NULLPTR ////////////////

        template <>
        struct ClassDeclaration<std::nullptr_t> {

            auto operator()() const {

                auto definition = Class::Definition<std::nullptr_t>("std::nullptr_t");

                definition.DefineNameAlias("nullptr_t");

                return definition;

            }

        };

        //////////////// BOOLEAN TYPES ////////////////

        template <>
        struct ClassDeclaration<bool> {

            auto operator()() const {

                return Class::Definition<bool>("bool");

            }

        };

        //////////////// CHARACTER TYPES ////////////////
        
        template <>
        struct ClassDeclaration<signed char> {

            auto operator()() const {

                Class::Definition<signed char> definition("signed char");

                return AddFixedWidthIntegerTypeDefinition(definition);

            }

        };

        template <>
        struct ClassDeclaration<unsigned char> {

            auto operator()() const {
                
                Class::Definition<unsigned char> definition("unsigned char");

                return AddFixedWidthIntegerTypeDefinition(definition);

            }

        };

        template <>
        struct ClassDeclaration<char> {

            auto operator()() const {

                Class::Definition<char> definition("char");

                return AddFixedWidthIntegerTypeDefinition(definition);

            }

        };

        template <>
        struct ClassDeclaration<wchar_t> {

            auto operator()() const {

                Class::Definition<wchar_t> definition("wchar_t");

                return AddFixedWidthIntegerTypeDefinition(definition);

            }

        };

        template <>
        struct ClassDeclaration<char16_t> {

            auto operator()() const {

                Class::Definition<char16_t> definition("char16_t");

                return AddFixedWidthIntegerTypeDefinition(definition);

            }

        };

        template <>
        struct ClassDeclaration<char32_t> {

            auto operator()() const {

                Class::Definition<char32_t> definition("char32_t");

                return AddFixedWidthIntegerTypeDefinition(definition);

            }

        };

        //////////////// INTEGER TYPES ////////////////

        template <>
        struct ClassDeclaration<short int> {

            auto operator()() const {

                Class::Definition<short int> definition("short int");

                definition.DefineNameAlias("short");
                definition.DefineNameAlias("signed short");
                definition.DefineNameAlias("signed short int");

                return AddFixedWidthIntegerTypeDefinition(definition);

            }

        };

        template <>
        struct ClassDeclaration<unsigned short int> {

            auto operator()() const {

                Class::Definition<unsigned short int> definition("unsigned short int");

                definition.DefineNameAlias("unsigned short");
                
                return AddFixedWidthIntegerTypeDefinition(definition);

            }

        };

        template <>
        struct ClassDeclaration<int> {

            auto operator()() const {

                Class::Definition<int> definition("int");

                definition.DefineNameAlias("signed");
                definition.DefineNameAlias("signed int");

                return AddFixedWidthIntegerTypeDefinition(definition);

            }

        };

        template <>
        struct ClassDeclaration<unsigned int> {

            auto operator()() const {

                Class::Definition<unsigned int> definition("unsigned int");

                definition.DefineNameAlias("unsigned");

                return AddFixedWidthIntegerTypeDefinition(definition);

            }

        };

        template <>
        struct ClassDeclaration<long int> {

            auto operator()() const {

                Class::Definition<long int> definition("long int");
                                
                definition.DefineNameAlias("long");
                definition.DefineNameAlias("signed long");
                definition.DefineNameAlias("signed long int");

                return AddFixedWidthIntegerTypeDefinition(definition);

            }

        };

        template <>
        struct ClassDeclaration<unsigned long int> {

            auto operator()() const {

                Class::Definition<unsigned long int> definition("unsigned long int");

                definition.DefineNameAlias("unsigned long");

                return AddFixedWidthIntegerTypeDefinition(definition);

            }

        };

        template <>
        struct ClassDeclaration<long long int> {

            auto operator()() const {

                Class::Definition<long long int> definition("long long int");

                definition.DefineNameAlias("long long");
                definition.DefineNameAlias("signed long long");
                definition.DefineNameAlias("signed long long int");

                return AddFixedWidthIntegerTypeDefinition(definition);

            }

        };

        template <>
        struct ClassDeclaration<unsigned long long int> {

            auto operator()() const {

                Class::Definition<unsigned long long int> definition("unsigned long long int");

                definition.DefineNameAlias("unsigned long long");

                return AddFixedWidthIntegerTypeDefinition(definition);

            }

        };
        
        //////////////// FLOATING POINT TYPES ////////////////

        template <>
        struct ClassDeclaration<float> {

            auto operator()() const {

                return Class::Definition<float>("float");

            }

        };

        template <>
        struct ClassDeclaration<double> {

            auto operator()() const {

                return Class::Definition<double>("double");

            }

        };

        template <>
        struct ClassDeclaration<long double> {

            auto operator()() const {

                return Class::Definition<long double>("long double");

            }

        };

    }

}
