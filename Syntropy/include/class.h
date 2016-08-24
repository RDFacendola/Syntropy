
/// \file class.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <unordered_map>
#include <memory>
#include <type_traits>

#include <iostream>

#include "hashed_string.h"
#include "type_traits.h"
#include "method.h"
#include "any.h"

namespace syntropy {

    namespace reflection {

        class Class;

        class IClassDefinition;

        template <typename TClass>
        class ClassDefinition;

        template <typename TClass>
        struct ClassDeclaration;
        
        class IFactory;

        template <typename TClass>
        class Factory;

        template <ConstQualifier kConst>
        class BaseInstance;

        class Property;
        struct IClassProvider;
        
    }
    
}

namespace syntropy {

    namespace reflection {

        template <typename TClass>
        constexpr bool is_instantiable_v = !std::is_abstract_v<TClass> &&
                                            std::is_nothrow_default_constructible_v<TClass>;

        using ConstInstance = BaseInstance<ConstQualifier::kConst>;

        using Instance = BaseInstance<ConstQualifier::kNonConst>;
        
        /// \brief Describes a class type.
        /// \author Raffaele D. Facendola - 2016
        class Class {

        public:

            /// \brief No copy constructor.
            Class(const Class&) = delete;

            /// \brief No assignment operator.
            Class& operator=(const Class&) = delete;

            /// \brief Virtual destructor.
            virtual ~Class() = default;

            template <typename TClass>
            static Class& GetClass();

            /// \brief Get the name of the class.
            /// \return Returns the type string of the class.
            const HashedString& GetName() const noexcept;

            /// \brief Get the list of classes that are derived by this class.
            /// \return Returns the list of classes that are derived by this class.
            const std::vector<Class*>& GetBaseClasses() const noexcept;

            /// \brief Get a factory for this class.
            /// \return Returns an factory for this class if applicable. Returns nullptr otherwise.
            const IFactory* GetFactory() const noexcept;

            /// \brief Get a class property by name.
            /// \param property_name Name of the property to get.
            /// \return Returns a pointer to the requested property, if any. Returns nullptr otherwise.
            const Property* GetProperty(const HashedString& property_name) const noexcept;

            /// \brief Get a class method by name.
            /// \param method_name Name of the method to get.
            /// \return Returns a pointer tot he requested method if any. Returns nullptr otherwise.
            const Method* GetMethod(const HashedString& method_name) const noexcept;

            /// \brief Get the class properties list.
            /// \return Returns the class properties list.
            const std::unordered_map<size_t, Property>& GetProperties() const noexcept;

            /// \brief Check whether this class is abstract or not.
            /// \return Returns true if the class is abstract, returns false otherwise.
            bool IsAbstract() const noexcept;

            /// \brief Check whether this class is a base class for the specified one.
            /// \return Returns true if the class described by this instance is a base for the specified one or is the same type, returns false otherwise.
            bool IsBaseOf(const Class& other) const noexcept;

        private:
            
            template <typename TClass>
            static Class& GetUnqualifiedClass();

            /// \brief Create a new class via explicit class definition.
            Class(std::unique_ptr<IClassDefinition> definition);
            
            size_t class_id_;                                       ///< \brief Unique id. 
                                                                    ///< \remarks Can be different at each execution.

            std::unique_ptr<IClassDefinition> definition_;			///< \brief Class definition.


        };

        /// \brief Interface for class definition.
        /// \author Raffaele D. Facendola - 2016
        class IClassDefinition {

        public:

            /// \brief Virtual destructor.
            virtual ~IClassDefinition() = default;

            /// \brief Get the name of the class.
            /// \return Returns the type string of the class.
            virtual const HashedString& GetName() const noexcept = 0;

            /// \brief Get the list of classes that are derived by this class.
            /// \return Returns the list of classes that are derived by this class.
            virtual const std::vector<Class*>& GetBaseClasses() const noexcept = 0;

            /// \brief Get a factory for this class.
            /// \return Returns an factory for this class if applicable. Returns nullptr otherwise.
            virtual const IFactory* GetFactory() const noexcept = 0;

            /// \brief Get a class property by name.
            /// \param property_name Name of the property to get.
            /// \return Returns a pointer to the requested property, if any. Returns nullptr otherwise.
            virtual const Property* GetProperty(const HashedString& property_name) const noexcept = 0;

            /// \brief Get a class method by name.
            /// \param method_name Name of the method to get.
            /// \return Returns a pointer tot he requested method if any. Returns nullptr otherwise.
            virtual const Method* GetMethod(const HashedString& method_name) const noexcept = 0;

            /// \brief Get the class properties list.
            /// \return Returns the class properties list.
            virtual const std::unordered_map<size_t, Property>& GetProperties() const noexcept = 0;

            /// \brief Check whether this class is abstract or not.
            /// \return Returns true if the class is abstract, returns false otherwise.
            virtual bool IsAbstract() const noexcept = 0;

        };

        /// \brief Concrete class definition.
        /// \author Raffaele D. Facendola - 2016
        template <typename TClass>
        class ClassDefinition : public IClassDefinition {

        public:

            /// \brief No copy constructor.
            ClassDefinition(const ClassDefinition<TClass>& other) = delete;

            /// \brief No assignment operator.
            ClassDefinition<TClass>& operator=(const ClassDefinition<TClass>& other) = delete;

            /// \brief Create a named class definition.
            /// \param name name of the class.
            ClassDefinition(const HashedString& name) noexcept;
            
            virtual const HashedString& GetName() const noexcept override;

            virtual const std::vector<Class*>& GetBaseClasses() const noexcept override;

            virtual const IFactory* GetFactory() const noexcept override;

            virtual const Property* GetProperty(const HashedString& property_name) const noexcept override;

            virtual const Method* GetMethod(const HashedString& method_name) const noexcept override;

            virtual const std::unordered_map<size_t, Property>& GetProperties() const noexcept override;

            virtual bool IsAbstract() const noexcept override;

            template <typename TBaseClass>
            void DefineBaseClass() noexcept;

            /// Properties and getter of the form
            /// TProperty TClass::*
            /// TProperty (TClass::*)() const
            template <typename TProperty>
            void DefineProperty(const HashedString& name, TProperty&& property) noexcept;

            /// Getters and setters of the form
            /// TProperty (TClass::*)() const / void (TClass::*)(TProperty)
            /// const TProperty& (TClass::*)() const / TProperty& (TClass::*)()
            template <typename TGetter, typename TSetter>
            void DefineProperty(const HashedString& name, TGetter&& getter, TSetter&& setter) noexcept;

            //template <typename TMethod>
            //void DefineMethod(const HashedString& method_name, TMethod&& method) noexcept;

        private:

            HashedString name_;                                     ///< \brief Unique name.

            std::vector<Class*> base_classes_;                      ///< \brief List of all classes that are base of this one.

            std::unordered_map<size_t, Property> properties_;       ///< \brief List of all the supported properties.

            std::unordered_map<size_t, Method> methods_;            ///< \brief List of all the supported methods.

        };

        class IFactory {

        public:

            virtual Instance Instantiate() const noexcept = 0;

        };
        
        template <typename TClass>
        class Factory : public IFactory {

            static_assert(is_instantiable_v<TClass>, "TClass must be nothrow default constructible.");

        public:

            static Factory<TClass>& GetInstance() noexcept;

            virtual Instance Instantiate() const noexcept override;

        private:

            /// \brief Private constructor to prevent direct instantiation.
            Factory() = default;

        };

        template <typename TClass, typename = void>
        struct FactoryProvider {

            const IFactory* operator()() const noexcept {

                return nullptr;

            }

        };

        template <typename TClass>
        struct FactoryProvider<TClass,
                               typename std::enable_if_t<is_instantiable_v<TClass>>> {

            const IFactory* operator()() const noexcept {

                return std::addressof(Factory<typename drop_t<TClass>>::GetInstance());

            }

        };

        /// \brief Describes a polymorphic type-safe container for single values of any type.
        /// Loosely mimic the usage of a type-erased std::reference.
        /// \author Raffaele D. Facendola
        template <ConstQualifier kConstQualifier>
        class BaseInstance {

        public:
            
            template <typename TInstance>
            using qualify_t = typename std::conditional_t<kConstQualifier == ConstQualifier::kConst,
                                                          typename std::add_const_t<TInstance>,
                                                          typename std::remove_const_t<TInstance>>;

            /// \brief Create an empty instance.
            BaseInstance() = default;

            /// \brief Destructor.
            ~BaseInstance() = default;

            /// \brief Copy constructor.
            /// \param other Instance to copy.
            BaseInstance(const BaseInstance<kConstQualifier>& other) noexcept;
            
            operator ConstInstance() const noexcept;

            /// \brief Wraps an object to be referenced via Instance.
            /// \param instance Object to reference.
            template <typename TInstance>
            BaseInstance(TInstance* instance) noexcept;

            /// \brief Unified assignment operator.
            /// \param other Instance to assign from.
            BaseInstance& operator=(const BaseInstance<kConstQualifier>& other) noexcept;
            
            /// \brief Get a typed pointer to the wrapped object.
            /// \tparam TInstance Type of class to check against.
            /// \return Returns a pointer to the contained object if the underlying type derives from TInstance. Returns nullptr instead.
            template <typename TInstance>
            TInstance* As() const noexcept;

            bool IsEmpty() const noexcept;

            /// \brief Get the underlying type of the contained value.
            /// \return Returns the type of the contained value.
            const Class& GetClass() const noexcept;
            
        private:

            friend class BaseInstance<ConstQualifier::kNonConst>;       // Needed to grant access to the const-qualified version of the content

            /// \brief Base interface for the underlying content.
            struct IContent {

                /// \brief Virtual destructor.
                virtual ~IContent() = default;

                /// \brief Get the class of the instance being referenced.
                /// \return Returns the class of the instance being referenced.
                virtual const Class& GetClass() const noexcept = 0;

                /// \brief Clone the underlying value to another instance.
                /// \return Returns a pointer to the new copy of the value.
                virtual std::unique_ptr<typename BaseInstance<kConstQualifier>::IContent> Clone() const noexcept = 0;

                /// \brief Clone the underlying value to another const instance.
                /// \return Returns a pointer to the constant copy of the value.
                virtual std::unique_ptr<typename ConstInstance::IContent> ConstClone() const noexcept = 0;

            };

            /// \brief Strongly typed container for a single value.
            /// \tparam TValue Type of the contained value.
            template <typename TInstance>
            struct Content : public IContent {

                /// \brief Create a new container for a single value.
                ///\ param value Value to store inside the container.
                Content(TInstance& instance) noexcept;

                virtual const Class& GetClass() const noexcept override;

                virtual std::unique_ptr<typename BaseInstance<kConstQualifier>::IContent> Clone() const noexcept override;

                virtual std::unique_ptr<typename ConstInstance::IContent> ConstClone() const noexcept override {

                    // Possible MSVC bug
                    //
                    // Defining this function outside the class declaration yields error C2244: 
                    // 'Instance<kConstQualifier>::Content<TInstance>::ConstClone': unable to match function definition to an existing declaration
                    //
                    // [...]: note: existing declarations
                    // [...]: note: 'std::unique_ptr<Instance<kConstQualifier>::IContent> Instance<kConstQualifier>::Content<TInstance>::ConstClone(void) noexcept const'
                    //                                        ^^^^^^^^^^^^^^^                                                            ^^^^^^^^^^
                    //
                    // This declaration doesn't even exist...

                    return std::make_unique<ConstInstance::Content<ConstInstance::qualify_t<TInstance>>>(content_);      // Add const-ness

                }

                TInstance& content_;                            ///< \brief Actual content.

            };

            /// \brief Create a new instance from content pointer.
            BaseInstance(std::unique_ptr<IContent> content) noexcept;

            /// \brief Check whether the type of the object being referenced derives from the provided class.
            /// \tparam TInstance Type of class to check against.
            /// \return Returns true if the wrapped object's type derives from TInstance or is exactly TInstance. returns false otherwise.
            template <typename TInstance>
            bool Is() const noexcept;

            std::unique_ptr<IContent> content_;                 ///< \brief Wraps the actual value.

        };

        template <typename TInstance>
        ConstInstance wrap_const_instance(const TInstance& instance) noexcept;

        template <ConstQualifier kConstQualifier>
        ConstInstance wrap_const_instance(BaseInstance<kConstQualifier> instance) noexcept;

        template <typename TInstance>
        ConstInstance wrap_const_instance(const TInstance&&) = delete;

        template <typename TInstance>
        Instance wrap_instance(TInstance& instance) noexcept;
        
        Instance wrap_instance(ConstInstance instance) = delete;         // Denied: conversion loses qualifiers

        Instance wrap_instance(Instance instance) noexcept;
        
        template <typename TInstance>
        Instance wrap_instance(const TInstance&&) = delete;

        struct PropertyGetter {

            using TGetter = std::function<bool(const ConstInstance&, Any)>;

            template <typename TClass, typename TProperty>
            TGetter operator() (TProperty TClass::* field) const {

                return[field](const ConstInstance& instance, Any value) -> bool {

                    auto value_ptr = value.As<std::add_pointer_t<std::remove_const_t<std::remove_reference_t<TProperty>>>>();
                    auto instance_ptr = instance.As<const TClass>();

                    if (value_ptr && instance_ptr) {

                        **value_ptr = instance_ptr->*field;

                    }

                    return !!value_ptr && !!instance_ptr;

                };

            }

            template <typename TClass, typename TProperty>
            TGetter operator() (TProperty(TClass::* getter)() const) const {

                return[getter](const ConstInstance& instance, Any value) -> bool {

                    auto value_ptr = value.As<std::add_pointer_t<std::remove_const_t<std::remove_reference_t<TProperty>>>>();
                    auto instance_ptr = instance.As<const TClass>();

                    if (value_ptr && instance_ptr) {

                        **value_ptr = (instance_ptr->*getter)();

                    }

                    return !!value_ptr && !!instance_ptr;

                };

            }

        };

        struct PropertySetter {

            using TSetter = std::function<bool(const Instance&, Any)>;

            TSetter operator() () const {

                return[](const Instance&, Any) -> bool {

                    return false;

                };

            }

            template <typename TClass, typename TProperty>
            TSetter operator() (TProperty TClass::* property, typename std::enable_if_t<!std::is_const<TProperty>::value>* = nullptr) const {

                return[property](const Instance& instance, Any value) -> bool{

                    auto value_ptr = value.As<std::add_pointer_t<std::add_const_t<TProperty>>>();
                    auto instance_ptr = instance.As<TClass>();

                    if (value_ptr && instance_ptr) {

                        instance_ptr->*property = **value_ptr;

                    }

                    return !!value_ptr && !!instance_ptr;

                };

            }

            template <typename TClass, typename TProperty>
            TSetter operator() (TProperty TClass::*, typename std::enable_if_t<std::is_const<TProperty>::value>* = nullptr) const {

                return (*this)();

            }

            template <typename TClass, typename TProperty>
            TSetter operator() (void (TClass::* setter)(TProperty)) const {

                return[setter](const Instance& instance, Any value) -> bool {

                    auto value_ptr = value.As<std::add_pointer_t<std::add_const_t<TProperty>>>();
                    auto instance_ptr = instance.As<TClass>();

                    if (value_ptr && instance_ptr) {

                        (instance_ptr->*setter)(**value_ptr);

                    }

                    return !!value_ptr && !!instance_ptr;

                };

            }

            template <typename TClass, typename TProperty>
            TSetter operator() (TProperty& (TClass::* setter)()) const {

                return[setter](const Instance& instance, Any value) -> bool {

                    auto value_ptr = value.As<std::add_pointer_t<std::add_const_t<TProperty>>>();
                    auto instance_ptr = instance.As<TClass>();

                    if (value_ptr && instance_ptr) {

                        (instance_ptr->*setter)() = **value_ptr;

                    }

                    return !!value_ptr && !!instance_ptr;

                };

            }

        };
        
        struct IClassProvider {

            virtual const Class& operator()() const noexcept = 0;

        };

        template <typename TClass>
        struct ClassProvider : public IClassProvider {

            virtual const Class& operator()() const noexcept override;

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

            const Class& GetClass() const noexcept;

            template <typename TInstance, typename TValue>
            bool Get(const TInstance& instance, TValue& value) const;
            
            template <ConstQualifier kConstQualifier, typename TValue>
            bool Get(const BaseInstance<kConstQualifier>& instance, TValue& value) const;
            
            // SFINAE trick because TInstance& is no worse than const Instance& when TInstance = Instance. This would cause a sub-optimal overload resolution but won't produce any error whatsoever.
            template <typename TInstance, typename TValue, typename std::enable_if_t<!std::is_same<TInstance, Instance>::value, int> = 0>
            bool Set(TInstance& instance, const TValue& value) const;

            template <typename TValue>
            bool Set(const Instance& instance, const TValue& value) const;

        private:
            
            HashedString name_;                                     ///< \brief Property name.

            std::unique_ptr<IClassProvider> class_;                 ///< \brief Functor used to get the property class.

            PropertyGetter::TGetter getter_;                        ///< \brief Property getter.

            PropertySetter::TSetter setter_;                        ///< \brief Property setter.
            
        };

        /// \brief Functor used to fill a class definition.
        /// Specialize this functor for each class requiring reflection.
        /// \author Raffaele D. Facendola - 2016
        template <typename TClass>
        struct ClassDeclaration;

        template <>
        struct ClassDeclaration<void> {

        public:

            std::unique_ptr<ClassDefinition<void>> operator()() const {

                return std::make_unique<ClassDefinition<void>>("void");
                
            }

        };

        template <>
        struct ClassDeclaration<int> {

        public:

            std::unique_ptr<ClassDefinition<int>> operator()() const {

                return std::make_unique<ClassDefinition<int>>("int");

            }

        };

        template <>
        struct ClassDeclaration<float> {

        public:

            std::unique_ptr<ClassDefinition<float>> operator()() const {

                return std::make_unique<ClassDefinition<float>>("float");

            }

        };

        template <>
        struct ClassDeclaration<bool> {

        public:

            std::unique_ptr<ClassDefinition<bool>> operator()() const {

                return std::make_unique<ClassDefinition<bool>>("bool");

            }

        };

    }

}

namespace syntropy {

    namespace reflection {

        // Implementation

        //////////////// CLASS ////////////////

        template <typename TClass>
        inline static Class& Class::GetClass() {

            // Maps the singleton T, T*, const T and const T* to the same instance of Class<T>

            return GetUnqualifiedClass<drop_t<TClass>>();
            
        }

        template <typename TClass>
        inline static Class& Class::GetUnqualifiedClass() {

            static Class instance(ClassDeclaration<TClass>{}());

            return instance;

        }

        inline const HashedString& Class::GetName() const noexcept {

            return definition_->GetName();

        }
    
        inline const std::vector<Class*>& Class::GetBaseClasses() const noexcept {

            return definition_->GetBaseClasses();

        }

        inline const IFactory* Class::GetFactory() const noexcept {

            return definition_->GetFactory();

        }

        inline const Property* Class::GetProperty(const HashedString& property_name) const noexcept {

            return definition_->GetProperty(property_name);
                
        }

        inline const Method* Class::GetMethod(const HashedString& method_name) const noexcept {

            return definition_->GetMethod(method_name);

        }

        inline const std::unordered_map<size_t, Property>& Class::GetProperties() const noexcept {

            return definition_->GetProperties();

        }

        inline bool Class::IsAbstract() const noexcept {

            return definition_->IsAbstract();

        }
        
        //////////////// CLASS DEFINITION ////////////////
            
        template <typename TClass>
        inline ClassDefinition<TClass>::ClassDefinition(const HashedString& name) noexcept
            : name_(name) {}

        template <typename TClass>
        inline const HashedString& ClassDefinition<TClass>::GetName() const noexcept {

            return name_;

        }
    
        template <typename TClass>
        inline const std::vector<Class*>& ClassDefinition<TClass>::GetBaseClasses() const noexcept {

            return base_classes_;

        }

        template <typename TClass>
        inline const IFactory* ClassDefinition<TClass>::GetFactory() const noexcept {

            return FactoryProvider<TClass>()();
            
        }

        template <typename TClass>
        inline const Property* ClassDefinition<TClass>::GetProperty(const HashedString& property_name) const noexcept {

            auto it = properties_.find(std::hash<HashedString>()(property_name));

            return it != properties_.end() ?
                   std::addressof(it->second) :
                   nullptr;
                
        }

        template <typename TClass>
        inline const Method* ClassDefinition<TClass>::GetMethod(const HashedString& method_name) const noexcept {

            auto it = methods_.find(std::hash<HashedString>()(method_name));

            return it != methods_.end() ?
                   std::addressof(it->second) :
                   nullptr;
        
        }

        template <typename TClass>
        inline const std::unordered_map<size_t, Property>& ClassDefinition<TClass>::GetProperties() const noexcept {

            return properties_;

        }

        template <typename TClass>
        inline bool ClassDefinition<TClass>::IsAbstract() const noexcept {

            return std::is_abstract_v<TClass>;

        }

        template <typename TClass>
        template <typename TBaseClass>
        inline void ClassDefinition<TClass>::DefineBaseClass() noexcept {

            static_assert(std::is_base_of_v<TBaseClass, TClass>, "The class being defined does not derive from TBaseClass");
            static_assert(!std::is_same<TBaseClass, TClass>::value, "A class cannot derive from itself");

            base_classes_.push_back(std::addressof(Class::GetClass<TBaseClass>()));

        }

        template <typename TClass>
        template <typename TProperty>
        void ClassDefinition<TClass>::DefineProperty(const HashedString& name, TProperty&& property) noexcept {

            properties_.emplace(std::make_pair(std::hash<HashedString>()(name),
                                               Property(name, 
                                                        std::forward<TProperty>(property))));

        }

        template <typename TClass>
        template <typename TGetter, typename TSetter>
        void ClassDefinition<TClass>::DefineProperty(const HashedString& name, TGetter&& getter, TSetter&& setter) noexcept {

            properties_.emplace(std::make_pair(std::hash<HashedString>()(name),
                                               Property(name, 
                                                        std::forward<TGetter>(getter),
                                                        std::forward<TSetter>(setter))));

        }

        //////////////// FACTORY ////////////////

        template <typename TClass>
        Factory<TClass>& Factory<TClass>::GetInstance() noexcept{

            static Factory<TClass> factory;

            return factory;

        }

        template <typename TClass>
        Instance Factory<TClass>::Instantiate() const noexcept {

            return wrap_instance(*new TClass());

        }

        //////////////// BASE INSTANCE ////////////////

        template <ConstQualifier kConstQualifier>
        inline BaseInstance<kConstQualifier>::BaseInstance(const BaseInstance<kConstQualifier>& other) noexcept
            : content_(other.content_ ? other.content_->Clone() : nullptr) {}

        template <ConstQualifier kConstQualifier>
        BaseInstance<kConstQualifier>::BaseInstance(std::unique_ptr<IContent> content) noexcept
            : content_(std::move(content)) {}

        template <ConstQualifier kConstQualifier>
        template <typename TInstance>
        inline BaseInstance<kConstQualifier>::BaseInstance(TInstance* instance) noexcept
            : content_(instance ? std::make_unique<Content<qualify_t<TInstance>>>(*instance) : nullptr) {
        
            static_assert(std::is_convertible_v<TInstance*, qualify_t<TInstance>*>, "Conversion loses qualifiers.");

        }

        template <ConstQualifier kConstQualifier>
        inline BaseInstance<kConstQualifier>& BaseInstance<kConstQualifier>::operator=(const BaseInstance<kConstQualifier>& other) noexcept {

            content_ = !other.IsEmpty() ?
                       other.content_->Clone() :
                       nullptr;
            
            return *this;

        }
        
        template <ConstQualifier kConstQualifier>
        inline const Class& BaseInstance<kConstQualifier>::GetClass() const noexcept {

            return content_ ?
                   content_->GetClass() :
                   Class::GetClass<void>();

        }

        template <ConstQualifier kConstQualifier>
        template <typename TInstance>
        inline bool BaseInstance<kConstQualifier>::Is() const noexcept {

            return Class::GetClass<TInstance>().IsBaseOf(GetClass());

        }

        template <ConstQualifier kConstQualifier>
        template <typename TInstance>
        inline TInstance* BaseInstance<kConstQualifier>::As() const noexcept {

            return (content_ && Is<TInstance>()) ?
                   std::addressof(static_cast<Content<qualify_t<TInstance>>*>(content_.get())->content_) :
                   nullptr;

        }

        template <ConstQualifier kConstQualifier>
        inline BaseInstance<kConstQualifier>::operator ConstInstance() const noexcept {

            return ConstInstance(content_->ConstClone());

        }
        
        template <ConstQualifier kConstQualifier>
        inline bool BaseInstance<kConstQualifier>::IsEmpty() const noexcept {

            return !!content_;

        }

        //////////////// INSTANCE :: CONTENT ////////////////

        template <ConstQualifier kConstQualifier>
        template <typename TInstance>
        inline BaseInstance<kConstQualifier>::Content<TInstance>::Content(TInstance& instance) noexcept
            : content_(instance) {}

        template <ConstQualifier kConstQualifier>
        template <typename TInstance>
        inline const Class& BaseInstance<kConstQualifier>::Content<TInstance>::GetClass() const noexcept {

            return Class::GetClass<TInstance>();

        }

        template <ConstQualifier kConstQualifier>
        template <typename TInstance>
        inline std::unique_ptr<typename BaseInstance<kConstQualifier>::IContent> BaseInstance<kConstQualifier>::Content<TInstance>::Clone() const noexcept {

            return std::make_unique<BaseInstance<kConstQualifier>::Content<TInstance>>(content_);       // Preserve qualifiers
            
        }

        //////////////// WRAP INSTANCE ////////////////

        template <typename TInstance>
        inline ConstInstance wrap_const_instance(const TInstance& instance) noexcept {

            return ConstInstance(std::addressof(instance));

        }

        template <ConstQualifier kConstQualifier>
        inline ConstInstance wrap_const_instance(BaseInstance<kConstQualifier> instance) noexcept {

            return instance;

        }
                        
        template <typename TInstance>
        inline Instance wrap_instance(TInstance& instance) noexcept {

            return Instance(std::addressof(instance));

        }

        inline Instance wrap_instance(Instance instance) noexcept {

            return instance;

        }

        //////////////// CLASS PROVIDER ////////////////

        template <typename TClass>
        inline const Class& ClassProvider<TClass>::operator()() const noexcept {

            return Class::GetClass<TClass>();

        }

        //////////////// PROPERTY ////////////////

        template <typename TClass, typename TProperty>
        Property::Property(const HashedString& name, TProperty TClass::* field) noexcept
            : name_(name)
            , class_(std::make_unique<ClassProvider<TProperty>>())
            , getter_(PropertyGetter()(field))
            , setter_(PropertySetter()(field)){}

        template <typename TClass, typename TProperty>
        Property::Property(const HashedString& name, TProperty(TClass::* getter)() const) noexcept
            : name_(name)
            , class_(std::make_unique<ClassProvider<TProperty>>())
            , getter_(PropertyGetter()(getter))
            , setter_(PropertySetter()()) {}

        template <typename TClass, typename TProperty>
        Property::Property(const HashedString& name, TProperty(TClass::* getter)() const, void(TClass::* setter)(TProperty)) noexcept
            : name_(name)
            , class_(std::make_unique<ClassProvider<TProperty>>())
            , getter_(PropertyGetter()(getter))
            , setter_(PropertySetter()(setter)) {}

        template <typename TClass, typename TProperty>
        Property::Property(const HashedString& name, const TProperty& (TClass::* getter)() const, TProperty& (TClass::* setter)()) noexcept
            : name_(name)
            , class_(std::make_unique<ClassProvider<TProperty>>())
            , getter_(PropertyGetter()(getter))
            , setter_(PropertySetter()(setter)) {}

        inline const HashedString& Property::GetName() const noexcept {

            return name_;

        }

        inline const Class& Property::GetClass() const noexcept {

            return (*class_)();

        }
        
        template <typename TInstance, typename TValue>
        inline bool Property::Get(const TInstance& instance, TValue& value) const {

            return getter_(wrap_const_instance(instance),
                           std::addressof(value));

        }

        template <ConstQualifier kConstQualifier, typename TValue>
        inline bool Property::Get(const BaseInstance<kConstQualifier>& instance, TValue& value) const {
    
            return getter_(instance,
                           std::addressof(value));

        }
                
        template <typename TInstance, typename TValue, typename std::enable_if_t<!std::is_same<TInstance, Instance>::value, int>>
        inline bool Property::Set(TInstance& instance, const TValue& value) const {

            return setter_(wrap_instance(instance),
                           std::addressof(value));

        }

        template <typename TValue>
        inline bool Property::Set(const Instance& instance, const TValue& value) const {

            return setter_(instance,
                           std::addressof(value));

        }

    }

}