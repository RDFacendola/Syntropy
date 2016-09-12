#include "reflection/type.h"

#include "reflection/class.h"

namespace syntropy {

    namespace reflection {

        namespace {

            /// \brief Appends the array extents of the given type to an output stream.
            inline void AppendExtents(std::ostream& out, const Type& type) {

                size_t elements;

                for (size_t dimension = 0; dimension < type.GetArrayRank(); ++dimension) {

                    elements = type.GetArraySize(dimension);

                    if (elements > 0) {

                        out << "[" << elements << "]";

                    }
                    else {

                        out << "[]";

                    }

                }

            }

            /// \brief Appends the references literals (& or &&) of the given type to an output stream.
            inline void AppendReferences(std::ostream& out, const Type& type) {

                if (type.IsLValueReference()) {
                
                    out << "&";
                
                }
                
                if (type.IsRValueReference()) {
                
                    out << "&&";
                
                }

            }

            /// \brief Appends the pointer literal (*) of the given type to an output stream.
            inline void AppendPointers(std::ostream& out, const Type& type) {

                if (type.IsPointer()) {

                    out << "*";

                }

            }

            /// \brief Appends the qualifier literals (const and\or volatile) of the given type to an output stream.
            inline void AppendQualifiers(std::ostream& out, const Type& type) {

                //Caveat - arrays have the same qualifiers as their nested types : to prevent the same qualifier being appended twice, we skip this level and let the next one handle that.

                if (!type.IsArray()) {

                    if (type.IsConst()) {

                        out << " const";

                    }

                    if (type.IsVolatile()) {

                        out << " volatile";

                    }

                }

            }

        }

        bool Type::operator ==(const Type& other) const noexcept {

            return GetCommonTypeInfo() == other.GetCommonTypeInfo() &&
                   (!IsConst() || other.IsConst()) &&                           // Const check is required because typeid ignores top-level cv qualifiers
                   GetClass() == other.GetClass();

        }

        bool Type::operator!=(const Type& other) const noexcept {

            return GetCommonTypeInfo() != other.GetCommonTypeInfo() ||
                   (IsConst() && !other.IsConst()) ||                           // Const check is required because typeid ignores top-level cv qualifiers
                   GetClass() != other.GetClass();

        }

        std::ostream& operator<<(std::ostream& out, const Type& type) {

            auto nested_type = type.GetNext();

            if (nested_type) {

                out << *nested_type;            // Append in reverse order

            }
            else {

                out << type.GetClass();         // Output class name

            }

            // Extents, references and pointers are mutually exclusive

            AppendExtents(out, type);
            AppendReferences(out, type);
            AppendPointers(out, type);

            AppendQualifiers(out, type);

            return out;

        }

    }

}

