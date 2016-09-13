
/// \file property.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <functional>
#include <unordered_map>
#include <typeindex>

#include "hashed_string.h"
#include "type_traits.h"

#include "reflection/type.h"
#include "reflection/instance.h"

#include "linb/any/any.hpp"

namespace syntropy {

    namespace reflection {

        struct PropertyGetter {

            using TGetter = std::function<bool(Instance, Instance)>;

            template <typename TClass, typename TProperty>
            TGetter operator() (TProperty TClass::* field) const {

                return[field](Instance instance, Instance value) -> bool {

                    auto value_ptr = value.As<std::decay_t<TProperty>>();
                    auto instance_ptr = instance.As<const TClass>();

                    if (value_ptr && instance_ptr) {

                        *value_ptr = instance_ptr->*field;

                    }

                    return value_ptr && instance_ptr;

                };

            }

            template <typename TClass, typename TProperty>
            TGetter operator() (TProperty(TClass::* getter)() const) const {

                return[getter](Instance instance, Instance value) -> bool {

                    auto value_ptr = value.As<std::decay_t<TProperty>>();
                    auto instance_ptr = instance.As<const TClass>();

                    if (value_ptr && instance_ptr) {

                        *value_ptr = (instance_ptr->*getter)();

                    }

                    return value_ptr && instance_ptr;

                };

            }

        };

        struct PropertySetter {

            using TSetter = std::function<bool(Instance, Instance)>;

            TSetter operator() () const {

                return[](Instance, Instance) -> bool {

                    return false;

                };

            }

            template <typename TClass, typename TProperty>
            TSetter operator() (TProperty TClass::* property, typename std::enable_if_t<!std::is_const<TProperty>::value>* = nullptr) const {

                return [property](Instance instance, Instance value) -> bool{

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
        
        class Property {

        public:

            template <typename TClass, typename TProperty>
            Property(const HashedString& name, TProperty TClass::* field) noexcept;

            template <typename TClass, typename TProperty>
            Property(const HashedString& name, TProperty (TClass::* getter)() const) noexcept;

            template <typename TClass, typename TProperty>
            Property(const HashedString& name, TProperty(TClass::* getter)() const, void(TClass::* setter)(TProperty)) noexcept;

            template <typename TClass, typename TProperty>
            Property(const HashedString& name, const TProperty&(TClass::* getter)() const, TProperty&(TClass::* setter)()) noexcept;
            
            const HashedString& GetName() const noexcept;

            const Type& GetType() const noexcept;
            
            template <typename TInterface, typename... TArguments>
            bool AddInterface(TArguments&&... arguments);

            template <typename TInterface>
            const TInterface* GetInterface() const;

            template <typename TInterface>
            TInterface* GetInterface();

            template <typename TInstance, typename TValue>
            bool Get(const TInstance& instance, TValue&& value) const;

            template <typename TInstance, typename TValue>
            bool Set(TInstance&& instance, const TValue& value) const;

        private:

            HashedString name_;                                             ///< \brief Property name.

            const Type& type_;                                              ///< \brief Property type.

            typename PropertyGetter::TGetter getter_;                       ///< \brief Property getter.

            typename PropertySetter::TSetter setter_;                       ///< \brief Property setter.

            std::unordered_map<std::type_index, linb::any> interfaces_;     ///< \brief Set of interfaces assigned to the property.

        };

    }

}

namespace syntropy{ 

    namespace reflection {

        //////////////// PROPERTY ////////////////

        template <typename TClass, typename TProperty>
        Property::Property(const HashedString& name, TProperty TClass::* field) noexcept
            : name_(name)
            , type_(TypeOf<TProperty>())
            , getter_(PropertyGetter()(field))
            , setter_(PropertySetter()(field)){}

        template <typename TClass, typename TProperty>
        Property::Property(const HashedString& name, TProperty(TClass::* getter)() const) noexcept
            : name_(name)
            , type_(TypeOf<TProperty>())
            , getter_(PropertyGetter()(getter))
            , setter_(PropertySetter()()) {}

        template <typename TClass, typename TProperty>
        Property::Property(const HashedString& name, TProperty(TClass::* getter)() const, void(TClass::* setter)(TProperty)) noexcept
            : name_(name)
            , type_(TypeOf<TProperty>())
            , getter_(PropertyGetter()(getter))
            , setter_(PropertySetter()(setter)) {}

        template <typename TClass, typename TProperty>
        Property::Property(const HashedString& name, const TProperty& (TClass::* getter)() const, TProperty& (TClass::* setter)()) noexcept
            : name_(name)
            , type_(TypeOf<TProperty>())
            , getter_(PropertyGetter()(getter))
            , setter_(PropertySetter()(setter)) {}

        template <typename TInterface, typename... TArguments>
        bool Property::AddInterface(TArguments&&... arguments) {

            auto interface_type = std::type_index(typeid(TInterface));

            if (interfaces_.find(interface_type) == interfaces_.end()) {

                interfaces_.insert(std::make_pair(interface_type,
                                                  linb::any(TInterface(std::forward<TArguments>(arguments)...))));

                return true;

            }
            else {

                return false;

            }

        }

        template <typename TInterface>
        const TInterface* Property::GetInterface() const{

            auto interface_type = std::type_index(typeid(TInterface));

            auto it = interfaces_.find(interface_type);

            return it != interfaces_.end() ?
                   linb::any_cast<TInterface>(&(it->second)) :
                   nullptr;

        }

        template <typename TInterface>
        TInterface* Property::GetInterface() {

            return const_cast<TInterface*>(static_cast<const Property*>(this)->GetInterface<TInterface>());

        }

        template <typename TInstance, typename TValue>
        bool Property::Get(const TInstance& instance, TValue&& value) const {

            return getter_(MakeConstInstance(instance),
                           MakeInstance(std::forward<TValue>(value)));

        }

        template <typename TInstance, typename TValue>
        bool Property::Set(TInstance&& instance, const TValue& value) const {

            return setter_(MakeInstance(std::forward<TInstance>(instance)),
                           MakeConstInstance(value));

        }

    }

}