
/// \file property.h
/// \brief This header is part of the syntropy reflection system. It contains classes used to handle class properties.
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

        /// \brief Describes a class property.
        /// A property can either be a member field, a getter method (for read-only properties) or a getter\setter method pairs.
        /// \author Raffaele D. Facendola - 2016
        class Property {

        public:

            /// \brief Structure containing the actual member pointers to the underlying property.
            /// See template specializations.
            template <typename... TAccessors>
            class PropertyT;

            /// \brief Create a new named property.
            Property(const char* name);

            /// \brief Default destructor.
            virtual ~Property() = default;

            /// \brief Get the property name.
            /// \return Returns the property name.
            const HashedString& GetName() const noexcept;

            /// \brief Get the property type.
            /// \brief Returns the property type.
            virtual const Type& GetType() const noexcept = 0;
            
            /// \brief Get the value of the property for the specified instance.
            /// The method fails if the instance doesn't define the specified property or the specified value cannot contain the property value due to a wrong type.
            /// \param instance Instance to read the property from.
            /// \param value If the method succeeds contains the property value.
            /// \return Returns true if the property could be read, returns false otherwise.
            template <typename TInstance, typename TValue>
            bool Get(const TInstance& instance, TValue&& value) const;

            /// \brief Set the value of the property for the specified instance.
            /// The method fails if the instance doesn't define the specified property or the specified value cannot be written due to a wrong type.
            /// \param instance Instance to set the property to.
            /// \param value Value to set.
            /// \return Returns true if the property could be written, returns false otherwise.
            template <typename TInstance, typename TValue>
            bool Set(TInstance&& instance, const TValue& value) const;

            /// \brief Add a new interface to the property.
            /// The method creates an instance of TInstance using TArgs as construction parameters.
            /// TInstance must be copy constructible.
            /// Only one interface per type can be added.
            /// \usage property.AddInterface<Foo>(bar, baz);            // Assign an instance Foo(bar, baz) to the property
            ///        property.GetInterface<Foo>()->DoFoo();           // Query for the property for a specified interface.
            /// \param arguments Arguments to pass to the constructor of TInterface.
            /// \return Returns true if the method succeeds, return false if there was another interface of type TInterface.
            template <typename TInterface, typename... TArgs>
            bool AddInterface(TArgs&&... arguments);

            /// \brief Query the property for an interface of type TInterface.
            /// \return If an interface of type TInterface was previously added via AddInterface(.), returns a pointer to that interface, otherwise returns nullptr.
            /// \remarks This method doesn't account for polymorphism. If a class of type Foo derived from Bar is added to the property, GetInterface<Bar>() will return nullptr even if a conversion exists.
            template <typename TInterface>
            const TInterface* GetInterface() const;

            /// \brief Query the property for an interface of type TInterface.
            /// \return If an interface of type TInterface was previously added via AddInterface(.), returns a pointer to that interface, otherwise returns nullptr.
            /// \remarks This method doesn't account for polymorphism. If a class of type Foo derived from Bar is added to the property, GetInterface<Bar>() will return nullptr even if a conversion exists.
            template <typename TInterface>
            TInterface* GetInterface();
            
        private:

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

            HashedString name_;                                             ///< \brief Property name.

            std::unordered_map<std::type_index, linb::any> interfaces_;     ///< \brief Set of interfaces assigned to the property.

        };

    }

}

namespace syntropy{ 

    namespace reflection {

        //////////////// PROPERTY :: PROPERTY T ////////////////
        
        /// \brief Template specialization of PropertyT to handle member fields.
        /// \author Raffaele D. Facendola - 2016
        template <typename TClass, typename TField>
        class Property::PropertyT<TField (TClass::*)> : public Property {

        public:

            PropertyT(const char* name, TField(TClass::* field))
                : Property(name)
                , field_(field) {}

            virtual const Type& GetType() const noexcept override {

                return TypeOf<TField>();

            }

            /// \brief Apply a functor to this property.
            /// The arguments passed to the functor are the property and a pointer to the field.
            /// \usage: property << Foo();          // Resolves to Foo()(property, field)
            /// \return Returns a reference to the property.
            template <typename TFunctor>
            auto operator<<(TFunctor&& functor) {

                functor(*this, field_);

                return *this;

            }

        private:

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

            TField TClass::* field_;                ///< \brief Member field.

        };

        /// \brief Template specialization of PropertyT to handle read-only properties via a getter method.
        /// \author Raffaele D. Facendola - 2016
        template <typename TClass, typename TProperty>
        class Property::PropertyT<TProperty(TClass::*)() const> : public Property {

        public:

            PropertyT(const char* name, TProperty(TClass::* getter)() const)
                : Property(name)
                , getter_(getter) {}

            virtual const Type& GetType() const noexcept override {

                return TypeOf<std::remove_reference_t<TProperty>>();

            }

            /// \brief Apply a functor to this property.
            /// The arguments passed to the functor are the property and a pointer to the getter.
            /// \usage: property << Foo();          // Resolves to Foo()(property, getter)
            /// \return Returns a reference to the property.
            template <typename TFunctor>
            auto operator<<(TFunctor&& functor) {

                functor(*this, getter_);

                return *this;

            }

        private:

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

            TProperty(TClass::* getter_)() const;           ///< \brief Getter method of the property.

        };

        /// \brief Template specialization of PropertyT to handle properties via a getter\setter method pair.
        /// The getter has the form of 'TProperty Getter() const'
        /// The setter has the form of '? Setter(TProperty)'
        /// \author Raffaele D. Facendola - 2016
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

            /// \brief Apply a functor to this property.
            /// The arguments passed to the functor are the property and a pointer to both the getter and the setter.
            /// \usage: property << Foo();          // Resolves to Foo()(property, getter, setter)
            /// \return Returns a reference to the property.
            template <typename TFunctor>
            auto operator<<(TFunctor&& functor) {

                functor(*this, getter_, setter_);

                return *this;

            }

        private:

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

            TProperty(TClass::* getter_)() const;           ///< \brief Getter method of the property.

            TReturn(TClass::* setter_)(TProperty);          ///< \brief Setter method of the property.

        };

        /// \brief Template specialization of PropertyT to handle properties via a getter\setter method pair.
        /// The getter has the form of 'const TProperty& Getter() const'
        /// The setter has the form of 'TProperty& Setter()'
        /// \author Raffaele D. Facendola - 2016
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

            /// \brief Apply a functor to this property.
            /// The arguments passed to the functor are the property and a pointer to both the getter and the setter.
            /// \usage: property << Foo();          // Resolves to Foo()(property, getter, setter)
            /// \return Returns a reference to the property.
            template <typename TFunctor>
            auto operator<<(TFunctor functor) {

                functor(*this, getter_, setter_);

                return *this;

            }

        private:

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

    }

}