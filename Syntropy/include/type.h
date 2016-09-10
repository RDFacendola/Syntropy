
/// \file type.h
/// \brief This header is part of the syntropy reflection system. It contains methods and classes used to handle types.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <typeinfo>
#include <ostream>

#include "type_traits.h"

namespace syntropy {

    namespace reflection {

        class Class;

        template <typename TType>
        const Class& ClassOf();

        /// \brief Describes a type.
        /// A 'type' is made out of class names, pointers, qualifiers, references and\or extents.
        /// A type has exactly one class name and it may describe a reference, an array or a value, but not both at the same time.
        /// The class supports multiple level of (qualified) indirections.
        /// \remarks This class is a singleton.
        /// \author Raffaele D. Facendola - August 2016
        class Type {

        public:

            /// \brief No copy constructor.
            Type(const Type&) = delete;

            /// \brief No assignment operator.
            Type& operator=(const Type&) = delete;

            /// \brief Virtual destructor.
            virtual ~Type() = default;

            /// \brief Get the type associated to TType.
            /// \return Returns a reference to the singleton describing TType.
            template <typename TType>
            static const Type& GetType();

            /// \brief Equality comparison.
            /// Check whether the type described by this instance *is* the same as the one described by the other one.
            /// \return Returns true if this type is exactly the same or derives from the other one, returns false otherwise.
            /// \remarks This method accounts for inheritance but not for implicit conversion. Type(Derived) == Type(Base) yields true, while Type(int) == Type(float) yields false.
            bool operator==(const Type& other) const noexcept;

            /// \brief Inequality comparison.
            /// Check whether the type described by this instance is not the same as the one described by the other one.
            /// \return Returns true if this type *is not* the other one, return false otherwise.
            /// \remarks This method accounts for inheritance: Type(Derived) != Type(Base) yields false, while Type(Base) != Type(Derived) yields true.
            bool operator!=(const Type& other) const noexcept;

            /// \brief Get the class associated to the type.
            /// \return Returns a reference to the type class.
            /// \remarks Pointer, qualifiers, references and extents are not accounted for. For instance Type(float*) and Type(float) have the same class.
            virtual const Class& GetClass() const = 0;

            /// \brief Get the typeid associated to the type.
            /// \return Returns the type if associated to the type.
            virtual const std::type_info& GetTypeInfo() const noexcept = 0;

            /// \brief Check whether this type refers to a pointer.
            /// \return Returns true if the type is a pointer, returns false otherwise.
            virtual bool IsPointer() const noexcept = 0;

            /// \brief Check whether this type is const-qualified.
            /// \return Returns true if the type is const-qualified, returns false otherwise.
            virtual bool IsConst() const noexcept = 0;

            /// \brief Check whether this type is volatile-qualified.
            /// \return Returns true if the type is volatile-qualified, returns false otherwise.
            virtual bool IsVolatile() const noexcept = 0;

            /// \brief Check whether this type describes an L-value reference.
            /// \return Returns true if the type describes an L-value reference, returns false otherwise.
            virtual bool IsLValueReference() const noexcept = 0;
            
            /// \brief Check whether this type describes an R-value reference.
            /// \return Returns true if the type describes an R-value reference, returns false otherwise.
            virtual bool IsRValueReference() const noexcept = 0;
            
            /// \brief Check whether this type describes an array.
            /// \return Returns true if the type describes an array, returns false otherwise.
            /// \remarks Arrays get the same qualifiers as the type they are referring to, while references do not. For instance, "const float[]" is constant, while "const float&" is not.
            virtual bool IsArray() const noexcept = 0;

            /// \brief Get the array rank (aka number of dimensions).
            /// \return If the type described by this instance is an array, this method returns the rank of that array, otherwise it returns 0.
            virtual size_t GetArrayRank() const noexcept = 0;

            /// \brief Get the number of elements along a particular dimension of the array.
            /// \param dimension Dimension.
            /// \return Returns the elements along the dimension-th array dimension. If the type is not an array or the size of the array is not specified returns 0.
            virtual size_t GetArraySize(size_t dimension = 0) const noexcept = 0;

            /// \brief Strip a level of indirection, reference or extents and get the resulting type.
            /// If the described type is an array, all its extents are removed at once.
            /// \return Returns a pointer to the type resulting from stripping a level of indirection, references or extents from the current type. If the current type contains the qualified class name only, returns nullptr.
            /// \remarks The type "(((const float) * const)[])", yields the following type sequence: "(?)[]" -> "(?) * const" -> "const float" -> nullptr.
            virtual const Type* GetNext() const noexcept = 0;

        protected:

            /// \brief Get the type info associated to a known common type with the same form as this type one's.
            /// The common type info is used to simplify type checks while still being able to detect correct inheritance.
            /// For instance even if typeid(Derived**) and typeid(Base**) are not the same, the check Type(Derived**) == Type(Base**) should yield true.
            /// To solve the issue we can replace the class name with something else to check the form first and perform inheritance check using the underlying class then.
            /// In the example above we replace each class name with "int", then we check typeid(int**) against typeid(int**) which yields true.
            /// At this point we may check whether Class(Derived) == Class(Base), which is indeed true.
            /// \return Returns the type info associated to a type which is the same as the one described by this type, except that the class name is replaced with a known common type.
            virtual const std::type_info& GetCommonTypeInfo() const noexcept = 0;

        private:

            template <typename TType>
            class TypeT;

            Type() = default;
                        
        };

        /// \brief Describes an actual type.
        /// Used to perform type-erasure.
        /// \author Raffaele D. Facendola - August 2016
        template <typename TType>
        class Type::TypeT: public Type{

        public:

            virtual const Class& GetClass() const override;

            virtual const std::type_info& GetTypeInfo() const noexcept override;

            virtual bool IsPointer() const noexcept override;

            virtual bool IsConst() const noexcept override;

            virtual bool IsVolatile() const noexcept override;

            virtual bool IsLValueReference() const noexcept override;

            virtual bool IsRValueReference() const noexcept override;

            virtual bool IsArray() const noexcept override;

            virtual size_t GetArrayRank() const noexcept override;

            virtual size_t GetArraySize(size_t dimension = 0) const noexcept override;

            virtual const Type* GetNext() const noexcept override;

        protected:

            virtual const std::type_info& GetCommonTypeInfo() const noexcept override;

        };

        /// \brief Get the type associated to TType.
        /// \return Returns a reference to the type associated to TType.
        template <typename TType>
        const Type& TypeOf();

        /// \brief Stream insertion for Type.
        std::ostream& operator<<(std::ostream& out, const Type& type);

    }

    /// \brief Partial specialization of type_get<> for syntropy::reflection::Type.
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

            return ClassOf<TType>();

        }

        template <typename TType>
        const std::type_info& Type::TypeT<TType>::GetTypeInfo() const noexcept {

            return typeid(TType);

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
        inline const Type* Type::TypeT<TType>::GetNext() const noexcept {

            using TSubType = std::conditional_t<std::is_array<TType>::value || std::is_reference<TType>::value,
                                                std::remove_all_extents_t<std::remove_reference_t<TType>>,          // Remove references and extents from the outermost level (mutually exclusive)
                                                std::conditional_t<std::is_pointer<TType>::value,
                                                                   std::remove_pointer_t<TType>,                    // Remove a pointer level (removes qualifiers as well)
                                                                   std::remove_cv_t<TType>>>;                       // Remove const and volatile qualifiers from the innermost level

            return is_class_name_v<TSubType> ?
                   nullptr :
                   std::addressof(TypeOf<TSubType>());

        }

        template <typename TType>
        inline const std::type_info& Type::TypeT<TType>::GetCommonTypeInfo() const noexcept {

            return typeid(replace_class_name_t<TType, int>);   

        }
        
        //////////////// TYPE OF ////////////////

        template <typename TType>
        const Type& TypeOf() {

            return Type::GetType<TType>();

        }

    }

}