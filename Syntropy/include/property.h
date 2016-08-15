
/// \file property.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "hashed_string.h"

#include "instance.h"
#include "utils.h"

#include "any.h"

namespace syntropy {

    namespace reflection {

        class Class;
        class Property;
        class Method;

        struct IClassProvider;
        
        template <typename TClass>
        struct ClassProvider;

        class Instance;

    }

}

namespace syntropy {

    namespace reflection {

        struct PropertyGetter {

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

        struct PropertySetter {

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

            static_assert(!std::is_const<TValue>::value, "The value must be a modifiable lvalue");

            return getter_(std::addressof(instance),
                           std::addressof(value));

        }

        template <typename TInstance, typename TValue>
        bool Property::Set(TInstance& instance, const TValue& value) const {

            static_assert(!std::is_const<TInstance>::value, "The instance must be a modifiable lvalue");

            return setter_(std::addressof(instance),
                           std::addressof(value));


        }

    }

}

namespace std {

    /// \brief Custom template specialization of std::hash for syntropy::reflection::Property.
    /// \author Raffaele D. Facendola - 2016
    template <>
    struct hash<syntropy::reflection::Property> {

        using result_type = size_t;

        using argument_type = syntropy::reflection::Property;

        result_type operator()(const argument_type& argument) {

            return argument.GetName().GetHash();

        }

    };

}