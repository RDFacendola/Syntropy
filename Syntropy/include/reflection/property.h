
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

            /// \brief Structure containing the actual member pointers to the underlying property.
            /// See template specializations.
            template <typename... TAccessors>
            class PropertyT;

            Property(const char* name);

            /// \brief Default destructor.
            virtual ~Property() = default;

            const HashedString& GetName() const noexcept;

            /// \brief Get the property type.
            /// \brief Returns the property type.
            virtual const Type& GetType() const noexcept = 0;
            
            template <typename TInstance, typename TValue>
            bool Get(const TInstance& instance, TValue&& value) const;

            template <typename TInstance, typename TValue>
            bool Set(TInstance&& instance, const TValue& value) const;

            template <typename TInterface, typename... TArgs>
            bool AddInterface(TArgs&&... arguments);

            template <typename TInterface>
            const TInterface* GetInterface() const;

            template <typename TInterface>
            TInterface* GetInterface();
            
        protected:

            /// \brief Get the property value.
            /// \param instance Instance the property refers to.
            /// \param value If the method succeeds, it contains the value read from the property.
            /// \return Return true if the property was legitimate for the provided instance and the value could be read, returns false otherwise.
            virtual bool PropertyGet(Instance instance, Instance value) const = 0;

            /// \brief Set the property value.
            /// \param instance Instance the property refers to.
            /// \param value Value to set.
            /// \return Return true if the property was legitimate for the provided instance and the value could be written, returns false otherwise.
            virtual bool PropertySet(Instance instance, Instance value) const = 0;

        private:

            HashedString name_;                                             ///< \brief Property name.

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
        class Property::PropertyT<TField (TClass::*)> : public Property {

        public:

            PropertyT(const char* name, TField(TClass::* field))
                : Property(name)
                , field_(field) {}

            virtual const Type& GetType() const noexcept override {

                return TypeOf<TField>();

            }

        protected:

            virtual bool PropertyGet(Instance instance, Instance value) const override{

                auto concrete_instance = instance.As<const TClass>();
                auto concrete_value = value.As<std::decay_t<TField>>();

                if (concrete_value && concrete_instance) {

                    *concrete_value = concrete_instance->*field_;

                    return true;

                }

                return false;

            }

            virtual bool PropertySet(Instance instance, Instance value) const override {

                auto concrete_instance = instance.As<TClass>();
                auto concrete_value = value.As<const TField>();

                if (concrete_value && concrete_instance) {

                    conditional_assign(concrete_instance->*field_, *concrete_value);

                    return !std::is_const<TField>::value;

                }

                return false;

            }

        private:

            TField TClass::* field_;                ///< \brief Member field.

        };

        template <typename TClass, typename TProperty>
        class Property::PropertyT<TProperty(TClass::*)() const> : public Property {

        public:

            PropertyT(const char* name, TProperty(TClass::* getter)() const)
                : Property(name)
                , getter_(getter) {}

            virtual const Type& GetType() const noexcept override {

                return TypeOf<std::remove_reference_t<TProperty>>();

            }

        protected:

            virtual bool PropertyGet(Instance instance, Instance value) const override {

                auto concrete_instance = instance.As<const TClass>();
                auto concrete_value = value.As<std::decay_t<TProperty>>();

                if (concrete_value && concrete_instance) {

                    *concrete_value = (concrete_instance->*getter_)();

                    return true;

                }

                return false;

            }

            virtual bool PropertySet(Instance instance, Instance value) const override {

                return false;

            }

        private:

            TProperty(TClass::* getter_)() const;           ///< \brief Getter method of the property.

        };

        template <typename TClass, typename TProperty, typename TReturn>
        class Property::PropertyT<TProperty(TClass::*)() const, TReturn(TClass::*)(TProperty)> : public Property {

        public:

            PropertyT(const char* name, TProperty(TClass::* getter)() const, TReturn(TClass::* setter)(TProperty))
                : Property(name)
                , getter_(getter)
                , setter_(setter){}

            virtual const Type& GetType() const noexcept override {

                return TypeOf<std::remove_cv_t<std::remove_reference_t<TProperty>>>();

            }

        protected:

            virtual bool PropertyGet(Instance instance, Instance value) const override {

                auto concrete_instance = instance.As<const TClass>();
                auto concrete_value = value.As<std::decay_t<TProperty>>();

                if (concrete_value && concrete_instance) {

                    *concrete_value = (concrete_instance->*getter_)();

                    return true;

                }

                return false;

            }

            virtual bool PropertySet(Instance instance, Instance value) const override {

                auto concrete_instance = instance.As<TClass>();
                auto concrete_value = value.As<const std::remove_reference_t<TProperty>>();

                if (concrete_value && concrete_instance) {

                    (concrete_instance->*setter_)(*concrete_value);

                    return true;

                }

                return false;

            }

        private:

            TProperty(TClass::* getter_)() const;           ///< \brief Getter method of the property.

            TReturn(TClass::* setter_)(TProperty);          ///< \brief Setter method of the property.

        };

        template <typename TClass, typename TProperty>
        class Property::PropertyT<const TProperty&(TClass::*)() const, TProperty&(TClass::*)()> : public Property {

        public:

            PropertyT(const char* name, const TProperty&(TClass::* getter)() const, TProperty&(TClass::* setter)())
                : Property(name)
                , getter_(getter)
                , setter_(setter) {}

            virtual const Type& GetType() const noexcept override {

                return TypeOf<TProperty>();

            }

        protected:

            virtual bool PropertyGet(Instance instance, Instance value) const override {

                auto concrete_instance = instance.As<const TClass>();
                auto concrete_value = value.As<std::decay_t<TProperty>>();

                if (concrete_value && concrete_instance) {

                    *concrete_value = (concrete_instance->*getter_)();

                    return true;

                }

                return false;

            }

            virtual bool PropertySet(Instance instance, Instance value) const override {

                auto concrete_instance = instance.As<TClass>();
                auto concrete_value = value.As<const TProperty>();

                if (concrete_value && concrete_instance) {

                    (concrete_instance->*setter_)() = *concrete_value;

                    return true;

                }

                return false;

            }

        private:

            const TProperty&(TClass::* getter_)() const;       ///< \brief Getter method of the property.

            TProperty&(TClass::* setter_)();                   ///< \brief Setter method of the property.

        };

        //////////////// PROPERTY ////////////////

        template <typename TInterface, typename... TArgs>
        bool Property::AddInterface(TArgs&&... arguments){

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

            return PropertyGet(MakeConstInstance(instance),
                               MakeInstance(std::forward<TValue>(value)));

        }

        template <typename TInstance, typename TValue>
        bool Property::Set(TInstance&& instance, const TValue& value) const {

            return PropertySet(MakeInstance(std::forward<TInstance>(instance)), 
                               MakeConstInstance(value));

        }

        //////////////// INTERFACE DECLARATION ////////////////

        template <typename TInterface, typename... TArgs>
        bool InterfaceDeclaration::CreateInterface(TArgs&&... arguments){

            return property_.AddInterface<TInterface>(std::forward<TArgs>(arguments)...);

        }

    }

}