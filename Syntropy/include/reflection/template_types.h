
/// \file template_types.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "type.h"

namespace syntropy {

    namespace reflection {

        /// \brief Expands a template class name.
        /// \author Raffaele D. Facendola
        template <typename... TType>
        struct TemplateClassName {

            /// \brief Expands a template class name for the provided class.
            /// The expanded name has the form "base_name<template0, template1, ...>".
            /// \param base_name Name of the class to expand.
            /// \return Returns the provided base class name decorated with its template arguments names.
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

        /// \brief Helper class for TemplateClassName. Expands a template argument list as a comma separated string.
        /// \author Raffaele D. Facendola
        template <typename TType, typename... TRestType>
        struct TemplateClassArgumentName {

            /// \brief Expands a template argument list as a comma separated list of names.
            /// The expanded name has the form "template0, template1, ..."
            /// \return Returns a comma separated list of names whose elements represent the default name of each template argument.
            std::string operator()() const noexcept {

                std::stringstream name;

                name << TypeOf<TType>()
                     << ", "
                     << TemplateClassArgumentName<TRestType...>()();

                return name.str();

            }

        };

        /// \brief Template specialization for TemplateClassArgumentName with just one template argument
        /// \author Raffaele D. Facendola
        template <typename TType>
        struct TemplateClassArgumentName<TType> {

            /// \brief Expands a template argument list as a comma separated list of names.
            /// \return Returns the name of the provided template argument.
            std::string operator()() const noexcept {

                std::stringstream name;

                name << TypeOf<TType>();

                return name.str();

            }

        };

    }

}