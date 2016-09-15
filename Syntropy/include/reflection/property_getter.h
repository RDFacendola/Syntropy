
/// \file property_getter.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <memory>

#include "reflection/instance.h"

namespace syntropy {

    namespace reflection {

        struct PropertyGetter {

            virtual ~PropertyGetter() = default;

            virtual bool operator()(Instance instance, Instance value) const = 0;

        };

        template <typename TProperty>
        struct PropertyGetterT : PropertyGetter {};

        template <typename TClass, typename TField>
        struct PropertyGetterT<TField TClass::*> : PropertyGetter {

            PropertyGetterT(TField TClass::* field)
                : field_(field) {}

            bool operator()(Instance instance, Instance value) const override {

                auto instance_ptr = instance.As<const TClass>();
                auto value_ptr = value.As<std::decay_t<TField>>();

                if (value_ptr && instance_ptr) {

                    *value_ptr = instance_ptr->*field_;

                    return true;

                }

                return false;

            }

        private:

            TField TClass::* field_;

        };

        template <typename TClass, typename TGetter>
        struct PropertyGetterT<TGetter(TClass::*)() const> : PropertyGetter {

            PropertyGetterT(TGetter(TClass::*getter)() const)
                : getter_(getter) {}

            bool operator()(Instance instance, Instance value) const override {

                auto value_ptr = value.As<std::decay_t<TGetter>>();
                auto instance_ptr = instance.As<const TClass>();

                if (value_ptr && instance_ptr) {

                    *value_ptr = (instance_ptr->*getter_)();

                    return true;

                }

                return false;

            }

        private:

            TGetter(TClass::* getter_)() const;

        };

        template <typename TGetter>
        std::unique_ptr<TGetter> MakePropertyGetter(TGetter getter) {

            //return std::make_unique<PropertyGetterT<TGetter>>(getter);
            return nullptr;

        }

    }

}