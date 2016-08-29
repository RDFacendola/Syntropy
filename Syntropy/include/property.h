
/// \file property.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <functional>

#include "hashed_string.h"
#include "type_traits.h"

#include "type.h"
#include "any.h"
#include "any_reference.h"

namespace syntropy {

    namespace reflection {

        struct PropertyGetter {

            using TInstance = typename AnyReferenceWrapper<ConstQualifier::kConst, typename Type>;
            
            using TGetter = std::function<bool(const TInstance&, Any)>;

            template <typename TClass, typename TProperty>
            TGetter operator() (TProperty TClass::* field) const {

                return[field](const TInstance& instance, Any value) -> bool {

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

                return[getter](const TInstance& instance, Any value) -> bool {

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

            using TInstance = typename AnyReferenceWrapper<ConstQualifier::kNone, typename Type>;

            using TSetter = std::function<bool(const TInstance&, Any)>;

            TSetter operator() () const {

                return[](const TInstance&, Any) -> bool {

                    return false;

                };

            }

            template <typename TClass, typename TProperty>
            TSetter operator() (TProperty TClass::* property, typename std::enable_if_t<!std::is_const<TProperty>::value>* = nullptr) const {

                return[property](const TInstance& instance, Any value) -> bool{

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

                return[setter](const TInstance& instance, Any value) -> bool {

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

                return[setter](const TInstance& instance, Any value) -> bool {

                    auto value_ptr = value.As<std::add_pointer_t<std::add_const_t<TProperty>>>();
                    auto instance_ptr = instance.As<TClass>();

                    if (value_ptr && instance_ptr) {

                        (instance_ptr->*setter)() = **value_ptr;

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

            const typename Type& GetType() const noexcept;

            template <typename TInstance, typename TValue>
            bool Get(const TInstance& instance, TValue& value) const;

            template <ConstQualifier kQualifier, typename TValue>
            bool Get(AnyReferenceWrapper<kQualifier, typename Type> instance, TValue& value) const;
            
            template <typename TInstance, typename TValue>
            bool Set(TInstance& instance, const TValue& value) const;

            template <typename TValue>
            bool Set(AnyReferenceWrapper<ConstQualifier::kNone, Type> instance, const TValue& value) const;

            template <typename TValue>
            bool Set(AnyReferenceWrapper<ConstQualifier::kConst, Type> instance, const TValue& value) const = delete;

            template <typename TInstance, typename TValue>
            bool Set(TInstance&& instance, const TValue& value) const = delete;

        private:
            
            HashedString name_;                                     ///< \brief Property name.

            const Type& type_;                                      ///< \brief Property type.

            typename PropertyGetter::TGetter getter_;               ///< \brief Property getter.

            typename PropertySetter::TSetter setter_;               ///< \brief Property setter.
            
        };

    }

}

namespace syntropy {

    namespace reflection {

        //////////////// PROPERTY ////////////////

        template <typename TClass, typename TProperty>
        Property::Property(const HashedString& name, TProperty TClass::* field) noexcept
            : name_(name)
            , type_(type_get<Type, TProperty>()())
            , getter_(PropertyGetter()(field))
            , setter_(PropertySetter()(field)){}

        template <typename TClass, typename TProperty>
        Property::Property(const HashedString& name, TProperty(TClass::* getter)() const) noexcept
            : name_(name)
            , type_(type_get<Type, TProperty>()())
            , getter_(PropertyGetter()(getter))
            , setter_(PropertySetter()()) {}

        template <typename TClass, typename TProperty>
        Property::Property(const HashedString& name, TProperty(TClass::* getter)() const, void(TClass::* setter)(TProperty)) noexcept
            : name_(name)
            , type_(type_get<Type, TProperty>()())
            , getter_(PropertyGetter()(getter))
            , setter_(PropertySetter()(setter)) {}

        template <typename TClass, typename TProperty>
        Property::Property(const HashedString& name, const TProperty& (TClass::* getter)() const, TProperty& (TClass::* setter)()) noexcept
            : name_(name)
            , type_(type_get<Type, TProperty>()())
            , getter_(PropertyGetter()(getter))
            , setter_(PropertySetter()(setter)) {}

        inline const HashedString& Property::GetName() const noexcept {

            return name_;

        }

        inline const typename Type& Property::GetType() const noexcept {

            return type_;

        }

        template <typename TInstance, typename TValue>
        inline bool Property::Get(const TInstance& instance, TValue& value) const {

            return getter_(std::addressof(instance),
                           std::addressof(value));

        }

        template <ConstQualifier kQualifier, typename TValue>
        bool Property::Get(AnyReferenceWrapper<kQualifier, Type> instance, TValue& value) const {

            return getter_(instance,
                           std::addressof(value));

        }

        template <typename TInstance, typename TValue>
        inline bool Property::Set(TInstance& instance, const TValue& value) const {

            return setter_(std::addressof(instance),
                           std::addressof(value));

        }

        template <typename TValue>
        bool Property::Set(AnyReferenceWrapper<ConstQualifier::kNone, Type> instance, const TValue& value) const {

            return setter_(instance,
                           std::addressof(value));

        }

    }

}