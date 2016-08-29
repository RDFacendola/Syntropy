
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

        class Class;

        /// \brief Describes a type.
        /// \author Raffaele D. Facendola.
        class Type {

        public:

            /// \brief No copy constructor.
            Type(const Type&) = delete;

            /// \brief No assignment operator.
            Type& operator=(const Type&) = delete;

            /// \brief Virtual destructor.
            virtual ~Type() = default;

            template <typename TType>
            static const Type& GetType();

            bool operator==(const Type& other) const noexcept;

            virtual const Class& GetClass() const = 0;

            virtual bool IsPointer() const noexcept = 0;

            virtual bool IsConst() const noexcept = 0;

            virtual bool IsVolatile() const noexcept = 0;

            virtual bool IsLValueReference() const noexcept = 0;
 
            virtual bool IsRValueReference() const noexcept = 0;
            
            virtual bool IsArray() const noexcept = 0;

            virtual size_t GetArrayRank() const noexcept = 0;

            virtual size_t GetArraySize(size_t dimension = 0) const noexcept = 0;

            virtual std::unique_ptr<Type> GetNext() const noexcept = 0;

        protected:

            virtual const std::type_info& GetTypeInfo() const noexcept = 0;

        private:

            template <typename TType>
            class TypeT;

            Type() = default;
                        
        };

        template <typename TType>
        class Type::TypeT: public Type{

        public:

            virtual const Class& GetClass() const override;

            virtual bool IsPointer() const noexcept override;

            virtual bool IsConst() const noexcept override;

            virtual bool IsVolatile() const noexcept override;

            virtual bool IsLValueReference() const noexcept override;

            virtual bool IsRValueReference() const noexcept override;

            virtual bool IsArray() const noexcept override;

            virtual size_t GetArrayRank() const noexcept override;

            virtual size_t GetArraySize(size_t dimension = 0) const noexcept override;

            virtual std::unique_ptr<Type> GetNext() const noexcept override;

        protected:

            virtual const std::type_info& GetTypeInfo() const noexcept override;

        };

        template <typename TType>
        const Type& type_of();

    }

    template <typename TType>
    struct type_get<typename reflection::Type, TType> {

        const reflection::Type& operator()() const noexcept {

            return reflection::Type::GetType<TType>();

        }

    };

}

namespace syntropy {

    namespace reflection {

        // Implementation

        //////////////// TYPE ////////////////

        template <typename TType>
        inline const Type& Type::GetType() {

            static TypeT<TType> type;

            return type;

        }

        //////////////// TYPE :: TYPE T ////////////////

        template <typename TType>
        inline const Class& Type::TypeT<TType>::GetClass() const {

            return class_get<Class, TType>()();

        }

        template <typename TType>
        inline bool Type::TypeT<TType>::IsPointer() const noexcept {

            return std::is_pointer<TType>::value;

        }

        template <typename TType>
        inline bool Type::TypeT<TType>::IsConst() const noexcept {

            return std::is_const<TType>::value;

        }

        template <typename TType>
        inline bool Type::TypeT<TType>::IsVolatile() const noexcept {

            return std::is_volatile<TType>::value;

        }

        template <typename TType>
        inline bool Type::TypeT<TType>::IsLValueReference() const noexcept {

            return std::is_lvalue_reference<TType>::value;

        }

        template <typename TType>
        inline bool Type::TypeT<TType>::IsRValueReference() const noexcept {

            return std::is_rvalue_reference<TType>::value;

        }

        template <typename TType>
        inline bool Type::TypeT<TType>::IsArray() const noexcept {

            return std::is_array<TType>::value;

        }

        template <typename TType>
        inline size_t Type::TypeT<TType>::GetArrayRank() const noexcept {

            return std::rank<TType>::value;

        }

        template <typename TType>
        inline size_t Type::TypeT<TType>::GetArraySize(size_t /*dimension*/) const noexcept{

            return std::extent<TType, 0>::value;

        }

        template <typename TType>
        inline std::unique_ptr<Type> Type::TypeT<TType>::GetNext() const noexcept {

            using TSubType = std::conditional_t<std::is_array<TType>::value || std::is_reference<TType>::value,
                                                std::remove_all_extents_t<std::remove_reference_t<TType>>,          // Remove references and extents from the outermost level (mutually exclusive)
                                                std::conditional_t<std::is_pointer<TType>::value,
                                                                   std::remove_pointer_t<TType>,                    // Remove a pointer level (removes qualifiers as well)
                                                                   std::remove_cv_t<TType>>>;                       // Remove const and volatile qualifiers from the innermost level
                
            return is_class_name_v<TSubType> ?
                   nullptr :
                   std::make_unique<TypeT<TSubType>>();

        }

        template <typename TType>
        inline const std::type_info& Type::TypeT<TType>::GetTypeInfo() const noexcept {

            // The type info alone is not enough to check for inheritance.
            // Instead we store the type info of a known class (int) with the same form\qualifiers of the original type.
            // Checking for a type conversion is a matter of checking whether the form is the same and whether both classes are in the same hierarchy.
            
            return typeid(replace_class_name_t<TType, int>);   

        }
        
        //////////////// TYPE OF ////////////////

        template <typename TType>
        const Type& type_of() {

            return Type::GetType<TType>();

        }

    }

}