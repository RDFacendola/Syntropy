
/// \file any_reference.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <typeinfo>
#include <functional>

#include "type_traits.h"

namespace syntropy {

    template <ConstQualifier kConstQualifier, typename TType>
    class AnyReferenceWrapper;

    using AnyReference = AnyReferenceWrapper<ConstQualifier::kNone, std::type_info>;

    using AnyConstReference = AnyReferenceWrapper<ConstQualifier::kConst, std::type_info>;

    /// \brief Represents a type-safe container that wraps a reference of any type in a copyable, assignable object.
    /// The container is const qualified to preserve const-correctness at compile time. Both const and non-const references can be implicitly converted to const-references.
    /// The container grants type-safety via any custom type TType that can be determined via a proper specialization of syntropy::type_get[TType, TInstance].
    /// \author Raffaele D. Facendola
    /// \remarks Inspired to both std::reference_wrapper and std::any
    template <ConstQualifier kConstQualifier, typename TType>
    class AnyReferenceWrapper {

    public:
            
        using TReference = AnyReferenceWrapper<ConstQualifier::kNone, TType>;

        using TConstReference = AnyReferenceWrapper<ConstQualifier::kConst, TType>;
        
        template <typename TInstance>
        using qualify_t = typename std::conditional_t<kConstQualifier == ConstQualifier::kConst,
                                                      typename std::add_const_t<TInstance>,
                                                      typename std::remove_const_t<TInstance>>;
        
        /// \brief Create an empty instance.
        AnyReferenceWrapper() = default;

        /// \brief Destructor.
        ~AnyReferenceWrapper() = default;

        /// \brief Copy constructor.
        /// \param other Instance to copy.
        AnyReferenceWrapper(const AnyReferenceWrapper<kConstQualifier, TType>& other) noexcept;
            
        operator TConstReference() const noexcept;

        /// \brief Wraps an object to be referenced via Instance.
        /// \param instance Object to reference.
        template <typename TInstance>
        AnyReferenceWrapper(TInstance* instance) noexcept;

        /// \brief Unified assignment operator.
        /// \param other Instance to assign from.
        AnyReferenceWrapper& operator=(const AnyReferenceWrapper<kConstQualifier, TType>& other) noexcept;
            
        /// \brief Get a typed pointer to the wrapped object.
        /// \tparam TInstance Type to check against.
        /// \return Returns a pointer to the contained object if the underlying type derives from TInstance. Returns nullptr instead.
        template <typename TInstance>
        TInstance* As() const noexcept;

        bool IsEmpty() const noexcept;

        /// \brief Get the underlying type of the contained value.
        /// \return Returns the type of the contained value.
        const TType& GetType() const noexcept;
            
    private:

        friend class TReference;  // Needed to grant access to the const-qualified version of the content

        /// \brief Base interface for the underlying content.
        struct IContent {

            /// \brief Virtual destructor.
            virtual ~IContent() = default;

            /// \brief Get the type of the instance being referenced.
            /// \return Returns the type of the instance being referenced.
            virtual const TType& GetType() const noexcept = 0;

            /// \brief Clone the underlying value to another instance.
            /// \return Returns a pointer to the new copy of the value.
            virtual std::unique_ptr<IContent> Clone() const noexcept = 0;

            /// \brief Clone the underlying value to another const instance.
            /// \return Returns a pointer to the constant copy of the value.
            virtual std::unique_ptr<typename TConstReference::IContent> ConstClone() const noexcept = 0;

        };

        /// \brief Strongly typed container for a single value.
        /// \tparam TValue Type of the contained value.
        template <typename TInstance>
        struct Content : public IContent {

            /// \brief Create a new container for a single value.
            ///\ param value Value to store inside the container.
            Content(TInstance& instance) noexcept;

            virtual const TType& GetType() const noexcept override;

            virtual std::unique_ptr<IContent> Clone() const noexcept override;

            virtual std::unique_ptr<typename TConstReference::IContent> ConstClone() const noexcept override {

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

                return std::make_unique<TConstReference::Content<TConstReference::qualify_t<TInstance>>>(content_);      // Add const-ness

            }

            TInstance& content_;                            ///< \brief Actual content.

        };

        /// \brief Create a new instance from content pointer.
        AnyReferenceWrapper(std::unique_ptr<IContent> content) noexcept;

        std::unique_ptr<IContent> content_;                 ///< \brief Wraps the actual value.

    };

    template <typename TType = std::type_info, typename TInstance>
    AnyReferenceWrapper<ConstQualifier::kConst, TType> any_cref(const TInstance& instance) noexcept;

    template <typename TType = std::type_info, ConstQualifier kConstQualifier>
    AnyReferenceWrapper<ConstQualifier::kConst, TType> any_cref(AnyReferenceWrapper<kConstQualifier, TType> instance) noexcept;

    template <typename TType = std::type_info, typename TInstance>
    AnyReferenceWrapper<ConstQualifier::kConst, TType> any_cref(const TInstance&&) = delete;

    template <typename TType = std::type_info, typename TInstance>
    AnyReferenceWrapper<ConstQualifier::kNone, TType> any_ref(TInstance& instance) noexcept;
        
    template <typename TType = std::type_info>
    AnyReferenceWrapper<ConstQualifier::kNone, TType> any_ref(AnyReferenceWrapper<ConstQualifier::kConst, TType> instance) = delete;         // Denied: conversion loses qualifiers

    template <typename TType = std::type_info>
    AnyReferenceWrapper<ConstQualifier::kNone, TType> any_ref(AnyReferenceWrapper<ConstQualifier::kNone, TType> instance) noexcept;
        
    template <typename TType = std::type_info, typename TInstance>
    AnyReferenceWrapper<ConstQualifier::kNone, TType> any_ref(const TInstance&&) = delete;
    
}

namespace syntropy {

    // Implementation

    //////////////// ANY REFERENCE ////////////////

    template <ConstQualifier kConstQualifier, typename TType>
    inline AnyReferenceWrapper<kConstQualifier, TType>::AnyReferenceWrapper(const AnyReferenceWrapper<kConstQualifier, TType>& other) noexcept
        : content_(other.content_ ? other.content_->Clone() : nullptr) {}

    template <ConstQualifier kConstQualifier, typename TType>
    AnyReferenceWrapper<kConstQualifier, TType>::AnyReferenceWrapper(std::unique_ptr<IContent> content) noexcept
        : content_(std::move(content)) {}

    template <ConstQualifier kConstQualifier, typename TType>
    template <typename TInstance>
    inline AnyReferenceWrapper<kConstQualifier, TType>::AnyReferenceWrapper(TInstance* instance) noexcept
        : content_(instance ? std::make_unique<Content<qualify_t<TInstance>>>(*instance) : nullptr) {
        
        static_assert(std::is_convertible_v<TInstance*, qualify_t<TInstance>*>, "Conversion loses qualifiers.");

    }

    template <ConstQualifier kConstQualifier, typename TType>
    inline AnyReferenceWrapper<kConstQualifier, TType>& AnyReferenceWrapper<kConstQualifier, TType>::operator=(const AnyReferenceWrapper<kConstQualifier, TType>& other) noexcept {

        content_ = !other.IsEmpty() ?
                   other.content_->Clone() :
                   nullptr;
            
        return *this;

    }
        
    template <ConstQualifier kConstQualifier, typename TType>
    inline const TType& AnyReferenceWrapper<kConstQualifier, TType>::GetType() const noexcept {

        return content_ ?
               content_->GetType() :
               type_get<TType, void>()();

    }

    template <ConstQualifier kConstQualifier, typename TType>
    template <typename TInstance>
    inline TInstance* AnyReferenceWrapper<kConstQualifier, TType>::As() const noexcept {

        return (content_ && GetType() == type_get<TType, TInstance>()()) ?
                std::addressof(static_cast<Content<qualify_t<TInstance>>*>(content_.get())->content_) :
                nullptr;

    }

    template <ConstQualifier kConstQualifier, typename TType>
    inline AnyReferenceWrapper<kConstQualifier, TType>::operator TConstReference() const noexcept {

        return content_->ConstClone();

    }
        
    template <ConstQualifier kConstQualifier, typename TType>
    inline bool AnyReferenceWrapper<kConstQualifier, TType>::IsEmpty() const noexcept {

        return !!content_;

    }

    //////////////// ANY REFERENCE :: CONTENT ////////////////

    template <ConstQualifier kConstQualifier, typename TType>
    template <typename TInstance>
    inline AnyReferenceWrapper<kConstQualifier, TType>::Content<TInstance>::Content(TInstance& instance) noexcept
        : content_(instance) {}

    template <ConstQualifier kConstQualifier, typename TType>
    template <typename TInstance>
    inline const TType& AnyReferenceWrapper<kConstQualifier, TType>::Content<TInstance>::GetType() const noexcept {

        return type_get<TType, TInstance>()();

    }

    template <ConstQualifier kConstQualifier, typename TType>
    template <typename TInstance>
    inline std::unique_ptr<typename AnyReferenceWrapper<kConstQualifier, TType>::IContent> AnyReferenceWrapper<kConstQualifier, TType>::Content<TInstance>::Clone() const noexcept {

        return std::make_unique<Content<TInstance>>(content_);       // Preserve qualifiers
            
    }

    //////////////// ANY CREF \\ ANY REF ////////////////

    template <typename TType, typename TInstance>
    inline AnyReferenceWrapper<ConstQualifier::kConst, TType> any_cref(const TInstance& instance) noexcept {

        return std::addressof(instance);

    }

    template <typename TType, ConstQualifier kConstQualifier>
    inline AnyReferenceWrapper<ConstQualifier::kConst, TType> any_cref(AnyReferenceWrapper<kConstQualifier, TType> instance) noexcept {

        return instance;

    }

    template <typename TType, typename TInstance>
    inline AnyReferenceWrapper<ConstQualifier::kNone, TType> any_ref(TInstance& instance) noexcept {

        return std::addressof(instance);

    }

    template<typename TType>
    inline AnyReferenceWrapper<ConstQualifier::kNone, TType> any_ref(AnyReferenceWrapper<ConstQualifier::kNone, TType> instance) noexcept {

        return instance;

    }
    
}