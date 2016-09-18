
/// \file property.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <functional>
#include <unordered_map>
#include <typeindex>
#include <memory>

#include "hashed_string.h"
#include "type_traits.h"

#include "reflection/type.h"
#include "reflection/instance.h"
#include "reflection/property/property_getter.h"
#include "reflection/property/property_setter.h"

#include "linb/any/any.hpp"

namespace syntropy {

    namespace reflection {

        class Property {

        public:

            /// \brief No copy constructor.
            Property(const Property&) = delete;
            
            /// \brief Move constructor.
            Property(Property&& other) noexcept;

            template <typename TClass, typename TProperty>
            Property(const HashedString& name, TProperty TClass::* field, std::enable_if_t<!std::is_const<TProperty>::value>* = nullptr) noexcept;

            template <typename TClass, typename TProperty>
            Property(const HashedString& name, TProperty TClass::* field, std::enable_if_t<std::is_const<TProperty>::value>* = nullptr) noexcept;

            template <typename TClass, typename TProperty>
            Property(const HashedString& name, TProperty (TClass::* getter)() const) noexcept;

            template <typename TClass, typename TProperty, typename TReturn>
            Property(const HashedString& name, TProperty(TClass::* getter)() const, TReturn(TClass::* setter)(TProperty)) noexcept;

            template <typename TClass, typename TProperty>
            Property(const HashedString& name, const TProperty&(TClass::* getter)() const, TProperty&(TClass::* setter)()) noexcept;
            
            /// \brief No assignment operator.
            Property& operator=(const Property&) = delete;

            /// \brief Default destructor.
            ~Property() = default;

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

            std::unique_ptr<PropertyGetter> getter_;                        ///< \brief Property getter.

            std::unique_ptr<PropertySetter> setter_;                        ///< \brief Property setter.

            std::unordered_map<std::type_index, linb::any> interfaces_;     ///< \brief Set of interfaces assigned to the property.

        };

    }

}

namespace syntropy{ 

    namespace reflection {

        //////////////// PROPERTY ////////////////

        template <typename TClass, typename TProperty>
        Property::Property(const HashedString& name, TProperty TClass::* field, std::enable_if_t<!std::is_const<TProperty>::value>*) noexcept
            : name_(name)
            , type_(TypeOf<TProperty>())
            , getter_(MakePropertyGetter(field))
            , setter_(MakePropertySetter(field)){}

        template <typename TClass, typename TProperty>
        Property::Property(const HashedString& name, TProperty TClass::* field, std::enable_if_t<std::is_const<TProperty>::value>*) noexcept
            : name_(name)
            , type_(TypeOf<TProperty>())
            , getter_(MakePropertyGetter(field)) {}

        template <typename TClass, typename TProperty>
        Property::Property(const HashedString& name, TProperty(TClass::* getter)() const) noexcept
            : name_(name)
            , type_(TypeOf<TProperty>())
            , getter_(MakePropertyGetter(getter)) {}

        template <typename TClass, typename TProperty, typename TReturn>
        Property::Property(const HashedString& name, TProperty(TClass::* getter)() const, TReturn(TClass::* setter)(TProperty)) noexcept
            : name_(name)
            , type_(TypeOf<TProperty>())
            , getter_(MakePropertyGetter(getter))
            , setter_(MakePropertySetter(setter)) {}

        template <typename TClass, typename TProperty>
        Property::Property(const HashedString& name, const TProperty& (TClass::* getter)() const, TProperty& (TClass::* setter)()) noexcept
            : name_(name)
            , type_(TypeOf<TProperty>())
            , getter_(MakePropertyGetter(getter))
            , setter_(MakePropertySetter(setter)) {}

        template <typename TInterface, typename... TArguments>
        bool Property::AddInterface(TArguments&&... arguments) {

            auto interface_type = std::type_index(typeid(TInterface));

            if (interfaces_.find(interface_type) == interfaces_.end()) {

                interfaces_.insert(std::make_pair(interface_type,
                                                  linb::any(TInterface(std::forward<TArguments>(arguments)...))));

                return true;

            }

            return false;

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

            return (*getter_)(MakeConstInstance(instance),
                              MakeInstance(std::forward<TValue>(value)));

        }

        template <typename TInstance, typename TValue>
        bool Property::Set(TInstance&& instance, const TValue& value) const {
            
            return setter_ ?
                   (*setter_)(MakeInstance(std::forward<TInstance>(instance)), MakeConstInstance(value)) :
                   false;

        }

    }

}