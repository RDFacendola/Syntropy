
/// \file property.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <functional>

#include "hashed_string.h"
#include "any_reference.h"
#include "type_traits.h"

#include "any.h"
#include "type.h"

namespace syntropy {

    namespace reflection {

        template <typename TClass>
        struct PropertyGetter {

            using TInstance = typename AnyReferenceWrapper<ConstQualifier::kConst, typename BasicType<TClass>>;
            
            using TGetter = std::function<bool(const TInstance&, Any)>;

            template <typename TSubject, typename TProperty>
            TGetter operator() (TProperty TSubject::* field) const {

                return[field](const TInstance& instance, Any value) -> bool {

                    auto value_ptr = value.As<std::add_pointer_t<std::remove_const_t<std::remove_reference_t<TProperty>>>>();
                    auto instance_ptr = instance.As<const TSubject>();

                    if (value_ptr && instance_ptr) {

                        **value_ptr = instance_ptr->*field;

                    }

                    return !!value_ptr && !!instance_ptr;

                };

            }

            template <typename TSubject, typename TProperty>
            TGetter operator() (TProperty(TSubject::* getter)() const) const {

                return[getter](const TInstance& instance, Any value) -> bool {

                    auto value_ptr = value.As<std::add_pointer_t<std::remove_const_t<std::remove_reference_t<TProperty>>>>();
                    auto instance_ptr = instance.As<const TSubject>();

                    if (value_ptr && instance_ptr) {

                        **value_ptr = (instance_ptr->*getter)();

                    }

                    return !!value_ptr && !!instance_ptr;

                };

            }

        };

        template <typename TClass>
        struct PropertySetter {

            using TInstance = typename AnyReferenceWrapper<ConstQualifier::kNone, typename BasicType<TClass>>;

            using TSetter = std::function<bool(const TInstance&, Any)>;

            TSetter operator() () const {

                return[](const TInstance&, Any) -> bool {

                    return false;

                };

            }

            template <typename TSubject, typename TProperty>
            TSetter operator() (TProperty TSubject::* property, typename std::enable_if_t<!std::is_const<TProperty>::value>* = nullptr) const {

                return[property](const TInstance& instance, Any value) -> bool{

                    auto value_ptr = value.As<std::add_pointer_t<std::add_const_t<TProperty>>>();
                    auto instance_ptr = instance.As<TSubject>();

                    if (value_ptr && instance_ptr) {

                        instance_ptr->*property = **value_ptr;

                    }

                    return !!value_ptr && !!instance_ptr;

                };

            }

            template <typename TSubject, typename TProperty>
            TSetter operator() (TProperty TSubject::*, typename std::enable_if_t<std::is_const<TProperty>::value>* = nullptr) const {

                return (*this)();

            }

            template <typename TSubject, typename TProperty>
            TSetter operator() (void (TSubject::* setter)(TProperty)) const {

                return[setter](const TInstance& instance, Any value) -> bool {

                    auto value_ptr = value.As<std::add_pointer_t<std::add_const_t<TProperty>>>();
                    auto instance_ptr = instance.As<TSubject>();

                    if (value_ptr && instance_ptr) {

                        (instance_ptr->*setter)(**value_ptr);

                    }

                    return !!value_ptr && !!instance_ptr;

                };

            }

            template <typename TSubject, typename TProperty>
            TSetter operator() (TProperty& (TSubject::* setter)()) const {

                return[setter](const TInstance& instance, Any value) -> bool {

                    auto value_ptr = value.As<std::add_pointer_t<std::add_const_t<TProperty>>>();
                    auto instance_ptr = instance.As<TSubject>();

                    if (value_ptr && instance_ptr) {

                        (instance_ptr->*setter)() = **value_ptr;

                    }

                    return !!value_ptr && !!instance_ptr;

                };

            }

        };
        
        template <typename TClass>
        class BasicProperty {

        public:

            template <typename TSubject, typename TProperty>
            BasicProperty(const HashedString& name, TProperty TSubject::* field) noexcept;

            template <typename TSubject, typename TProperty>
            BasicProperty(const HashedString& name, TProperty (TSubject::* getter)() const) noexcept;

            template <typename TSubject, typename TProperty>
            BasicProperty(const HashedString& name, TProperty(TSubject::* getter)() const, void(TSubject::* setter)(TProperty)) noexcept;

            template <typename TSubject, typename TProperty>
            BasicProperty(const HashedString& name, const TProperty&(TSubject::* getter)() const, TProperty&(TSubject::* setter)()) noexcept;
            
            const HashedString& GetName() const noexcept;

            const typename BasicType<TClass>& GetType() const noexcept;

            template <typename TInstance, typename TValue>
            bool Get(const TInstance& instance, TValue& value) const;

            template <ConstQualifier kQualifier, typename TValue>
            bool Get(AnyReferenceWrapper<kQualifier, typename BasicType<TClass>> instance, TValue& value) const;
            
            template <typename TInstance, typename TValue>
            bool Set(TInstance& instance, const TValue& value) const;

            template <typename TValue>
            bool Set(AnyReferenceWrapper<ConstQualifier::kNone, BasicType<TClass>> instance, const TValue& value) const;

            template <typename TValue>
            bool Set(AnyReferenceWrapper<ConstQualifier::kConst, BasicType<TClass>> instance, const TValue& value) const = delete;

            template <typename TInstance, typename TValue>
            bool Set(TInstance&& instance, const TValue& value) const = delete;

        private:
            
            HashedString name_;                                     ///< \brief Property name.

            const typename BasicType<TClass>& type_;                ///< \brief Property type.

            typename PropertyGetter<TClass>::TGetter getter_;       ///< \brief Property getter.

            typename PropertySetter<TClass>::TSetter setter_;       ///< \brief Property setter.
            
        };

    }

}

namespace syntropy {

    namespace reflection {

        //////////////// PROPERTY ////////////////

        template <typename TClass>
        template <typename TSubject, typename TProperty>
        BasicProperty<TClass>::BasicProperty(const HashedString& name, TProperty TSubject::* field) noexcept
            : name_(name)
            , type_(type_get<BasicType<TClass>, TProperty>()())
            , getter_(PropertyGetter<TClass>()(field))
            , setter_(PropertySetter<TClass>()(field)){}

        template <typename TClass>
        template <typename TSubject, typename TProperty>
        BasicProperty<TClass>::BasicProperty(const HashedString& name, TProperty(TSubject::* getter)() const) noexcept
            : name_(name)
            , type_(type_get<BasicType<TClass>, TProperty>()())
            , getter_(PropertyGetter<TClass>()(getter))
            , setter_(PropertySetter<TClass>()()) {}

        template <typename TClass>
        template <typename TSubject, typename TProperty>
        BasicProperty<TClass>::BasicProperty(const HashedString& name, TProperty(TSubject::* getter)() const, void(TSubject::* setter)(TProperty)) noexcept
            : name_(name)
            , type_(type_get<BasicType<TClass>, TProperty>()())
            , getter_(PropertyGetter<TClass>()(getter))
            , setter_(PropertySetter<TClass>()(setter)) {}

        template <typename TClass>
        template <typename TSubject, typename TProperty>
        BasicProperty<TClass>::BasicProperty(const HashedString& name, const TProperty& (TSubject::* getter)() const, TProperty& (TSubject::* setter)()) noexcept
            : name_(name)
            , type_(type_get<BasicType<TClass>, TProperty>()())
            , getter_(PropertyGetter<TClass>()(getter))
            , setter_(PropertySetter<TClass>()(setter)) {}

        template <typename TClass>
        inline const HashedString& BasicProperty<TClass>::GetName() const noexcept {

            return name_;

        }

        template <typename TClass>
        inline const typename BasicType<TClass>& BasicProperty<TClass>::GetType() const noexcept {

            return type_;

        }

        template <typename TClass>
        template <typename TInstance, typename TValue>
        inline bool BasicProperty<TClass>::Get(const TInstance& instance, TValue& value) const {

            return getter_(std::addressof(instance),
                           std::addressof(value));

        }

        template <typename TClass>
        template <ConstQualifier kQualifier, typename TValue>
        bool BasicProperty<TClass>::Get(AnyReferenceWrapper<kQualifier, BasicType<TClass>> instance, TValue& value) const {

            return getter_(instance,
                           std::addressof(value));

        }

        template <typename TClass>
        template <typename TInstance, typename TValue>
        inline bool BasicProperty<TClass>::Set(TInstance& instance, const TValue& value) const {

            return setter_(std::addressof(instance),
                           std::addressof(value));

        }

        template <typename TClass>
        template <typename TValue>
        bool BasicProperty<TClass>::Set(AnyReferenceWrapper<ConstQualifier::kNone, BasicType<TClass>> instance, const TValue& value) const {

            return setter_(instance,
                           std::addressof(value));

        }

    }

}