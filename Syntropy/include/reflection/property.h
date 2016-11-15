
/// \file property.h
/// \brief This header is part of the syntropy reflection system. It contains classes used to handle class properties.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <unordered_map>
#include <typeindex>

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

            /// \brief Move the value of the property for the specified instance.
            /// The method fails if the instance doesn't define the specified property or the specified value cannot contain the property value due to a wrong type.
            /// \param instance Instance to move the property from.
            /// \param value If the method succeeds contains the property value.
            /// \return Returns true if the property could be moved, returns false otherwise.
            template <typename TInstance, typename TValue>
            bool Move(TInstance& instance, TValue&& value) const;

            /// \brief Set the value of the property for the specified instance.
            /// The method fails if the instance doesn't define the specified property or the specified value cannot be written due to a wrong type.
            /// \param instance Instance to set the property to.
            /// \param value Value to set.
            /// \return Returns true if the property could be written, returns false otherwise.
            template <typename TInstance, typename TValue>
            bool Set(TInstance&& instance, const TValue& value) const;

            /// \brief Set the value of the property for the specified instance.
            /// The method fails if the instance doesn't define the specified property or the specified value cannot be written due to a wrong type.
            /// \param instance Instance to set the property to.
            /// \param value Value to move.
            /// \return Returns true if the property could be written, returns false otherwise.
            template <typename TInstance, typename TValue, typename = std::enable_if_t<!std::is_lvalue_reference<TValue>::value>>
            bool Set(TInstance&& instance, TValue&& value) const {

                return PropertyMoveSet(MakeInstance(std::forward<TInstance>(instance)),
                                       MakeInstance(value));

            }

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

            /// \brief Move the property value.
            /// \param instance Instance the property refers to.
            /// \param value If the method succeeds, it contains the value moved from the property.
            /// \return Return true if the property was legitimate for the provided instance and the value could be moved, returns false otherwise.
            virtual bool PropertyMove(Instance instance, Instance value) const = 0;

            /// \brief Set the property value.
            /// \param instance Instance the property refers to.
            /// \param value Value to set.
            /// \return Return true if the property was legitimate for the provided instance and the value could be written, returns false otherwise.
            virtual bool PropertySet(Instance instance, Instance value) const = 0;

            /// \brief Move the property value.
            /// \param instance Instance the property refers to.
            /// \param value Value to move.
            /// \return Return true if the property was legitimate for the provided instance and the value could be moved, returns false otherwise.
            virtual bool PropertyMoveSet(Instance instance, Instance value) const = 0;

            HashedString name_;                                             ///< \brief Property name.

            std::unordered_map<std::type_index, linb::any> interfaces_;     ///< \brief Set of interfaces assigned to the property.

        };

        /// \brief Get a property value via a pointer to a member method \ field.
        /// \param instance Object where to get the value from.
        /// \param value If the method succeeds contains the value read from the instance.
        /// \param getter Getter method or field.
        template <typename TGetter>
        bool GetPropertyValue(Instance instance, Instance value, TGetter getter);

        /// \brief Move a property value via a pointer to a member method \ field.
        /// \param instance Object where to move the value from.
        /// \param value If the method succeeds contains the value moved from the instance.
        /// \param getter Mover method or field.
        template <typename TMover>
        bool MovePropertyValue(Instance instance, Instance value, TMover mover);

        /// \brief Set a property value via a pointer to a member method \ field.
        /// This method requires the setter method to be copy-constructible or the field to be copy-assignable.
        /// \param instance Object where to write the value to.
        /// \param value Value to copy.
        /// \param setter Setter method or field.
        template <typename TSetter>
        bool SetPropertyValue(Instance instance, Instance value, TSetter setter);

        /// \brief Set a property value via a pointer to a member method \ field.
        /// This method requires the setter method to be move-constructible or the field to be move-assignable. Fallback to copy-construction or copy-assignment if no such capabilities are provided.
        /// \param instance Object where to write the value to.
        /// \param value Value to copy.
        /// \param setter Setter method or field.
        template <typename TSetter>
        bool MoveSetPropertyValue(Instance instance, Instance value, TSetter setter);

    }

}

namespace syntropy{ 

    namespace reflection {

        // Utility methods for getting or setting a property value.

        // Getter

        template <typename TGetter, bool is_member_field, typename = void>
        struct PropertyGetter {

            bool operator()(...) const { 
                
                return false; 
            
            }

        };

        template <typename TClass, typename TField>
        struct PropertyGetter<TField(TClass::*), true, std::enable_if_t<std::is_copy_assignable_v<std::decay_t<TField>>>> {

            bool operator()(Instance instance, Instance value, TField (TClass::* field)) const {

                auto concrete_instance = instance.As<const TClass>();
                auto concrete_value = value.As<std::decay_t<TField>>();

                if (concrete_value && concrete_instance) {

                    *concrete_value = concrete_instance->*field;       // Copy assignment

                    return true;

                }

                return false;

            }

        };

        template <typename TClass, typename TProperty>
        struct PropertyGetter<TProperty(TClass::*)() const, false, std::enable_if_t<std::is_copy_assignable_v<std::decay_t<TProperty>>>> {
 
             bool operator()(Instance instance, Instance value, TProperty(TClass::* getter)() const) const {
 
                 auto concrete_instance = instance.As<const TClass>();
                 auto concrete_value = value.As<std::decay_t<TProperty>>();
 
                 if (concrete_value && concrete_instance) {
 
                     *concrete_value = (concrete_instance->*getter)();
 
                     return true;
 
                 }
 
                 return false;
 
             }
 
         };

        template <typename TGetter>
        bool GetPropertyValue(Instance instance, Instance value, TGetter getter){

            return PropertyGetter<TGetter, std::is_member_object_pointer_v<TGetter>>()(instance, value, getter);

        }

        // Mover

        template <typename TMover, bool is_member_field, typename = void>
        struct PropertyMover {

            bool operator()(Instance instance, Instance value, TMover mover) const {

                return GetPropertyValue(instance, value, mover);

            }

        };

        template <typename TClass, typename TField>
        struct PropertyMover<TField(TClass::*), true, std::enable_if_t<std::is_move_assignable_v<std::decay_t<TField>>>> {

            bool operator()(Instance instance, Instance value, TField(TClass::* field)) const {

                auto concrete_instance = instance.As<TClass>();
                auto concrete_value = value.As<std::decay_t<TField>>();

                if (concrete_value && concrete_instance) {

                    *concrete_value = std::move(concrete_instance->*field);       // Move assignment

                    return true;

                }

                return false;

            }

        };

        template <typename TClass, typename TProperty>
        struct PropertyMover<TProperty(TClass::*)() const, false, std::enable_if_t<std::is_move_assignable_v<std::remove_reference_t<TProperty>>>> {

            bool operator()(Instance instance, Instance value, TProperty(TClass::* mover)() const) const {

                auto concrete_instance = instance.As<TClass>();
                auto concrete_value = value.As<std::decay_t<TProperty>>();

                if (concrete_value && concrete_instance) {

                    *concrete_value = std::move((concrete_instance->*mover)());     // Move assignment

                    return true;

                }

                return false;

            }

        };

        template <typename TClass, typename TProperty>
        struct PropertyMover<TProperty&(TClass::*)(), false, std::enable_if_t<std::is_move_assignable_v<TProperty>>> {

            bool operator()(Instance instance, Instance value, TProperty&(TClass::* mover)()) const {

                auto concrete_instance = instance.As<TClass>();
                auto concrete_value = value.As<std::decay_t<TProperty>>();

                if (concrete_value && concrete_instance) {

                    *concrete_value = std::move((concrete_instance->*mover)());     // Move assignment

                    return true;

                }

                return false;

            }

        };

        template <typename TMover>
        bool MovePropertyValue(Instance instance, Instance value, TMover mover) {

            return PropertyMover<TMover, std::is_member_object_pointer_v<TMover>>()(instance, value, mover);

        }

        // Setter

        template <typename TSetter, bool is_member_field, typename = void>
        struct PropertySetter {

            bool operator()(...) const { 
                
                return false; 
            
            }

        };

        template <typename TClass, typename TField>
        struct PropertySetter<TField(TClass::*), true, std::enable_if_t<std::is_copy_assignable_v<TField>>> {

            bool operator()(Instance instance, Instance value, TField(TClass::* field)) const {

                auto concrete_instance = instance.As<TClass>();
                auto concrete_value = value.As<const TField>();

                if (concrete_value && concrete_instance) {

                    concrete_instance->*field = *concrete_value;        // Copy assignment

                    return true;

                }

                return false;

            }

        };

        template<typename TClass, typename TProperty, typename TAny>
        struct PropertySetter<TAny(TClass::*)(TProperty), false, std::enable_if_t<std::is_copy_constructible_v<TProperty>>> {

            bool operator()(Instance instance, Instance value, TAny(TClass::* setter)(TProperty)) const {

                auto concrete_instance = instance.As<TClass>();
                auto concrete_value = value.As<const std::remove_reference_t<TProperty>>();

                if (concrete_value && concrete_instance) {

                    (concrete_instance->*setter)(*concrete_value);      // Copy constructor

                    return true;

                }

                return false;

            }

        };

        template <typename TClass, typename TProperty>
        struct PropertySetter<TProperty&(TClass::*)(), false, std::enable_if_t<std::is_copy_assignable_v<TProperty>>> {

            bool operator()(Instance instance, Instance value, TProperty&(TClass::* setter)()) const {

                auto concrete_instance = instance.As<TClass>();
                auto concrete_value = value.As<const TProperty>();

                if (concrete_value && concrete_instance) {

                    (concrete_instance->*setter)() = *concrete_value;       // Copy assignment

                    return true;

                }

                return false;

            }

        };

        template <typename TSetter>
        bool SetPropertyValue(Instance instance, Instance value, TSetter setter) {

            return PropertySetter<TSetter, std::is_member_object_pointer_v<TSetter>>()(instance, value, setter);

        }
        
        // Move-setter

        template <typename TSetter, bool is_member_field, typename = void>
        struct PropertyMoveSetter {

            bool operator()(...) const {

                return false;

            }

        };

        template <typename TClass, typename TField>
        struct PropertyMoveSetter<TField(TClass::*), true, std::enable_if_t<std::is_move_assignable_v<TField>>> {

            bool operator()(Instance instance, Instance value, TField(TClass::* field)) const {

                auto concrete_instance = instance.As<TClass>();
                auto concrete_value = value.As<TField>();

                if (concrete_value && concrete_instance) {

                    concrete_instance->*field = std::move(*concrete_value);        // Move assignment

                    return true;

                }

                return false;

            }

        };

        template<typename TClass, typename TProperty, typename TAny>
        struct PropertyMoveSetter<TAny(TClass::*)(TProperty), false, std::enable_if_t<std::is_move_constructible_v<TProperty>>> {

            bool operator()(Instance instance, Instance value, TAny(TClass::* setter)(TProperty)) const {

                auto concrete_instance = instance.As<TClass>();
                auto concrete_value = value.As<std::remove_reference_t<TProperty>>();

                if (concrete_value && concrete_instance) {

                    (concrete_instance->*setter)(std::move(*concrete_value));      // Move constructor

                    return true;

                }

                return false;

            }

        };

        template <typename TClass, typename TProperty>
        struct PropertyMoveSetter<TProperty&(TClass::*)(), false, std::enable_if_t<std::is_move_assignable_v<TProperty>>> {

            bool operator()(Instance instance, Instance value, TProperty&(TClass::* setter)()) const {

                auto concrete_instance = instance.As<TClass>();
                auto concrete_value = value.As<TProperty>();

                if (concrete_value && concrete_instance) {

                    (concrete_instance->*setter)() = std::move(*concrete_value);   // Move assignment

                    return true;

                }

                return false;

            }

        };

        template <typename TSetter>
        bool MoveSetPropertyValue(Instance instance, Instance value, TSetter setter) {

            return PropertyMoveSetter<TSetter, std::is_member_object_pointer_v<TSetter>>()(instance, value, setter);

        }

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

                return GetPropertyValue(instance, value, field_);

            }

            virtual bool PropertyMove(Instance instance, Instance value) const override {

                return MovePropertyValue(instance, value, field_);

            }

            virtual bool PropertySet(Instance instance, Instance value) const override {

                return SetPropertyValue(instance, value, field_);

            }

            virtual bool PropertyMoveSet(Instance instance, Instance value) const override {

                return MoveSetPropertyValue(instance, value, field_);

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

                return GetPropertyValue(instance, value, getter_);

            }

            virtual bool PropertyMove(Instance instance, Instance value) const override {

                return MovePropertyValue(instance, value, getter_);

            }

            virtual bool PropertySet(Instance instance, Instance value) const override {

                return false;

            }

            virtual bool PropertyMoveSet(Instance instance, Instance value) const override {

                return false;

            }

            TProperty(TClass::* getter_)() const;           ///< \brief Getter method of the property.

        };

        /// \brief Template specialization of PropertyT to handle properties via a getter\setter method pair.
        /// The getter has the form of 'TProperty Getter() const'
        /// The setter has the form of '? Setter(TProperty)'
        /// \author Raffaele D. Facendola - 2016
        template <typename TClass, typename TPropertyGet, typename TPropertySet, typename TAny>
        class Property::PropertyT<TPropertyGet(TClass::*)() const, TAny(TClass::*)(TPropertySet)> : public Property {

        public:

            PropertyT(const char* name, TPropertyGet(TClass::* getter)() const, TAny(TClass::* setter)(TPropertySet))
                : Property(name)
                , getter_(getter)
                , setter_(setter){}

            virtual const Type& GetType() const noexcept override {

                return TypeOf<std::remove_cv_t<std::remove_reference_t<TPropertyGet>>>();

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

                return GetPropertyValue(instance, value, getter_);

            }

            virtual bool PropertyMove(Instance instance, Instance value) const override {

                return MovePropertyValue(instance, value, getter_);

            }

            virtual bool PropertySet(Instance instance, Instance value) const override {

                return SetPropertyValue(instance, value, setter_);

            }

            virtual bool PropertyMoveSet(Instance instance, Instance value) const override {

                return MoveSetPropertyValue(instance, value, setter_);

            }

            TPropertyGet(TClass::* getter_)() const;            ///< \brief Getter method of the property.

            TAny(TClass::* setter_)(TPropertySet);              ///< \brief Setter method of the property.

        };

        /// \brief Template specialization of PropertyT to handle properties via a getter\setter method pair.
        /// The getter has the form of 'const TProperty& Getter() const'
        /// The setter has the form of 'TProperty& Setter()'
        /// \author Raffaele D. Facendola - 2016
        template <typename TClass, typename TPropertyGet, typename TPropertySet>
        class Property::PropertyT<const TPropertyGet&(TClass::*)() const, TPropertySet&(TClass::*)()> : public Property {

        public:

            PropertyT(const char* name, const TPropertyGet&(TClass::* getter)() const, TPropertySet&(TClass::* setter)())
                : Property(name)
                , getter_(getter)
                , setter_(setter) {}

            virtual const Type& GetType() const noexcept override {

                return TypeOf<TPropertyGet>();

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

                return GetPropertyValue(instance, value, getter_);

            }

            virtual bool PropertyMove(Instance instance, Instance value) const override {

                return MovePropertyValue(instance, value, setter_);     // The getter is pure. Use the setter to access the property instead.

            }

            virtual bool PropertySet(Instance instance, Instance value) const override {

                return SetPropertyValue(instance, value, setter_);

            }

            virtual bool PropertyMoveSet(Instance instance, Instance value) const override {

                return MoveSetPropertyValue(instance, value, setter_);

            }

            const TPropertyGet&(TClass::* getter_)() const;         ///< \brief Getter method of the property.

            TPropertySet&(TClass::* setter_)();                     ///< \brief Setter method of the property.

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
        bool Property::Move(TInstance& instance, TValue&& value) const {

            return PropertyMove(MakeInstance(instance),
                                MakeInstance(std::forward<TValue>(value)));

        }

        template <typename TInstance, typename TValue>
        bool Property::Set(TInstance&& instance, const TValue& value) const {

            return PropertySet(MakeInstance(std::forward<TInstance>(instance)), 
                               MakeConstInstance(value));

        }
        
    }

}