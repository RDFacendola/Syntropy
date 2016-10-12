
/// \file stl_types.h
/// \brief This header is part of the syntropy reflection system. It contains the definition of all the SLT types.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <type_traits>
#include <string>
#include <memory>

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
