#include "reflection/type.h"

#include "reflection/class.h"

namespace syntropy
{
    namespace reflection
    {

        /************************************************************************/
        /* TYPE                                                                 */
        /************************************************************************/

        bool Type::operator==(const Type& other) const noexcept
        {
            return this == std::addressof(other);
        }

        bool Type::operator!=(const Type& other) const noexcept
        {
            return this != std::addressof(other);
        }

        bool Type::IsConvertibleTo(const Type& other) const noexcept
        {
            return GetClass().IsA(other.GetClass()) &&                                  // The class must be the same or a base class.
                array_size_ == other.array_size_ &&                                     // Rank and array size must be the same in each dimension.
                indirection_levels_ == other.indirection_levels_ &&                     // Both types must have the same level of indirection.
                (const_mask_ & ~other.const_mask_) == 0 &&                              // Const qualifiers must be preserved (a const cannot become non-const).
                (volatile_mask_ & ~other.volatile_mask_) == 0;                          // Volatile qualifiers must be preserved (a volatile cannot become non-volatile).
        }

        const Class& Type::GetClass() const
        {
            return class_;
        }

        bool Type::IsPointer() const noexcept
        {
            return indirection_levels_ > 0;
        }

        bool Type::IsConst() const noexcept
        {
            return const_mask_ & 0x1;
        }

        bool Type::IsVolatile() const noexcept
        {
            return volatile_mask_ & 0x1;
        }

        bool Type::IsLValueReference() const noexcept
        {
            return is_lvalue_reference_;
        }

        bool Type::IsRValueReference() const noexcept
        {
            return is_rvalue_reference_;
        }

        size_t Type::GetArrayRank() const noexcept
        {
            return array_size_.size();
        }

        size_t Type::GetArraySize(size_t dimension) const noexcept
        {
            return array_size_[dimension];
        }

        std::ostream& operator<<(std::ostream& out, const Type& type)
        {
            // Qualified class name

            if (type.IsConst())
            {
                out << "const ";
            }

            if (type.IsVolatile())
            {
                out << "volatile ";
            }

            out << type.GetClass();

            // Qualified levels of indirection

            for (uint8_t indirection_level = 0; indirection_level < type.indirection_levels_; ++indirection_level)
            {
                out << "*";

                if (type.const_mask_ & (0x2 << indirection_level))
                {
                    out << " const";
                }

                if (type.volatile_mask_ & (0x2 << indirection_level))
                {
                    out << " volatile";
                }
            }

            // Extents

            for (size_t dimension = 0; dimension < type.GetArrayRank(); ++dimension)
            {
                auto elements = type.GetArraySize(dimension);

                if (elements > 0)
                {
                    out << "[" << elements << "]";
                }
                else
                {
                    out << "[]";
                }
            }

            // References

            if (type.IsLValueReference())
            {
                out << "&";
            }
            else if (type.IsRValueReference())
            {
                out << "&&";
            }

            return out;
        }

    }

}

