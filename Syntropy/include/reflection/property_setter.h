
/// \file property_setter.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <functional>

#include "reflection/instance.h"

namespace syntropy {

    namespace reflection {

        struct _PropertySetter {

            using TSetter = std::function<bool(Instance, Instance)>;

            TSetter operator() () const {

                return[](Instance, Instance) -> bool {

                    return false;

                };

            }

            template <typename TClass, typename TProperty>
            TSetter operator() (TProperty TClass::* property, typename std::enable_if_t<!std::is_const<TProperty>::value>* = nullptr) const {

                return[property](Instance instance, Instance value) -> bool{

                    auto value_ptr = value.As<const TProperty>();
                    auto instance_ptr = instance.As<TClass>();

                    if (value_ptr && instance_ptr) {

                        instance_ptr->*property = *value_ptr;

                    }

                    return value_ptr && instance_ptr;

                };

            }

            template <typename TClass, typename TProperty>
            TSetter operator() (TProperty TClass::*, typename std::enable_if_t<std::is_const<TProperty>::value>* = nullptr) const {

                return (*this)();

            }

            template <typename TClass, typename TProperty>
            TSetter operator() (void (TClass::* setter)(TProperty)) const {

                return[setter](Instance instance, Instance value) -> bool {

                    auto value_ptr = value.As<const std::remove_reference_t<TProperty>>();
                    auto instance_ptr = instance.As<TClass>();

                    if (value_ptr && instance_ptr) {

                        (instance_ptr->*setter)(*value_ptr);

                    }

                    return value_ptr && instance_ptr;

                };

            }

            template <typename TClass, typename TProperty>
            TSetter operator() (TProperty& (TClass::* setter)()) const {

                return[setter](Instance instance, Instance value) -> bool {

                    auto value_ptr = value.As<const TProperty>();
                    auto instance_ptr = instance.As<TClass>();

                    if (value_ptr && instance_ptr) {

                        (instance_ptr->*setter)() = *value_ptr;

                    }

                    return value_ptr && instance_ptr;

                };

            }

        };


    }

}