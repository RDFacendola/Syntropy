
/// \file meta_class.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <memory>
#include <unordered_map>
#include <type_traits>
#include <functional>

#include "any.h"
#include "syntropy.h"
#include "hashed_string.h"

namespace syntropy {

    // Forward declarations

    template <typename TType>
    class MetaClassDefinition;

    class MetaFactory;

    class MetaClass;
        
    class MetaClassProperty;

    class MetaClassMethod;

    class MetaInstance;
    
}

namespace syntropy {

    template <typename TType>
    struct meta_type {

        using type = std::remove_pointer_t<std::remove_cv_t<std::remove_reference_t<TType>>>;

    };

    template <typename TType>
    using meta_type_t = typename meta_type<TType>::type;

    /// \brief Contains the list of all meta class registered to the application.
    /// This class is used to access any metaclass by name.
    /// \author Raffaele D. Facendola - 2016
    class MetaClassRegistry {

        friend class MetaClass;

    public:
        
        SYN_SINGLETON_CLASS(MetaClassRegistry);
        
        /// \brief Get a metaclass instance by class name.
        /// \param class_name Name of the metaclass to get.
        /// \return Returns a pointer to the metaclass whose name is the specified one, if any. Returns nullptr otherwise.
        const MetaClass* GetClass(const HashedString& class_name);

    private:

        /// \brief Register a new metaclass to the metaclass registry.
        /// \param meta_class Metaclass to register.
        void RegisterMetaClass(MetaClass& meta_class);

        /// \brief Private constructor to prevent instantiation.
        MetaClassRegistry();

        std::unordered_map<HashedString, MetaClass*> meta_classes_;       ///< \brief List of metaclasses registered so far.

    };

    /// \brief Contains the declaration of a class type.
    /// \author Raffaele D. Facendola - 2016
    class MetaClassDeclaration {

    public:

        /// \brief Virtual destructor.
        virtual ~MetaClassDeclaration();

        /// \brief Get the name of the metaclass.
        /// \return Returns the type string of the metaclass.
        const HashedString& GetName() const;

        /// \brief Get the list of classes that are derived by this class.
        /// \return Returns the list of classes that are derived by this class.
        const std::vector<MetaClass*>& GetBaseClasses() const;

        /// \brief Get a class property by name.
        /// \param property_name Name of the property to get.
        /// \return Returns a pointer to the requested property, if any. Returns nullptr otherwise.
        const MetaClassProperty* GetProperty(const HashedString& property_name) const;

        /// \brief Get a class method by name.
        /// \param method_name Name of the method to get.
        /// \return Returns a pointer to the requested method if any. Returns nullptr otherwise.
        const MetaClassMethod* GetMethod(const HashedString& method_name) const;

    protected:
        
        MetaClassDeclaration(const HashedString& name);

        template <typename TBaseClass>
        void DefineBaseClass();

        template <typename TClass, typename TProperty>
        void DefineProperty(const HashedString& property_name, TProperty TClass::* property);

        template <typename TClass, typename TProperty>
        void DefineProperty(const HashedString& property_name, TProperty (TClass::* getter)() const, void (TClass::* setter)(TProperty));

        template <typename TClass, typename TProperty>
        void DefineProperty(const HashedString& property_name, TProperty (TClass::* getter)() const);

        template <typename TClass, typename TProperty>
        void DefineProperty(const HashedString& property_name, const TProperty& (TClass::* getter)() const, TProperty& (TClass::* setter)());

        //template <typename TMethod>
        //void DefineMethod(const HashedString& method_name, TMethod&& method);

    private:

        HashedString name_;

        std::vector<MetaClass*> base_classes_;

        std::unordered_map<HashedString, MetaClassProperty> properties_;

        std::unordered_map<HashedString, MetaClassMethod> methods_;

    };

    /// \brief Contains the concrete definition of a class type.
    /// Specialize this template in the same compilation unit where access to the proper meta class is needed.
    /// \author Raffaele D. Facendola - 2016
    template <typename TType>
    class MetaClassDefinition : public MetaClassDeclaration {};

    /// \brief Describes a class type.
    /// \author Raffaele D. Facendola - 2016
    class MetaClass {

    public:

        SYN_NON_COPYABLE_CLASS(MetaClass);

        template <typename TClass>
        static MetaClass& GetClass();

        /// \brief Get the name of the metaclass.
        /// \return Returns the type string of the metaclass.
        const HashedString& GetName() const;
        
        /// \brief Get the list of classes that are derived by this class.
        /// \return Returns the list of classes that are derived by this class.
        const std::vector<MetaClass*>& GetBaseClasses() const;

        /// \brief Get a factory for this metaclass.
        /// \return Returns an factory for this metaclass if applicable. Returns nullptr otherwise.
        const MetaFactory* GetFactory() const;

        /// \brief Get a class property by name.
        /// \param property_name Name of the property to get.
        /// \return Returns a pointer to the requested property, if any. Returns nullptr otherwise.
        const MetaClassProperty* GetProperty(const HashedString& property_name) const;

        /// \brief Get a class method by name.
        /// \param method_name Name of the method to get.
        /// \return Returns a pointer tot he requested method if any. Returns nullptr otherwise.
        const MetaClassMethod* GetMethod(const HashedString& method_name) const;
   
        /// \brief Check whether this meta class is convertible to the specified one.
        /// A class is convertible if it is the same type or derives from another one.
        /// \return Returns true if the class is convertible to the specified type, return false otherwise.
        bool IsConvertibleTo(const MetaClass& other) const;

    private:

        /// \brief Create a new metaclass from class declaration.
        MetaClass(std::unique_ptr<MetaClassDeclaration> declaration);

        size_t class_id_;                                      ///< \brief Unique id of the metaclass.

        std::unique_ptr<MetaClassDeclaration> class_;          ///< \brief Declaration of the described class.

    };

    class MetaFactory {

    public:



    private:

    };
    
    class MetaClassProperty {

    public:

        template <typename TGetter, typename TSetter>
        MetaClassProperty(const HashedString& name, const type_info& type, TGetter&& getter, TSetter&& setter);
        
        const HashedString& GetName() const;

        template <typename TValue>
        bool Read(const MetaInstance& instance, TValue& value) const;

        template <typename TValue>
        bool Write(MetaInstance& instance, const TValue& value) const;

    private:

        HashedString name_;                                             ///< \brief Property name.

        const std::type_info& type_;                                    ///< \brief Type of the property.

        std::function<bool(const MetaInstance&, Any&)> getter_;         ///< \brief Property getter.

        std::function<bool(MetaInstance&, const Any&)> setter_;         ///< \brief Property setter.

    };

    class MetaClassMethod {
    
    public:
        
    };

    class MetaInstance {

    public:

        template <typename TType>
        MetaInstance(TType& instance);

        template <typename TType>
        const TType* As() const;

        template <typename TType>
        TType* As();

    private:

        void* instance_;                            ///< \brief Pointer to the actual object.

        const MetaClass& meta_class_;               ///< \brief Describes the type of the object.

    };

    struct MetaClassPropertyGetter {

        using TGetter = std::function<bool(const MetaInstance&, Any&)>;

        template <typename TClass, typename TProperty>
        TGetter operator() (TProperty TClass::* field) const {

            return[field](const MetaInstance& instance, Any& value) -> bool {

                auto value_ptr = value.As<std::add_pointer_t<std::remove_const_t<std::remove_reference_t<TProperty>>>>();
                auto instance_ptr = instance.As<TClass>();

                if (value_ptr && instance_ptr) {

                    **value_ptr = instance_ptr->*field;

                }

                return !!value_ptr && !!instance_ptr;

            };

        }

        template <typename TClass, typename TProperty>
        TGetter operator() (TProperty(TClass::* getter)() const) const {

            return[getter](const MetaInstance& instance, Any& value) -> bool {

                auto value_ptr = value.As<std::add_pointer_t<std::remove_const_t<std::remove_reference_t<TProperty>>>>();
                auto instance_ptr = instance.As<TClass>();

                if (value_ptr && instance_ptr) {

                    **value_ptr = (instance_ptr->*getter)();

                }

                return !!value_ptr && !!instance_ptr;

            };

        }

    };

    template <typename TType, bool kReadOnly = std::is_const<TType>::value >
    struct MetaClassPropertySetter {};

    template <typename TType>
    struct MetaClassPropertySetter<TType, true> {

        using TSetter = std::function<bool(MetaInstance&, const Any&)>;

        template <typename TAny>
        TSetter operator() (TAny) const {

            return [](MetaInstance&, const Any&) -> bool{

                return false;       // Readonly

            };

        }

    };

    template <typename TType>
    struct MetaClassPropertySetter<TType, false> {

        using TSetter = std::function<bool(MetaInstance&, const Any&)>;

        template <typename TClass, typename TProperty>
        TSetter operator() (TProperty TClass::* property) const {

            return[property](MetaInstance& instance, const Any& value) -> bool{

                auto value_ptr = value.As<std::add_pointer_t<std::add_const_t<TProperty>>>();
                auto instance_ptr = instance.As<TClass>();

                if (value_ptr && instance_ptr) {

                    instance_ptr->*property = **value_ptr;

                }

                return !!value_ptr && !!instance_ptr;

            };

        }
        
        template <typename TClass, typename TProperty>
        TSetter operator() (void (TClass::* setter)(TProperty)) const {

            return[setter](MetaInstance& instance, const Any& value) -> bool {

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

            return[setter](MetaInstance& instance, const Any& value) -> bool {

                auto value_ptr = value.As<std::add_pointer_t<std::add_const_t<TProperty>>>();
                auto instance_ptr = instance.As<TClass>();

                if (value_ptr && instance_ptr) {

                    (instance_ptr->*setter)() = **value_ptr;

                }

                return !!value_ptr && !!instance_ptr;

            };

        }

    };

    //////////////// META CLASS ////////////////

    template <typename TClass>
    inline static MetaClass& MetaClass::GetClass() {

        static MetaClass meta_class(std::make_unique<MetaClassDefinition<TClass>>());

        return meta_class;

    }
        
    inline const HashedString& MetaClass::GetName() const {

        return class_->GetName();

    }
        
    inline const std::vector<MetaClass*>& MetaClass::GetBaseClasses() const {

        return class_->GetBaseClasses();

    }

    inline const MetaFactory* MetaClass::GetFactory() const {

        return nullptr;

    }

    inline const MetaClassProperty* MetaClass::GetProperty(const HashedString& property_name) const {

        return class_->GetProperty(property_name);

    }
        
    inline const MetaClassMethod* MetaClass::GetMethod(const HashedString& method_name) const {

        return class_->GetMethod(method_name);

    }

    inline bool MetaClass::IsConvertibleTo(const MetaClass& other) const {

        return class_id_ % other.class_id_ == 0;

    }

    //////////////// META CLASS DECLARATION ////////////////

    inline MetaClassDeclaration::~MetaClassDeclaration() {}

    inline MetaClassDeclaration::MetaClassDeclaration(const HashedString& name)
        : name_(name) {}

    inline const HashedString& MetaClassDeclaration::GetName() const {

        return name_;

    }
    
    inline const std::vector<MetaClass*>& MetaClassDeclaration::GetBaseClasses() const {

        return base_classes_;

    }

    inline const MetaClassProperty* MetaClassDeclaration::GetProperty(const HashedString& property_name) const {

        auto it = properties_.find(property_name);

        return it != properties_.end() ?
               std::addressof(it->second) :
               nullptr;
                
    }

    inline const MetaClassMethod* MetaClassDeclaration::GetMethod(const HashedString& method_name) const {

        auto it = methods_.find(method_name);

        return it != methods_.end() ?
               std::addressof(it->second) :
               nullptr;
        
    }

    template <typename TBaseClass>
    inline void MetaClassDeclaration::DefineBaseClass() {

        // TODO: Check for unrelated class types.

        base_classes_.push_back(std::addressof(MetaClass::GetClass<TBaseClass>()));

    }

    template <typename TClass, typename TProperty>
    void MetaClassDeclaration::DefineProperty(const HashedString& property_name, TProperty TClass::* property) {

        properties_.insert(std::make_pair(property_name,
                                          MetaClassProperty(property_name,
                                                            typeid(TProperty),
                                                            MetaClassPropertyGetter{}(property),
                                                            MetaClassPropertySetter<TProperty>{}(property))));

    }
    
    template <typename TClass, typename TProperty>
    void MetaClassDeclaration::DefineProperty(const HashedString& property_name, TProperty(TClass::* getter)() const, void (TClass::* setter)(TProperty)) {

        properties_.insert(std::make_pair(property_name,
                                          MetaClassProperty(property_name,
                                                            typeid(TProperty),
                                                            MetaClassPropertyGetter{}(getter),
                                                            MetaClassPropertySetter<TProperty>{}(setter))));

    }

    template <typename TClass, typename TProperty>
    void MetaClassDeclaration::DefineProperty(const HashedString& property_name, TProperty(TClass::* getter)() const) {

        properties_.insert(std::make_pair(property_name,
                                          MetaClassProperty(property_name,
                                                            typeid(TProperty),
                                                            MetaClassPropertyGetter{}(getter),
                                                            MetaClassPropertySetter<TProperty, true>{}(nullptr))));

    }

    template <typename TClass, typename TProperty>
    void MetaClassDeclaration::DefineProperty(const HashedString& property_name, const TProperty& (TClass::* getter)() const, TProperty& (TClass::* setter)()) {

        properties_.insert(std::make_pair(property_name,
                                          MetaClassProperty(property_name,
                                                            typeid(TProperty),
                                                            MetaClassPropertyGetter{}(getter),
                                                            MetaClassPropertySetter<TProperty>{}(setter))));

    }

    //////////////// META CLASS PROPERTY ////////////////

    template <typename TGetter, typename TSetter>
    inline MetaClassProperty::MetaClassProperty(const HashedString& name, const type_info& type, TGetter&& getter, TSetter&& setter)
        : name_(name)
        , type_(type)
        , getter_(std::forward<TGetter>(getter))
        , setter_(std::forward<TSetter>(setter)){}

    inline const HashedString& MetaClassProperty::GetName() const {

        return name_;

    }

    template <typename TValue>
    inline bool MetaClassProperty::Read(const MetaInstance& instance, TValue& value) const {

        Any ptr_value(std::addressof(value));

        return getter_(instance, ptr_value);

    }

    template <typename TValue>
    inline bool MetaClassProperty::Write(MetaInstance& instance, const TValue& value) const {

        return setter_(instance, std::addressof(value));

    }

    //////////////// META INSTANCE ////////////////

    template <typename TType>
    inline MetaInstance::MetaInstance(TType& instance)
        : instance_(std::addressof(instance))
        , meta_class_(MetaClass::GetClass<meta_type_t<TType>>() ){}

    template <typename TType>
    inline const TType* MetaInstance::As() const {

        return meta_class_.IsConvertibleTo(MetaClass::GetClass<meta_type_t<TType>>()) ?
               reinterpret_cast<const TType*>(instance_) :
               nullptr;

    }

    template <typename TType>
    inline TType* MetaInstance::As() {

        return meta_class_.IsConvertibleTo(MetaClass::GetClass<meta_type_t<TType>>()) ?
               reinterpret_cast<TType*>(instance_) :
               nullptr;

    }

}