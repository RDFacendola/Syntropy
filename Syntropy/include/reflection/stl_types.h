
/// \file stl_types.h
/// \brief This header is part of the syntropy reflection system. It contains the definition of all the SLT types.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <type_traits>
#include <string>
#include <memory>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

#include "class.h"
#include "template_types.h"

namespace syntropy {

    namespace reflection {

        //////////////// STD::NULLPTR ////////////////

        template <>
        struct ClassDeclaration<std::nullptr_t> {

            constexpr const char* GetName() const noexcept {

                return "std::nullptr_t";

            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                definition.DefineNameAlias("nullptr_t");

            }

        };

        //////////////// STD::VECTOR ////////////////

        template <typename TType>
        struct ClassDeclaration<std::vector<TType>> {

            const char* GetName() const noexcept {

                static const std::string name(TemplateClassName<TType>()("std::vector"));

                return name.c_str();

            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                static const std::string alias(TemplateClassName<TType>()("vector"));

                definition.DefineNameAlias(alias.c_str());

            }

        };

        //////////////// STD::SET ////////////////

        template <typename TType>
        struct ClassDeclaration<std::set<TType>> {

            const char* GetName() const noexcept {

                static const std::string name(TemplateClassName<TType>()("std::set"));

                return name.c_str();

            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                static const std::string alias(TemplateClassName<TType>()("set"));

                definition.DefineNameAlias(alias.c_str());

            }

        };

        template <typename TType>
        struct ClassDeclaration<std::unordered_set<TType>> {

            const char* GetName() const noexcept {

                static const std::string name(TemplateClassName<TType>()("std::unordered_set"));

                return name.c_str();

            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                static const std::string alias(TemplateClassName<TType>()("unordered_set"));

                definition.DefineNameAlias(alias.c_str());

            }

        };

        //////////////// STD::MULTISET ////////////////

        template <typename TType>
        struct ClassDeclaration<std::multiset<TType>> {

            const char* GetName() const noexcept {

                static const std::string name(TemplateClassName<TType>()("std::multiset"));

                return name.c_str();

            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                static const std::string alias(TemplateClassName<TType>()("multiset"));

                definition.DefineNameAlias(alias.c_str());

            }

        };

        template <typename TType>
        struct ClassDeclaration<std::unordered_multiset<TType>> {

            const char* GetName() const noexcept {

                static const std::string name(TemplateClassName<TType>()("std::unordered_multiset"));

                return name.c_str();

            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                static const std::string alias(TemplateClassName<TType>()("unordered_multiset"));

                definition.DefineNameAlias(alias.c_str());

            }

        };

        //////////////// STD::PAIR ////////////////

        template <typename TKey, typename TValue>
        struct ClassDeclaration<std::pair<TKey, TValue>> {

            const char* GetName() const noexcept {

                static const std::string name(TemplateClassName<TKey, TValue>()("std::pair"));

                return name.c_str();

            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                static const std::string alias(TemplateClassName<TKey, TValue>()("pair"));

                definition.DefineNameAlias(alias.c_str());

            }

        };

        //////////////// STD::MAP ////////////////

        template <typename TKey, typename TValue>
        struct ClassDeclaration<std::map<TKey, TValue>> {

            const char* GetName() const noexcept {

                static const std::string name(TemplateClassName<TKey, TValue>()("std::map"));

                return name.c_str();

            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                static const std::string alias(TemplateClassName<TKey, TValue>()("map"));

                definition.DefineNameAlias(alias.c_str());

            }

        };

        template <typename TKey, typename TValue>
        struct ClassDeclaration<std::unordered_map<TKey, TValue>> {

            const char* GetName() const noexcept {

                static const std::string name(TemplateClassName<TKey, TValue>()("std::unordered_map"));

                return name.c_str();

            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                static const std::string alias(TemplateClassName<TKey, TValue>()("unordered_map"));

                definition.DefineNameAlias(alias.c_str());

            }

        };

        //////////////// STD::MULTIMAP ////////////////

        template <typename TKey, typename TValue>
        struct ClassDeclaration<std::multimap<TKey, TValue>> {

            const char* GetName() const noexcept {

                static const std::string name(TemplateClassName<TKey, TValue>()("std::multimap"));

                return name.c_str();

            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                static const std::string alias(TemplateClassName<TKey, TValue>()("multimap"));

                definition.DefineNameAlias(alias.c_str());

            }

        };

        template <typename TKey, typename TValue>
        struct ClassDeclaration<std::unordered_multimap<TKey, TValue>> {

            const char* GetName() const noexcept {

                static const std::string name(TemplateClassName<TKey, TValue>()("std::unordered_multimap"));

                return name.c_str();

            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                static const std::string alias(TemplateClassName<TKey, TValue>()("unordered_multimap"));

                definition.DefineNameAlias(alias.c_str());

            }

        };

        //////////////// STD::STRING ////////////////

        template <>
        struct ClassDeclaration<std::string> {

            constexpr const char* GetName() const noexcept {

                return "std::string";

            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                definition.DefineNameAlias("string");

            }

        };
        
        //////////////// STD::WSTRING ////////////////

        template <>
        struct ClassDeclaration<std::wstring> {

            constexpr const char* GetName() const noexcept {

                return "std::wstring";

            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                definition.DefineNameAlias("wstring");

            }

        };
        
        //////////////// STD::UNIQUE_PTR ////////////////

        template <typename TType>
        struct ClassDeclaration<std::unique_ptr<TType>> {

            const char* GetName() const noexcept {

                static const std::string name(TemplateClassName<TType>()("std::unique_ptr"));

                return name.c_str();

            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                static const std::string alias(TemplateClassName<TType>()("unique_ptr"));

                definition.DefineNameAlias(alias.c_str());

            }

        };

        //////////////// STD::SHARED_PTR ////////////////

        template <typename TType>
        struct ClassDeclaration<std::shared_ptr<TType>> {

            const char* GetName() const noexcept {

                static const std::string name(TemplateClassName<TType>()("std::shared_ptr"));

                return name.c_str();

            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                static const std::string alias(TemplateClassName<TType>()("std::shared_ptr"));

                definition.DefineNameAlias(alias.c_str());

            }

        };

    }

}
