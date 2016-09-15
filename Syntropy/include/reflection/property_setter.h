
/// \file property_setter.h
/// \brief Defines functors used to set the value of a property.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <functional>

#include "type_traits.h"
#include "reflection/instance.h"

namespace syntropy {

    namespace reflection {

        /// \brief Base interface for property setters.
        /// \author Raffaele D. Facendola - September 2016
        struct PropertySetter {

            /// \brief Default destructor.
            virtual ~PropertySetter() = default;

            /// \brief Write the property value of the provided instance property.
            /// \param instance Instance whose property should be written to.
            /// \param value Value to assign to the property.
            /// \return Returns true if the method succeeds, returns false otherwise. 
            ///         The method fails if instance doesn't have the given property, the value is not compatible with the property type or the property is read-only.
            virtual bool operator()(Instance instance, Instance value) const = 0;

        };

        /// \brief Concrete property setter.
        /// Fallback implementation for read-only properties.
        /// \author Raffaele D. Facendola - September 2016
        template <typename TProperty, typename = void>
        struct PropertySetterT : PropertySetter {

            /// \brief Create a new dummy property setter that does nothing.
            PropertySetterT(_){}

            bool operator()(Instance instance, Instance value) const override {

                return false;

            }


        };

        /// \brief Concrete property setter.
        /// Fallback implementation for read-only properties.
        /// \author Raffaele D. Facendola - September 2016
        template <>
        struct PropertySetterT<std::nullptr_t, void> : PropertySetter {

            /// \brief Create a new dummy property setter that does nothing.
            PropertySetterT() {}

            bool operator()(Instance instance, Instance value) const override {

                return false;

            }


        };

        /// \brief Concrete property setter.
        /// Template specialization for non-const member variables.
        /// \author Raffaele D. Facendola - September 2016
        template <typename TClass, typename TField>
        struct PropertySetterT<TField (TClass::*), std::enable_if_t<!std::is_const<TField>::value>> : PropertySetter {

            /// \brief Create a new property setter for the given member field.
            /// \param field Field to write.
            PropertySetterT(TField TClass::* field)
                : field_(field) {}

            bool operator()(Instance instance, Instance value) const override {

                auto concrete_value = value.As<const TProperty>();
                auto concrete_instance = instance.As<TClass>();

                if (concrete_value && concrete_instance) {

                    concrete_instance->*field_ = *concrete_value;

                    return true;

                }

                return false;

            }

        private:

            TField TClass::* field_;                ///< \brief Member field to be write.
            
        };
        
        /// \brief Concrete property setter.
        /// Template specialization for setters of the form TReturn TClass::SetProperty(TSetter). 
        /// TSetter must not be void.
        /// \author Raffaele D. Facendola - September 2016
        template <typename TClass, typename TSetter, typename TReturn>
        struct PropertySetterT<TReturn(TClass::*)(TSetter), void> : PropertySetter {

            /// \brief Create a new property setter using the provided setter method.
            /// \param setter Setter method used to write the value inside the property.
            PropertySetterT(TReturn(TClass::* setter)(TSetter))
                : setter_(setter) {}

            bool operator()(Instance instance, Instance value) const override {

                auto concrete_instance = instance.As<TClass>();
                auto concrete_value = value.As<const std::remove_reference_t<TSetter>>();

                if (concrete_value && concrete_instance) {

                    (concrete_instance->*setter_)(*value_ptr);

                    return true;

                }

                return false;

            }

        private:

            TReturn(TClass::* setter_)(TSetter);     /// \brief Setter method used to write inside the property

        };

        /// \brief Concrete property setter.
        /// Template specialization for setters of the form TSetter& TClass::AccessProperty().
        /// The accessor must return reference to the actual property object to write.
        /// \author Raffaele D. Facendola - September 2016
        template <typename TClass, typename TSetter>
        struct PropertySetterT<TSetter&(TClass::*)(), void> : PropertySetter {

            /// \brief Create a new property setter using the provided setter method.
            /// \param setter Setter method used to write the value inside the property.
            PropertySetterT(TSetter&(TClass::* setter)())
                : setter_(setter) {}

            bool operator()(Instance instance, Instance value) const override {

                auto concrete_instance = instance.As<TClass>();
                auto concrete_value = value.As<const TProperty>();

                if (concrete_value && concrete_instance) {

                    (concrete_instance->*setter)() = *concrete_value;

                    return true;

                }

                return false;

            }

        private:

            TSetter&(TClass::* setter_)();     /// \brief Setter method used to access the property to write.

        };

    }

}
