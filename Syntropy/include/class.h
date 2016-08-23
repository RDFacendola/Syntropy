
/// \file class.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <unordered_map>
#include <memory>
#include <type_traits>

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

        class Instance;
        class Property;
        struct IClassProvider;
        
    }

}

namespace syntropy {

    namespace reflection {

        template <typename TClass>
        constexpr bool is_instantiable_v = !std::is_abstract_v<TClass> &&
                                            std::is_nothrow_default_constructible_v<TClass>;

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

                return std::addressof(Factory<typename std::remove_cv_t<TClass>>::GetInstance());

            }

        };

        /// \brief Describes a polymorphic type-safe container for single values of any type.
        /// \author Raffaele D. Facendola, based on "Valued Conversion" by Kevlin Henney
        class Instance {

        public:

            /// \brief Create an empty instance.
            Instance() = default;

            /// \brief Destructor.
            ~Instance() = default;

            /// \brief Copy constructor.
            /// \param other Instance to copy.
            Instance(const Instance& other) noexcept;

            /// \brief Move constructor
            /// \param other Instance to move.
            Instance(Instance&& other) noexcept;

            /// \brief Wraps an object to be referenced via Instance.
            /// \param instance Object to reference.
            template <typename TInstance>
            Instance(TInstance* instance) noexcept;

            /// \brief Unified assignment operator.
            /// \param other Instance to assign from.
            Instance& operator=(Instance other) noexcept;

            /// \brief Wraps a new object via this instance.
            /// \param other Object to reference via this instance.
            template <typename TInstance>
            Instance& operator=(TInstance& other) noexcept;

            /// \brief Check whether the type of the object being referenced derives from the provided class.
            /// \tparam TInstance Type of class to check against.
            /// \return Returns true if the wrapped object's type derives from TInstance or is exactly TInstance. returns false otherwise.
            template <typename TInstance>
            bool Is() const noexcept;

            /// \brief Get a typed pointer to the wrapped object.
            /// \tparam TInstance Type of class to check against.
            /// \return Returns a pointer to the contained object if the underlying type derives from TInstance. Returns nullptr instead.
            template <typename TInstance>
            const TInstance* As() const noexcept;

            /// \brief Get a typed pointer to the wrapped object.
            /// \tparam TInstance Type of class to check against.
            /// \return Returns a pointer to the contained object if the underlying type derives from TInstance. Returns nullptr instead.
            template <typename TInstance>
            TInstance* As() noexcept;

            bool IsEmpty() const noexcept;

            /// \brief Get the underlying type of the contained value.
            /// \return Returns the type of the contained value.
            const Class& GetClass() const noexcept;

            /// \brief Swaps two instances.
            /// \param other Object to swap with the current instance.
            Instance& swap(Instance& other) noexcept;

        private:

            /// \brief Base interface for the underlying content.
            struct IContent {

                /// \brief Virtual destructor.
                virtual ~IContent() = default;

                /// \brief Get the class of the instance being referenced.
                /// \return Returns the class of the instance being referenced.
                virtual const Class& GetClass() const noexcept = 0;

                /// \brief Clone the underlying value to another instance.
                /// \return Returns a pointer to the new copy of the value.
                virtual std::unique_ptr<IContent> Clone() const noexcept = 0;

            };

            /// \brief Strongly typed container for a single value.
            /// \tparam TValue Type of the contained value.
            template <typename TInstance>
            struct Content : public IContent {

                /// \brief Create a new container for a single value.
                ///\ param value Value to store inside the container.
                Content(TInstance* instance) noexcept;

                virtual const Class& GetClass() const noexcept override;

                virtual std::unique_ptr<IContent> Clone() const noexcept override;

                TInstance* content_;                    ///< \brief Actual content.

            };

            std::unique_ptr<IContent> content_;         ///< \brief Wraps the actual value.

        };

        template <typename TInstance>
        Instance wrap_instance(const TInstance& instance);

        template <typename TInstance, typename std::enable_if_t<!std::is_const<TInstance>::value>>
        Instance wrap_instance(TInstance& instance);

        Instance wrap_instance(Instance instance);

        struct PropertyGetter {

            using TGetter = std::function<bool(Instance, Any)>;

            template <typename TClass, typename TProperty>
            TGetter operator() (TProperty TClass::* field) const {

                return[field](Instance instance, Any value) -> bool {

                    auto value_ptr = value.As<std::add_pointer_t<std::remove_const_t<std::remove_reference_t<TProperty>>>>();
                    auto instance_ptr = instance.As<std::add_const_t<TClass>>();

                    if (value_ptr && instance_ptr) {

                        **value_ptr = instance_ptr->*field;

                    }

                    return !!value_ptr && !!instance_ptr;

                };

            }

            template <typename TClass, typename TProperty>
            TGetter operator() (TProperty(TClass::* getter)() const) const {

                return[getter](Instance instance, Any value) -> bool {

                    auto value_ptr = value.As<std::add_pointer_t<std::remove_const_t<std::remove_reference_t<TProperty>>>>();
                    auto instance_ptr = instance.As<std::add_const_t<TClass>>();

                    if (value_ptr && instance_ptr) {

                        **value_ptr = (instance_ptr->*getter)();

                    }

                    return !!value_ptr && !!instance_ptr;

                };

            }

        };

        struct PropertySetter {

            using TSetter = std::function<bool(Instance, Any)>;

            TSetter operator() () const {

                return[](Instance, Any) -> bool {

                    return false;

                };

            }

            template <typename TClass, typename TProperty>
            TSetter operator() (TProperty TClass::* property, typename std::enable_if_t<!std::is_const<TProperty>::value>* = nullptr) const {

                return[property](Instance instance, Any value) -> bool{

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

                return[setter](Instance instance, Any value) -> bool {

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

                return[setter](Instance instance, Any value) -> bool {

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

            template <typename TInstance, typename TValue>
            bool Set(TInstance& instance, const TValue& value) const;

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

            return Instance(new TClass());

        }

        //////////////// INSTANCE ////////////////

        inline Instance::Instance(const Instance& other) noexcept :
            content_(other.content_ ? other.content_->Clone() : nullptr) {}

        inline Instance::Instance(Instance&& other) noexcept :
            content_(std::move(other.content_)) {}

        template <typename TInstance>
        inline Instance::Instance(TInstance* instance) noexcept :
            content_(std::make_unique<Content<TInstance>>(instance)) {}

        inline Instance& Instance::operator=(Instance other) noexcept {

            return Instance(other).swap(*this);

        }

        template <typename TInstance>
        inline Instance& Instance::operator=(TInstance& other) noexcept {

            return Instance(other).swap(*this);

        }

        inline const Class& Instance::GetClass() const noexcept {

            return content_ ?
                   content_->GetClass() :
                   Class::GetClass<void>();

        }

        template <typename TInstance>
        inline bool Instance::Is() const noexcept {

            return Class::GetClass<TInstance>().IsBaseOf(GetClass());

        }

        template <typename TInstance>
        inline const TInstance* Instance::As() const noexcept {

            return (content_ && Is<TInstance>()) ?
                   static_cast<Content<TInstance>*>(content_.get())->content_ :
                   nullptr;

        }

        template <typename TInstance>
        inline TInstance* Instance::As() noexcept {

            return (content_ && Is<TInstance>()) ?
                    static_cast<Content<TInstance>*>(content_.get())->content_ :
                    nullptr;

        }

        inline bool Instance::IsEmpty() const noexcept {

            return !!content_;

        }

        inline Instance& Instance::swap(Instance& other) noexcept {

            std::swap(content_, other.content_);

            return *this;

        }

        //////////////// INSTANCE :: CONTENT ////////////////

        template <typename TInstance>
        inline Instance::Content<TInstance>::Content(TInstance* instance) noexcept
            : content_(instance) {}

        template <typename TInstance>
        inline const Class& Instance::Content<TInstance>::GetClass() const noexcept {

            return Class::GetClass<TInstance>();

        }

        template <typename TInstance>
        inline std::unique_ptr<Instance::IContent> Instance::Content<TInstance>::Clone() const noexcept {

            return std::make_unique<Content<TInstance>>(content_);

        }

        //////////////// INSTANCE ////////////////

        template <typename TInstance>
        inline Instance wrap_instance(const TInstance& instance) {

            return Instance(std::addressof(instance));

        }

        template <typename TInstance, typename std::enable_if_t<!std::is_const<TInstance>::value>>
        inline Instance wrap_instance(TInstance& instance) {

            return Instance(std::addressof(instance));

        }

        inline Instance wrap_instance(Instance instance) {

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
        bool Property::Get(const TInstance& instance, TValue& value) const {

            static_assert(!std::is_const<TValue>::value, "TValue must be a modifiable lvalue");

            return getter_(wrap_instance(instance),
                           std::addressof(value));

        }

        template <typename TInstance, typename TValue>
        bool Property::Set(TInstance& instance, const TValue& value) const {

            return setter_(wrap_instance(instance),
                           std::addressof(value));

        }

    }

}