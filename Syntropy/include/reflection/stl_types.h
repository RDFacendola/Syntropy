
/// \file stl_types.h
/// \brief This header is part of the syntropy reflection system. It contains the definition of all the SLT types.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <type_traits>
#include <string>

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


    }

}
