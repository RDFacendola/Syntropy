
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

        template <typename TClass, typename TClassComparator>
        class BasicType;

        struct type_is;

    }

}

namespace syntropy {

    namespace reflection {

        /// \brief Describes a type.
        /// \author Raffaele D. Facendola.
        template <typename TClass,
                  typename TClassComparator>
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

            bool Is(const BasicType& other) const noexcept;

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

        template <typename TClass, typename TClassComparator>
        template <typename TType>
        class BasicType<TClass, TClassComparator>::SubType: public BasicType{

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

        struct type_is {

            template <typename TClass, typename TClassComparator>
            bool operator()(const reflection::BasicType<TClass, TClassComparator>& from, const reflection::BasicType<TClass, TClassComparator>& to) const noexcept {

                return from.Is(to);

            }

        };

    }

    template <typename TInstance, typename TClass, typename TClassComparator>
    struct type_get<typename reflection::BasicType<TClass, TClassComparator>, TInstance> {

        const reflection::BasicType<TClass, TClassComparator>& operator()() const noexcept {

            return reflection::BasicType<TClass, TClassComparator>::GetType<TInstance>();

        }

    };

}

namespace syntropy {

    namespace reflection {

        // Implementation

        //////////////// TYPE ////////////////

        template <typename TClass, typename TClassComparator>
        template <typename TType>
        inline const BasicType<TClass, TClassComparator>& BasicType<TClass, TClassComparator>::GetType() {

            static SubType<TType> type;

            return type;

        }

        template <typename TClass, typename TClassComparator>
        bool BasicType<TClass, TClassComparator>::Is(const BasicType<TClass, TClassComparator>& other) const noexcept {

            return other.GetTypeInfo() == GetTypeInfo() &&
                   TClassComparator()(GetClass(), other.GetClass());

        }

        //////////////// TYPE :: SUBTYPE ////////////////

        template <typename TClass, typename TClassComparator>
        template <typename TType>
        inline const TClass& BasicType<TClass, TClassComparator>::SubType<TType>::GetClass() const {

            return class_get<TClass, TType>()();

        }

        template <typename TClass, typename TClassComparator>
        template <typename TType>
        inline bool BasicType<TClass, TClassComparator>::SubType<TType>::IsPointer() const noexcept {

            return std::is_pointer<TType>::value;

        }

        template <typename TClass, typename TClassComparator>
        template <typename TType>
        inline bool BasicType<TClass, TClassComparator>::SubType<TType>::IsConst() const noexcept {

            return std::is_const<TType>::value;

        }

        template <typename TClass, typename TClassComparator>
        template <typename TType>
        inline bool BasicType<TClass, TClassComparator>::SubType<TType>::IsVolatile() const noexcept {

            return std::is_volatile<TType>::value;

        }

        template <typename TClass, typename TClassComparator>
        template <typename TType>
        inline bool BasicType<TClass, TClassComparator>::SubType<TType>::IsLValueReference() const noexcept {

            return std::is_lvalue_reference<TType>::value;

        }

        template <typename TClass, typename TClassComparator>
        template <typename TType>
        inline bool BasicType<TClass, TClassComparator>::SubType<TType>::IsRValueReference() const noexcept {

            return std::is_rvalue_reference<TType>::value;

        }

        template <typename TClass, typename TClassComparator>
        template <typename TType>
        inline bool BasicType<TClass, TClassComparator>::SubType<TType>::IsArray() const noexcept {

            return std::is_array<TType>::value;

        }

        template <typename TClass, typename TClassComparator>
        template <typename TType>
        inline size_t BasicType<TClass, TClassComparator>::SubType<TType>::GetArrayRank() const noexcept {

            return std::rank<TType>::value;

        }

        template <typename TClass, typename TClassComparator>
        template <typename TType>
        inline size_t BasicType<TClass, TClassComparator>::SubType<TType>::GetArraySize(size_t /*dimension*/) const noexcept{

            return std::extent<TType, 0>::value;

        }

        template <typename TClass, typename TClassComparator>
        template <typename TType>
        inline std::unique_ptr<BasicType<TClass, TClassComparator>> BasicType<TClass, TClassComparator>::SubType<TType>::GetNext() const noexcept {

            using TSubType = std::conditional_t<std::is_array<TType>::value || std::is_reference<TType>::value,
                                                std::remove_all_extents_t<std::remove_reference_t<TType>>,          // Remove references and extents from the outermost level (mutually exclusive)
                                                std::conditional_t<std::is_pointer<TType>::value,
                                                                   std::remove_pointer_t<TType>,                    // Remove a pointer level (removes qualifiers as well)
                                                                   std::remove_cv_t<TType>>>;                       // Remove const and volatile qualifiers from the innermost level
                
            return is_class_name_v<TSubType> ?
                   nullptr :
                   std::make_unique<SubType<TSubType>>();

        }

        template <typename TClass, typename TClassComparator>
        template <typename TType>
        inline const std::type_info& BasicType<TClass, TClassComparator>::SubType<TType>::GetTypeInfo() const noexcept {

            // The type info of the actual type is not enough to check for inheritance.
            // We store the type info of a known class (int) with the same form\qualifiers of the original type.
            // Checking for a type conversion is a matter of checking whether the form is the same and whether both classes are in the same hierarchy.
            
            return typeid(replace_t<TType, int>);   

        }
        
    }

}