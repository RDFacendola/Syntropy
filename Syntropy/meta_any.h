
/// \file meta_any.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "any.h"
#include "meta_class.h"

namespace syntropy {

    /// \brief Describes a type-safe container for a single value of any type with its metaclass.
    /// This class behaves exactly like syntropy::Any, except it supports safe upcasting and downcasting for pointers
    /// and safe value conversions for scalars.
    /// MetaAny is a realization of Any, while the converse is not true.
    /// \author Raffaele D. Facendola
    class MetaAny {

    public:

        /// \brief Create an empty instance.
        MetaAny();

        /// \brief Destructor.
        ~MetaAny();

        /// \brief Copy constructor.
        /// \param other Instance to copy.
        MetaAny(const MetaAny& other) = default;

        /// \brief Move constructor
        /// \param other Instance to move.
        MetaAny(MetaAny&& other);

        /// \brief Create an instance containing a specific object.
        /// \param value Value to store inside the instance.
        template <typename TValue>
        MetaAny(const TValue& value);

        /// \brief Implicit conversion to Any type.
        operator Any();

        /// \brief Implicit conversion to Any type.
        operator const Any() const;

        /// \brief Unified assignment operator.
        /// \param other Instance to assign from.
        MetaAny& operator=(MetaAny other);

        /// \brief Assign a new value to this instance.
        /// \param other Value to store inside the instance.
        template <typename TValue>
        MetaAny& operator=(const TValue& other);

        /// \brief Check whether the type of value contained inside this instance is the one specified by the template argument.
        template <typename TValue>
        bool Is() const;

        /// \brief Get a typed pointer to the contained value.
        /// \return Returns a pointer to the contained value if the underlying type is exactly the one specified by the template parameters. Returns nullptr instead.
        template <typename TValue>
        const TValue* As() const;

        /// \brief Get a typed pointer to the contained value.
        /// \return Returns a pointer to the contained value if the underlying type is exactly the one specified by the template parameters. Returns nullptr instead.
        template <typename TValue>
        TValue* As();

        /// \brief Swaps two instances.
        /// \param other Object to swap with the current instance.
        MetaAny& swap(MetaAny& other) noexcept;

    private:

        Any any_;               ///< \brief Holds the actual value.

        MetaClass* class_;      ///< \brief Infos about the value class and its hierarchy. Nullptr if no value is being held.

    };

    //////////////// META ANY ////////////////

    inline MetaAny::MetaAny()
        : class_(nullptr) {};

    inline MetaAny::~MetaAny() {}

    inline MetaAny::MetaAny(MetaAny&& other)
        : any_(std::move(other.any_))
        , class_(other.class_) {}

    template <typename TValue>
    inline MetaAny::MetaAny(const TValue& value) {

    }

    inline MetaAny::operator Any() {
        
        return any_;

    }
        
    inline MetaAny::operator const Any() const {

        return any_;

    }

    inline MetaAny& MetaAny::operator=(MetaAny other) {

        return MetaAny(other).swap(*this);

    }

    template <typename TValue>
    inline MetaAny& MetaAny::operator=(const TValue& other) {

        return *this;

    }

    template <typename TValue>
    inline bool MetaAny::Is() const {

        return !any_.IsEmpty() ?
               class_->IsConvertibleTo(MetaClass::GetClass<TValue>()) :
               typeid(TValue) == typeid(void);
        
    }

    template <typename TValue>
    inline const TValue* MetaAny::As() const {

        return any_.Is<TValue>() ?
               any_.As<TValue>() :
               nullptr;

    }

    template <typename TValue>
    inline TValue* MetaAny::As() {

        return any_.Is<TValue>() ?
               any_.As<TValue>() :
               nullptr;

    }

    inline MetaAny& MetaAny::swap(MetaAny& other) noexcept {

        any_.swap(other.any_);

        std::swap(class_, other.class_);

    }


}