
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
#include "reflection/property/property_traits.h"

#include "linb/any/any.hpp"

namespace syntropy {

    namespace reflection {

        class Property {

        public:

            /// \brief No copy constructor.
            Property(const Property&) = delete;
            
            /// \brief Move constructor.
            Property(Property&& other) noexcept;

            template <typename TProperty>
            Property(const HashedString& name, TProperty property) noexcept;

            template <typename TGetter, typename TSetter>
            Property(const HashedString& name, TGetter getter, TSetter setter) noexcept;
                        
            /// \brief No assignment operator.
            Property& operator=(const Property&) = delete;

            /// \brief Default destructor.
            ~Property() = default;

            const HashedString& GetName() const noexcept;

            const Type& GetType() const noexcept;
            
            template <typename TProvider, typename... TRestProviders>
            void AddInterfaces(TProvider&& provider, TRestProviders&&... rest);

            template <typename TInterface>
            const TInterface* GetInterface() const;

            template <typename TInterface>
            TInterface* GetInterface();

            template <typename TInstance, typename TValue>
            bool Get(const TInstance& instance, TValue&& value) const;

            template <typename TInstance, typename TValue>
            bool Set(TInstance&& instance, const TValue& value) const;

        private:

            void AddInterfaces();

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

        template <typename TProperty>
        Property::Property(const HashedString& name, TProperty property) noexcept
            : name_(name)
            , type_(TypeOf<property_traits_property_t<TProperty>>())
            , getter_(MakePropertyGetter(property))
            , setter_(MakePropertySetter(property)){}

        template <typename TGetter, typename TSetter>
        Property::Property(const HashedString& name, TGetter getter, TSetter setter) noexcept
            : name_(name)
            , type_(TypeOf<property_traits_property_t<TGetter, TSetter>>())
            , getter_(MakePropertyGetter(getter))
            , setter_(MakePropertySetter(setter)) {}

        template <typename TProvider, typename... TRestProviders>
        void Property::AddInterfaces(TProvider&& provider, TRestProviders&&... rest){

            // (1) TODO: Add support to Many-interfaces providers (ie: a single provider adding more than one interface)
            // (2) TODO: Add support to optional providers (ie: a provider that decides not to add anything based on the type). Should be included in (1)

            // Create an functor interface InterfaceDeclaration used to add interfaces to the property.
            // Call the provider with both the getter, the setter and the interface above and let the provider handle everything

            // void provider::operator()(TGetter, TSetter, InterfaceDeclarator)
            // void provider::operator()(TProperty, InterfaceDeclarator)

            auto interface_type = std::type_index(typeid(decltype(provider())));

            if (interfaces_.find(interface_type) == interfaces_.end()) {

                interfaces_.insert(std::make_pair(interface_type,
                                                  linb::any(provider())));

            }

            AddInterfaces(rest...);

        }

        inline void Property::AddInterfaces() {}

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