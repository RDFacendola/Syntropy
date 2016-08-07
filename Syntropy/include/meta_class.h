
/// \file meta_class.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <memory>
#include <unordered_map>
#include <type_traits>
#include <functional>
#include <sstream>

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

        /// \brief Get the class properties list.
        /// \return Returns the class properties list.
        const std::unordered_map<HashedString, MetaClassProperty>& GetProperties() const;

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
   
        /// \brief Get the class properties list.
        /// \return Returns the class properties list.
        const std::unordered_map<HashedString, MetaClassProperty>& GetProperties() const;

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
    

    struct MetaClassPropertyGetter {

        using TGetter = std::function<bool(Any, Any)>;

        template <typename TClass, typename TProperty>
        TGetter operator() (TProperty TClass::* field) const {

            return[field](Any instance, Any value) -> bool {

                auto value_ptr = value.As<std::add_pointer_t<std::remove_const_t<std::remove_reference_t<TProperty>>>>();
                auto instance_ptr = instance.As<std::add_pointer_t<std::add_const_t<TClass>>>();

                if (value_ptr && instance_ptr) {

                    **value_ptr = (*instance_ptr)->*field;

                }

                return !!value_ptr && !!instance_ptr;

            };

        }

        template <typename TClass, typename TProperty>
        TGetter operator() (TProperty(TClass::* getter)() const) const {

            return[getter](Any instance, Any value) -> bool {

                auto value_ptr = value.As<std::add_pointer_t<std::remove_const_t<std::remove_reference_t<TProperty>>>>();
                auto instance_ptr = instance.As<std::add_pointer_t<std::add_const_t<TClass>>>();

                if (value_ptr && instance_ptr) {

                    **value_ptr = ((*instance_ptr)->*getter)();

                }

                return !!value_ptr && !!instance_ptr;

            };

        }

    };

    struct MetaClassPropertySetter {

        using TSetter = std::function<bool(Any, Any)>;

        TSetter operator() () const {

            return[](Any, Any) -> bool {

                return false;

            };

        }

        template <typename TClass, typename TProperty>
        TSetter operator() (TProperty TClass::* property, typename std::enable_if_t<!std::is_const<TProperty>::value>* = nullptr) const {

            return[property](Any instance, Any value) -> bool{

                auto value_ptr = value.As<std::add_pointer_t<std::add_const_t<TProperty>>>();
                auto instance_ptr = instance.As<std::add_pointer_t<TClass>>();

                if (value_ptr && instance_ptr) {

                    (*instance_ptr)->*property = **value_ptr;

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

            return[setter](Any instance, Any value) -> bool {

                auto value_ptr = value.As<std::add_pointer_t<std::add_const_t<TProperty>>>();
                auto instance_ptr = instance.As<std::add_pointer_t<TClass>>();

                if (value_ptr && instance_ptr) {

                    ((*instance_ptr)->*setter)(**value_ptr);

                }

                return !!value_ptr && !!instance_ptr;

            };

        }

        template <typename TClass, typename TProperty>
        TSetter operator() (TProperty& (TClass::* setter)()) const {

            return[setter](Any instance, Any value) -> bool {

                auto value_ptr = value.As<std::add_pointer_t<std::add_const_t<TProperty>>>();
                auto instance_ptr = instance.As<std::add_pointer_t<TClass>>();

                if (value_ptr && instance_ptr) {

                    ((*instance_ptr)->*setter)() = **value_ptr;

                }

                return !!value_ptr && !!instance_ptr;

            };

        }

    };

    struct MetaClassPropertyParser {

        using TParser = std::function<bool(Any, std::stringstream&)>;

        template <typename TProperty>
        struct is_parseable : std::integral_constant<bool,
                                                     !std::is_const<TProperty>::value &&
                                                     !std::is_pointer<TProperty>::value> {};

        TParser operator() () const {

            return[](Any, std::stringstream&) -> bool {

                return false;

            };

        }

        template <typename TClass, typename TProperty>
        TParser operator() (TProperty TClass::* property, typename std::enable_if_t<is_parseable<TProperty>::value>* = nullptr) const {

            return[property](Any instance, std::stringstream& sstream) -> bool{

                auto instance_ptr = instance.As<std::add_pointer_t<TClass>>();

                if (instance_ptr) {

                    sstream >> ((*instance_ptr)->*property);

                }

                return instance_ptr &&
                       !sstream.fail();

            };

        }

        template <typename TClass, typename TProperty>
        TParser operator() (TProperty TClass::*, typename std::enable_if_t<!is_parseable<TProperty>::value>* = nullptr) const {

            return (*this)();

        }

        template <typename TClass, typename TProperty>
        TParser operator() (void (TClass::* setter)(TProperty), typename std::enable_if_t<is_parseable<TProperty>::value>* = nullptr) const {

            return[setter](Any instance, std::stringstream& sstream) -> bool {

                auto instance_ptr = instance.As<std::add_pointer_t<TClass>>();

                if (instance_ptr) {

                    std::decay_t<TProperty> property_value;

                    sstream >> property_value;

                    if (!sstream.fail()) {

                        ((*instance_ptr)->*setter)(property_value);

                    }

                }

                return instance_ptr &&
                       !sstream.fail();

            };

        }

        template <typename TClass, typename TProperty>
        TParser operator() (void (TClass::*)(TProperty), typename std::enable_if_t<!is_parseable<TProperty>::value>* = nullptr) const {

            return (*this)();

        }
        
        template <typename TClass, typename TProperty>
        TParser operator() (TProperty& (TClass::* setter)(), typename std::enable_if_t<is_parseable<TProperty>::value>* = nullptr) const {

            return[setter](Any instance, std::stringstream& sstream) -> bool {

                auto instance_ptr = instance.As<std::add_pointer_t<TClass>>();

                if (instance_ptr) {

                    sstream >> ((*instance_ptr)->*setter)();

                }

                return instance_ptr &&
                       !sstream.fail();

            };

        }

        template <typename TClass, typename TProperty>
        TParser operator() (TProperty& (TClass::*)(), typename std::enable_if_t<!is_parseable<TProperty>::value>* = nullptr) const {

            return (*this)();

        }
        
    };

    class MetaClassProperty {

    public:

        template <typename TGetter, typename TSetter, typename TParser>
        MetaClassProperty(const type_info& type, TGetter&& getter, TSetter&& setter, TParser&& parser);
        
        /// \brief Get the property type.
        /// \return Returns the property type.
        const std::type_info& GetType() const;

        template <typename TInstance, typename TValue>
        bool Read(const TInstance& instance, TValue& value) const;

        template <typename TInstance, typename TValue>
        bool Write(TInstance& instance, const TValue& value) const;

        template <typename TInstance>
        bool Parse(TInstance& instance, const std::string& string) const;

        template <typename TInstance>
        bool Parse(TInstance& instance, const std::string& string, std::ios_base::fmtflags flags) const;

        template <typename TInstance, typename TValue>
        bool Interpret(TInstance& instance, const TValue& value) const;
       
        template <typename TInstance, typename TValue>
        bool Interpret(TInstance& instance, const TValue& value, std::ios_base::fmtflags flags) const;

    private:

        HashedString name_;                                     ///< \brief Property name.

        const std::type_info& type_;                            ///< \brief Type of the property.

        MetaClassPropertyGetter::TGetter getter_;               ///< \brief Property getter.

        MetaClassPropertySetter::TSetter setter_;               ///< \brief Property setter.

        MetaClassPropertyParser::TParser parser_;               ///< \brief Property parser.

    };

    class MetaClassMethod {
    
    public:
        
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

    inline const std::unordered_map<HashedString, MetaClassProperty>& MetaClass::GetProperties() const {

        return class_->GetProperties();

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

    inline const std::unordered_map<HashedString, MetaClassProperty>& MetaClassDeclaration::GetProperties() const {

        return properties_;

    }

    template <typename TBaseClass>
    inline void MetaClassDeclaration::DefineBaseClass() {

        // TODO: Check for unrelated class types.

        base_classes_.push_back(std::addressof(MetaClass::GetClass<TBaseClass>()));

    }

    template <typename TClass, typename TProperty>
    void MetaClassDeclaration::DefineProperty(const HashedString& property_name, TProperty TClass::* property) {

        properties_.insert(std::make_pair(property_name,
                                          MetaClassProperty(typeid(TProperty),
                                                            MetaClassPropertyGetter{}(property),
                                                            MetaClassPropertySetter{}(property),
                                                            MetaClassPropertyParser{}(property))));

    }
    
    template <typename TClass, typename TProperty>
    void MetaClassDeclaration::DefineProperty(const HashedString& property_name, TProperty(TClass::* getter)() const, void (TClass::* setter)(TProperty)) {

        properties_.insert(std::make_pair(property_name,
                                          MetaClassProperty(typeid(TProperty),
                                                            MetaClassPropertyGetter{}(getter),
                                                            MetaClassPropertySetter{}(setter),
                                                            MetaClassPropertyParser{}(setter))));

    }

    template <typename TClass, typename TProperty>
    void MetaClassDeclaration::DefineProperty(const HashedString& property_name, TProperty(TClass::* getter)() const) {

        properties_.insert(std::make_pair(property_name,
                                          MetaClassProperty(typeid(TProperty),
                                                            MetaClassPropertyGetter{}(getter),
                                                            MetaClassPropertySetter{}(),
                                                            MetaClassPropertyParser{}())));

    }

    template <typename TClass, typename TProperty>
    void MetaClassDeclaration::DefineProperty(const HashedString& property_name, const TProperty& (TClass::* getter)() const, TProperty& (TClass::* setter)()) {

        properties_.insert(std::make_pair(property_name,
                                          MetaClassProperty(typeid(TProperty),
                                                            MetaClassPropertyGetter{}(getter),
                                                            MetaClassPropertySetter{}(setter),
                                                            MetaClassPropertyParser{}(setter))));

    }

    //////////////// META CLASS PROPERTY ////////////////

    template <typename TGetter, typename TSetter, typename TParser>
    inline MetaClassProperty::MetaClassProperty(const type_info& type, TGetter&& getter, TSetter&& setter, TParser&& parser)
        : type_(type)
        , getter_(std::forward<TGetter>(getter))
        , setter_(std::forward<TSetter>(setter))
        , parser_(std::forward<TParser>(parser)){}

    inline const std::type_info& MetaClassProperty::GetType() const {

        return type_;

    }

    template <typename TInstance, typename TValue>
    inline bool MetaClassProperty::Read(const TInstance& instance, TValue& value) const {

        static_assert(!std::is_const<TValue>::value, "The value must be a modifiable lvalue");

        return getter_(std::addressof(instance), 
                       std::addressof(value));

    }

    template <typename TInstance, typename TValue>
    inline bool MetaClassProperty::Write(TInstance& instance, const TValue& value) const {

        static_assert(!std::is_const<TInstance>::value, "The instance must be a modifiable lvalue");

        return setter_(std::addressof(instance), 
                       std::addressof(value));

    }

    template <typename TInstance>
    inline bool MetaClassProperty::Parse(TInstance& instance, const std::string& string) const {

        std::stringstream sstream(string);

        return parser_(std::addressof(instance),
                       sstream);

    }

    template <typename TInstance>
    inline bool MetaClassProperty::Parse(TInstance& instance, const std::string& string, std::ios_base::fmtflags flags) const {

        std::stringstream sstream(string);
        
        sstream.setf(flags);

        return parser_(std::addressof(instance),
                       sstream);

    }

    template <typename TInstance, typename TValue>
    inline bool MetaClassProperty::Interpret(TInstance& instance, const TValue& value) const {

        std::stringstream sstream;

        sstream << value;

        return parser_(std::addressof(instance),
                       sstream);

    }

    template <typename TInstance, typename TValue>
    inline bool MetaClassProperty::Interpret(TInstance& instance, const TValue& value, std::ios_base::fmtflags flags) const {

        std::stringstream sstream;

        sstream.setf(flags);

        sstream << value;

        return parser_(std::addressof(instance),
                       sstream);

    }


}