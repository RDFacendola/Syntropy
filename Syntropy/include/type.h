
/// \file type.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <memory>
#include <typeinfo>

#include "type_traits.h"

namespace syntropy {

    namespace reflection {

        template <typename TClass>
        class BasicType;

        struct type_is;

    }

}

namespace syntropy {

    namespace reflection {

        /// \brief Describes a type.
        /// \author Raffaele D. Facendola.
        template <typename TClass>
        class BasicType {

        public:

            /// \brief No copy constructor.
            BasicType(const BasicType&) = delete;

            /// \brief No assignment operator.
            BasicType& operator=(const BasicType&) = delete;

            /// \brief Virtual destructor.
            virtual ~BasicType() = default;

            template <typename TType>
            static const BasicType& GetType();

            bool operator==(const BasicType<TClass>& other) const noexcept;

            virtual const TClass& GetClass() const = 0;

            virtual bool IsPointer() const noexcept = 0;

            virtual bool IsConst() const noexcept = 0;

            virtual bool IsVolatile() const noexcept = 0;

            virtual bool IsLValueReference() const noexcept = 0;
 
            virtual bool IsRValueReference() const noexcept = 0;
            
            virtual bool IsArray() const noexcept = 0;

            virtual size_t GetArrayRank() const noexcept = 0;

            virtual size_t GetArraySize(size_t dimension = 0) const noexcept = 0;

            virtual std::unique_ptr<BasicType> GetNext() const noexcept = 0;

        protected:

            virtual const std::type_info& GetTypeInfo() const noexcept = 0;

        private:

            template <typename TType>
            class SubType;

            BasicType() = default;
                        
        };

        template <typename TClass>
        template <typename TType>
        class BasicType<TClass>::SubType: public BasicType{

        public:

            virtual const TClass& GetClass() const override;

            virtual bool IsPointer() const noexcept override;

            virtual bool IsConst() const noexcept override;

            virtual bool IsVolatile() const noexcept override;

            virtual bool IsLValueReference() const noexcept override;

            virtual bool IsRValueReference() const noexcept override;

            virtual bool IsArray() const noexcept override;

            virtual size_t GetArrayRank() const noexcept override;

            virtual size_t GetArraySize(size_t dimension = 0) const noexcept override;

            virtual std::unique_ptr<BasicType> GetNext() const noexcept override;

        protected:

            virtual const std::type_info& GetTypeInfo() const noexcept override;

        };

    }

    template <typename TInstance, typename TClass>
    struct type_get<typename reflection::BasicType<TClass>, TInstance> {

        const reflection::BasicType<TClass>& operator()() const noexcept {

            return reflection::BasicType<TClass>::GetType<TInstance>();

        }

    };

}

namespace syntropy {

    namespace reflection {

        // Implementation

        //////////////// TYPE ////////////////

        template <typename TClass>
        template <typename TType>
        inline const BasicType<TClass>& BasicType<TClass>::GetType() {

            static SubType<TType> type;

            return type;

        }

        template <typename TClass>
        bool BasicType<TClass>::operator ==(const BasicType<TClass>& other) const noexcept {

            return GetTypeInfo() == other.GetTypeInfo() &&
                   GetClass() == other.GetClass();

        }

        //////////////// TYPE :: SUBTYPE ////////////////

        template <typename TClass>
        template <typename TType>
        inline const TClass& BasicType<TClass>::SubType<TType>::GetClass() const {

            return class_get<TClass, TType>()();

        }

        template <typename TClass>
        template <typename TType>
        inline bool BasicType<TClass>::SubType<TType>::IsPointer() const noexcept {

            return std::is_pointer<TType>::value;

        }

        template <typename TClass>
        template <typename TType>
        inline bool BasicType<TClass>::SubType<TType>::IsConst() const noexcept {

            return std::is_const<TType>::value;

        }

        template <typename TClass>
        template <typename TType>
        inline bool BasicType<TClass>::SubType<TType>::IsVolatile() const noexcept {

            return std::is_volatile<TType>::value;

        }

        template <typename TClass>
        template <typename TType>
        inline bool BasicType<TClass>::SubType<TType>::IsLValueReference() const noexcept {

            return std::is_lvalue_reference<TType>::value;

        }

        template <typename TClass>
        template <typename TType>
        inline bool BasicType<TClass>::SubType<TType>::IsRValueReference() const noexcept {

            return std::is_rvalue_reference<TType>::value;

        }

        template <typename TClass>
        template <typename TType>
        inline bool BasicType<TClass>::SubType<TType>::IsArray() const noexcept {

            return std::is_array<TType>::value;

        }

        template <typename TClass>
        template <typename TType>
        inline size_t BasicType<TClass>::SubType<TType>::GetArrayRank() const noexcept {

            return std::rank<TType>::value;

        }

        template <typename TClass>
        template <typename TType>
        inline size_t BasicType<TClass>::SubType<TType>::GetArraySize(size_t /*dimension*/) const noexcept{

            return std::extent<TType, 0>::value;

        }

        template <typename TClass>
        template <typename TType>
        inline std::unique_ptr<BasicType<TClass>> BasicType<TClass>::SubType<TType>::GetNext() const noexcept {

            using TSubType = std::conditional_t<std::is_array<TType>::value || std::is_reference<TType>::value,
                                                std::remove_all_extents_t<std::remove_reference_t<TType>>,          // Remove references and extents from the outermost level (mutually exclusive)
                                                std::conditional_t<std::is_pointer<TType>::value,
                                                                   std::remove_pointer_t<TType>,                    // Remove a pointer level (removes qualifiers as well)
                                                                   std::remove_cv_t<TType>>>;                       // Remove const and volatile qualifiers from the innermost level
                
            return is_class_name_v<TSubType> ?
                   nullptr :
                   std::make_unique<SubType<TSubType>>();

        }

        template <typename TClass>
        template <typename TType>
        inline const std::type_info& BasicType<TClass>::SubType<TType>::GetTypeInfo() const noexcept {

            // The type info of the actual type is not enough to check for inheritance.
            // We store the type info of a known class (int) with the same form\qualifiers of the original type.
            // Checking for a type conversion is a matter of checking whether the form is the same and whether both classes are in the same hierarchy.
            
            return typeid(replace_class_name_t<TType, int>);   

        }
        
    }

}