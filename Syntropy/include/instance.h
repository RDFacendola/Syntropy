/// \file instance.h
/// \brief This header is part of the syntropy reflection system. It contains methods and classes used to handle object instances.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <type_traits>

#include "type.h"
#include "any_reference.h"

namespace syntropy {

    namespace reflection {

        template <ConstQualifier kConstQualifier>
        using AnyInstance = AnyReferenceWrapper<kConstQualifier, Type>;

        using ConstInstance = AnyReferenceWrapper<ConstQualifier::kConst, Type>;

        using Instance = AnyReferenceWrapper<ConstQualifier::kNone, Type>;

        template <typename TInstance>
        ConstInstance as_cinstance(const TInstance& instance) noexcept;

        template <ConstQualifier kConstQualifier>
        ConstInstance as_cinstance(AnyInstance<kConstQualifier> instance) noexcept;

        template <typename TInstance>
        ConstInstance as_cinstance(const TInstance&&) = delete;

        template <typename TInstance>
        Instance as_instance(TInstance& instance) noexcept;

        Instance as_instance(ConstInstance instance) = delete;         // Denied: conversion loses qualifiers

        Instance as_instance(Instance instance) noexcept;

        template <typename TInstance>
        Instance as_instance(const TInstance&&) = delete;

        template <typename TClass, typename = void>
        struct instantiate {

            Instance operator()() const noexcept {

                return Instance();

            }

        };

        template <typename TClass>
        struct instantiate<TClass, typename std::enable_if_t<std::is_default_constructible_v<TClass>>> {

            Instance operator()() const noexcept {

                return new TClass();

            }

        };

    }

}

namespace syntropy {

    namespace reflection {

        // Implementation
        
        //////////////// AS CINSTANCE ////////////////

        template <typename TInstance>
        inline ConstInstance as_cinstance(const TInstance& instance) noexcept {

            return std::addressof(instance);

        }

        template <ConstQualifier kConstQualifier>
        inline ConstInstance as_cinstance(AnyInstance<kConstQualifier> instance) noexcept {

            return instance;

        }

        //////////////// AS INSTANCE ////////////////

        template <typename TInstance>
        inline Instance as_instance(TInstance& instance) noexcept {

            return std::addressof(instance);

        }

        inline Instance as_instance(Instance instance) noexcept {

            return instance;

        }

    }

}