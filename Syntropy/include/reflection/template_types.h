
/// \file template_types.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

namespace syntropy {

    namespace reflection {

        template <typename... TType>
        struct TemplateClassName {

            std::string operator()(const char* base_name) const noexcept {

                std::string name_;

                name_.reserve(256);

                name_.append(base_name)
                     .append("<")
                     .append(TemplateClassArgumentName<TType...>()())
                     .append(">");

                return name_;

            }

        };

        template <typename TType, typename... TRestType>
        struct TemplateClassArgumentName {

            std::string operator()() const noexcept {

                std::string name_;
                
                name_.reserve(256);

                name_.append(ClassDeclaration<TType>().GetName())
                     .append(", ")
                     .append(TemplateClassArgumentName<TRestType...>()());

                return name_;

            }

        };

        template <typename TType>
        struct TemplateClassArgumentName<TType> {

            std::string operator()() const noexcept {

                return ClassDeclaration<TType>().GetName();

            }

        };

    }

}