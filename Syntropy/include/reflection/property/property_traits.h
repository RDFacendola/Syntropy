
/// \file property_traits.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

namespace syntropy {

    namespace reflection {

        template <typename... TProperty>
        struct property_traits {};

        template <typename TClass, typename TField>
        struct property_traits<TField TClass::*> {
        
            using property_type = typename TField;

            using class_type = typename TClass;
        
        };

        template <typename TClass, typename TGetter>
        struct property_traits<TGetter(TClass::*)() const> {

            using property_type = typename std::remove_cv_t<std::remove_reference_t<TGetter>>;

            using class_type = typename TClass;

        };

        template <typename TClass, typename TProperty, typename TReturn>
        struct property_traits<TProperty(TClass::*)() const, TReturn(TClass::*)(TProperty)> {

            using property_type = typename std::remove_cv_t<std::remove_reference_t<TProperty>>;

            using class_type = typename TClass;

        };

        template <typename TClass, typename TProperty>
        struct property_traits<const TProperty& (TClass::*)() const, TProperty& (TClass::*)()> {

            using property_type = typename TProperty;

            using class_type = typename TClass;

        };

        template <typename... TProperty>
        using property_traits_class_t = typename property_traits<TProperty...>::class_type;

        template <typename... TProperty>
        using property_traits_property_t = typename property_traits<TProperty...>::class_type;

    }

}