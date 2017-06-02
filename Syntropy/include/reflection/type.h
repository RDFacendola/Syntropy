
/// \file type.h
/// \brief This header is part of the syntropy reflection system. It contains methods and classes used to handle types.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <typeinfo>
#include <ostream>
#include <array>

#include "utility.h"
#include "type_traits.h"

namespace syntropy
{
    namespace reflection
    {

        class Class;

        template <typename TType>
        const Class& ClassOf();

        /// \brief Describes a type.
        /// A 'type' is made out of class names, pointers, qualifiers, references and\or extents.
        /// A type has exactly one class name and it may describe a reference, an array or a value, but not both at the same time.
        /// The class supports multiple level of (qualified) indirections.
        /// \remarks This class is a singleton.
        /// \author Raffaele D. Facendola - August 2016
        class Type
        {

            friend std::ostream& operator<<(std::ostream& out, const Type& type);

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
            static constexpr const Type& GetType();

            /// \brief Equality comparison.
            /// Check whether this type is exactly the same as another type.
            /// \brief Returns true if this type is the same as the other type, returns false otherwise.
            bool operator==(const Type& other) const noexcept;

            /// \brief Inequality comparison.
            /// Check whether this type is not the same as another type.
            /// \brief Returns true if this type is not the same as the other type, returns false otherwise.
            bool operator!=(const Type& other) const noexcept;

            /// \brief Check whether a variable of this type can be converted to another type.
            /// The check doesn't account for implicit type conversion but will account for polymorphism.
            /// This method will check for level of indirections and cv qualifiers.
            /// Example: TypeOf<int*>().IsConvertibleTo(TypeOf<float*>()) yields false whereas TypeOf<Cat*>().IsConvertibleTo(TypeOf<const Animal*>()) yields true.
            /// \return Returns true if a variable of this type can be statically assigned to a variable of type other, returns false otherwise.
            bool IsConvertibleTo(const Type& other) const noexcept;

            /// \brief Get the class associated to the type.
            /// \return Returns a reference to the type class.
            /// \remarks Pointer, qualifiers, references and extents are not accounted for. For instance Type(float*) and Type(float) have the same class.
            const Class& GetClass() const;

            /// \brief Check whether this type refers to a pointer.
            /// \return Returns true if the type is a pointer, returns false otherwise.
            bool IsPointer() const noexcept;

            /// \brief Check whether this type is const-qualified.
            /// \return Returns true if the type is const-qualified, returns false otherwise.
            bool IsConst() const noexcept;

            /// \brief Check whether this type is volatile-qualified.
            /// \return Returns true if the type is volatile-qualified, returns false otherwise.
            bool IsVolatile() const noexcept;

            /// \brief Check whether this type describes a l-value reference.
            /// \return Returns true if the type describes a l-value reference, returns false otherwise.
            bool IsLValueReference() const noexcept;

            /// \brief Check whether this type describes a r-value reference.
            /// \return Returns true if the type describes a r-value reference, returns false otherwise.
            bool IsRValueReference() const noexcept;

            /// \brief Get the array rank.
            /// \return Get the number of dimensions of the array. Returns 0 if the type is not an array.
            size_t GetArrayRank() const noexcept;

            /// \brief Get the number of elements inside a particular dimension of the array.
            /// \param dimension Dimension to query. Must be less than GetArrayRank().
            /// \return Get the number of elements inside a particular dimension of the array.
            size_t GetArraySize(size_t dimension = 0) const noexcept;

        private:

            /// \brief Create a new type.
            template <typename TType>
            constexpr Type(tag_t<TType>);

            const Class& class_;                        ///< \brief Class this type refers to.

            std::vector<size_t> array_size_;            ///< \brief Array size along each dimension.

            uint8_t indirection_levels_;                ///< \brief Levels of indirection.

            int8_t const_mask_;                         ///< \brief Bitmask containing the "const-ness" of each indirection level, starting from the innermost type.

            int8_t volatile_mask_;                      ///< \brief Bitmask containing the "volatile-ness" of each indirection level, starting from the innermost type.

            bool is_lvalue_reference_ : 1;              ///< \brief Whether the type is a l-value reference.

            bool is_rvalue_reference_ : 1;              ///< \brief Whether the type is a r-value reference.

        };

        /// \brief Get the type associated to TType.
        /// \return Returns a reference to the type associated to TType.
        template <typename TType>
        constexpr const Type& TypeOf();

        /// \brief Stream insertion for Type.
        std::ostream& operator<<(std::ostream& out, const Type& type);

    }

}

// Implementation

namespace syntropy
{
    namespace reflection
    {

        /************************************************************************/
        /* TYPE                                                                 */
        /************************************************************************/

        template <typename TType>
        static constexpr const Type& Type::GetType()
        {
            static Type type(tag<TType>);
            return type;
        }

        template <typename TType>
        constexpr Type::Type(tag_t<TType>)
            : class_(ClassOf<TType>())
            , array_size_(std::begin(array_extents<TType>::value), std::end(array_extents<TType>::value))
            , indirection_levels_(static_cast<int8_t>(indirection_levels_v<TType>))
            , const_mask_(static_cast<int8_t>(predicate_mask_v<std::is_const, TType>))
            , volatile_mask_(static_cast<int8_t>(predicate_mask_v<std::is_volatile, TType>))
            , is_lvalue_reference_(std::is_lvalue_reference_v<TType>)
            , is_rvalue_reference_(std::is_rvalue_reference_v<TType>)
        {
            static_assert(indirection_levels_v<TType> < 8 * sizeof(const_mask_), "Too many levels of indirection.");
            static_assert(indirection_levels_v<TType> < 8 * sizeof(volatile_mask_), "Too many levels of indirection.");
        }

        template <typename TType>
        constexpr const Type& TypeOf()
        {
            return Type::GetType<TType>();
        }

    }

}