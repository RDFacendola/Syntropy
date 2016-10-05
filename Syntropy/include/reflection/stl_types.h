
/// \file stl_types.h
/// \brief This header is part of the syntropy reflection system. It contains the definition of all the SLT types.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <type_traits>
#include <string>
#include <memory>

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

        //////////////// STD::STRING ////////////////

        template <>
        struct ClassDeclaration <std::string> {

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
        struct ClassDeclaration <std::wstring> {

            constexpr const char* GetName() const noexcept {

                return "std::wstring";

            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                definition.DefineNameAlias("wstring");

            }

        };

        template <typename... TType>
        struct TemplateClassName {

            constexpr std::string GetName(const char* base_name) const noexcept{

                std::stringstream name;

                name << base_name << "<" << TemplateClassArgumentName<TType...>().GetName() << ">";

                return name.str();

            }

        };

        template <typename TType, typename... TRestType>
        struct TemplateClassArgumentName {

            constexpr std::string GetName() const noexcept {

                std::stringstream name;

                name << ClassDeclaration<TType>().GetName() << ", " << TemplateClassArgumentName<TRestType>().GetName()();

                return name.str();

            }

        };

        template <typename TType>
        struct TemplateClassArgumentName<TType> {

            constexpr std::string GetName() const noexcept {

                std::stringstream name;

                name << ClassDeclaration<TType>().GetName();

                return name.str();

            }

        };

        //////////////// STD::UNIQUE_PTR ////////////////

        template <typename TType>
        struct ClassDeclaration<std::unique_ptr<TType>> {

            constexpr const char* GetName() const noexcept {

                static std::string name(TemplateClassName<TType>()("std::unique_ptr"));

                return name.c_str();

            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                definition.DefineNameAlias(TemplateClassName<TType>()("unique_ptr"));

            }

        };

        //////////////// STD::UNIQUE_PTR ////////////////

        template <typename TType>
        struct ClassDeclaration<std::shared_ptr<TType>> {

            constexpr const char* GetName() const noexcept {

                return TemplateClassName<TType>().GetName("std::shared_ptr").c_str();

            }

            template <typename TDefinition>
            void operator()(TDefinition& definition) const {

                definition.DefineNameAlias(TemplateClassName<TType>()("shared_ptr"));

            }

        };

    }

}
