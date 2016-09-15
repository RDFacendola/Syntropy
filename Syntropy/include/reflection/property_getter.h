
/// \file property_getter.h
/// \brief Defines functors used to get the value of a property.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <memory>

#include "reflection/instance.h"

namespace syntropy {

    namespace reflection {

        /// \brief Base interface for property getters.
        /// \author Raffaele D. Facendola - September 2016
        struct PropertyGetter {

            /// \brief Default destructor.
            virtual ~PropertyGetter() = default;

            /// \brief Read the property value of the provided instance.
            /// \param instance Instance whose property should be read.
            /// \param value If the method succeeds contains the value read.
            /// \return Returns true if the method succeeds, returns false otherwise. 
            ///         The method fails if instance doesn't have the given property or the value is not compatible with the property type.
            virtual bool operator()(Instance instance, Instance value) const = 0;

        };

        /// \brief Unspecialized concrete property getters.
        /// Properties should always have a getter.
        /// \author Raffaele D. Facendola - September 2016
        template <typename TProperty>
        struct PropertyGetterT : PropertyGetter {};

        /// \brief Property getter.
        /// Template specialization for member variables.
        /// \author Raffaele D. Facendola - September 2016
        template <typename TClass, typename TField>
        struct PropertyGetterT<TField TClass::*> : PropertyGetter {

            /// \brief Create a new property getter for the given member field.
            /// \param field Field to read.
            PropertyGetterT(TField TClass::* field)
                : field_(field) {}

            bool operator()(Instance instance, Instance value) const override {

                auto concrete_instance = instance.As<const TClass>();
                auto concrete_value = value.As<std::decay_t<TField>>();

                if (concrete_value && concrete_instance) {

                    *concrete_value = concrete_instance->*field_;

                    return true;

                }

                return false;

            }

        private:

            TField TClass::* field_;                ///< \brief Member field to be read.

        };

        /// \brief Property getter.
        /// Template specialization for const getters.
        /// The const getter has the form T GetProperty() const. T can either be const or non const.
        /// \author Raffaele D. Facendola - September 2016
        template <typename TClass, typename TGetter>
        struct PropertyGetterT<TGetter(TClass::*)() const> : PropertyGetter {

            /// \brief Create a new property getter for the given getter method.
            /// \param getter Getter method to read.
            PropertyGetterT(TGetter(TClass::*getter)() const)
                : getter_(getter) {}

            bool operator()(Instance instance, Instance value) const override {

                auto concrete_instance = instance.As<const TClass>();
                auto concrete_value = value.As<std::decay_t<TGetter>>();

                if (concrete_value && concrete_instance) {

                    *concrete_value = (concrete_instance->*getter_)();

                    return true;

                }

                return false;

            }

        private:

            TGetter(TClass::* getter_)() const;     /// \brief Getter method used to read the value of the property.

        };

    }

}