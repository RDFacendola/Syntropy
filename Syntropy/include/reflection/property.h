
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

#include "linb/any/any.hpp"

namespace syntropy {

    namespace reflection {

        class Property {

        public:

            /// \brief No copy constructor.
            Property(const Property& other) = delete;
            
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
            
            template <typename TInterface, typename... TArgs>
            bool CreateInterface(TArgs&&... arguments);

            template <typename TInterface>
            const TInterface* GetInterface() const;

            template <typename TInterface>
            TInterface* GetInterface();

            template <typename TInstance, typename TValue>
            bool Get(const TInstance& instance, TValue&& value) const;

            template <typename TInstance, typename TValue>
            bool Set(TInstance&& instance, const TValue& value) const;

        private:

            /// \brief Common interface used to access the underlying property.
            struct IProperty {

                /// \brief Get the property type.
                /// \brief Returns the property type.
                virtual const Type& GetType() const noexcept = 0;

                /// \brief Get the property value.
                /// \param instance Instance the property refers to.
                /// \param value If the method succeeds, it contains the value read from the property.
                /// \return Return true if the property was legitimate for the provided instance and the value could be read, returns false otherwise.
                virtual bool Get(Instance instance, Instance value) const = 0;

                /// \brief Set the property value.
                /// \param instance Instance the property refers to.
                /// \param value Value to set.
                /// \return Return true if the property was legitimate for the provided instance and the value could be written, returns false otherwise.
                virtual bool Set(Instance instance, Instance value) const = 0;

            };

            /// \brief Structure containing the actual member pointers to the underlying property.
            /// See template specializations.
            template <typename... TAccessors>
            struct PropertyT : IProperty {};

            void AddInterfaces();

            HashedString name_;                                             ///< \brief Property name.

            std::unique_ptr<IProperty> property_;                           ///< \brief Actual property pointer.

            std::unordered_map<std::type_index, linb::any> interfaces_;     ///< \brief Set of interfaces assigned to the property.

        };

        class InterfaceDeclaration{

        public:

            InterfaceDeclaration(Property& property);

            template <typename TInterface, typename... TArgs>
            bool CreateInterface(TArgs&&... arguments);

        private:

            Property& property_;

        };

    }

}

namespace syntropy{ 

    namespace reflection {

        //////////////// PROPERTY :: PROPERTY T ////////////////
        
        template <typename TClass, typename TField>
        struct Property::PropertyT<TField (TClass::*)> : Property::IProperty {

            PropertyT(TField(TClass::* field))
                : field_(field) {}

            virtual const Type& GetType() const noexcept override {

                return TypeOf<TField>();

            }

            virtual bool Get(Instance instance, Instance value) const override{

                auto concrete_instance = instance.As<const TClass>();
                auto concrete_value = value.As<std::decay_t<TField>>();

                if (concrete_value && concrete_instance) {

                    *concrete_value = concrete_instance->*field_;

                    return true;

                }

                return false;

            }

            virtual bool Set(Instance instance, Instance value) const override {

                auto concrete_instance = instance.As<TClass>();
                auto concrete_value = value.As<const TField>();

                if (concrete_value && concrete_instance) {

                    conditional_assign(concrete_instance->*field_, *concrete_value);

                    return !std::is_const<TField>::value;

                }

                return false;

            }

            TField TClass::* field_;                ///< \brief Member field.

        };

        template <typename TClass, typename TProperty>
        struct Property::PropertyT<TProperty(TClass::*)() const> : Property::IProperty {

            PropertyT(TProperty(TClass::* getter)() const)
                : getter_(getter) {}

            virtual const Type& GetType() const noexcept override {

                return TypeOf<std::remove_reference_t<TProperty>>();

            }

            virtual bool Get(Instance instance, Instance value) const override {

                auto concrete_instance = instance.As<const TClass>();
                auto concrete_value = value.As<std::decay_t<TProperty>>();

                if (concrete_value && concrete_instance) {

                    *concrete_value = (concrete_instance->*getter_)();

                    return true;

                }

                return false;

            }

            virtual bool Set(Instance instance, Instance value) const override {

                return false;

            }

            TProperty(TClass::* getter_)() const;           ///< \brief Getter method of the property.

        };

        template <typename TClass, typename TProperty, typename TReturn>
        struct Property::PropertyT<TProperty(TClass::*)() const, TReturn(TClass::*)(TProperty)> : Property::IProperty {

            PropertyT(TProperty(TClass::* getter)() const, TReturn(TClass::* setter)(TProperty))
                : getter_(getter)
                , setter_(setter){}

            virtual const Type& GetType() const noexcept override {

                return TypeOf<std::remove_cv_t<std::remove_reference_t<TProperty>>>();

            }

            virtual bool Get(Instance instance, Instance value) const override {

                auto concrete_instance = instance.As<const TClass>();
                auto concrete_value = value.As<std::decay_t<TProperty>>();

                if (concrete_value && concrete_instance) {

                    *concrete_value = (concrete_instance->*getter_)();

                    return true;

                }

                return false;

            }

            virtual bool Set(Instance instance, Instance value) const override {

                auto concrete_instance = instance.As<TClass>();
                auto concrete_value = value.As<const std::remove_reference_t<TProperty>>();

                if (concrete_value && concrete_instance) {

                    (concrete_instance->*setter_)(*concrete_value);

                    return true;

                }

                return false;

            }

            TProperty(TClass::* getter_)() const;           ///< \brief Getter method of the property.

            TReturn(TClass::* setter_)(TProperty);          ///< \brief Setter method of the property.

        };

        template <typename TClass, typename TProperty>
        struct Property::PropertyT<const TProperty&(TClass::*)() const, TProperty&(TClass::*)()> : Property::IProperty {

            PropertyT(const TProperty&(TClass::* getter)() const, TProperty&(TClass::* setter)())
                : getter_(getter)
                , setter_(setter) {}

            virtual const Type& GetType() const noexcept override {

                return TypeOf<TProperty>();

            }

            virtual bool Get(Instance instance, Instance value) const override {

                auto concrete_instance = instance.As<const TClass>();
                auto concrete_value = value.As<std::decay_t<TProperty>>();

                if (concrete_value && concrete_instance) {

                    *concrete_value = (concrete_instance->*getter_)();

                    return true;

                }

                return false;

            }

            virtual bool Set(Instance instance, Instance value) const override {

                auto concrete_instance = instance.As<TClass>();
                auto concrete_value = value.As<const TProperty>();

                if (concrete_value && concrete_instance) {

                    (concrete_instance->*setter_)() = *concrete_value;

                    return true;

                }

                return false;

            }

            const TProperty&(TClass::* getter_)() const;       ///< \brief Getter method of the property.

            TProperty&(TClass::* setter_)();                   ///< \brief Setter method of the property.

        };

        //////////////// PROPERTY ////////////////

        template <typename TProperty>
        Property::Property(const HashedString& name, TProperty property) noexcept
            : name_(name)
            , property_(std::make_unique<PropertyT<TProperty>>(property)) {}

        template <typename TGetter, typename TSetter>
        Property::Property(const HashedString& name, TGetter getter, TSetter setter) noexcept
            : name_(name)
            , property_(std::make_unique<PropertyT<TGetter, TSetter>>(getter, setter)) {}

        template <typename TProvider, typename... TRestProviders>
        void Property::AddInterfaces(TProvider&& provider, TRestProviders&&... rest){

            provider(/*getter, setter, */InterfaceDeclaration(*this));

            AddInterfaces(rest...);

        }

        template <typename TInterface, typename... TArgs>
        bool Property::CreateInterface(TArgs&&... arguments){

            auto interface_type = std::type_index(typeid(TInterface));

            if (interfaces_.find(interface_type) == interfaces_.end()){

                interfaces_.insert(std::make_pair(interface_type,
                                                  linb::any(TInterface(std::forward<TArgs>(arguments)...))));

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

            return property_->Get(MakeConstInstance(instance),
                                  MakeInstance(std::forward<TValue>(value)));

        }

        template <typename TInstance, typename TValue>
        bool Property::Set(TInstance&& instance, const TValue& value) const {

            return property_->Set(MakeInstance(std::forward<TInstance>(instance)), 
                                  MakeConstInstance(value));

        }

        //////////////// INTERFACE DECLARATION ////////////////

        template <typename TInterface, typename... TArgs>
        bool InterfaceDeclaration::CreateInterface(TArgs&&... arguments){

            return property_.CreateInterface<TInterface>(std::forward<TArgs>(arguments)...);

        }

    }

}